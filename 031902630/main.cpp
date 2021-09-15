#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
#include<map>
#include<locale>
#include<cstdlib>
using namespace std;

class Answer        //答案类
{
public:
    int Line;       //行号
    string Mgc;     //敏感词
    string Text;     //文本
};
Answer answer[10000];
int ansTotal = 0;

class TreeNode		//前缀树节点类
{
public:
    bool is_End = false;		//是否为结尾

    void setEnd(string s)       //设置为结尾
    {
        is_End = true;
        allMgc = s;
    }

    map<char, int> sonNodes;		//子节点的下标
    string allMgc;      //敏感词
};
TreeNode rootNode;
TreeNode treeNode[3000];
int nodeNum = 0;

void createTree(string s)       //创建树
{
    TreeNode* tempNode = &rootNode;       //临时节点
    for (int i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (c >= 'a' && c <= 'z')
            c -= 32;

        map<char, int>::iterator iter = tempNode->sonNodes.find(c);
        if (iter == tempNode->sonNodes.end())
        {
            tempNode->sonNodes.insert(map<char, int>::value_type(c, nodeNum));      //添加子节点
            tempNode = &treeNode[nodeNum];
            nodeNum++;
        }
        else
            tempNode = &treeNode[iter->second];

        if (i == s.size() - 1)
        {
            tempNode->setEnd(s);
        }
    }
}

void searchMgc(string s, int line)        //搜索敏感词
{
    TreeNode* tempNode = &rootNode;     //临时节点
    int begin = 0;      //开始位置
    int position = 0;       //当前位置

    for (; begin < s.size();)
    {
        position = begin;
        tempNode = &rootNode;
        char c = s[begin];
        if (c >= 'a' && c <= 'z')
            c -= 32;

        if (c < 0)      //中文内容
        {
            map<char, int>::iterator iter = tempNode->sonNodes.find(c);
            if (iter != tempNode->sonNodes.end())
            {
                int charNum = 0;
                tempNode = &treeNode[iter->second];

                for (charNum = 0; charNum < 20;)
                {
                    if (tempNode->is_End == true)       //匹配成功
                    {
                        answer[ansTotal].Line = line;
                        answer[ansTotal].Mgc = tempNode->allMgc;
                        answer[ansTotal].Text = s.substr(begin, position - begin + 1);
                        ansTotal++;
                        begin = position + 1;
                        break;
                    }

                    position++;
                    char w = s[position];
                    if (w >= 'a' && w <= 'z')
                        w -= 32;

                    if (w < 0)
                    {
                        iter = tempNode->sonNodes.find(w);
                        if (iter != tempNode->sonNodes.end())
                        {
                            charNum = 0;
                            tempNode = &treeNode[iter->second];
                        }
                        else
                        {
                            begin += 3;
                            break;
                        }
                    }
                    else if ((w >= 'a' && w <= 'z') || (w >= 'A' && w <= 'Z') || (w >= '0' && w <= '9'))
                    {
                        begin += 3;
                        break;
                    }
                    else
                        charNum++;
                }

                if (charNum >= 20)
                    begin += 3;
            }
            else
                begin += 3;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))      //英文内容
        {
            map<char, int>::iterator iter = tempNode->sonNodes.find(c);
            if (iter != tempNode->sonNodes.end())
            {
                int charNum = 0;
                tempNode = &treeNode[iter->second];
                
                for (charNum = 0; charNum < 20;)
                {
                    if (tempNode->is_End == true)       //匹配成功
                    {
                        answer[ansTotal].Line = line;
                        answer[ansTotal].Mgc = tempNode->allMgc;
                        answer[ansTotal].Text = s.substr(begin, position - begin + 1);
                        ansTotal++;
                        begin = position + 1;
                        break;
                    }

                    position++;
                    char w = s[position];
                    if (w >= 'a' && w <= 'z')
                        w -= 32;

                    if (w < 0)
                    {
                        begin++;
                        break;
                    }
                    else if ((w >= 'a' && w <= 'z') || (w >= 'A' && w <= 'Z'))
                    {
                        iter = tempNode->sonNodes.find(w);
                        if (iter != tempNode->sonNodes.end())
                        {
                            charNum = 0;
                            tempNode = &treeNode[iter->second];
                        }
                        else
                        {
                            begin++;
                            break;
                        }
                    }
                    else
                        charNum++;
                }

                if (charNum >= 20)
                    begin++;
            }
            else
                begin++;
        }
        else        //字符内容
            begin++;
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
        createTree(s);      //创建树
    }

    inMgc.close();
}

void readFile()		//读入待检测文本
{
    string s;
    int line = 1;
    ifstream inFile("org.txt");
    if (!inFile.is_open())
        cout << "can not open org.txt" << endl;

    while (getline(inFile, s))
    {
        searchMgc(s, line);       //搜索敏感词
        line++;
    }

    inFile.close();
}

void writeFile()		//输出答案
{
    ofstream outFile("ans.txt", ios::trunc);

    outFile << "Total: " << ansTotal << endl;
    for (int i = 0; i < ansTotal; i++)
    {
        outFile << "Line" << answer[i].Line << ": <" << answer[i].Mgc << "> " << answer[i].Text << endl;
    }

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