#define _CRT_SECURE_NO_WARNINGS 
#include"LexParser.h"
#include"LexRuleReader.h"
#include<iostream>
#include"NFA.h"
#include"NFAtoDFA.h"

using namespace std;

int main() {




	cout << "新的测试：：" << endl;
	LexRuleReader lex("c99.l");
	lex.parseRules();
	map<string,string> RDmap = lex.RDs;
	for (auto& x : RDmap) {
		x.second = LexParser::simplyCharacters(x.second);
	}//获取读入并处理过的rules

	//regToDFA.init();
	vector<RegularExpression> regs = lex.REs;
	for (auto& x : regs) {
		//判断first的首字母和尾字母是否均为“\”“，带有”“的会变成\"..\”，代表这个是关键字
		if (*x.first.begin() == '\"' && *x.first.rbegin() == '\"') {
			x.first = LexParser::keywordFilter(x.first);
		}
		else {
			x.first = LexParser::otherFilter(x.first, RDmap);
		}
	}
	opNFA NFA ;
	for (auto& x : regs) {
		
		NFA.REtoNFA(x.first,x.second);
		
	}
    //NFA.mergeNFA();
	
	
	//initmDFA();
	initallow_chars();
	//NFANode node0 = initNFA2();
	NFANode *node0 = NFA.mergeNFA();
	cout << NFANodes[6478].id << "zhelishiid"<<endl;

	printNFA(node0);
	
	int state = NFAtoDFATable(node0);

	cout << "这里输出状态" << state << endl;
	constructDFA(state);
	printDFA();
	initDFAStateTable();
	printDFATable();
	miniDFA();
	printDFA();
	
	

}