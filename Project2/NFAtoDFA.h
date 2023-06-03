
#include <map>
#include <set>
#include <queue>
#include <vector>
#include<stack>
#include <iostream>
#include"NFA.h"

using namespace std;
#define MAX 10000

struct DFANode {
	bool istrue = 0;
	int id;
	map<char, DFANode*> transfer;//输入字符 指向的下一个节点
};
DFANode DFANodes[10000];  //DFA节点
DFANode miDFA[10000];      //最小化DFA后的节点
static int isDFAFinal[MAX];   //用来记录每个DFA状态是否是终态，初始化为-1  值>=0说明是终态
static int ismDFAFinal[MAX];   //用来记录每个mDFA状态是否是终态，初始化为-1  值>=0说明是终态
vector<set<int> > DFAStates;//记录简化后的DFA节点
int DFATransitions[10000];//index是DFA的节点编号，对应的值为mDFA的编号

//初始化，stateCnt为DFA状态数量	
void initmDFA(int stateCnt) {
		map<int,set<int> > initialSets;//int对应终结状态的编号，set<int>对应当前状态对应的DFA节点编号
		//记录下所有终结状态，用存储在map中的key（即在l文件中的顺序）来记录节点
		for (int i = 0; i < stateCnt; ++i) {
			int isfinal = isDFAFinal[i];
			//记录每个终结状态对应的DFA节点编号
			initialSets[isfinal].insert(i);
		}
		for (auto& p : initialSets) {
			//将DFA终结状态计入DFAStates
			DFAStates.push_back(p.second);
			//记录每个DFA节点的编号和其对应的mDFA的编号
			for (int idx : p.second) {
				DFATransitions[idx] = DFAStates.size() - 1;
			}
		}
	}

// 这个函数的目标是将相似的状态划分到同一组，以实现状态的简化
void divideDFANodes() {
		int currentSize; // 用于记录每次循环开始时DFA状态集合的大小
		// 以下是一个循环，只有在DFA状态集合的大小发生改变时才会继续执行
		do {
			// 记录本次循环开始时DFA状态集合的大小
			currentSize = DFAStates.size();

			// 对每一个状态集合进行处理
			for (int i = 0; i < DFAStates.size(); ++i) {
				auto& currentSet = DFAStates[i]; // 当前处理的状态集合

				// divideSet用于存储需要从当前状态集合中剥离出去的状态
				set<int> divideSet;

				// first是当前状态集合中的一个状态，transitionTable是这个状态的转移表
				int first = *(currentSet.begin());
				auto& transitionTable = DFANodes[first].transfer;

				// 检查当前状态集合中的每一个状态
				for (int e : currentSet) {
					auto& table = DFANodes[e].transfer; // e的转移表

					bool notSame = false; // 标记e的转移表是否与transitionTable相同

					// 如果两个状态的转移表大小相同，则需要检查内容是否一致
					if (transitionTable.size() == table.size()) {
						for (auto p : transitionTable) {
							auto it = table.find(p.first);
							// 如果在e的转移表中找不到在transitionTable中的某个输入，或者对应的转移状态在简化后的DFA中的位置不同，则说明两个状态不相同
							if (it == table.end())
							{
								notSame = true;
								break;
							}
							else
							{//判断指向的节点是否位于同一个集合
								if (DFATransitions[p.second->id] != DFATransitions[it->second->id]) {
									notSame = true;
									break;
								}
							}
						}
					}
					else  {
						// 如果两个状态的转移表大小不同，则必定不相同
						notSame = true;
					}

					// 如果e与first不相同，则将e加入divideSet
					if (notSame) divideSet.insert(e);
				}
				// 如果存在需要剥离的状态
				if (!divideSet.empty()) {
					// 将divideSet作为新的状态集合加入DFAStates
					DFAStates.push_back(divideSet);
					int newindex = DFAStates.size() - 1; // 新状态集合在DFAStates中的位置
					// 对每一个需要剥离的状态
					for (int e : divideSet) {
						DFAStates[i].erase(e); // 从原状态集合中移除
						DFATransitions[e] = newindex; // 更新其在简化后的DFA中的位置
					}
				}
			}
			// 如果在一次循环中，DFA状态集合的大小没有发生改变，则说明所有的状态都已经不能再被剥离，于是结束循环
		} while (DFAStates.size() != currentSize);
		cout << "DFA的大小" << DFAStates.size() << endl;
	}

void showmDFA() {
		for (int i = 0; i < DFAStates.size(); ++i) {
			printf("%3d : ", i);
			for (int j : DFAStates[i]) {
				printf("%5d", j);
			}
			printf("\n");
		}
	}

int generatemDFA() {
		// 遍历 DFA 状态集 DFAStates 中的每一个状态集
		for (int i = 0; i < DFAStates.size(); ++i) {
			// 设置mDFA 状态 miDFA 的 id 为当前遍历的下标 i
			miDFA[i].id = i;

			// 选取当前遍历的 DFA 状态集中的第一个元素
			auto e = *(DFAStates[i].begin());
			// 根据 isDFAFinal 数组来设置 ismDFAFinal 数组
			ismDFAFinal[i] = isDFAFinal[e];
			// 遍历原 DFA 节点的所有映射关系
	
			for (auto& p : DFANodes[e].transfer) {
				// 提取映射关系中的字符
				char c = p.first;

				// 根据映射关系中的目标状态 id，查找对应的mDFA 状态下标
				int idx = DFATransitions[p.second->id];

				// 建立简洁 DFA 状态的映射关系，即由当前字符 c 映射到下标为 idx 的简洁 DFA 状态
				miDFA[i].transfer[c] = &miDFA[idx];
			}
		}

		// 返回mDFA 的状态数量，即原 DFA 状态集的数量
		return DFAStates.size();
	}

static set<int> epolisonSet[MAX];//记录所有闭包集合，表格左边的部分  如epolisonSet[1]表示1可以到达的所有闭包
static map<char, set<int> > NFAStateTable[MAX];
//记录所有目标状态，char指输入的字符，set指转移后的闭包结果  NFAStatetable[1]['a']表示1状态在接受a字符后可以到达的闭包集合
static vector<char> totalChars;//对应ASCII码，记录可以输入的字符
static set<int>NFAFinalStates; //里面存放NFA所有节点为终态的点的编号
static int intNFAFinalStates[MAX];//用来记录NFA的终态 -1是非终态   >=0是终态

//static int mDFA[MAX];          //用来存储结点合并 比如mDFA[2]=0,表示状态2可以和状态0合并，初始化为-1
static void printNFAtable();
void initNFAFinalState()//
{
	for (auto& a : NFANodes)
	{
		if (a.id != -1 && a.convert1 == NULL && a.convert2 == NULL)
		{
			intNFAFinalStates[a.id] = a.id;
			NFAFinalStates.insert(a.id);
		}

	}

}

static void initmDFA() {
	for (int i = 0; i < MAX; i++)
	{
		//mDFA[i] = -1;
		isDFAFinal[i] = -1;
		intNFAFinalStates[i] = -1;
		ismDFAFinal[i] = -1;
	}
	initNFAFinalState();
}
static void initTotalChars() {
	for (int i = 1; i < 127; ++i) {
	
			//记录下ASCII码
			totalChars.push_back(i);
		
	}
}
static NFANode* getNodeById(int id) {
	return &NFANodes[id]; // 假设 id 就是节点在 vector 中的索引
}
static NFANode initNFA3()
{
	NFANode node0, node1, node2, node3, node4;
	node0.id = 0; node0.input1 = 'a'; node0.input2 = 'b';
	node1.id = 1; node1.input1 = 'a'; node1.input2 = 'b';
	node2.id = 2; node2.input1 = 'a'; node2.input2 = 'b';
	node3.id = 3;
	node4.id = 4;
	node0.convert1 = getNodeById(1);
	node0.convert2 = getNodeById(2);
	node1.convert1 = getNodeById(3);
	node1.convert2 = getNodeById(4);
	node2.convert1 = getNodeById(3);
	node2.convert2 = getNodeById(4);

	NFANodes[0] = node0;
	NFANodes[1] = node1;
	NFANodes[2] = node2;
	NFANodes[3] = node3;
	NFANodes[4] = node4;

	NFAFinalStates.insert(3);
	NFAFinalStates.insert(4);
	return node0;
}
static NFANode initNFA2()
{
	//第二组测试用例
  // 创建NFA
	NFANode node0, node1, node2, node3, node4, node5, node6, node7, node8, node9;

	// 初始化各个状态
	node0.id = 0; node0.input1 = node0.input2 = '\0'; // 起始状态，仅通过epsilon转移
	node1.id = 1; node1.input1 = node1.input2 = '\0'; // 仅通过epsilon转移
	node2.id = 2; node2.input1 = 'a';
	node3.id = 3; node3.input1 = '\0'; // 仅通过epsilon转移
	node4.id = 4; node4.input1 = 'b';
	node5.id = 5; node5.input1 = '\0'; // 仅通过epsilon转移
	node6.id = 6; node6.input1 = node6.input2 = '\0'; // 仅通过epsilon转移
	node7.id = 7; node7.input1 = 'a';
	node8.id = 8; node8.input1 = 'b';
	node9.id = 9; node9.input1 = node9.input2 = NULL; // 终点状态，没有出度

	NFAFinalStates.insert(9);
	// 添加到nfanodes中
	NFANodes[0] = node0;
	NFANodes[1] = node1;
	NFANodes[2] = node2;
	NFANodes[3] = node3;
	NFANodes[4] = node4;
	NFANodes[5] = node5;
	NFANodes[6] = node6;
	NFANodes[7] = node7;
	NFANodes[8] = node8;
	NFANodes[9] = node9;

	// 设置next1和next2
	NFANodes[0].convert1 = getNodeById(1);
	NFANodes[0].convert2 = getNodeById(7);
	NFANodes[1].convert1 = getNodeById(2);
	NFANodes[1].convert2 = getNodeById(4);
	NFANodes[2].convert1 = getNodeById(3);
	NFANodes[4].convert1 = getNodeById(5);
	NFANodes[3].convert1 = getNodeById(6);
	NFANodes[5].convert1 = getNodeById(6);
	NFANodes[6].convert1 = getNodeById(1);
	NFANodes[6].convert2 = getNodeById(7);
	NFANodes[7].convert1 = getNodeById(8);
	NFANodes[8].convert1 = getNodeById(9);

	return node0;
}


static void printNFAtable()
{
	std::cout << "Non-empty elements in epolisonSet:" << std::endl;
	for (int i = 0; i < MAX; ++i) {
		if (!epolisonSet[i].empty()) {
			std::cout << "epolisonSet[" << i << "]: ";
			for (const auto& element : epolisonSet[i]) {
				std::cout << element << " ";
			}
			std::cout << std::endl;
		}
	}

	// 打印 NFAStateTable 中不为空的元素
	std::cout << "Non-empty elements in NFAStateTable:" << std::endl;
	for (int i = 0; i < MAX; ++i) {
		for (const auto& kvp : NFAStateTable[i]) {
			if (!kvp.second.empty()) {
				std::cout << "NFAStateTable[" << i << "]['" << kvp.first << "']: ";
				for (const auto& element : kvp.second) {
					std::cout << element << " ";
				}
				std::cout << std::endl;
			}
		}
	}

}
//求一组状态的闭包
//输入元素set1为一系列的节点编号

//获得某一个点开始的闭包
static void getStateClosure(set<int>& set1) {

	queue<int> que;
	for (int id : set1) {
		que.push(id);
	}
	//使用BFS搜索 从一个编号出去经过epolison所有可到达的节点
	while (!que.empty()) {
		int id = que.front();
		que.pop();
		NFANode p = NFANodes[id];
		 //先判断第一条边，如果指向的是epolison，并且在set集合里没有这个编号，那么将该编号添加到set集合里
		if (p.input1 == '\0' && p.convert1 != NULL && set1.find(p.convert1->id) == set1.end())
		{
			set1.insert(p.convert1->id);
			que.push(p.convert1->id);
		}
		
		//再判断第二条边，如果指向的是epolison，并且在set集合里没有这个编号，那么将该编号添加到set集合里
		if (p.input2 == '\0' && p.convert2 != NULL && set1.find(p.convert2->id) == set1.end()) {
			set1.insert(p.convert2->id);
			que.push(p.convert2->id);
		}
	}
}
//生成NFA-->DFA的转化图
static int NFAtoDFATable(NFANode* startNode) {
	//先将起始节点放入状态集合中
	epolisonSet[0].insert(startNode->id);
	//求出epil（0）的结果，放入state_set中
	getStateClosure(epolisonSet[0]);
	//记录共有多少种状态DFA
	int currentState = 0;
	//当没有新状态时，停止循环
	for (int i = 0; i <= currentState; ++i) {
		//初始状态
		auto& currentEpolisonSet = epolisonSet[i];
		//目标转移状态
		auto& currentTable = NFAStateTable[i];
		//遍历可使用的ASCII码
		for (char c : totalChars)
		{
			//这里的table表示NFAStateTable[i]在接受字符c后能进入的状态集合
			auto& table = currentTable[c];
			for (int id : currentEpolisonSet)
			{
				//获取当前id对应的NFA节点
				auto& node = NFANodes[id];
				//当输入字符c时，能够跳转到新状态，记录下新状态的id
				//map中不存在相同节点
				if (node.input1 == c) {
					table.insert(node.convert1->id);
				}
				if (node.input2 == c) {
					table.insert(node.convert2->id);
				}
			}
			//寻找这组目标状态的等价类
			getStateClosure(table);

			if (!table.empty()) {
				//记录是否为新状态
				bool newOne = true;
				//寻找是否已经有当前集合
				for (int j = 0; j <= currentState; ++j) {
					if (table == epolisonSet[j]) {
						newOne = false;
						break;
					}
				}
				//新状态放入状态表中
				if (newOne) {
					currentState++;
					epolisonSet[currentState] = table;
					vector<int> intersection;
					set_intersection(table.begin(), table.end(), NFAFinalStates.begin(), NFAFinalStates.end(), back_inserter(intersection));   //将闭包与NFA的终态map取交集 
					if (!intersection.empty()) { //如果不为空，那么说明该DFA状态是终态
					isDFAFinal[currentState] = intNFAFinalStates[intersection[0]];
					}
				}
			}
		}

	}
	return currentState + 1;
}
//构建DFA图
//变量：stateNum为DFA状态数量
static void DFAgeneration(int stateNum) {
	for (int i = 0; i < stateNum; ++i) {
		DFANodes[i].id = i;
		DFANodes[i].istrue = 1;
		map<char, set<int> > tempTable = NFAStateTable[i];
		for (char c : totalChars)
		{
			//判断是否为通路
			if (tempTable[c].empty()) continue;
			//由于是通路，可以进行状态转移，获取当前字符对应的状态集合
			auto& totalState = tempTable[c];      //totalState为当前状态i在输入c字符下能够转移到的状态
			for (int j = 0; j < stateNum; ++j) {
				//寻找到当前节点对应的节点，进行链接
				if (totalState == epolisonSet[j]) {
					DFANodes[i].transfer[c] = &DFANodes[j];
					break;
				}
			}
		}

	}
}


static void printNFA()
{
	// Print NFA
	cout << "NFA:\n";

	for (const auto& node : NFANodes) {
		//cout <<"ID:"<< node.id << endl;
		if (node.id >= 0)
		{
			if (node.input1 != NULL && node.input1 != '\0') {
				cout << "State " << node.id << ": On input '" << node.input1 << "', transition to state " << node.convert1->id << ".\n";
			}
			else if (node.input1 == '\0' && node.convert1 != NULL) {
				cout << "State " << node.id << ": On epsilon transition to state " << node.convert1->id << ".\n";
			}

			if (node.input2 != NULL && node.input2 != '\0') {
				cout << "State " << node.id << ": On input '" << node.input2 << "', transition to state " << node.convert2->id << ".\n";
			}
			else if (node.input2 == '\0' && node.convert2 != NULL) {
				cout << "State " << node.id << ": On epsilon transition to state " << node.convert2->id << ".\n";
			}

			if (node.input1 == NULL && node.input2 == NULL && node.convert1 == NULL && node.convert2 == NULL) {
				cout << "State " << node.id << ": Final state.\n";
			}
		}
	}
}
static void printNFA(NFANode* startNode, set<int>& visitedNodes) {
	// Check if the node has already been visited
	if (visitedNodes.find(startNode->id) != visitedNodes.end()) {
		return;
	}
	// Add the node to the set of visited nodes
	visitedNodes.insert(startNode->id);
	// Check the first transition
	if (startNode->input1 != NULL && startNode->input1 != '\0') {
		cout << "State " << startNode->id << ": On input '" << startNode->input1 << "', transition to state ";
		if (startNode->convert1 != NULL) {
			cout << startNode->convert1->id;
			printNFA(startNode->convert1, visitedNodes);
		}
		//cout << ".\n";
	}
	else if (startNode->input1 == '\0' && startNode->convert1 != NULL) {
		cout << "State " << startNode->id << ": On epsilon transition to state " << startNode->convert1->id << ".\n";
		printNFA(startNode->convert1, visitedNodes);
	}

	// Check the second transition
	if (startNode->input2 != NULL && startNode->input2 != '\0') {
		cout << "State " << startNode->id << ": On input '" << startNode->input2 << "', transition to state ";
		if (startNode->convert2 != NULL) {
			cout << startNode->convert2->id;
			printNFA(startNode->convert2, visitedNodes);
		}
		cout << ".\n";
	}
	else if (startNode->input2 == '\0' && startNode->convert2 != NULL) {
		cout << "State " << startNode->id << ": On epsilon transition to state " << startNode->convert2->id << ".\n";
		printNFA(startNode->convert2, visitedNodes);
	}

	// Check if the node is a final state
	if (startNode->input1 == NULL && startNode->input2 == NULL && startNode->convert1 == NULL && startNode->convert2 == NULL) {
		cout << "State " << startNode->id << ": Final state.\n";
	}
}

// Helper function to initiate the set
static void printNFA(NFANode* startNode) {
	set<int> visitedNodes;
	printNFA(startNode, visitedNodes);
}


static void printDFA()
{
	for (int i = 0; i < MAX; ++i) {
		if (DFANodes[i].istrue == 1)
		{  // 或其他你设定的“已经初始化”的条件
			//cout << "DFANode " << i << ": " << endl;
			cout << "ID: " << DFANodes[i].id << endl;
			cout << "transfer: " << endl;
			for (const auto& pair : DFANodes[i].transfer) {
				//if (mDFA[pair.second->id] == -1)
				{
					cout << "    " << pair.first << ": " << pair.second->id << endl;  // 假设这里 pair.second 指向的 DFANode 已经初始化
				}
				//else
				//{

				//	cout << "    " << pair.first << ": " << mDFA[pair.second->id] << endl;  // 假设这里 pair.second 指向的 DFANode 已经初始化
				//}
			}
			cout << "------------------" << endl;
		}

	}

}
//static void initDFAStateTable()
//{
//	for (int i = 0; i < MAX; i++)
//	{
//
//
//		if (DFANodes[i].istrue == 1)
//		{
//
//			for (char c : totalChars)
//			{
//
//				if (DFANodes[i].transfer.find(c) != DFANodes[i].transfer.end())
//				{
//
//					int tempid = DFANodes[i].transfer[c]->id;
//					DFAStateTable[i][c].insert(tempid);
//				}
//			}
//
//		}
//	}
//}








//
//int main()
//{
//	NFANode  node0 = initNFA3();
//	//printNFA();
//	initNFAFinalState();
//	initmDFA();
//	initTotalChars();
//	//NFANode node0 = initNFA2();
//
//	//NFA.debugNFA(node0);
//	int state = NFAtoDFATable(&node0);
//
//	//cout << "这里输出状态" << state << endl;
//	DFAgeneration(state);
//	//printDFA();
//	initDFAStateTable();
//
//
//
//
//	DFAMinimizer.init(state);
//	DFAMinimizer.divideDFANodes();
//	int nodes;
//	nodes = DFAMinimizer.generatemDFA();
//	cout << "经过mDFA后一共有" << nodes << "个节点" << endl;
//	//for (int i = 0; i < 10; i++)
//	//{
//	//	cout << isDFAFinal[i] << " ";
//	//}cout << endl;
//	//for (int i = 0; i < 10; i++)
//	//{
//	//	cout << ismDFAFinal[i] << " ";
//	//}cout << endl;
//	//DFAMinimizer.show();
//	for (int i = 0; i < nodes; ++i) {
//		printf("%3d : ", i);
//		//printf(" [ %4d ] ", miDFA[i].pos);
//		printf(" [ %4d ] ", ismDFAFinal[i]);
//		for (auto& p_ : miDFA[i].transfer) {
//			printf(" < %c , %3d >", p_.first, p_.second->id);
//		}
//		printf("\n\n");
//	}
//	//printminiDFA();
//}
