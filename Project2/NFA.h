#pragma once
#ifndef _NFA_H
#define _NFA_H
#include <cstddef>
#include <stack>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <queue>
using namespace std;

struct NFANode {
	int id = -1;//idΪ�ڵ㱻������˳��,δ������ʱΪ-1
	char input1 = NULL;//������ַ�
	NFANode * convert1 = NULL;//��һ��״̬
	char input2 = NULL;
	NFANode* convert2 = NULL;
};

	static int flag[65536];//debug�ñ�ʶ��
	static NFANode NFANodes[65536];//������нڵ�
	static int countNFANode ;
	static stack<char> opStack;//������ջ
	static stack<pair<NFANode*, NFANode*> > NFAStack;//�ڵ�ջ
	static map<int, string>REopMap;

class opNFA {
public:
	



	static NFANode* newNFANode();//�����½ڵ�

	static void opNFANode(char op);//����NFA,opΪ������:��|������+��*��\0��

	static pair<NFANode*, NFANode*> REtoNFA(string RE, string REop);//REתNFA

	static void debugNFA(NFANode* NFA);//���һ��NFAͼ

	static NFANode* mergeNFA();//���ϲ�ջ��ʣ�������NFA
};

#endif