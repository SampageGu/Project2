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

NFANode NFANodes[10000];//������нڵ�
map<int, string>REopMap;//��̬��Ӧ��string������intΪ��̬id

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
	}//��ȡ���벢�������rules

	//regToDFA.init();
	vector<RegularExpression> regs = lex.REs;
	for (auto& x : regs) {
		//�ж�first������ĸ��β��ĸ�Ƿ��Ϊ��\���������С����Ļ���\"..\������������ǹؼ���
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
	cout << "�������DFA��״̬" << state << endl;
	DFAgeneration(state);
	//printDFA();
	//initDFAStateTable();

	initmDFA(state);
	divideDFANodes();
	int nodes;
	nodes = generatemDFA();
	cout << "����mDFA��һ����" << nodes << "���ڵ�" << endl;

	map<string, set<int>> case_list;
	//case_list.insert({ "��һ�����",{1,2,3,4} });
	//case_list.insert({ "�ڶ������",{10,12,13} });
	//cout<<switchGenerator("a", case_list);

	//���湹��int DFANodeIsFinal(void);����
	//�������е㣬�����Ƿ�Ϊ��̬
	for (int i = 0; i < MAX; i++) {
		if (ismDFAFinal[i] >= 0) {	//˵�����������̬
			case_list["{return 1;}"].insert(i);
		}
	}
	//ȷ����ǰ״̬�Ƿ�Ϊ��̬������̬�ķ���1��������̬�ķ���0����default����0
	string Func_DFANodeIsFinal = "int DFANodeIsFinal(void){ " + switchGenerator("DFAState", case_list, "return 0;") + "} ";


	case_list.clear();
	//���湹��int DFAResult(void);����
	//�������е㣬�����Ƿ�Ϊ��̬
	for (int i = 0; i < MAX; i++) {
		if (ismDFAFinal[i] >= 0) {	//˵�����������̬
			string code = REopMap[ismDFAFinal[i]];
			case_list[code].insert(i);
		}
	}
	//�����̬����Ӧ��RE�ڶ����֣�����Ӧ״̬Ҫִ�еĴ��룬������Ӧtoken������
	string Func_DFAResult = "int DFAResult(void) { " + switchGenerator("DFAState", case_list, "error(\"unexpected word\");") + "return 0; } ";


	case_list.clear();
	for (int i = 0; i < 10000;i++) {	//�������н��
		DFANode node = miDFA[i];
		if (node.id >= 0)		//��������
		{
			map<string, set<int>> gotoNext;		//�洢ȥ����һ����������ַ�����һ����ı��
			for (auto next : node.transfer) { //�������������к��
				char cin = next.first;		//����cinʱȥ����һ����
				int t = next.second->id;	//��һ����ı��
				string order = "{ DFAState = " + to_string(t) + "; }";//�ı�DFAState��ţ�DFAState��¼��ǰ״̬
				gotoNext[order].insert(cin);	//��˼������cin��ִ��order�����״̬ת����һ�����
			}
			//����switch��ĳһ�ڵ�ת����һ���ڵ��Ӧ��switch���
			//��DFAState = node.idʱ��Ҫ����node����������cin�ٽ���״̬�ĸı�
			//����1��ʾ����
			string content = "{ " + switchGenerator("cin", gotoNext, "{return 1;}") + " }";
			//���ⲿ��������map�У���¼�¶�Ӧ��mDFA�ڵ���
			case_list[content].insert(node.id);
		}
	}
	string Func_DFAStateChange = "int DFAStateChange(char cin) { " +
		switchGenerator("DFAState", case_list, "{return 0;}") + " return 0; }";
	Func_DFAStateChange = removeLastCase0(Func_DFAStateChange);

	//ǰ����ѡ���ַ���switch��default������ɹ�ѡ�У��ı�DFAState���ִ��return 0.

	string DFAfuction = Func_DFANodeIsFinal + '\n' + Func_DFAResult + '\n' + Func_DFAStateChange;

	cout << endl << endl << "******������Ҫ�ĺ��� DFAfuction ��*******" << endl << DFAfuction;

	lexYyCGenerator("lex.yy.c", lex.auxiliaryDefinitions, lex.userSubRoutines, DFAfuction);

}