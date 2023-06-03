
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
	map<char, DFANode*> transfer;//�����ַ� ָ�����һ���ڵ�
};
DFANode DFANodes[10000];  //DFA�ڵ�
DFANode miDFA[10000];      //��С��DFA��Ľڵ�
static int isDFAFinal[MAX];   //������¼ÿ��DFA״̬�Ƿ�����̬����ʼ��Ϊ-1  ֵ>=0˵������̬
static int ismDFAFinal[MAX];   //������¼ÿ��mDFA״̬�Ƿ�����̬����ʼ��Ϊ-1  ֵ>=0˵������̬
vector<set<int> > DFAStates;//��¼�򻯺��DFA�ڵ�
int DFATransitions[10000];//index��DFA�Ľڵ��ţ���Ӧ��ֵΪmDFA�ı��

//��ʼ����stateCntΪDFA״̬����	
void initmDFA(int stateCnt) {
		map<int,set<int> > initialSets;//int��Ӧ�ս�״̬�ı�ţ�set<int>��Ӧ��ǰ״̬��Ӧ��DFA�ڵ���
		//��¼�������ս�״̬���ô洢��map�е�key������l�ļ��е�˳������¼�ڵ�
		for (int i = 0; i < stateCnt; ++i) {
			int isfinal = isDFAFinal[i];
			//��¼ÿ���ս�״̬��Ӧ��DFA�ڵ���
			initialSets[isfinal].insert(i);
		}
		for (auto& p : initialSets) {
			//��DFA�ս�״̬����DFAStates
			DFAStates.push_back(p.second);
			//��¼ÿ��DFA�ڵ�ı�ź����Ӧ��mDFA�ı��
			for (int idx : p.second) {
				DFATransitions[idx] = DFAStates.size() - 1;
			}
		}
	}

// ���������Ŀ���ǽ����Ƶ�״̬���ֵ�ͬһ�飬��ʵ��״̬�ļ�
void divideDFANodes() {
		int currentSize; // ���ڼ�¼ÿ��ѭ����ʼʱDFA״̬���ϵĴ�С
		// ������һ��ѭ����ֻ����DFA״̬���ϵĴ�С�����ı�ʱ�Ż����ִ��
		do {
			// ��¼����ѭ����ʼʱDFA״̬���ϵĴ�С
			currentSize = DFAStates.size();

			// ��ÿһ��״̬���Ͻ��д���
			for (int i = 0; i < DFAStates.size(); ++i) {
				auto& currentSet = DFAStates[i]; // ��ǰ�����״̬����

				// divideSet���ڴ洢��Ҫ�ӵ�ǰ״̬�����а����ȥ��״̬
				set<int> divideSet;

				// first�ǵ�ǰ״̬�����е�һ��״̬��transitionTable�����״̬��ת�Ʊ�
				int first = *(currentSet.begin());
				auto& transitionTable = DFANodes[first].transfer;

				// ��鵱ǰ״̬�����е�ÿһ��״̬
				for (int e : currentSet) {
					auto& table = DFANodes[e].transfer; // e��ת�Ʊ�

					bool notSame = false; // ���e��ת�Ʊ��Ƿ���transitionTable��ͬ

					// �������״̬��ת�Ʊ��С��ͬ������Ҫ��������Ƿ�һ��
					if (transitionTable.size() == table.size()) {
						for (auto p : transitionTable) {
							auto it = table.find(p.first);
							// �����e��ת�Ʊ����Ҳ�����transitionTable�е�ĳ�����룬���߶�Ӧ��ת��״̬�ڼ򻯺��DFA�е�λ�ò�ͬ����˵������״̬����ͬ
							if (it == table.end())
							{
								notSame = true;
								break;
							}
							else
							{//�ж�ָ��Ľڵ��Ƿ�λ��ͬһ������
								if (DFATransitions[p.second->id] != DFATransitions[it->second->id]) {
									notSame = true;
									break;
								}
							}
						}
					}
					else  {
						// �������״̬��ת�Ʊ��С��ͬ����ض�����ͬ
						notSame = true;
					}

					// ���e��first����ͬ����e����divideSet
					if (notSame) divideSet.insert(e);
				}
				// ���������Ҫ�����״̬
				if (!divideSet.empty()) {
					// ��divideSet��Ϊ�µ�״̬���ϼ���DFAStates
					DFAStates.push_back(divideSet);
					int newindex = DFAStates.size() - 1; // ��״̬������DFAStates�е�λ��
					// ��ÿһ����Ҫ�����״̬
					for (int e : divideSet) {
						DFAStates[i].erase(e); // ��ԭ״̬�������Ƴ�
						DFATransitions[e] = newindex; // �������ڼ򻯺��DFA�е�λ��
					}
				}
			}
			// �����һ��ѭ���У�DFA״̬���ϵĴ�Сû�з����ı䣬��˵�����е�״̬���Ѿ������ٱ����룬���ǽ���ѭ��
		} while (DFAStates.size() != currentSize);
		cout << "DFA�Ĵ�С" << DFAStates.size() << endl;
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
		// ���� DFA ״̬�� DFAStates �е�ÿһ��״̬��
		for (int i = 0; i < DFAStates.size(); ++i) {
			// ����mDFA ״̬ miDFA �� id Ϊ��ǰ�������±� i
			miDFA[i].id = i;

			// ѡȡ��ǰ������ DFA ״̬���еĵ�һ��Ԫ��
			auto e = *(DFAStates[i].begin());
			// ���� isDFAFinal ���������� ismDFAFinal ����
			ismDFAFinal[i] = isDFAFinal[e];
			// ����ԭ DFA �ڵ������ӳ���ϵ
	
			for (auto& p : DFANodes[e].transfer) {
				// ��ȡӳ���ϵ�е��ַ�
				char c = p.first;

				// ����ӳ���ϵ�е�Ŀ��״̬ id�����Ҷ�Ӧ��mDFA ״̬�±�
				int idx = DFATransitions[p.second->id];

				// ������� DFA ״̬��ӳ���ϵ�����ɵ�ǰ�ַ� c ӳ�䵽�±�Ϊ idx �ļ�� DFA ״̬
				miDFA[i].transfer[c] = &miDFA[idx];
			}
		}

		// ����mDFA ��״̬��������ԭ DFA ״̬��������
		return DFAStates.size();
	}

static set<int> epolisonSet[MAX];//��¼���бհ����ϣ������ߵĲ���  ��epolisonSet[1]��ʾ1���Ե�������бհ�
static map<char, set<int> > NFAStateTable[MAX];
//��¼����Ŀ��״̬��charָ������ַ���setָת�ƺ�ıհ����  NFAStatetable[1]['a']��ʾ1״̬�ڽ���a�ַ�����Ե���ıհ�����
static vector<char> totalChars;//��ӦASCII�룬��¼����������ַ�
static set<int>NFAFinalStates; //������NFA���нڵ�Ϊ��̬�ĵ�ı��
static int intNFAFinalStates[MAX];//������¼NFA����̬ -1�Ƿ���̬   >=0����̬

//static int mDFA[MAX];          //�����洢���ϲ� ����mDFA[2]=0,��ʾ״̬2���Ժ�״̬0�ϲ�����ʼ��Ϊ-1
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
	
			//��¼��ASCII��
			totalChars.push_back(i);
		
	}
}
static NFANode* getNodeById(int id) {
	return &NFANodes[id]; // ���� id ���ǽڵ��� vector �е�����
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
	//�ڶ����������
  // ����NFA
	NFANode node0, node1, node2, node3, node4, node5, node6, node7, node8, node9;

	// ��ʼ������״̬
	node0.id = 0; node0.input1 = node0.input2 = '\0'; // ��ʼ״̬����ͨ��epsilonת��
	node1.id = 1; node1.input1 = node1.input2 = '\0'; // ��ͨ��epsilonת��
	node2.id = 2; node2.input1 = 'a';
	node3.id = 3; node3.input1 = '\0'; // ��ͨ��epsilonת��
	node4.id = 4; node4.input1 = 'b';
	node5.id = 5; node5.input1 = '\0'; // ��ͨ��epsilonת��
	node6.id = 6; node6.input1 = node6.input2 = '\0'; // ��ͨ��epsilonת��
	node7.id = 7; node7.input1 = 'a';
	node8.id = 8; node8.input1 = 'b';
	node9.id = 9; node9.input1 = node9.input2 = NULL; // �յ�״̬��û�г���

	NFAFinalStates.insert(9);
	// ��ӵ�nfanodes��
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

	// ����next1��next2
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

	// ��ӡ NFAStateTable �в�Ϊ�յ�Ԫ��
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
//��һ��״̬�ıհ�
//����Ԫ��set1Ϊһϵ�еĽڵ���

//���ĳһ���㿪ʼ�ıհ�
static void getStateClosure(set<int>& set1) {

	queue<int> que;
	for (int id : set1) {
		que.push(id);
	}
	//ʹ��BFS���� ��һ����ų�ȥ����epolison���пɵ���Ľڵ�
	while (!que.empty()) {
		int id = que.front();
		que.pop();
		NFANode p = NFANodes[id];
		 //���жϵ�һ���ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
		if (p.input1 == '\0' && p.convert1 != NULL && set1.find(p.convert1->id) == set1.end())
		{
			set1.insert(p.convert1->id);
			que.push(p.convert1->id);
		}
		
		//���жϵڶ����ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
		if (p.input2 == '\0' && p.convert2 != NULL && set1.find(p.convert2->id) == set1.end()) {
			set1.insert(p.convert2->id);
			que.push(p.convert2->id);
		}
	}
}
//����NFA-->DFA��ת��ͼ
static int NFAtoDFATable(NFANode* startNode) {
	//�Ƚ���ʼ�ڵ����״̬������
	epolisonSet[0].insert(startNode->id);
	//���epil��0���Ľ��������state_set��
	getStateClosure(epolisonSet[0]);
	//��¼���ж�����״̬DFA
	int currentState = 0;
	//��û����״̬ʱ��ֹͣѭ��
	for (int i = 0; i <= currentState; ++i) {
		//��ʼ״̬
		auto& currentEpolisonSet = epolisonSet[i];
		//Ŀ��ת��״̬
		auto& currentTable = NFAStateTable[i];
		//������ʹ�õ�ASCII��
		for (char c : totalChars)
		{
			//�����table��ʾNFAStateTable[i]�ڽ����ַ�c���ܽ����״̬����
			auto& table = currentTable[c];
			for (int id : currentEpolisonSet)
			{
				//��ȡ��ǰid��Ӧ��NFA�ڵ�
				auto& node = NFANodes[id];
				//�������ַ�cʱ���ܹ���ת����״̬����¼����״̬��id
				//map�в�������ͬ�ڵ�
				if (node.input1 == c) {
					table.insert(node.convert1->id);
				}
				if (node.input2 == c) {
					table.insert(node.convert2->id);
				}
			}
			//Ѱ������Ŀ��״̬�ĵȼ���
			getStateClosure(table);

			if (!table.empty()) {
				//��¼�Ƿ�Ϊ��״̬
				bool newOne = true;
				//Ѱ���Ƿ��Ѿ��е�ǰ����
				for (int j = 0; j <= currentState; ++j) {
					if (table == epolisonSet[j]) {
						newOne = false;
						break;
					}
				}
				//��״̬����״̬����
				if (newOne) {
					currentState++;
					epolisonSet[currentState] = table;
					vector<int> intersection;
					set_intersection(table.begin(), table.end(), NFAFinalStates.begin(), NFAFinalStates.end(), back_inserter(intersection));   //���հ���NFA����̬mapȡ���� 
					if (!intersection.empty()) { //�����Ϊ�գ���ô˵����DFA״̬����̬
					isDFAFinal[currentState] = intNFAFinalStates[intersection[0]];
					}
				}
			}
		}

	}
	return currentState + 1;
}
//����DFAͼ
//������stateNumΪDFA״̬����
static void DFAgeneration(int stateNum) {
	for (int i = 0; i < stateNum; ++i) {
		DFANodes[i].id = i;
		DFANodes[i].istrue = 1;
		map<char, set<int> > tempTable = NFAStateTable[i];
		for (char c : totalChars)
		{
			//�ж��Ƿ�Ϊͨ·
			if (tempTable[c].empty()) continue;
			//������ͨ·�����Խ���״̬ת�ƣ���ȡ��ǰ�ַ���Ӧ��״̬����
			auto& totalState = tempTable[c];      //totalStateΪ��ǰ״̬i������c�ַ����ܹ�ת�Ƶ���״̬
			for (int j = 0; j < stateNum; ++j) {
				//Ѱ�ҵ���ǰ�ڵ��Ӧ�Ľڵ㣬��������
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
		{  // ���������趨�ġ��Ѿ���ʼ����������
			//cout << "DFANode " << i << ": " << endl;
			cout << "ID: " << DFANodes[i].id << endl;
			cout << "transfer: " << endl;
			for (const auto& pair : DFANodes[i].transfer) {
				//if (mDFA[pair.second->id] == -1)
				{
					cout << "    " << pair.first << ": " << pair.second->id << endl;  // �������� pair.second ָ��� DFANode �Ѿ���ʼ��
				}
				//else
				//{

				//	cout << "    " << pair.first << ": " << mDFA[pair.second->id] << endl;  // �������� pair.second ָ��� DFANode �Ѿ���ʼ��
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
//	//cout << "�������״̬" << state << endl;
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
//	cout << "����mDFA��һ����" << nodes << "���ڵ�" << endl;
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
