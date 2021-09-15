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

    map<string, int> sonNodes;		//子节点的下标
    string allMgc;      //敏感词
};
TreeNode rootNode;
TreeNode treeNode[3000];
int nodeNum = 0;

void createTree(string s)       //创建树
{
    TreeNode* tempNode = &rootNode;       //临时节点
    for (int i = 0; i < s.size();)
    {
        string c;
        char l = s[i];

        if (l < 0)       //是汉字或中文字符
        {
            c = s.substr(i, 3);
            i += 3;
        }
        else
        {
            if (l >= 'a' && l <= 'z')       //是英文
                l -= 32;
            c = l;
            i++;
        }

        map<string, int>::iterator iter = tempNode->sonNodes.find(c);
        if (iter == tempNode->sonNodes.end())
        {
            tempNode->sonNodes.insert(map<string, int>::value_type(c, nodeNum));      //添加子节点
            tempNode = &treeNode[nodeNum];
            nodeNum++;
        }
        else
            tempNode = &treeNode[iter->second];

        if (i == s.size())
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
        string c;
        char l = s[position];

        if (l < 0)       //是汉字或中文字符
        {
            c = s.substr(position, 3);
            position += 2;
        }
        else
        {
            if (l >= 'a' && l <= 'z')       //是英文
                l -= 32;
            c = l;
        }

        if (l < 0)      //中文内容
        {
            map<string, int>::iterator iter = tempNode->sonNodes.find(c);
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
                    char w = s[position];       //下一个字
                    if (w < 0)       //是汉字或中文字符
                    {
                        c = s.substr(position, 3);
                        position += 2;
                    }
                    else
                    {
                        if (w >= 'a' && w <= 'z')       //是英文
                            w -= 32;
                        c = w;
                    }

                    if (w < 0)      //中文
                    {
                        iter = tempNode->sonNodes.find(c);
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
                    else if ((w >= 'a' && w <= 'z') || (w >= 'A' && w <= 'Z') || (w >= '0' && w <= '9'))        //中文中间有英文和数字
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
        else if ((l >= 'a' && l <= 'z') || (l >= 'A' && l <= 'Z'))      //英文内容
        {
            map<string, int>::iterator iter = tempNode->sonNodes.find(c);
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
                    char w = s[position];       //下一个字
                    if (w < 0)       //是汉字或中文字符
                    {
                        c = s.substr(position, 3);
                        position += 2;
                    }
                    else
                    {
                        if (w >= 'a' && w <= 'z')       //是英文
                            w -= 32;
                        c = w;
                    }

                    if (w < 0)      //英文中间有中文
                    {
                        begin++;
                        break;
                    }
                    else if ((w >= 'a' && w <= 'z') || (w >= 'A' && w <= 'Z'))      //英文
                    {
                        iter = tempNode->sonNodes.find(c);
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