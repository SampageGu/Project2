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

typedef pair<string, string> RegularExpression;//<������ʽ, �﷨����>
typedef pair<string, string> RegularDefinition;//<��������, ��������>

class LexRuleReader
{
public:
	string lexFileContent;//lex�ļ�������
	map<string, string> RDs;//���涨��
	vector<RegularExpression> REs;//������ʽ
	string auxiliaryDefinitions;//�������岿��
	string userSubRoutines;//�û��ӳ��򲿷�
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
		//��������ʽ���ļ���Ϊ�������
		if (regex_search(lexFileContent, match, pattern)) {
			auxiliaryDefinitions = trim(match.str(1));//�������岿��
			REs = parseREs(trim(match.str(2)));//������ʽ
			RDs = parseRDs(trim(match.prefix()));//���涨��
			userSubRoutines = trim(match.suffix());//�û��ӳ��򲿷�
		}
	}
	//ȥ���ַ������˵Ŀո�ͻ��з�
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
	//���ַ�������divider�ָ�õ�һ������
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
	//�������涨��
	map<string, string> parseRDs(string str) {
		map<string, string> RDs = map<string, string>();
		vector<string> lines = spilit(str, "\n");//���ջ��з������涨�岿�ַ�Ϊ����
		for (auto line : lines) {
			vector<string> temp = spilit(line, "\\s+");
			RegularDefinition RD = RegularDefinition(temp[0], temp[1]);
			RDs.insert(RD);
		}
		return RDs;
	}
	//���ļ��еĵ���������ʽ���н���
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
	//����������ʽ����
	vector<RegularExpression> parseREs(string str) {
		vector<RegularExpression> REs = vector<RegularExpression>();
		vector<string> lines = spilit(str, "\n");//���ջ��з���������ʽ���ַ�Ϊ����
		for (auto line : lines) {
			RegularExpression RE = parseRE(line);
			REs.push_back(RE);
		}
		return REs;
	}
	//���������
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

