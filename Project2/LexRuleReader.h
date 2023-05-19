#pragma once
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<map>
#include<vector>
#include<streambuf>
#include<regex>
using namespace std;

typedef pair<string, string> RegularExpression;//<正规表达式, 语法动作>
typedef pair<string, string> RegularDefinition;//<定义名称, 定义内容>

class LexRuleReader
{
public:
	string lexFileContent;//lex文件的内容
	map<string, string> RDs;//正规定义
	vector<RegularExpression> REs;//正规表达式
	string auxiliaryDefinitions;//辅助定义部分
	string userSubRoutines;//用户子程序部分
public:
	LexRuleReader(string lexFileName) {
		ifstream ifile(lexFileName);
		string str((std::istreambuf_iterator<char>(ifile)),
			std::istreambuf_iterator<char>());	
		lexFileContent = str;
	}
	void parseRules() {
		regex pattern("%\\{([\\s|\\S]+?)%\\}[\\s|\\S]*?%%([\\s|\\S]+?)%%");
		smatch match;
		//用正则表达式将文件分为多个部分
		if (regex_search(lexFileContent, match, pattern)) {
			auxiliaryDefinitions = trim(match.str(1));//辅助定义部分
			REs = parseREs(trim(match.str(2)));//正规表达式
			RDs = parseRDs(trim(match.prefix()));//正规定义
			userSubRoutines = trim(match.suffix());//用户子程序部分
		}
	}
	//去除字符串两端的空格和换行符
	string trim(string s){
		if (s.empty())
		{
			return s;
		}
		s.erase(0, s.find_first_not_of("\n"));
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		s.erase(s.find_last_not_of("\n") + 1);
		return s;
	}
	//将字符串按照divider分割得到一个数组
	vector<string> spilit(string str, string divider) {
		vector<string> strVec = vector<string>();
		regex reg(divider);
		sregex_token_iterator pos(str.begin(), str.end(), reg, -1);
		decltype(pos) end;
		for (; pos != end; ++pos)
		{
			if (pos->str().length()>0) {
				//cout << pos->str() << endl;
				strVec.push_back(pos->str());
			}
		}
		return strVec;
	}
	//解析正规定义
	map<string, string> parseRDs(string str) {
		map<string, string> RDs = map<string, string>();
		vector<string> lines = spilit(str, "\n");//按照换行符将正规定义部分分为多行
		for (auto line : lines) {
			vector<string> temp = spilit(line, "\\s+");
			RegularDefinition RD = RegularDefinition(temp[0], temp[1]);
			RDs.insert(RD);
		}
		return RDs;
	}
	//对文件中的单行正规表达式进行解析
	RegularExpression parseRE(string str) {
		regex pattern("\\s+\\{([\\s|\\S]+?)\\}");
		smatch match;
		RegularExpression RE = RegularExpression();
		if (regex_search(str, match, pattern)) {
			RE.first = match.prefix();
			RE.second = "{" + match.str(1) + "}";
		}
		return RE;
	}
	//解析正规表达式部分
	vector<RegularExpression> parseREs(string str) {
		vector<RegularExpression> REs = vector<RegularExpression>();
		vector<string> lines = spilit(str, "\n");//按照换行符将正规表达式部分分为多行
		for (auto line : lines) {
			RegularExpression RE = parseRE(line);
			REs.push_back(RE);
		}
		return REs;
	}
	//输出测试用
	string toString() {
		string str = "";
		str += "RDs:\n";
		for (auto RD : RDs) {
			str += RD.first + " " + RD.second + "\n";
		}
		str += "AuxiliaryDefinitions:\n" + auxiliaryDefinitions + "\n";
		str += "REs:\n";
		for (auto RE : REs) {
			str += RE.first + " " + RE.second + "\n";
		}
		str += "userSubRoutine:\n" + userSubRoutines + "\n";
		return str;
	}
};

