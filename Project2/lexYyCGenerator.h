#pragma once

#define _CRT_SECURE_NO_WARNINGS  

//解决fopen函数的不安全
#include<iostream>
#include<map>
#include<set>
#include<string>
using namespace std;

//用来生成switch， switch（varName）{}。   
//case_list是不同case的集合,第二项是不同情况序号的集合，第一项是此时要执行的语句  
//case_list对第二项中所有结点，执行第一项中语句
//defaultCase是default情况的执行
string switchGenerator(string varName, map<string,set<int>> case_list, string defaultCase = "error(\"\");") {
	string start = "switch(" + varName + ") {";
	string middle = "";
	for (auto eachCase : case_list) {
		string caseInside = "";
		for (int i : eachCase.second) {
			caseInside += " case " + to_string(i) + " : ";
		}
		//caseInside += "{ " + eachCase.first + " break; }";
		middle += caseInside + " { " + eachCase.first + " break; } ";
	}
	string end = " default:  { " + defaultCase + " } break; }";
	string rel = start + middle + end;
	return rel;
}


string prepared_program1 = R""(
#include <string.h>

// 定义一些预处理指令简化输出信息的操作
#define ECHO echo_str(yytext)
#define error(x) printf("Line<%d>, Col<%d> : Lexical analysis fail: %s.\n", word_line, word_column+1, x)

// 定义一些全局变量用于读入和输出文件
FILE * yyin;     // 输入文件指针
FILE * yyout;    // 输出文件指针

// 定义一些全局变量用于存储词法分析的结果
char yytext[65536]; // 存储当前识别到的单词
int yyleng;         // 存储当前识别到的单词的长度
int yylineno;       // 当前行号
int column;         // 当前列号

// 词法分析的函数声明
int yylex(); // 执行词法分析
int yywrap(); // 检查是否到达输入末尾

// 用于存储整个输入的数组
char allText[3000000];
// 当前读取位置的指针
int textPtr = 0;

// 用于从 allText 数组中读取一个字符的函数
char input() {
	char ch = allText[textPtr];
	++textPtr;
	if (ch == '\n') ++yylineno;
	return ch;
}

// 用于将读取位置回退一个字符的函数
void unput() {
	if (textPtr <= 0) return;
	if (allText[textPtr - 1] == '\n') {
		--yylineno;
	}
	--textPtr;
}

// 声明echo函数，用于输出当前单词
void echo_str(char * yytext);

// 存储当前单词的行号和列号
int word_line;
int word_column;
)"";

//定义一些全局函数和变量
string prepared_program2 = R""(
// DFAState: 用于存储当前DFA状态的全局变量
int DFAState;

// DFANodeIsFinal: 一个函数，判断当前状态是否为终态
int DFANodeIsFinal(void);

// DFAResult: 一个函数，根据当前状态返回对应的识别结果
int DFAResult(void);

// DFAStateChange: 一个函数，根据输入的字符改变DFA状态
int DFAStateChange(char cin);    

// yylex: 词法分析的主要函数
int yylex() {
	// 记录词的开始位置
	word_line = yylineno;
	word_column = column;

	// 初始化单词长度和单词的第一个字符
	yyleng = 1;
	yytext[0] = input();

	// 检查是否到达输入的末尾
	if (yytext[0] == 0) {
		if (yywrap()) return -1;
	}

	// 初始化DFA的状态
	DFAState = 0;
	int lastTerState = 0; // 记录最近一次到达的终态
	int toUnput = 0; // 记录需要回退的字符数
	int state = DFAStateChange(yytext[0]); // 改变DFA的状态

	// 循环读入字符直到不能接收
	while (state == 0) {
		if (DFANodeIsFinal()) {  // 如果当前状态是终态
			lastTerState = DFAState;
			toUnput = 0;
		}
		else {
			++toUnput;
		}
		++yyleng;
		yytext[yyleng - 1] = input();
		if (yytext[yyleng - 1] == 0) break;			//0在ASC码中代表NULL，说明下一个字符为空
		state = DFAStateChange(yytext[yyleng - 1]);    // 改变DFA的状态，正常接收这个字符返回0，不正常返回1
	}
	// 回退不应被接收的字符
	++toUnput;
	for (int i = 0; i < toUnput; ++i) {
		unput();
		--yyleng;
	}

	// 如果没有读入任何字符，那么读入一个字符
	if (yyleng == 0) {
		yyleng = 1;
		yytext[0] = input();
	}
	
	// 将词的字符串形式结尾处设为'\0'
	yytext[yyleng] = '\0';

	// 根据最后一个终态返回对应的识别结果
	int label = DFAResult();

	return label;
}

// 定义一个足够大的字符串，用于存储输出信息
char out_str[65536];

// echo_str: 一个函数，将识别到的词的信息输出
void echo_str(char * yytext) {
	sprintf(out_str, " < %d , %d > %s\x06\n", word_line, word_column, yytext);
}
)"";

//定义输出文件的主函数等
string executer = R""(
// 输入文件名和输出文件名的字符串
char inputFilename[65536];
char outputFilename[65536];

// 主函数
int main(int argc, char * argv[]) {
	// 默认情况下，输入来自标准输入(stdin)，输出到标准输出(stdout)
	yyin = stdin;
	yyout = stdout;

	// 默认的输入文件名和输出文件名
	strcpy(inputFilename, "test.c");
	strcpy(outputFilename, "test.txt");

	// 如果命令行参数数量为3，则第一个和第二个参数分别为输入文件名和输出文件名
	if (argc == 3) {
		strcpy(inputFilename, argv[1]);
		strcpy(outputFilename, argv[2]);
	}

	// 打开输入文件和输出文件
	FILE * outFile = NULL;

	//任意一个文件打开失败时
	if ((fopen_s(&yyin,inputFilename, "r")) != 0 ||
		(fopen_s(&outFile,outputFilename, "w")) != 0) {
		printf("fail\n");
		return 1;
	};

	// 读入输入文件的所有内容，存储到allText中
	memset(allText, 0, sizeof allText);
	char c = 0;
	int p = 0;
	while ((c = fgetc(yyin)) != EOF) {
		allText[p++] = c;
	}

	// 初始化行号和列号
	yylineno = 1;
	column = 0;

	// 循环调用词法分析器，处理输入文件的所有内容
	while (1) {
		out_str[0] = '\0';
		int x = yylex();
		if (x == -1) break;
		if (x != 0){
			char echoed_str[65536] = "";
			sprintf(echoed_str, "%d", x);
			strcat(echoed_str, out_str);
			fprintf(outFile, "%s", echoed_str);
		}
	}

	// 关闭输入文件和输出文件
	fclose(yyin);
	fclose(outFile);

	return 0;
}
)"";

//分别传入生成文件的名字，c99.l文件的第一部分中的%和%之间的部分，c99.l文件的第三部分，构造的用switch表示的DFA
void lexYyCGenerator(string fileName,string c99Lfirst,string c99Lthird,string DFAfunction) {
	FILE* f;
	fopen_s(&f,fileName.c_str(), "w");
	string out = c99Lfirst + prepared_program1 + c99Lthird + prepared_program2 + DFAfunction + executer;
	//if (hasExec) out += executer;
	fprintf(f, "#define _CRT_SECURE_NO_WARNINGS\n%s", out.c_str());
}
