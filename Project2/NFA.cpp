#include "NFA.h"

using namespace std;




NFANode* opNFA::newNFANode()
{
	NFANodes[countNFANode].id = countNFANode;
	return &NFANodes[countNFANode++];
}



void opNFA::opNFANode(char op) {
	//cout << op << endl;
	pair<NFANode*, NFANode*>  NFA1 = NFAStack.top();
	NFAStack.pop();
	NFANode* start1 = NFA1.first, * end1 = NFA1.second;
	//cout << start1->id << ' ' << end1->id << endl;
	switch (op) {
	case'\0':/*���Ӳ���*/ {
		pair<NFANode*, NFANode*>  NFA2 = NFAStack.top();
		NFAStack.pop();
		NFANode* start2 = NFA2.first, * end2 = NFA2.second;
		//cout << start2->id << ' ' << end2->id << endl;
		end2->input1 = start1->input1;
		end2->convert1 = start1->convert1;
		end2->input2 = start1->input2;
		end2->convert2 = start1->convert2;
		pair<NFANode*, NFANode*>  newNFA = { start2,end1 };
		NFAStack.push(newNFA);
		break;
	}
	case'|':/*�����*/ {
		pair<NFANode*, NFANode*>  NFA2 = NFAStack.top();
		NFAStack.pop();
		NFANode* start2 = NFA2.first, * end2 = NFA2.second;
		//cout << start2->id << ' ' << end2->id << endl;
		NFANode* newStart = newNFANode();
		NFANode* newEnd = newNFANode();
		newStart->input1 = '\0';
		newStart->convert1 = start1;
		newStart->input2 = '\0';
		newStart->convert2 = start2;
		end1->input1 = '\0';
		end1->convert1 = newEnd;
		end2->input1 = '\0';
		end2->convert1 = newEnd;
		NFAStack.push({ newStart,newEnd });
		break;
	}
	case'*':
	case'+':
	case'?': {
		NFANode* newStart = newNFANode();
		NFANode* newEnd = newNFANode();
		newStart->input1 = '\0';
		newStart->convert1 = start1;
		end1->input1 = '\0';
		end1->convert1 = newEnd;
		if (op == '*' || op == '+') {
			end1->input2 = '\0';
			end1->convert2 = start1;
		}
		if (op == '*' || op == '?') {
			newStart->input2 = '\0';
			newStart->convert2 = newEnd;
		}
		NFAStack.push({ newStart,newEnd });
		break;
	}
	default://������op
		fprintf(stderr, "error op");
		break;
	}
}

pair<NFANode*, NFANode*> opNFA::REtoNFA(string RE, string REop)
{
	//���op_statck
	while (!(opStack.empty()))
		opStack.pop();

	int pos = 0;//��¼��ȡλ��
	char str = RE[pos];//��ǰ�ַ�
	NFANode* start = newNFANode();//��ʼ״̬
	NFAStack.push({ start,start });

	while (str != '\0') {
		if (str != '\\')/*��ת���������Ҫ�����һλ*/ {
			switch (str) {
			case'(': {
				opStack.push(str);
				//cout << opStack.top() << endl;
				NFANode* newStrat = newNFANode();
				NFAStack.push({ newStrat,newStrat });
				//cout << newStrat->id << " " << newStrat->id << endl;
				break;
			}
			case'|': {
				if (!opStack.empty()) {
					char opTemp = opStack.top();
					while (opTemp != '(') {
						opNFANode(opTemp);
						opStack.pop();
						if (opStack.empty())
							break;
						else
							opTemp = opStack.top();
					}
				}
				opStack.push(str);
				NFANode* newStrat = newNFANode();
				NFAStack.push({ newStrat,newStrat });
				break;
			}
			case')':/*��������֮ǰ�Ķ����㣬������һ����/0����ʾ���Ӳ�������ֹ���֡�����+�������*/ {
				char opTemp = opStack.top();
				while (opTemp != '(') {
					opNFANode(opTemp);
					opStack.pop();
					if (opStack.empty())
					{
						cout << "!!!!!!!"<<endl;
						cout << RE;
					}
					opTemp = opStack.top();
				}
				if (opTemp == '(') {
					opStack.pop();
					opStack.push('\0');
				}
				break;
			}
			case'*':
			case'+':
			case'?':/*��Ŀ�����ֱ�Ӽ���*/ {
				opNFANode(str);
				break;
			}
			default:/*���Ӳ���,���������սڵ㣬���Ӳ�ѹ��ջ�У��������Ӳ���ѹ��ջ��*/ {
				char opTemp = NULL;
				if (!opStack.empty())
					opTemp = opStack.top();
				NFANode* newStart = newNFANode();
				NFANode* newEnd = newNFANode();
				newStart->input1 = str;
				newStart->convert1 = newEnd;
				while (!(opStack.empty()) && opTemp == '\0')/*����ջ�����Ӳ���*/ {
					opNFANode(opTemp);
					opStack.pop();
					opTemp == NULL;
					if (!(opStack.empty()))
						opTemp = opStack.top();
				}
				opStack.push('\0');
				NFAStack.push({ newStart,newEnd });
			}
			}
		}
		else {
			str = RE[++pos];//����һλ
			if (str != '\0') /*���Ӳ���*/ {
				char opTemp = NULL;
				if (!opStack.empty())
					opTemp = opStack.top();
				NFANode* newStart = newNFANode();
				NFANode* newEnd = newNFANode();
				newStart->input1 = str;
				newStart->convert1 = newEnd;
				while (!(opStack.empty()) && opTemp == '\0')/*����ջ�����Ӳ���*/ {
					opNFANode(opTemp);
					opStack.pop();
					opTemp == NULL;
					if (!(opStack.empty()))
						opTemp = opStack.top();
				}
				opStack.push('\0');
				NFAStack.push({ newStart,newEnd });
			}
		}
		str = RE[++pos];

	}
	//ȷ�����в���״̬�����ϲ�
	while (!(opStack.empty())) {
		char op = opStack.top();
		opNFANode(op);
		opStack.pop();
	}
	//���غϲ�������NFAͼ
	pair<NFANode*, NFANode*> fin = NFAStack.top();
	REopMap[fin.second->id] = REop;
	return fin;
}

NFANode* opNFA::mergeNFA() {
	while (!NFAStack.empty()) {
		NFANode* start1 = NFAStack.top().first;
		NFAStack.pop();
		if (NFAStack.empty()) {
			return start1;
		}
		NFANode* start2 = NFAStack.top().first;
		NFAStack.pop();
		NFANode* newStart = newNFANode();
		newStart->input1 = '\0';
		newStart->convert1 = start1;
		newStart->input2 = '\0';
		newStart->convert2 = start2;
		NFAStack.push({ newStart,newStart });
		cout << 1;
	}
}

void opNFA::debugNFA(NFANode* NFA) {
	NFANode* start = NFA;
	stack<NFANode*> tempStack;
	tempStack.push(NFA);
	while (!tempStack.empty()) {
		NFANode* tempNFA = tempStack.top();
		flag[tempNFA->id] = 1;
		cout << tempNFA->id;
		if (tempNFA->convert1 != NULL && flag[tempNFA->convert1->id] == 0) {
			cout << " ---" << tempNFA->input1 << "---> ";
			tempStack.push(tempNFA->convert1);
			continue;
		}
		if (tempNFA->convert2 != NULL && flag[tempNFA->convert2->id] == 0) {
			cout << " ---" << tempNFA->input2 << "---> ";
			tempStack.push(tempNFA->convert2);
			continue;
		}
		else if (tempNFA->convert2 != NULL && flag[tempNFA->convert2->id] == 1) {
			cout << " ---" << tempNFA->input2 << "---> " << tempNFA->convert2->id << "!!" << endl;
		}
		else {
			cout << endl;
		}
		tempStack.pop();
	}
	
}


