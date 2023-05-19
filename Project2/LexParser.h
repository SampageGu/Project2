#pragma once

#include <string>
#include <map>
#include<queue>
#include<iostream>
using namespace std;

const int Brace_left = 1;
const int Brace_right = 2;
const int Bracket_left = 3;
const int Bracket_right = 4;
const int CHAR_link_bar = 5;


struct LexParser
{
	//�򻯴���ı��ʽ
	static string simplyCharacters(string s) {
		string rel = "";
		for (int i = 0; i < s.length(); i++) {
			char x = s[i];
			//�������źʹ�����ת��Ϊ�ض��ַ�����ֹ����
			switch (x)
			{
			case '[':
				x = Bracket_left; break;
			case ']':
				x = Bracket_right; break;
			case '{':
				x = Brace_left; break;
			case '}':
				x = Brace_right; break;
			default:
				break;
			}
			//x������������������Ż������ת����������������'\\'����һ����ת���ַ���
			if(x != '\\')
			rel += x;

			//���ʽ������'\'���ţ���x == '\\'��������ת���ַ���
			//�ļ����stringʱ���ַ�����\n����ɡ�\\n������ʱ��Ҫ�ѵ�һ��\ȥ��
			if (x == '\\' && i+1 < s.length()) {
				char temp = s[++i];
				switch (temp)
				{
				case 'p':
					rel += ' ';      //����ȥ����ת���    
					break;
				case 'r':
					rel += '\r';
					break;
				case 'n':
					rel += '\n';
					break;
				case 't':
					rel += '\t';		//����ǰ������'\'���'\t'
					break;
				case 'v':
					rel +='\v';
					break;
				case 'f':
					rel +='\f';		//e.g.	�ļ�����'\f',������ȥ��'\\f'��ǰ����\��ʾ'\',��ʾ��������'\f'
					break;			//��һ����ת���,�ڶ���������ʵ���ݡ�			
				default:
					rel += '\\'; rel += temp;	//e.g.  �ļ��е�'\\',����ȥ������\,��ʵ���ȥ���ĸ�\.
					break;
				}
			}
		}
		return rel;
	}

	//����ǰRD����RE�У�e.g. strsrc����{ D }��Ѱ��strBig����ǰ�ַ������Ƿ����D��Ҫ�滻����RE��D�������Ű�Χ��
	//������strΪ���ܴ���RD���ַ�����wordToSeekΪ��Ҫ���滻���ַ���replaceContentΪ�滻������
	static void replaceByRD(string& str, const string& wordToSeek, const string& replaceContent) {
		string wordToSeek1 = (char)Brace_left + wordToSeek + (char)Brace_right;
		int pos = 0;
		int srclen = wordToSeek1.size();
		int dstlen = replaceContent.size();
		while ((pos = str.find(wordToSeek1, pos)) != string::npos) {
			str.replace(pos, srclen, replaceContent);
			pos += dstlen;
		}
	}

	//��RD����RE�У��ú��������������RD��Ȼ�����replaceByRD��ʵ���滻
	//������singleREΪ����RE��RDmap����RD����
	static string allReplaceByRD(string singleRE, const map<string, string>& RDmap) {
		for (auto& p : RDmap) {
			//��������RD�������滻
			//replaceByRD(singleRE, (char)(Brace_left)+ p.first + (char)(Brace_right), p.second);
			replaceByRD(singleRE, p.first, p.second);
		}
			return singleRE;
		}

	//�滻���������ţ��������Ӧ���ַ�����չ����ͨ��ASCII�����ʵ��
	//������strΪҪ�����滻���ַ���
	//���������������ַ������������Ѿ�ȥ����������
	static string singleBracketReplace(string str) {
		string tmp = "";
		queue<char> queue;
		for (int i = 0; i < str.length(); i++) {
			char c = str[i];
			//���ڡ�\\���洢�ɡ�\\\\��������Ҫ���⴦��
			if (c == '\\') {
				++i;
				c = str[i];
				queue.push(c);
				tmp.push_back(c);
				continue;
			}
			//e.g. [0-9]�����м����ӷ������滻
			if (c == '-') { queue.push(CHAR_link_bar); tmp.push_back(c); }
			else { queue.push(c); tmp.push_back(c); }
		}
		str = tmp;		
		string rel = "";

		char first = str[0];
		if (first == '^') {
			int ASCcode[127] = { 0 };//ASCII��
			for (int i = 9; i < 127; i++) {
				//���������ASCIIλȫ����Ϊ1
				ASCcode[i] = 1;
			}
			string inverseRel = singleBracketReplaceAU(queue);			 
			for (int i = 0; i < inverseRel.length(); i++) {
				if(inverseRel[i] != '\|')
				ASCcode[inverseRel[i]] = 0;		//��Ҫȥ���ķ���
			}
			/*for (int i = 0; i < str.length(); i++) {
				if (str[i] == CHAR_link_bar) {
					for (int k = str[i - 1]; k <= str[i + 1]; k++) {
						ASCcode[k] = 0;
					}
					i++;
				}
				else ASCcode[str[i]] = 0;
			}*/
			for (int j = 9; j < 127; j++) {
				if (ASCcode[j] == 1) {
					char target = j;
					switch (target) {
					case '(':
					case '|':
					case ')':
					case '+':
					case '?':
					case '*':
					case '.':
					case '\\':
						//���������ַ�
						rel = rel + '\\' + target + '|';
						break;
					default:
						rel = rel + target + '|';
						break;
					}
				}
			}
			rel = rel.substr(0, rel.length() - 1);//delete last '|'
			return rel;
		}

		//����'^'�����
		rel = singleBracketReplaceAU(queue);
		rel = rel.substr(0, rel.length() - 1);//delete last '|'
		return rel;
	}


	//����singleBracketReplace����
	static string singleBracketReplaceAU(queue<char> queue) {
		string rel = "";
		while (!queue.empty()) {
			char first = queue.front();
			queue.pop();
			char second = '��'; //�����ܵ��ڵ�ֵ���൱���ÿ�
			char third = '��';
			if (!queue.empty()) {
				second = queue.front();
			}
			if (second == CHAR_link_bar && second != '��') {   //չ��0-9��a-z��
				queue.pop();  //pop��second
				if (!queue.empty()) {
					third = queue.front();
					queue.pop();
				}
				//char third = queue.front();
				//queue.pop();
				if(third != '��')
				for (auto i = first; i <= third; i++) {  //char��Ҳ����
					char a = i;
					switch (a) {
					case '(':
					case '|':
					case ')':
					case '+':
					case '?':
					case '*':
					case '.':
					case '\\':
						//���������ַ�
						rel = rel + '\\' + a + '|';
						break;
					default:
						rel = rel + a + '|';
						break;
					}
					//rel = rel + i + '|';
				}
			}
			else {
				switch (first) {
				case '(':
				case '|':
				case ')':
				case '+':
				case '?':
				case '*':
				case '.':
				case '\\':
					//���������ַ�
					rel = rel + '\\' + first + '|';
					break;
				default:
					rel = rel + first + '|';
					break;
				}
			}
			//rel = rel + first + '|' + second + '|';														
		}
		return rel;
	}
	
	//��RE�е�[]ȫ��չ������ɶ�Ӧ���ַ�
	//������strΪҪ�����滻���ַ���
	//
	static string replaceBracketPairs(string str) {
		//const string str1 = string("") + static_cast<char>(Bracket_left);
		string str1;  str1.push_back((char)(Bracket_left));
		//const string str2 = string("") + static_cast<char>(Bracket_right);
		string str2;  str2.push_back((char)(Bracket_right));
		int pos1 = 0;
		int pos2 = 0;

		//�ҵ�һ�Զ�Ӧ��������
		while (((pos1 = str.find(str1, pos1)) != string::npos) && ((pos2 = str.find(str2, pos1)) != string::npos))
		{
			//pos1 = str.find(str1, pos1);
			//pos2 = str.find(str2, pos2);
			int len = pos2 - pos1;//���������ַ�������

			string str3 = "(" + singleBracketReplace(str.substr(pos1 + 1, len - 1)) + ")";

			str = str.replace(pos1, len + 1, str3);
			pos1 += str3.size();
			pos2 += str3.size();
		}
		return str;
	}

	//����.���滻�������ַ���[\t-~]"
	static string replaceDots(string str) {
		int index = -1;
		for (int i = 0; i < str.length(); i++) {
			//���ڡ�\\"�ھ���quoteFiler����ɡ�\\\\"���ʺ�����λ����\\"���������\"��
			if (str[i] == '\\')
				i += 2;
			if (str[i] == '.') {
				index = i;
				break;
			}
		}
		if (index != -1) {		//�����ַ����Ա�ʾASC�� 9-126��
			string t = (char)(Bracket_left)+((char)(9) + string("-") + (char)(126)) + (char)(Bracket_right);
			str.replace(index, 1, t);
		}
		return str;
	}

	//����ǹؼ��ֵĺ���
	//������singleREΪ����RE��RDmap����RD����
	static string otherFilter(string singleRE, const map<string, string>& RDmap) {
		singleRE = simplyCharacters(singleRE);
		singleRE = allReplaceByRD(singleRE, RDmap);
		singleRE = replaceDots(singleRE);
		singleRE = replaceBracketPairs(singleRE);
		return singleRE;
	}

	//����ؼ��ֵĺ���
	//������strΪҪ���д�����ַ���
	static string keywordFilter(string str) {
		string ans = "";
		str = str.substr(1, str.length() - 2);
		for (char c : str) {
			//Ϊ�����жϷ��ţ��ʽ���Щ���ű��\\..
			switch (c) {
			case '(':
			case '|':
			case ')':
			case '+':
			case '?':
			case '*':
			case '.':
			case '\\':
				ans += '\\';
				break;
			default:
				break;
			}
			ans += c;
		}
		return ans;
	}
};


