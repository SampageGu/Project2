#pragma once

#define _CRT_SECURE_NO_WARNINGS  

//���fopen�����Ĳ���ȫ
#include<iostream>
#include<map>
#include<set>
#include<string>
using namespace std;

//��������switch�� switch��varName��{}��   
//case_list�ǲ�ͬcase�ļ���,�ڶ����ǲ�ͬ�����ŵļ��ϣ���һ���Ǵ�ʱҪִ�е����  
//case_list�Եڶ��������н�㣬ִ�е�һ�������
//defaultCase��default�����ִ��
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

// ����һЩԤ����ָ��������Ϣ�Ĳ���
#define ECHO echo_str(yytext)
#define error(x) printf("Line<%d>, Col<%d> : Lexical analysis fail: %s.\n", word_line, word_column+1, x)

// ����һЩȫ�ֱ������ڶ��������ļ�
FILE * yyin;     // �����ļ�ָ��
FILE * yyout;    // ����ļ�ָ��

// ����һЩȫ�ֱ������ڴ洢�ʷ������Ľ��
char yytext[65536]; // �洢��ǰʶ�𵽵ĵ���
int yyleng;         // �洢��ǰʶ�𵽵ĵ��ʵĳ���
int yylineno;       // ��ǰ�к�
int column;         // ��ǰ�к�

// �ʷ������ĺ�������
int yylex(); // ִ�дʷ�����
int yywrap(); // ����Ƿ񵽴�����ĩβ

// ���ڴ洢�������������
char allText[3000000];
// ��ǰ��ȡλ�õ�ָ��
int textPtr = 0;

// ���ڴ� allText �����ж�ȡһ���ַ��ĺ���
char input() {
	char ch = allText[textPtr];
	++textPtr;
	if (ch == '\n') ++yylineno;
	return ch;
}

// ���ڽ���ȡλ�û���һ���ַ��ĺ���
void unput() {
	if (textPtr <= 0) return;
	if (allText[textPtr - 1] == '\n') {
		--yylineno;
	}
	--textPtr;
}

// ����echo���������������ǰ����
void echo_str(char * yytext);

// �洢��ǰ���ʵ��кź��к�
int word_line;
int word_column;
)"";

//����һЩȫ�ֺ����ͱ���
string prepared_program2 = R""(
// DFAState: ���ڴ洢��ǰDFA״̬��ȫ�ֱ���
int DFAState;

// DFANodeIsFinal: һ���������жϵ�ǰ״̬�Ƿ�Ϊ��̬
int DFANodeIsFinal(void);

// DFAResult: һ�����������ݵ�ǰ״̬���ض�Ӧ��ʶ����
int DFAResult(void);

// DFAStateChange: һ������������������ַ��ı�DFA״̬
int DFAStateChange(char cin);    

// yylex: �ʷ���������Ҫ����
int yylex() {
	// ��¼�ʵĿ�ʼλ��
	word_line = yylineno;
	word_column = column;

	// ��ʼ�����ʳ��Ⱥ͵��ʵĵ�һ���ַ�
	yyleng = 1;
	yytext[0] = input();

	// ����Ƿ񵽴������ĩβ
	if (yytext[0] == 0) {
		if (yywrap()) return -1;
	}

	// ��ʼ��DFA��״̬
	DFAState = 0;
	int lastTerState = 0; // ��¼���һ�ε������̬
	int toUnput = 0; // ��¼��Ҫ���˵��ַ���
	int state = DFAStateChange(yytext[0]); // �ı�DFA��״̬

	// ѭ�������ַ�ֱ�����ܽ���
	while (state == 0) {
		if (DFANodeIsFinal()) {  // �����ǰ״̬����̬
			lastTerState = DFAState;
			toUnput = 0;
		}
		else {
			++toUnput;
		}
		++yyleng;
		yytext[yyleng - 1] = input();
		if (yytext[yyleng - 1] == 0) break;			//0��ASC���д���NULL��˵����һ���ַ�Ϊ��
		state = DFAStateChange(yytext[yyleng - 1]);    // �ı�DFA��״̬��������������ַ�����0������������1
	}
	// ���˲�Ӧ�����յ��ַ�
	++toUnput;
	for (int i = 0; i < toUnput; ++i) {
		unput();
		--yyleng;
	}

	// ���û�ж����κ��ַ�����ô����һ���ַ�
	if (yyleng == 0) {
		yyleng = 1;
		yytext[0] = input();
	}
	
	// ���ʵ��ַ�����ʽ��β����Ϊ'\0'
	yytext[yyleng] = '\0';

	// �������һ����̬���ض�Ӧ��ʶ����
	int label = DFAResult();

	return label;
}

// ����һ���㹻����ַ��������ڴ洢�����Ϣ
char out_str[65536];

// echo_str: һ����������ʶ�𵽵Ĵʵ���Ϣ���
void echo_str(char * yytext) {
	sprintf(out_str, " < %d , %d > %s\x06\n", word_line, word_column, yytext);
}
)"";

//��������ļ�����������
string executer = R""(
// �����ļ���������ļ������ַ���
char inputFilename[65536];
char outputFilename[65536];

// ������
int main(int argc, char * argv[]) {
	// Ĭ������£��������Ա�׼����(stdin)���������׼���(stdout)
	yyin = stdin;
	yyout = stdout;

	// Ĭ�ϵ������ļ���������ļ���
	strcpy(inputFilename, "test.c");
	strcpy(outputFilename, "test.txt");

	// ��������в�������Ϊ3�����һ���͵ڶ��������ֱ�Ϊ�����ļ���������ļ���
	if (argc == 3) {
		strcpy(inputFilename, argv[1]);
		strcpy(outputFilename, argv[2]);
	}

	// �������ļ�������ļ�
	FILE * outFile = NULL;

	//����һ���ļ���ʧ��ʱ
	if ((fopen_s(&yyin,inputFilename, "r")) != 0 ||
		(fopen_s(&outFile,outputFilename, "w")) != 0) {
		printf("fail\n");
		return 1;
	};

	// ���������ļ����������ݣ��洢��allText��
	memset(allText, 0, sizeof allText);
	char c = 0;
	int p = 0;
	while ((c = fgetc(yyin)) != EOF) {
		allText[p++] = c;
	}

	// ��ʼ���кź��к�
	yylineno = 1;
	column = 0;

	// ѭ�����ôʷ������������������ļ�����������
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

	// �ر������ļ�������ļ�
	fclose(yyin);
	fclose(outFile);

	return 0;
}
)"";

//�ֱ��������ļ������֣�c99.l�ļ��ĵ�һ�����е�%��%֮��Ĳ��֣�c99.l�ļ��ĵ������֣��������switch��ʾ��DFA
void lexYyCGenerator(string fileName,string c99Lfirst,string c99Lthird,string DFAfunction) {
	FILE* f;
	fopen_s(&f,fileName.c_str(), "w");
	string out = c99Lfirst + prepared_program1 + c99Lthird + prepared_program2 + DFAfunction + executer;
	//if (hasExec) out += executer;
	fprintf(f, "#define _CRT_SECURE_NO_WARNINGS\n%s", out.c_str());
}
