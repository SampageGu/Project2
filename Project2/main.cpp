#define _CRT_SECURE_NO_WARNINGS 
#include"LexParser.h"
#include"LexRuleReader.h"
#include<iostream>
#include"NFA.h"
#include"NFAtoDFA.h"
#include"LexParser.h"
#include"LexRuleReader.h"
#include<iostream>
#include"lexYyCGenerator.h"


using namespace std;

NFANode NFANodes[10000];//存放所有节点
map<int, string>REopMap;//终态对应的string操作，int为终态id

using namespace std;
string removeLastCase0(const string& str) {
	size_t pos = str.rfind("case 0");
	if (pos != string::npos) {
		return str.substr(0, pos) + str.substr(pos + 8);
	}
	return str;
}

int main() {
	LexRuleReader lex("c99.l");
	lex.parseRules();
	map<string, string> RDmap = lex.RDs;
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

	opNFA NFA;
	for (auto& x : regs) {

		NFA.REtoNFA(x.first, x.second);

	}
	
	//.mergeNFA();

	NFANode* node0 = NFA.mergeNFA();
	initmDFA();
	initTotalChars();
	//NFANode node0 = initNFA2();
	int state = NFAtoDFATable(node0);
	cout << "这里输出DFA的状态" << state << endl;
	DFAgeneration(state);
	//printDFA();
	//initDFAStateTable();

	initmDFA(state);
	divideDFANodes();
	int nodes;
	nodes = generatemDFA();
	cout << "经过mDFA后一共有" << nodes << "个节点" << endl;

	map<string, set<int>> case_list;
	//case_list.insert({ "第一种情况",{1,2,3,4} });
	//case_list.insert({ "第二种情况",{10,12,13} });
	//cout<<switchGenerator("a", case_list);

	//下面构造int DFANodeIsFinal(void);函数
	//遍历所有点，看它是否为终态
	for (int i = 0; i < MAX; i++) {
		if (ismDFAFinal[i] >= 0) {	//说明这个点是终态
			case_list["{return 1;}"].insert(i);
		}
	}
	//确定当前状态是否为终态，是终态的返回1，不是终态的返回0，即default返回0
	string Func_DFANodeIsFinal = "int DFANodeIsFinal(void){ " + switchGenerator("DFAState", case_list, "return 0;") + "} ";


	case_list.clear();
	//下面构造int DFAResult(void);函数
	//遍历所有点，看它是否为终态
	for (int i = 0; i < MAX; i++) {
		if (ismDFAFinal[i] >= 0) {	//说明这个点是终态
			string code = REopMap[ismDFAFinal[i]];
			case_list[code].insert(i);
		}
	}
	//输出终态结点对应的RE第二部分，即相应状态要执行的代码，返回相应token的名字
	string Func_DFAResult = "int DFAResult(void) { " + switchGenerator("DFAState", case_list, "error(\"unexpected word\");") + "return 0; } ";


	case_list.clear();
	for (int i = 0; i < 10000;i++) {	//遍历所有结点
		DFANode node = miDFA[i];
		if (node.id >= 0)		//这个点存在
		{
			map<string, set<int>> gotoNext;		//存储去向下一个点的输入字符和下一个点的编号
			for (auto next : node.transfer) { //遍历这个点的所有后继
				char cin = next.first;		//输入cin时去向下一个点
				int t = next.second->id;	//下一个点的编号
				string order = "{ DFAState = " + to_string(t) + "; }";//改变DFAState编号，DFAState记录当前状态
				gotoNext[order].insert(cin);	//意思是输入cin则执行order命令，即状态转向下一个结点
			}
			//生成switch中某一节点转到下一个节点对应的switch语句
			//即DFAState = node.id时，要考虑node后续的输入cin再进行状态的改变
			//返回1表示出错
			string content = "{ " + switchGenerator("cin", gotoNext, "{return 1;}") + " }";
			//将这部分语句存入map中，记录下对应的mDFA节点编号
			case_list[content].insert(node.id);
		}
	}
	string Func_DFAStateChange = "int DFAStateChange(char cin) { " +
		switchGenerator("DFAState", case_list, "{return 0;}") + " return 0; }";
	Func_DFAStateChange = removeLastCase0(Func_DFAStateChange);

	//前者是选择字符的switch的default，如果成功选中，改变DFAState后就执行return 0.

	string DFAfuction = Func_DFANodeIsFinal + '\n' + Func_DFAResult + '\n' + Func_DFAStateChange;

	cout << endl << endl << "******这是需要的函数 DFAfuction ：*******" << endl << DFAfuction;

	lexYyCGenerator("lex.yy.c", lex.auxiliaryDefinitions, lex.userSubRoutines, DFAfuction);

}