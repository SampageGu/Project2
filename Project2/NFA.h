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
	int id = -1;//id为节点被生产的顺序,未被创建时为-1
	char input1 = NULL;//输入的字符
	NFANode * convert1 = NULL;//下一个状态
	char input2 = NULL;
	NFANode* convert2 = NULL;
};

	static int flag[65536];//debug用标识符
	static NFANode NFANodes[65536];//存放所有节点
	static int countNFANode ;
	static stack<char> opStack;//操作符栈
	static stack<pair<NFANode*, NFANode*> > NFAStack;//节点栈
	static map<int, string>REopMap;

class opNFA {
public:
	



	static NFANode* newNFANode();//产生新节点

	static void opNFANode(char op);//操作NFA,op为操作符:“|、？、+、*、\0”

	static pair<NFANode*, NFANode*> REtoNFA(string RE, string REop);//RE转NFA

	static void debugNFA(NFANode* NFA);//输出一幅NFA图

	static NFANode* mergeNFA();//最后合并栈中剩余的所有NFA
};

#endif