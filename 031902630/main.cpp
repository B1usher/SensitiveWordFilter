#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
#include<map>
using namespace std;


class TreeNode		//前缀树节点类
{
public:
    bool is_End = false;		//是否为结尾

    void setEnd(bool end, string s)
    {
        is_End = end;
        allMgc = s;
    }       //设置为结尾

    map<char, int> sonNodes;		//子节点的下标
    string allMgc;      //敏感词
};
TreeNode rootNode;
TreeNode treeNode[3000];
int nodeNum = 0;

void createTree(string s)
{
    TreeNode* tempNode = &rootNode;       //临时节点
    for (int i = 0; i < s.size(); i++)
    {
        char c = s[i];

        map<char, int>::iterator iter = tempNode->sonNodes.find(c);
        if (iter == tempNode->sonNodes.end())
        {
            tempNode->sonNodes.insert(map<char, int>::value_type(c, nodeNum));
            tempNode = &treeNode[nodeNum];
            nodeNum++;
        }
        else
            tempNode = &treeNode[iter->second];

        if (i == s.size() - 1)
        {
            tempNode->setEnd(true, s);
        }
    }
}


void readMgc()    //读入敏感词
{
    string s;
    ifstream inMgc("words.txt");
    if (!inMgc.is_open())
        cout << "can not open words.txt" << endl;

    while (inMgc >> s)
    {
        cout << s << endl;
        createTree(s);
    }
    inMgc.close();
}

void readFile()		//读入待检测文本
{
    ifstream inFile("org.txt");
    if (!inFile.is_open())
        cout << "can not open org.txt" << endl;




    inFile.close();
}

void writeFile()		//输出答案
{
    ofstream outFile("ans.txt", ios::trunc);

    outFile << 12678;

    outFile.close();
}

int main()
{
    SetConsoleOutputCP(65001);		//将cmd设置为utf-8编码


    readMgc();

    readFile();

    writeFile();

    return 0;
}