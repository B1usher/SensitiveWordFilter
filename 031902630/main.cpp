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
Answer answer[30010];
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
TreeNode treeNode[10010];
int nodeNum = 0;

map<string, string> splitWord;      //汉字拆分为偏旁部首
void readDictionary(string words)    //读入字典
{
    string s;
    ifstream inDictionary;
    inDictionary.open(words);
    if (!inDictionary.is_open())
        cout << "can not open Dictionary.txt" << endl;

    string beforeSplit;     //拆分前原字
    string afterSplit;      //拆分后偏旁部首
    int begin = 0;;
    int position = 0;

    while (getline(inDictionary, s))
    {
        begin = 0;
        position = 0;
        for (; position < s.size(); position++)
        {
            if (s[position] == 34)       //读到第一个前"
            {
                begin = position + 1;
                for (position++; position < s.size(); position++)
                {
                    if (s[position] == 34)      //读到第一个后"
                    {
                        beforeSplit = s.substr(begin, position - begin);
                        break;
                    }
                }

                for (position++; position < s.size(); position++)
                {
                    if (s[position] == 34)      //读到第二个前"
                    {
                        begin = position + 1;
                        for (position++; position < s.size(); position++)
                        {
                            if (s[position] == 34)      //读到第二个后"
                            {
                                afterSplit = s.substr(begin, position - begin);
                                break;
                            }
                        }
                        break;
                    }
                }

                splitWord.insert(map<string, string>::value_type(beforeSplit, afterSplit));     //插入对应关系
                break;
            }
        }
    }

    inDictionary.close();
}

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
            int keepNum = nodeNum;      //保存最终下标
            nodeNum++;

            map<string, string>::iterator iterNew = splitWord.find(c);
            if (iterNew != splitWord.end())     //在字典中找到了拆分方式
            {
                string afterSplit = iterNew->second;
                for (int i = 0; i < afterSplit.size();)     //添加另一条路径
                {
                    string single;      //单个汉字（偏旁部首）
                    single = afterSplit.substr(i, 3);

                    i += 3;     //下一插入部分
                    if (i < afterSplit.size())      //下一部分不是最后一部分
                    {
                        tempNode->sonNodes.insert(map<string, int>::value_type(single, nodeNum));
                        tempNode = &treeNode[nodeNum];
                        nodeNum++;
                    }
                    else
                        tempNode->sonNodes.insert(map<string, int>::value_type(single, keepNum));
                }
            }

            tempNode = &treeNode[keepNum];
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

void readMgc(char* words)    //读入敏感词    void readMgc()      //
{
    string s;
    ifstream inMgc;
    inMgc.open(words);  //    inMgc.open("words.txt");        //
    if (!inMgc.is_open())
        cout << "can not open words.txt" << endl;

    while (inMgc >> s)
    {
        createTree(s);      //创建树
    }

    inMgc.close();
}

void readFile(char* org)		//读入待检测文本   void readFile()     //
{
    string s;
    int line = 1;
    ifstream inFile;
    inFile.open(org);   //    inFile.open("org.txt");     //
    if (!inFile.is_open())
        cout << "can not open org.txt" << endl;

    while (getline(inFile, s))
    {
        searchMgc(s, line);       //搜索敏感词
        line++;
    }

    inFile.close();
}

void writeFile(char* ans)		//输出答案  void writeFile()        //
{
    ofstream outFile;
    outFile.open(ans);  //    outFile.open("ans.txt");        //
    if (!outFile.is_open())
        cout << "can not open ans.txt" << endl;

    outFile << "Total: " << ansTotal << endl;
    for (int i = 0; i < ansTotal; i++)
    {
        outFile << "Line" << answer[i].Line << ": <" << answer[i].Mgc << "> " << answer[i].Text << endl;
    }

    outFile.close();
}

int main(int argc, char* argv[])    //int main()        //
{
    SetConsoleOutputCP(65001);		//将cmd设置为utf-8编码

    string address = argv[0];       //通过修改main.exe的地址得到Dictionary.txt的地址
    int position = address.rfind("main.exe");

    //cout << address << endl;

    if (position == -1)
    {
        int position = address.rfind("SensitiveWordFilter.exe");
        address.replace(position, 23, "Dictionary.txt");
    }
    else
        address.replace(position, 8, "Dictionary.txt");

    //cout << address << endl;
    
    readDictionary(address);

    /*
    for (int i = 0; i < argc; i++)
    {
        cout << "argv[" << i << "]:" << argv[i] << endl;
    }
    */

    readMgc(argv[1]);   //    readMgc();       //

    readFile(argv[2]);  //    readFile();     //

    writeFile(argv[3]); //    writeFile();        //

    return 0;
}