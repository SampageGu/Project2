#include <map>
#include <set>
#include <queue>
#include <vector>
#include<stack>
#include <iostream>
#include"NFA.h"
using namespace std;

#define MAX 65536
//struct NFANode {
//	int id = -1;  //idΪ����ʾû�б���ʼ��
//	char input1 = NULL;
//	NFANode* convert1 = NULL;
//	char input2 = NULL;
//	NFANode* convert2 = NULL;
//};
struct DFANode {
	bool istrue = 0;
	int id = -1;
	int pos = 0;
	map<char, DFANode*> ptrs;//ת�Ʒ���
};
static DFANode DFANodes[MAX];
static set<int> epolisonSet[MAX];//��¼���бհ����ϣ������ߵĲ���  ��epolisonSet[1]��ʾ1���Ե�������бհ�
static map<char, set<int> > NFAStateTable[MAX];
//��¼����Ŀ��״̬��charָ������ַ���setָת�ƺ�ıհ����  Statetable[1]['a']��ʾ1״̬�ڽ���a�ַ�����Ե���ıհ�
static map<char, set<int>>DFAStateTable[MAX];
static vector<char> allowed_chars;//��ӦASCII�룬��¼����������ַ�
//static NFANode NFANodes[MAX]; // �洢���е� NFANode ����
static void printNFAtable();
static int mDFA[MAX];          //�����洢���ϲ� ����mDFA[2]=0,��ʾ״̬2���Ժ�״̬0�ϲ�����ʼ��Ϊ-1
static set<int>NFAFinalState;//������¼NFA����̬
static bool isDFAFinal[MAX];   //������¼ÿ��DFA״̬�Ƿ�����̬����ʼ��Ϊ0
static void initmDFA() {
	for (int i = 0; i < MAX; i++)
	{
		mDFA[i] = -1;
		isDFAFinal[i] = 0;
		//isNFAFinal[i] = 0;
	}

}
static int REpos[65536];//��¼�ڵ��Ӧ��ִ�����
static void initallow_chars() {
	for (int i = 9; i < 127; ++i) {
		if (true/*i != '#'*/) {
			//��¼��ASCII��
			allowed_chars.push_back(i);
		}
	}
	memset(REpos, 0, sizeof REpos);
}
//static NFANode* getNodeById(int id) {
//	return &NFANodes[id]; // ���� id ���ǽڵ��� vector �е�����
//}
// 
//static NFANode  initNFA1()
//{
//	//�ڶ����������
//  // ����NFA
//	NFANode node0, node1, node2, node3, node4, node5;
//	node0.id = 0; node0.input1 = 'a';
//	node1.id = 1; node1.input1 = 'b'; node1.input2 = '\0';
//	node2.id = 2; node2.input1 = 'c'; node2.input2 = 'd';
//	node3.id = 3; node3.input1 = '\0';
//	node4.id = 4; node4.input1 = 'e'; node4.input2 = '\0';
//	node5.id = 5; node5.input1 = node5.input2 = NULL;
//
//	// ��ӵ�nodes��
//	NFANodes[0] = node0;
//	NFANodes[1] = node1;
//	NFANodes[2] = node2;
//	NFANodes[3] = node3;
//	NFANodes[4] = node4;
//	NFANodes[5] = node5;
//
//	// ����next1��next2
//	NFANodes[0].convert1 = getNodeById(1);
//	NFANodes[1].convert1 = getNodeById(2);
//	NFANodes[2].convert1 = getNodeById(3); NFANodes[2].convert2 = getNodeById(4);
//	NFANodes[4].convert1 = getNodeById(5);
//
//	// ����epsilon��
//	NFANodes[3].convert1 = getNodeById(4);
//	NFANodes[1].convert2 = getNodeById(3);
//	NFANodes[4].convert2 = getNodeById(2);
//	return node0;
//}
//static NFANode initNFA2()
//{
//	//�ڶ����������
//  // ����NFA
//	NFANode node0, node1, node2, node3, node4, node5, node6, node7, node8, node9;
//
//	// ��ʼ������״̬
//	node0.id = 0; node0.input1 = node0.input2 = '\0'; // ��ʼ״̬����ͨ��epsilonת��
//	node1.id = 1; node1.input1 = node1.input2 = '\0'; // ��ͨ��epsilonת��
//	node2.id = 2; node2.input1 = 'a';
//	node3.id = 3; node3.input1 = '\0'; // ��ͨ��epsilonת��
//	node4.id = 4; node4.input1 = 'b';
//	node5.id = 5; node5.input1 = '\0'; // ��ͨ��epsilonת��
//	node6.id = 6; node6.input1 = node6.input2 = '\0'; // ��ͨ��epsilonת��
//	node7.id = 7; node7.input1 = 'a';
//	node8.id = 8; node8.input1 = 'b';
//	node9.id = 9; node9.input1 = node9.input2 = NULL; // �յ�״̬��û�г���
//
//	NFAFinalState.insert(9);
//	// ��ӵ�nfanodes��
//	NFANodes[0] = node0;
//	NFANodes[1] = node1;
//	NFANodes[2] = node2;
//	NFANodes[3] = node3;
//	NFANodes[4] = node4;
//	NFANodes[5] = node5;
//	NFANodes[6] = node6;
//	NFANodes[7] = node7;
//	NFANodes[8] = node8;
//	NFANodes[9] = node9;
//
//	// ����next1��next2
//	NFANodes[0].convert1 = getNodeById(1);
//	NFANodes[0].convert2 = getNodeById(7);
//	NFANodes[1].convert1 = getNodeById(2);
//	NFANodes[1].convert2 = getNodeById(4);
//	NFANodes[2].convert1 = getNodeById(3);
//	NFANodes[4].convert1 = getNodeById(5);
//	NFANodes[3].convert1 = getNodeById(6);
//	NFANodes[5].convert1 = getNodeById(6);
//	NFANodes[6].convert1 = getNodeById(1);
//	NFANodes[6].convert2 = getNodeById(7);
//	NFANodes[7].convert1 = getNodeById(8);
//	NFANodes[8].convert1 = getNodeById(9);
//
//	return node0;
//}



//��һ��״̬�ıհ�
//����Ԫ��set1Ϊһϵ�еĽڵ���
static void getStateClosure(set<int>& set1) {

	queue<int> que;
	//cout << "!!!" << endl;
	for (int id : set1) {
		que.push(id);
		cout <<"!!!!!!!!!!"<< id << endl;
	}
	//ʹ��BFS���� ��һ����ų�ȥ����epolison���пɵ���Ľڵ�
	while (!que.empty()) {
		
		int id = que.front();
		que.pop();
		cout << "enter" <<id<< endl;
		NFANode& p = NFANodes[id];
		 /* if (p.input1 != '\0') continue;*/
		  //�ж��Ƿ�Ϊ��״̬��ͨ��find��������set�����޸�Ԫ�أ��򷵻�set1.end()
		  //���жϵ�һ���ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
		if (p.input1 == '\0' && p.convert1!=NULL&&set1.find(p.convert1->id) == set1.end())
		{
			cout << "!" << endl;
			set1.insert(p.convert1->id);
			que.push(p.convert1->id);
		}
		/*if (p.input2 != '\0') continue;*/
		//���жϵڶ����ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
		if (p.input2 == '\0' && p.convert2 != NULL && set1.find(p.convert2->id) == set1.end()) {
			cout << "!" << endl;
			set1.insert(p.convert2->id);
			que.push(p.convert2->id);
		}
	}
	 //if (!set1.empty())
	 //{
		// cout << "�������set1:";
		// for (const auto& element : set1) {
		//	 cout << element << " ";
		// }
		// cout << endl;
	 //}
}

static void getStateClosure(set<NFANode*>& set1) {

	queue<NFANode*> que;
	for (NFANode* node : set1) {
		que.push(node);
	}

	//ʹ��BFS���� ��һ����ų�ȥ����epolison���пɵ���Ľڵ�
	while (!que.empty()) {
		NFANode* p = que.front();
		que.pop();

		//�ж��Ƿ�Ϊ��״̬��ͨ��find��������set�����޸�Ԫ�أ��򷵻�set1.end()
		//���жϵ�һ���ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
		if (p->input1 == '\0' && p->convert1 && set1.find(p->convert1) == set1.end())
		{
			//cout << "!" << endl;
			set1.insert(p->convert1);
			que.push(p->convert1);
		}

		//���жϵڶ����ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
		if (p->input2 == '\0' && p->convert2 != NULL && set1.find(p->convert2) == set1.end()) {
			//cout << "!!" << endl;
			set1.insert(p->convert2);
			que.push(p->convert2);
		}
	}
}

static void initClosure(NFANode* startNode) {
	// ��ʼ״̬һ���������ת״̬
	//�Ƚ���ʼ�ڵ����״̬������
	cout << startNode->id << "�������׸�id" << endl;
	epolisonSet[startNode->id].insert(startNode->id);
	//���epil��0���Ľ��������state_set��

	getStateClosure(epolisonSet[startNode->id]);
	/*set<NFANode*>setnode;
	setnode.insert(startNode);
	getStateClosure(setnode);*/


	
}
static int NFAtoDFATable(NFANode* startNode) {
	
	
	initClosure(startNode);

	//��¼���ж�����״̬DFA
	int last_state_idx = 0;
	//��û����״̬ʱ��ֹͣѭ��
	for (int i = 0; i <= last_state_idx; ++i) {
		//��ʼ״̬
		auto& srcState = epolisonSet[i];
		//Ŀ��ת��״̬
		auto& tgtTable = NFAStateTable[i];
		//������ʹ�õ�ASCII��
		for (char c : allowed_chars)
		{
			//��ȡ�ܹ�����Ŀ���ַ���״̬
			auto& table = tgtTable[c];
			for (int id : srcState)
			{
				//��ȡ��ǰid��Ӧ��NFA�ڵ�
				auto& node = NFANodes[id];
				//�������ַ�cʱ���ܹ���ת����״̬����¼����״̬��id
				//map�в�������ͬ�ڵ�
				if (node.input1 == c) {
					//  cout << c << "\t"<<id<<"\t input1 match successfully" << endl;
					table.insert(node.convert1->id);
				}
				if (node.input2 == c) {
					// cout << c << "\t" << id << "\t input2 match successfully" << endl;
					table.insert(node.convert2->id);
				}
			}
			//Ѱ������Ŀ��״̬�ĵȼ���
			getStateClosure(table);

			if (!table.empty()) {
				//��¼�Ƿ�Ϊ��״̬
				bool newOne = true;
				//Ѱ���Ƿ��Ѿ��е�ǰ����
				for (int j = 0; j <= last_state_idx; ++j) {
					if (table == epolisonSet[j]) {
						newOne = false;
						break;
					}
				}
				//��״̬����״̬����
				if (newOne) {
					cout << "find new state" << endl;
					last_state_idx++;
					epolisonSet[last_state_idx] = table;
					vector<int> intersection;
					set_intersection(table.begin(), table.end(), NFAFinalState.begin(), NFAFinalState.end(), back_inserter(intersection));   //���հ���NFA����̬mapȡ���� 
					if (!intersection.empty()) { //�����Ϊ�գ���ô˵����DFA״̬����̬
						cout << "�ҵ��ˣ�����������������" << last_state_idx << endl;
						isDFAFinal[last_state_idx] = 1;
					}
				}
			}
		}

	}
	cout << "printN" << endl;
	printNFAtable();
	return last_state_idx + 1;
}
static void printNFAtable()
{
	std::cout << "Non-empty elements in epolisonSet:" << std::endl;
	for (int i = 0; i < 10; ++i) {
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
	for (int i = 0; i < 10; ++i) {
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
//����DFAͼ
//������stateCntΪDFA״̬����
static void constructDFA(int stateNum) {
	for (int i = 0; i < stateNum; ++i) {
		DFANodes[i].id = i;
		DFANodes[i].istrue = 1;
		DFANodes[i].pos = 0;
		//������ǰ�����е�����NFA�ڵ�
		//for (int j : epolisonSet[i]) {
		//    //��ǰ�ڵ�Ϊ�ɽ���״̬
		//    if (REpos[j] != 0) {
		//        //��¼��ǰ�ڵ�����Ӧ��RE�����ִ�������map�е�λ��
		//        //����Ӧ������ʱ����������ȶ����
		//        if (DFANodes[i].pos == 0 || REpos[j] < DFANodes[i].pos)
		//            DFANodes[i].pos = REpos[j];
		//    }
		//}
		//��õ�ǰ״̬��Ӧ������Ŀ��״̬������ұ߲��֣�
		map<char, set<int> > thisTable = NFAStateTable[i];
		for (char c : allowed_chars)
		{
			//�ж��Ƿ�Ϊͨ·
			if (thisTable[c].empty()) continue;
			//������ͨ·�����Խ���״̬ת�ƣ���ȡ��ǰ�ַ���Ӧ��״̬����
			auto& tgtState = thisTable[c];      //tgtStateΪ��ǰ״̬i������c�ַ����ܹ�ת�Ƶ���״̬
			for (int j = 0; j < stateNum; ++j) {
				//Ѱ�ҵ���ǰ�ڵ��Ӧ�Ľڵ㣬��������
				if (tgtState == epolisonSet[j]) {
					DFANodes[i].ptrs[c] = &DFANodes[j];
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
			else if (node.input1 == '\0'&&node.convert1!=NULL) {
				cout << "State " << node.id << ": On epsilon transition to state " << node.convert1->id << ".\n";
			}

			if (node.input2 != NULL && node.input2 != '\0') {
				cout << "State " << node.id << ": On input '" << node.input2 << "', transition to state " << node.convert2->id << ".\n";
			}
			else if (node.input2 == '\0'&&node.convert2!=NULL) {
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
			/*cout << "POS: " << DFANodes[i].pos << endl;*/
			cout << "Ptrs: " << endl;
			for (const auto& pair : DFANodes[i].ptrs) {
				if (mDFA[pair.second->id] == -1)
				{
					cout << "    " << pair.first << ": " << pair.second->id << endl;  // �������� pair.second ָ��� DFANode �Ѿ���ʼ��
				}
				else
				{

					cout << "    " << pair.first << ": " << mDFA[pair.second->id] << endl;  // �������� pair.second ָ��� DFANode �Ѿ���ʼ��
				}
			}
			cout << "------------------" << endl;
		}

	}

}
static void initDFAStateTable()
{
	for (int i = 0; i < MAX; i++)
	{


		if (DFANodes[i].istrue == 1)
		{

			for (char c : allowed_chars)
			{

				if (DFANodes[i].ptrs.find(c) != DFANodes[i].ptrs.end())
				{

					int tempid = DFANodes[i].ptrs[c]->id;
					DFAStateTable[i][c].insert(tempid);
				}
			}

		}
	}
}
static void printDFATable()
{
	// ��ӡ NFAStateTable �в�Ϊ�յ�Ԫ��
	std::cout << "Non-empty elements in DFAStateTable:" << std::endl;
	for (int i = 0; i < 10; ++i) {
		for (const auto& kvp : DFAStateTable[i]) {
			if (!kvp.second.empty()) {
				std::cout << "DFAStateTable[" << i << "]['" << kvp.first << "']: ";
				for (const auto& element : kvp.second) {
					std::cout << element << " ";
				}
				std::cout << std::endl;
			}
		}
	}
}
static void miniDFA()
{
	for (int i = 0; i < MAX; i++)
	{
		if (DFANodes[i].istrue == 1)              //istrue!=1˵����DFANodeû�б����� 
		{

			for (int j = i + 1; j < MAX; j++)
			{
				bool canMerge = true;
				if (DFANodes[j].istrue == 1 && isDFAFinal[j] != 1)          //isDFAFinal˵���ýڵ㲻����̬
				{
					//cout << "j id" << DFANodes[j].id << endl;

					for (char c : allowed_chars)
					{
						map<char, DFANode*> pairI = DFANodes[i].ptrs;
						map<char, DFANode*> pairJ = DFANodes[j].ptrs;

						// Check if both states have the transition
						bool hasTransitionI = pairI.find(c) != pairI.end();
						bool hasTransitionJ = pairJ.find(c) != pairJ.end();

						if (hasTransitionI != hasTransitionJ)
						{
							canMerge = false;
							break;
						}
						else if (hasTransitionI && hasTransitionJ)
						{
							// If they both have the transition, check if the destination state is the same
							if (pairI[c]->id != pairJ[c]->id)
							{
								canMerge = false;
								break;
							}
						}
					}

					if (canMerge)
					{
						mDFA[j] = i;

						//cout << DFANodes[i].id << " " << DFANodes[j].id << endl;
						DFANodes[i].ptrs.insert(DFANodes[j].ptrs.begin(), DFANodes[j].ptrs.end());

						//cout << DFANodes[0].ptrs['b']->id << endl;

						DFANodes[j].istrue = 0;


					}
				}



			}


		}
	}
}

//int main()
//{
//	initmDFA();
//	initallow_chars();
//	NFANode node0 = initNFA2();
//	printNFA();
//	int state = NFAtoDFATable(&node0);
//	cout << "�������״̬" << state << endl;
//	constructDFA(state);
//	printDFA();
//	initDFAStateTable();
//	printDFATable();
//	miniDFA();
//	printDFA();
//	return 0;
//}

