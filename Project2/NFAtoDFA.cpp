#include <map>
#include <set>
#include <queue>
#include <vector>
#include<stack>
#include <iostream>
#define CHAR_NULL 127
using namespace std;
#define MAX 999999
struct NFANode {
    int id=-1;  //id为负表示没有被初始化
    char input1 = CHAR_NULL;
    NFANode* next1 = NULL;
    char input2 = CHAR_NULL;
    NFANode* next2 = NULL;
};
struct DFANode {
    int id=-1;
    int pos = 0;
    map<char, DFANode*> ptrs;//转移方向
} DFANodes[MAX];
set<int> epolisonSet[MAX];//记录所有闭包集合，表格左边的部分  如epolisonSet[1]表示1可以到达的所有闭包
map<char, set<int> > NFAStateTable[MAX];
//记录所有目标状态，char指输入的字符，set指转移后的闭包结果  Statetable[1]['a']表示1状态在接受a字符后可以到达的闭包
map<char, set<int>>DFAStateTable[MAX];
vector<char> allowed_chars;//对应ASCII码，记录可以输入的字符
NFANode nfanodes[MAX]; // 存储所有的 NFANode 对象
void printNFAtable();

int REpos[65536];//记录节点对应的执行语句
void initallow_chars() {
    for (int i = 9; i < 127; ++i) {
        if (true/*i != '#'*/) {
            //记录下ASCII码
            allowed_chars.push_back(i);
        }
    }
    memset(REpos, 0, sizeof REpos);
}
NFANode* getNodeById(int id) {
    return &nfanodes[id]; // 假设 id 就是节点在 vector 中的索引
}
NFANode  initNFA1()
{
    //第二组测试用例
  // 创建NFA
    NFANode node0, node1, node2, node3, node4, node5;
    node0.id = 0; node0.input1 = 'a';
    node1.id = 1; node1.input1 = 'b'; node1.input2 = '\0';
    node2.id = 2; node2.input1 = 'c'; node2.input2 = 'd';
    node3.id = 3; node3.input1 = '\0';
    node4.id = 4; node4.input1 = 'e'; node4.input2 = '\0';
    node5.id = 5; node5.input1 = node5.input2 = CHAR_NULL;

    // 添加到nodes中
    nfanodes[0] = node0;
    nfanodes[1] = node1;
    nfanodes[2] = node2;
    nfanodes[3] = node3;
    nfanodes[4] = node4;
    nfanodes[5] = node5;

    // 设置next1和next2
    nfanodes[0].next1 = getNodeById(1);
    nfanodes[1].next1 = getNodeById(2);
    nfanodes[2].next1 = getNodeById(3); nfanodes[2].next2 = getNodeById(4);
    nfanodes[4].next1 = getNodeById(5);

    // 设置epsilon边
    nfanodes[3].next1 = getNodeById(4);
    nfanodes[1].next2 = getNodeById(3);
    nfanodes[4].next2 = getNodeById(2);
    return node0;
}
NFANode initNFA2()
{
    //第二组测试用例
  // 创建NFA
    NFANode node0, node1, node2, node3, node4, node5, node6, node7, node8, node9;

    // 初始化各个状态
    node0.id = 0; node0.input1 = node0.input2 = '\0'; // 起始状态，仅通过epsilon转移
    node1.id = 1; node1.input1 = node1.input2 = '\0'; // 仅通过epsilon转移
    node2.id = 2; node2.input1 = 'a'; 
    node3.id = 3; node3.input1 =  '\0'; // 仅通过epsilon转移
    node4.id = 4; node4.input1 = 'b'; 
    node5.id = 5; node5.input1 = '\0'; // 仅通过epsilon转移
    node6.id = 6; node6.input1 = node6.input2 = '\0'; // 仅通过epsilon转移
    node7.id = 7; node7.input1 = 'a'; 
    node8.id = 8; node8.input1 = 'b'; 
    node9.id = 9; node9.input1 = node9.input2 = CHAR_NULL; // 终点状态，没有出度

    // 添加到nfanodes中
    nfanodes[0] = node0;
    nfanodes[1] = node1;
    nfanodes[2] = node2;
    nfanodes[3] = node3;
    nfanodes[4] = node4;
    nfanodes[5] = node5;
    nfanodes[6] = node6;
    nfanodes[7] = node7;
    nfanodes[8] = node8;
    nfanodes[9] = node9;

    // 设置next1和next2
    nfanodes[0].next1 = getNodeById(1);
    nfanodes[0].next2 = getNodeById(7);
    nfanodes[1].next1 = getNodeById(2);
    nfanodes[1].next2 = getNodeById(4);
    nfanodes[2].next1 = getNodeById(3);
    nfanodes[4].next1 = getNodeById(5);
    nfanodes[3].next1 = getNodeById(6);
    nfanodes[5].next1 = getNodeById(6);
    nfanodes[6].next1 = getNodeById(1);
    nfanodes[6].next2 = getNodeById(7);
    nfanodes[7].next1 = getNodeById(8);
    nfanodes[8].next1 = getNodeById(9);

    return node0;
}

set<NFANode*> epsilonClosure(const set<NFANode*>& stateSet) {
    set<NFANode*> closure = stateSet;
    stack<NFANode*> s;
    set<NFANode*> visited;

    for (NFANode* state : stateSet) {
        s.push(state);
        visited.insert(state);
    }

    while (!s.empty()) {
        NFANode* t = s.top(); s.pop();
        if (t->input1 == CHAR_NULL && !visited.count(t->next1)) {
            closure.insert(t->next1);
            s.push(t->next1);
            visited.insert(t->next1);
        }
        if (t->input2 == CHAR_NULL && !visited.count(t->next2)) {
            closure.insert(t->next2);
            s.push(t->next2);
            visited.insert(t->next2);
        }
    }
    return closure;
}

//求一组状态的闭包
//输入元素set1为一系列的节点编号
void getStateClosure(set<int>& set1) {

    queue<int> que;
    for (int id : set1) {
        que.push(id);
    }
    //使用BFS搜索 从一个编号出去经过epolison所有可到达的节点
    while (!que.empty()) {
        int id = que.front();
        que.pop();
        NFANode& p = nfanodes[id];
      /*  if (p.input1 != '\0') continue;*/
        //判断是否为新状态，通过find函数遍历set，若无该元素，则返回set1.end()
        //先判断第一条边，如果指向的是epolison，并且在set集合里没有这个编号，那么将该编号添加到set集合里
        if (p.input1=='\0'&&set1.find(p.next1->id) == set1.end())
        {
            set1.insert(p.next1->id);
            que.push(p.next1->id);
        }
        /*if (p.input2 != '\0') continue;*/
        //再判断第二条边，如果指向的是epolison，并且在set集合里没有这个编号，那么将该编号添加到set集合里
        if (p.input2=='\0'&&set1.find(p.next2->id) == set1.end()) {
            set1.insert(p.next2->id);
            que.push(p.next2->id);
        }
    }
   /* if (!set1.empty())
    {
        cout << "这里输出set1:";
        for (const auto& element : set1) {
            cout << element << " ";
        }
        cout << endl;
    }*/
}
void initClosure(NFANode* startNode) {
    // 起始状态一定是输入空转状态
    //先将起始节点放入状态集合中
    epolisonSet[0].insert(startNode->id);
    //求出epil（0）的结果，放入state_set中
    getStateClosure(epolisonSet[0]);
}
int NFAtoDFATable(NFANode* startNode) {
    initClosure(startNode);
    //记录共有多少种状态DFA
    int last_state_idx = 0;
    //当没有新状态时，停止循环
    for (int i = 0; i <= last_state_idx; ++i) {
        //初始状态
        auto& srcState = epolisonSet[i];
        //目标转移状态
        auto& tgtTable = NFAStateTable[i];
        //遍历可使用的ASCII码
        for (char c : allowed_chars)
        {
            //获取能够接受目标字符的状态
            auto& table = tgtTable[c];
            for (int id : srcState)
            {
                //获取当前id对应的NFA节点
                auto& node = nfanodes[id];
                //当输入字符c时，能够跳转到新状态，记录下新状态的id
                //map中不存在相同节点
                if (node.input1 == c) {
                  //  cout << c << "\t"<<id<<"\t input1 match successfully" << endl;
                    table.insert(node.next1->id);
                }
                if (node.input2 == c) {
                   // cout << c << "\t" << id << "\t input2 match successfully" << endl;
                    table.insert(node.next2->id);
                }
            }
            //寻找这组目标状态的等价类
            getStateClosure(table);

            if (!table.empty()) {
                //记录是否为新状态
                bool newOne = true;
                //寻找是否已经有当前集合
                for (int j = 0; j <= last_state_idx; ++j) {
                    if (table == epolisonSet[j]) {
                        newOne = false;
                        break;
                    }
                }
                //新状态放入状态表中
                if (newOne) {
                    epolisonSet[++last_state_idx] = table;
                }
            }
        }

    }
    printNFAtable();
    return last_state_idx + 1;
}

void printNFAtable()
{
    std::cout << "Non-empty elements in epolisonSet:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        if (!epolisonSet[i].empty()) {
            std::cout << "epolisonSet[" << i << "]: ";
            for (const auto& element : epolisonSet[i]) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
    }

    // 打印 NFAStateTable 中不为空的元素
    std::cout << "Non-empty elements in NFAStateTable:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        for (const auto& kvp : NFAStateTable[i]) {
            if (!kvp.second.empty()) {
                std::cout << "NFAStateTable[" << i << "]['" << kvp.first << "']: ";
                for (const auto& element : kvp.second) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            }
        }
    }

}

//构建DFA图
//变量：stateCnt为DFA状态数量
void constructDFA(int stateNum) {
    for (int i = 0; i < stateNum; ++i) {
        DFANodes[i].id = i;
        DFANodes[i].pos = 0;
        //遍历当前集合中的所有NFA节点
        //for (int j : epolisonSet[i]) {
        //    //当前节点为可接受状态
        //    if (REpos[j] != 0) {
        //        //记录当前节点所对应的RE后面的执行语句在map中的位置
        //        //当对应多个结果时，优先输出先定义的
        //        if (DFANodes[i].pos == 0 || REpos[j] < DFANodes[i].pos)
        //            DFANodes[i].pos = REpos[j];
        //    }
        //}
        //获得当前状态对应的所有目标状态（表格右边部分）
        map<char, set<int> > thisTable = NFAStateTable[i];
        for (char c : allowed_chars)
        {
            //判断是否为通路
            if (thisTable[c].empty()) continue;
            //由于是通路，可以进行状态转移，获取当前字符对应的状态集合
            auto& tgtState = thisTable[c];      //tgtState为当前状态i在输入c字符下能够转移到的状态
            for (int j = 0; j < stateNum; ++j) {
                //寻找到当前节点对应的节点，进行链接
                if (tgtState == epolisonSet[j]) {
                    DFANodes[i].ptrs[c] = &DFANodes[j];
                    break;
                }
            }
        }
        
    }
}
void printNFA()
{
    // Print NFA
    cout << "NFA:\n";
    for (const auto& node : nfanodes) {
        if (node.id >= 0)
        {
            if (node.input1 != CHAR_NULL && node.input1 != '\0') {
                cout << "State " << node.id << ": On input '" << node.input1 << "', transition to state " << node.next1->id << ".\n";
            }
            else if (node.input1 == '\0') {
                cout << "State " << node.id << ": On epsilon transition to state " << node.next1->id << ".\n";
            }

            if (node.input2 != CHAR_NULL && node.input2 != '\0') {
                cout << "State " << node.id << ": On input '" << node.input2 << "', transition to state " << node.next2->id << ".\n";
            }
            else if (node.input2 == '\0') {
                cout << "State " << node.id << ": On epsilon transition to state " << node.next2->id << ".\n";
            }

            if (node.input1 == CHAR_NULL && node.input2 == CHAR_NULL && node.next1 == NULL && node.next2 == NULL) {
                cout << "State " << node.id << ": Final state.\n";
            }
        }
    }
}
void printDFA()
{
    for (int i = 0; i < MAX; ++i) {
        if (DFANodes[i].id >= 0)
        {  // 或其他你设定的“已经初始化”的条件
            //cout << "DFANode " << i << ": " << endl;
            cout << "ID: " << DFANodes[i].id << endl;
            /*cout << "POS: " << DFANodes[i].pos << endl;*/
            cout << "Ptrs: " << endl;
            for (const auto& pair : DFANodes[i].ptrs) {
                cout << "    " << pair.first << ": " << pair.second->id << endl;  // 假设这里 pair.second 指向的 DFANode 已经初始化
            }
            cout << "------------------" << endl;
        }
      
    }

}
void initDFAStateTable()
{
    for (int i = 0; i < MAX; i++)
    {
        if (DFANodes[i].id >= 0)
        {
            for (char c : allowed_chars)
            {
                if (DFANodes[i].ptrs[c] != NULL)
                {
                    int tempid = DFANodes[i].ptrs[c]->id;
                    DFAStateTable[i][c].insert(tempid);
                }
            }
            
        }
    }
}
void printDFATable()
{
    // 打印 NFAStateTable 中不为空的元素
    std::cout << "Non-empty elements in DFAStateTable:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        for (const auto& kvp : DFAStateTable[i]) {
            if (!kvp.second.empty()) {
                std::cout << "DFAStateTable[" << i << "]['" << kvp.first << "']: ";
                for (const auto& element : kvp.second) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}
int main() {
    initallow_chars();
    NFANode node0=initNFA2();
   printNFA();
   int state=NFAtoDFATable(&node0);
   constructDFA(state);
   printDFA();
   initDFAStateTable();
   printDFATable();
   //if(DFANodes[0].ptrs['c']==NULL)
   //    cout<<"null";
    return 0;
}

