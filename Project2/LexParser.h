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
	//简化传入的表达式
	static string simplyCharacters(string s) {
		string rel = "";
		for (int i = 0; i < s.length(); i++) {
			char x = s[i];
			//将中括号和大括号转化为特定字符，防止混淆
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
			//x可以是运算符，中括号或大括号转换后结果，不可以是'\\'（第一个是转义字符）
			if(x != '\\')
			rel += x;

			//表达式中若有'\'符号，则x == '\\'（增加了转义字符）
			//文件存进string时，字符串‘\n’存成‘\\n’，此时需要把第一个\去掉
			if (x == '\\' && i+1 < s.length()) {
				char temp = s[++i];
				switch (temp)
				{
				case 'p':
					rel += ' ';      //上面去掉了转义符    
					break;
				case 'r':
					rel += '\r';
					break;
				case 'n':
					rel += '\n';
					break;
				case 't':
					rel += '\t';		//加上前面加入的'\'组成'\t'
					break;
				case 'v':
					rel +='\v';
					break;
				case 'f':
					rel +='\f';		//e.g.	文件中是'\f',这里存进去是'\\f'，前两个\表示'\',显示出来就是'\f'
					break;			//第一个是转义符,第二个才是真实数据。			
				default:
					rel += '\\'; rel += temp;	//e.g.  文件中的'\\',看上去是两个\,其实存进去是四个\.
					break;
				}
			}
		}
		return rel;
	}

	//将当前RD带入RE中，e.g. strsrc代表{ D }，寻找strBig即当前字符串中是否存在D需要替换（在RE中D被大括号包围）
	//变量：str为可能带有RD的字符串，wordToSeek为需要被替换的字符，replaceContent为替换的内容
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

	//将RD带入RE中，该函数负责遍历所有RD，然后调用replaceByRD来实现替换
	//变量：singleRE为单条RE，RDmap代表RD集合
	static string allReplaceByRD(string singleRE, const map<string, string>& RDmap) {
		for (auto& p : RDmap) {
			//遍历所有RD来进行替换
			//replaceByRD(singleRE, (char)(Brace_left)+ p.first + (char)(Brace_right), p.second);
			replaceByRD(singleRE, p.first, p.second);
		}
			return singleRE;
		}

	//替换单个中括号，将里面对应的字符进行展开，通过ASCII码进行实现
	//变量：str为要进行替换的字符串
	//针对中括号里面的字符，传入数据已经去掉了中括号
	static string singleBracketReplace(string str) {
		string tmp = "";
		queue<char> queue;
		for (int i = 0; i < str.length(); i++) {
			char c = str[i];
			//由于”\\“存储成”\\\\“，故需要额外处理
			if (c == '\\') {
				++i;
				c = str[i];
				queue.push(c);
				tmp.push_back(c);
				continue;
			}
			//e.g. [0-9]，将中间链接符进行替换
			if (c == '-') { queue.push(CHAR_link_bar); tmp.push_back(c); }
			else { queue.push(c); tmp.push_back(c); }
		}
		str = tmp;		
		string rel = "";

		char first = str[0];
		if (first == '^') {
			int ASCcode[127] = { 0 };//ASCII码
			for (int i = 9; i < 127; i++) {
				//将可输入的ASCII位全部置为1
				ASCcode[i] = 1;
			}
			string inverseRel = singleBracketReplaceAU(queue);			 
			for (int i = 0; i < inverseRel.length(); i++) {
				if(inverseRel[i] != '\|')
				ASCcode[inverseRel[i]] = 0;		//需要去掉的符号
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
						//代表特殊字符
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

		//不是'^'的情况
		rel = singleBracketReplaceAU(queue);
		rel = rel.substr(0, rel.length() - 1);//delete last '|'
		return rel;
	}


	//辅助singleBracketReplace函数
	static string singleBracketReplaceAU(queue<char> queue) {
		string rel = "";
		while (!queue.empty()) {
			char first = queue.front();
			queue.pop();
			char second = '不'; //不可能等于的值，相当于置空
			char third = '不';
			if (!queue.empty()) {
				second = queue.front();
			}
			if (second == CHAR_link_bar && second != '不') {   //展开0-9，a-z等
				queue.pop();  //pop掉second
				if (!queue.empty()) {
					third = queue.front();
					queue.pop();
				}
				//char third = queue.front();
				//queue.pop();
				if(third != '不')
				for (auto i = first; i <= third; i++) {  //char型也可以
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
						//代表特殊字符
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
					//代表特殊字符
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
	
	//将RE中的[]全部展开，变成对应的字符
	//变量：str为要进行替换的字符串
	//
	static string replaceBracketPairs(string str) {
		//const string str1 = string("") + static_cast<char>(Bracket_left);
		string str1;  str1.push_back((char)(Bracket_left));
		//const string str2 = string("") + static_cast<char>(Bracket_right);
		string str2;  str2.push_back((char)(Bracket_right));
		int pos1 = 0;
		int pos2 = 0;

		//找到一对对应的中括号
		while (((pos1 = str.find(str1, pos1)) != string::npos) && ((pos2 = str.find(str2, pos1)) != string::npos))
		{
			//pos1 = str.find(str1, pos1);
			//pos2 = str.find(str2, pos2);
			int len = pos2 - pos1;//中括号中字符串长度

			string str3 = "(" + singleBracketReplace(str.substr(pos1 + 1, len - 1)) + ")";

			str = str.replace(pos1, len + 1, str3);
			pos1 += str3.size();
			pos2 += str3.size();
		}
		return str;
	}

	//将“.”替换成任意字符即[\t-~]"
	static string replaceDots(string str) {
		int index = -1;
		for (int i = 0; i < str.length(); i++) {
			//由于“\\"在经过quoteFiler后会变成”\\\\"，故后移两位（“\\"代表输出”\"）
			if (str[i] == '\\')
				i += 2;
			if (str[i] == '.') {
				index = i;
				break;
			}
		}
		if (index != -1) {		//任意字符可以表示ASC码 9-126号
			string t = (char)(Bracket_left)+((char)(9) + string("-") + (char)(126)) + (char)(Bracket_right);
			str.replace(index, 1, t);
		}
		return str;
	}

	//处理非关键字的函数
	//变量：singleRE为单条RE，RDmap代表RD集合
	static string otherFilter(string singleRE, const map<string, string>& RDmap) {
		singleRE = simplyCharacters(singleRE);
		singleRE = allReplaceByRD(singleRE, RDmap);
		singleRE = replaceDots(singleRE);
		singleRE = replaceBracketPairs(singleRE);
		return singleRE;
	}

	//处理关键字的函数
	//变量：str为要进行处理的字符串
	static string keywordFilter(string str) {
		string ans = "";
		str = str.substr(1, str.length() - 2);
		for (char c : str) {
			//为方便判断符号，故将这些符号变成\\..
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


