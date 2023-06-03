//#include <map>
//#include <set>
//#include <queue>
//#include <vector>
//#include<stack>
//#include <iostream>
//#include"NFA.h"
//using namespace std;
//#define NULL 127
//#define MAX 99
//struct NFANode {
//	int id = -1;  //idΪ����ʾû�б���ʼ��
//	char input1 = NULL;
//	NFANode* convert1 = NULL;
//	char input2 = NULL;
//	NFANode* convert2 = NULL;
//};
//struct DFANode {
//	bool istrue=0;
//	int id = -1;
//	int pos = 0;
//	map<char, DFANode*> transfer;//ת�Ʒ���
//} DFANodes[MAX];
//set<int> epolisonSet[MAX];//��¼���бհ����ϣ������ߵĲ���  ��epolisonSet[1]��ʾ1���Ե�������бհ�
//map<char, set<int> > NFAStateTable[MAX];
////��¼����Ŀ��״̬��charָ������ַ���setָת�ƺ�ıհ����  Statetable[1]['a']��ʾ1״̬�ڽ���a�ַ�����Ե���ıհ�
//map<char, set<int>>DFAStateTable[MAX];
//vector<char> totalChars;//��ӦASCII�룬��¼����������ַ�
//NFANode NFANodes[MAX]; // �洢���е� NFANode ����
//void printNFAtable();
//int mDFA[MAX];          //�����洢���ϲ� ����mDFA[2]=0,��ʾ״̬2���Ժ�״̬0�ϲ�����ʼ��Ϊ-1
//set<int>NFAFinalState;//������¼NFA����̬
//bool isDFAFinal[MAX];   //������¼ÿ��DFA״̬�Ƿ�����̬����ʼ��Ϊ0
//void initmDFA() {
//	for (int i = 0; i < MAX; i++)
//	{
//		mDFA[i] = -1;
//		isDFAFinal[i] = 0;
//		//isNFAFinal[i] = 0;
//	}
//
//}
//int REpos[65536];//��¼�ڵ��Ӧ��ִ�����
//void initTotalChars() {
//	for (int i = 9; i < 127; ++i) {
//		if (true/*i != '#'*/) {
//			//��¼��ASCII��
//			totalChars.push_back(i);
//		}
//	}
//	memset(REpos, 0, sizeof REpos);
//}
//NFANode* getNodeById(int id) {
//	return &NFANodes[id]; // ���� id ���ǽڵ��� vector �е�����
//}
//NFANode  initNFA1()
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
//NFANode initNFA2()
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
//
//
//
////��һ��״̬�ıհ�
////����Ԫ��set1Ϊһϵ�еĽڵ���
//void getStateClosure(set<int>& set1) {
//
//	queue<int> que;
//	for (int id : set1) {
//		que.push(id);
//	}
//	//ʹ��BFS���� ��һ����ų�ȥ����epolison���пɵ���Ľڵ�
//	while (!que.empty()) {
//		int id = que.front();
//		que.pop();
//		NFANode& p = NFANodes[id];
//		/*  if (p.input1 != '\0') continue;*/
//		  //�ж��Ƿ�Ϊ��״̬��ͨ��find��������set�����޸�Ԫ�أ��򷵻�set1.end()
//		  //���жϵ�һ���ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
//		if (p.input1 == '\0' && set1.find(p.convert1->id) == set1.end())
//		{
//			set1.insert(p.convert1->id);
//			que.push(p.convert1->id);
//		}
//		/*if (p.input2 != '\0') continue;*/
//		//���жϵڶ����ߣ����ָ�����epolison��������set������û�������ţ���ô���ñ����ӵ�set������
//		if (p.input2 == '\0' && set1.find(p.convert2->id) == set1.end()) {
//			set1.insert(p.convert2->id);
//			que.push(p.convert2->id);
//		}
//	}
//	/* if (!set1.empty())
//	 {
//		 cout << "�������set1:";
//		 for (const auto& element : set1) {
//			 cout << element << " ";
//		 }
//		 cout << endl;
//	 }*/
//}
//void initClosure(NFANode* startNode) {
//	// ��ʼ״̬һ���������ת״̬
//	//�Ƚ���ʼ�ڵ����״̬������
//	epolisonSet[0].insert(startNode->id);
//	//���epil��0���Ľ��������state_set��
//	getStateClosure(epolisonSet[0]);
//}
//int NFAtoDFATable(NFANode* startNode) {
//	initClosure(startNode);
//	//��¼���ж�����״̬DFA
//	int last_state_idx = 0;
//	//��û����״̬ʱ��ֹͣѭ��
//	for (int i = 0; i <= last_state_idx; ++i) {
//		//��ʼ״̬
//		auto& srcState = epolisonSet[i];
//		//Ŀ��ת��״̬
//		auto& tgtTable = NFAStateTable[i];
//		//������ʹ�õ�ASCII��
//		for (char c : totalChars)
//		{
//			//��ȡ�ܹ�����Ŀ���ַ���״̬
//			auto& table = tgtTable[c];
//			for (int id : srcState)
//			{
//				//��ȡ��ǰid��Ӧ��NFA�ڵ�
//				auto& node = NFANodes[id];
//				//�������ַ�cʱ���ܹ���ת����״̬����¼����״̬��id
//				//map�в�������ͬ�ڵ�
//				if (node.input1 == c) {
//					//  cout << c << "\t"<<id<<"\t input1 match successfully" << endl;
//					table.insert(node.convert1->id);
//				}
//				if (node.input2 == c) {
//					// cout << c << "\t" << id << "\t input2 match successfully" << endl;
//					table.insert(node.convert2->id);
//				}
//			}
//			//Ѱ������Ŀ��״̬�ĵȼ���
//			getStateClosure(table);
//
//			if (!table.empty()) {
//				//��¼�Ƿ�Ϊ��״̬
//				bool newOne = true;
//				//Ѱ���Ƿ��Ѿ��е�ǰ����
//				for (int j = 0; j <= last_state_idx; ++j) {
//					if (table == epolisonSet[j]) {
//						newOne = false;
//						break;
//					}
//				}
//				//��״̬����״̬����
//				if (newOne) {
//					last_state_idx++;
//					epolisonSet[last_state_idx] = table;
//					vector<int> intersection;
//					set_intersection(table.begin(), table.end(), NFAFinalState.begin(),NFAFinalState.end(), back_inserter(intersection));   //���հ���NFA����̬mapȡ���� 
//					if (!intersection.empty()) { //�����Ϊ�գ���ô˵����DFA״̬����̬
//						cout << "�ҵ��ˣ�����������������"<<last_state_idx << endl; 
//						isDFAFinal[last_state_idx] = 1;
//					}
//				}
//			}
//		}
//
//	}
//	printNFAtable();
//	return last_state_idx + 1;
//}
//void printNFAtable()
//{
//	std::cout << "Non-empty elements in epolisonSet:" << std::endl;
//	for (int i = 0; i < 10; ++i) {
//		if (!epolisonSet[i].empty()) {
//			std::cout << "epolisonSet[" << i << "]: ";
//			for (const auto& element : epolisonSet[i]) {
//				std::cout << element << " ";
//			}
//			std::cout << std::endl;
//		}
//	}
//
//	// ��ӡ NFAStateTable �в�Ϊ�յ�Ԫ��
//	std::cout << "Non-empty elements in NFAStateTable:" << std::endl;
//	for (int i = 0; i < 10; ++i) {
//		for (const auto& kvp : NFAStateTable[i]) {
//			if (!kvp.second.empty()) {
//				std::cout << "NFAStateTable[" << i << "]['" << kvp.first << "']: ";
//				for (const auto& element : kvp.second) {
//					std::cout << element << " ";
//				}
//				std::cout << std::endl;
//			}
//		}
//	}
//
//}
////����DFAͼ
////������stateCntΪDFA״̬����
//void DFAgeneration(int stateNum) {
//	for (int i = 0; i < stateNum; ++i) {
//		DFANodes[i].id = i;
//		DFANodes[i].istrue = 1;
//		DFANodes[i].pos = 0;
//		//������ǰ�����е�����NFA�ڵ�
//		//for (int j : epolisonSet[i]) {
//		//    //��ǰ�ڵ�Ϊ�ɽ���״̬
//		//    if (REpos[j] != 0) {
//		//        //��¼��ǰ�ڵ�����Ӧ��RE�����ִ�������map�е�λ��
//		//        //����Ӧ������ʱ����������ȶ����
//		//        if (DFANodes[i].pos == 0 || REpos[j] < DFANodes[i].pos)
//		//            DFANodes[i].pos = REpos[j];
//		//    }
//		//}
//		//��õ�ǰ״̬��Ӧ������Ŀ��״̬������ұ߲��֣�
//		map<char, set<int> > thisTable = NFAStateTable[i];
//		for (char c : totalChars)
//		{
//			//�ж��Ƿ�Ϊͨ·
//			if (thisTable[c].empty()) continue;
//			//������ͨ·�����Խ���״̬ת�ƣ���ȡ��ǰ�ַ���Ӧ��״̬����
//			auto& tgtState = thisTable[c];      //tgtStateΪ��ǰ״̬i������c�ַ����ܹ�ת�Ƶ���״̬
//			for (int j = 0; j < stateNum; ++j) {
//				//Ѱ�ҵ���ǰ�ڵ��Ӧ�Ľڵ㣬��������
//				if (tgtState == epolisonSet[j]) {
//					DFANodes[i].transfer[c] = &DFANodes[j];
//					break;
//				}
//			}
//		}
//
//	}
//}
//void printNFA()
//{
//	// Print NFA
//	cout << "NFA:\n";
//	for (const auto& node : NFANodes) {
//		if (node.id >= 0)
//		{
//			if (node.input1 != NULL && node.input1 != '\0') {
//				cout << "State " << node.id << ": On input '" << node.input1 << "', transition to state " << node.convert1->id << ".\n";
//			}
//			else if (node.input1 == '\0') {
//				cout << "State " << node.id << ": On epsilon transition to state " << node.convert1->id << ".\n";
//			}
//
//			if (node.input2 != NULL && node.input2 != '\0') {
//				cout << "State " << node.id << ": On input '" << node.input2 << "', transition to state " << node.convert2->id << ".\n";
//			}
//			else if (node.input2 == '\0') {
//				cout << "State " << node.id << ": On epsilon transition to state " << node.convert2->id << ".\n";
//			}
//
//			if (node.input1 == NULL && node.input2 == NULL && node.convert1 == NULL && node.convert2 == NULL) {
//				cout << "State " << node.id << ": Final state.\n";
//			}
//		}
//	}
//}
//void printDFA()
//{
//	for (int i = 0; i < MAX; ++i) {
//		if (DFANodes[i].istrue == 1)
//		{  // ���������趨�ġ��Ѿ���ʼ����������
//			//cout << "DFANode " << i << ": " << endl;
//			cout << "ID: " << DFANodes[i].id << endl;
//			/*cout << "POS: " << DFANodes[i].pos << endl;*/
//			cout << "transfer: " << endl;
//			for (const auto& pair : DFANodes[i].transfer) {
//				if (mDFA[pair.second->id] == -1)
//				{
//					cout << "    " << pair.first << ": " << pair.second->id << endl;  // �������� pair.second ָ��� DFANode �Ѿ���ʼ��
//				}
//				else
//				{
//					
//					cout << "    " << pair.first << ": " << mDFA[pair.second->id] << endl;  // �������� pair.second ָ��� DFANode �Ѿ���ʼ��
//				}
//			}
//			cout << "------------------" << endl;
//		}
//
//	}
//
//}
//void initDFAStateTable()
//{
//	for (int i = 0; i < MAX; i++)
//	{
//
//
//		if (DFANodes[i].istrue== 1)
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
//void printDFATable()
//{
//	// ��ӡ NFAStateTable �в�Ϊ�յ�Ԫ��
//	std::cout << "Non-empty elements in DFAStateTable:" << std::endl;
//	for (int i = 0; i < 10; ++i) {
//		for (const auto& kvp : DFAStateTable[i]) {
//			if (!kvp.second.empty()) {
//				std::cout << "DFAStateTable[" << i << "]['" << kvp.first << "']: ";
//				for (const auto& element : kvp.second) {
//					std::cout << element << " ";
//				}
//				std::cout << std::endl;
//			}
//		}
//	}
//}
//void miniDFA()
//{
//	for (int i = 0; i < MAX; i++)
//	{
//		if (DFANodes[i].istrue== 1)              //istrue!=1˵����DFANodeû�б����� 
//		{
//
//			for (int j = i + 1; j < MAX; j++)
//			{
//				bool canMerge = true;
//				if (DFANodes[j].istrue== 1&&isDFAFinal[j]!=1)          //isDFAFinal˵���ýڵ㲻����̬
//				{
//					//cout << "j id" << DFANodes[j].id << endl;
//
//					for (char c : totalChars)
//					{
//						map<char, DFANode*> pairI = DFANodes[i].transfer;
//						map<char, DFANode*> pairJ = DFANodes[j].transfer;
//
//						// Check if both states have the transition
//						bool hasTransitionI = pairI.find(c) != pairI.end();
//						bool hasTransitionJ = pairJ.find(c) != pairJ.end();
//
//						if (hasTransitionI != hasTransitionJ)
//						{
//							canMerge = false;
//							break;
//						}
//						else if (hasTransitionI && hasTransitionJ)
//						{
//							// If they both have the transition, check if the destination state is the same
//							if (pairI[c]->id != pairJ[c]->id)
//							{
//								canMerge = false;
//								break;
//							}
//						}
//					}
//
//					if (canMerge)
//					{
//						mDFA[j] = i;
//						
//						//cout << DFANodes[i].id << " " << DFANodes[j].id << endl;
//						DFANodes[i].transfer.insert(DFANodes[j].transfer.begin(), DFANodes[j].transfer.end());
//						
//						//cout << DFANodes[0].transfer['b']->id << endl;
//			
//						DFANodes[j].istrue =0;
//						
//						
//					}
//				}
//				
//
//
//			}
//
//
//		}
//	}
//}
//
////int main()
////{
////	initmDFA();
////	initTotalChars();
////	NFANode node0 = initNFA2();
////	printNFA();
////	int state = NFAtoDFATable(&node0);
////	cout << "�������״̬" << state << endl;
////	DFAgeneration(state);
////	printDFA();
////	initDFAStateTable();
////	printDFATable();
////	miniDFA();
////	printDFA();
////	return 0;
////}
//
