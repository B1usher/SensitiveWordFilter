#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
#include<map>
#include<locale>
#include<cstdlib>
using namespace std;

class Answer        //����
{
public:
    int Line;       //�к�
    string Mgc;     //���д�
    string Text;     //�ı�
};
Answer answer[30010];
int ansTotal = 0;

class TreeNode		//ǰ׺���ڵ���
{
public:
    bool is_End = false;		//�Ƿ�Ϊ��β

    void setEnd(string s)       //����Ϊ��β
    {
        is_End = true;
        allMgc = s;
    }

    map<string, int> sonNodes;		//�ӽڵ���±�
    string allMgc;      //���д�
};
TreeNode rootNode;
TreeNode treeNode[10010];
int nodeNum = 0;

map<string, string> splitWord;      //���ֲ��Ϊƫ�Բ���
void readDictionary(string words)    //�����ֵ�
{
    string s;
    ifstream inDictionary;
    inDictionary.open(words);
    if (!inDictionary.is_open())
        cout << "can not open Dictionary.txt" << endl;

    string beforeSplit;     //���ǰԭ��
    string afterSplit;      //��ֺ�ƫ�Բ���
    int begin = 0;;
    int position = 0;

    while (getline(inDictionary, s))
    {
        begin = 0;
        position = 0;
        for (; position < s.size(); position++)
        {
            if (s[position] == 34)       //������һ��ǰ"
            {
                begin = position + 1;
                for (position++; position < s.size(); position++)
                {
                    if (s[position] == 34)      //������һ����"
                    {
                        beforeSplit = s.substr(begin, position - begin);
                        break;
                    }
                }

                for (position++; position < s.size(); position++)
                {
                    if (s[position] == 34)      //�����ڶ���ǰ"
                    {
                        begin = position + 1;
                        for (position++; position < s.size(); position++)
                        {
                            if (s[position] == 34)      //�����ڶ�����"
                            {
                                afterSplit = s.substr(begin, position - begin);
                                break;
                            }
                        }
                        break;
                    }
                }

                splitWord.insert(map<string, string>::value_type(beforeSplit, afterSplit));     //�����Ӧ��ϵ
                break;
            }
        }
    }

    inDictionary.close();
}

void createTree(string s)       //������
{
    TreeNode* tempNode = &rootNode;       //��ʱ�ڵ�
    for (int i = 0; i < s.size();)
    {
        string c;
        char l = s[i];

        if (l < 0)       //�Ǻ��ֻ������ַ�
        {
            c = s.substr(i, 3);
            i += 3;
        }
        else
        {
            if (l >= 'a' && l <= 'z')       //��Ӣ��
                l -= 32;
            c = l;
            i++;
        }

        map<string, int>::iterator iter = tempNode->sonNodes.find(c);
        if (iter == tempNode->sonNodes.end())
        {
            tempNode->sonNodes.insert(map<string, int>::value_type(c, nodeNum));      //����ӽڵ�
            int keepNum = nodeNum;      //���������±�
            nodeNum++;

            map<string, string>::iterator iterNew = splitWord.find(c);
            if (iterNew != splitWord.end())     //���ֵ����ҵ��˲�ַ�ʽ
            {
                string afterSplit = iterNew->second;
                for (int i = 0; i < afterSplit.size();)     //�����һ��·��
                {
                    string single;      //�������֣�ƫ�Բ��ף�
                    single = afterSplit.substr(i, 3);

                    i += 3;     //��һ���벿��
                    if (i < afterSplit.size())      //��һ���ֲ������һ����
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

void searchMgc(string s, int line)        //�������д�
{
    TreeNode* tempNode = &rootNode;     //��ʱ�ڵ�
    int begin = 0;      //��ʼλ��
    int position = 0;       //��ǰλ��

    for (; begin < s.size();)
    {
        position = begin;
        tempNode = &rootNode;
        string c;
        char l = s[position];

        if (l < 0)       //�Ǻ��ֻ������ַ�
        {
            c = s.substr(position, 3);
            position += 2;
        }
        else
        {
            if (l >= 'a' && l <= 'z')       //��Ӣ��
                l -= 32;
            c = l;
        }

        if (l < 0)      //��������
        {
            map<string, int>::iterator iter = tempNode->sonNodes.find(c);
            if (iter != tempNode->sonNodes.end())
            {
                int charNum = 0;
                tempNode = &treeNode[iter->second];

                for (charNum = 0; charNum < 20;)
                {
                    if (tempNode->is_End == true)       //ƥ��ɹ�
                    {
                        answer[ansTotal].Line = line;
                        answer[ansTotal].Mgc = tempNode->allMgc;
                        answer[ansTotal].Text = s.substr(begin, position - begin + 1);
                        ansTotal++;
                        begin = position + 1;
                        break;
                    }

                    position++;
                    char w = s[position];       //��һ����
                    if (w < 0)       //�Ǻ��ֻ������ַ�
                    {
                        c = s.substr(position, 3);
                        position += 2;
                    }
                    else
                    {
                        if (w >= 'a' && w <= 'z')       //��Ӣ��
                            w -= 32;
                        c = w;
                    }

                    if (w < 0)      //����
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
                    else if ((w >= 'a' && w <= 'z') || (w >= 'A' && w <= 'Z') || (w >= '0' && w <= '9'))        //�����м���Ӣ�ĺ�����
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
        else if ((l >= 'a' && l <= 'z') || (l >= 'A' && l <= 'Z'))      //Ӣ������
        {
            map<string, int>::iterator iter = tempNode->sonNodes.find(c);
            if (iter != tempNode->sonNodes.end())
            {
                int charNum = 0;
                tempNode = &treeNode[iter->second];
                
                for (charNum = 0; charNum < 20;)
                {
                    if (tempNode->is_End == true)       //ƥ��ɹ�
                    {
                        answer[ansTotal].Line = line;
                        answer[ansTotal].Mgc = tempNode->allMgc;
                        answer[ansTotal].Text = s.substr(begin, position - begin + 1);
                        ansTotal++;
                        begin = position + 1;
                        break;
                    }

                    position++;
                    char w = s[position];       //��һ����
                    if (w < 0)       //�Ǻ��ֻ������ַ�
                    {
                        c = s.substr(position, 3);
                        position += 2;
                    }
                    else
                    {
                        if (w >= 'a' && w <= 'z')       //��Ӣ��
                            w -= 32;
                        c = w;
                    }

                    if (w < 0)      //Ӣ���м�������
                    {
                        begin++;
                        break;
                    }
                    else if ((w >= 'a' && w <= 'z') || (w >= 'A' && w <= 'Z'))      //Ӣ��
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
        else        //�ַ�����
            begin++;
    }
}

void readMgc(char* words)    //�������д�    void readMgc()      //
{
    string s;
    ifstream inMgc;
    inMgc.open(words);  //    inMgc.open("words.txt");        //
    if (!inMgc.is_open())
        cout << "can not open words.txt" << endl;

    while (inMgc >> s)
    {
        createTree(s);      //������
    }

    inMgc.close();
}

void readFile(char* org)		//���������ı�   void readFile()     //
{
    string s;
    int line = 1;
    ifstream inFile;
    inFile.open(org);   //    inFile.open("org.txt");     //
    if (!inFile.is_open())
        cout << "can not open org.txt" << endl;

    while (getline(inFile, s))
    {
        searchMgc(s, line);       //�������д�
        line++;
    }

    inFile.close();
}

void writeFile(char* ans)		//�����  void writeFile()        //
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
    SetConsoleOutputCP(65001);		//��cmd����Ϊutf-8����

    string address = argv[0];       //ͨ���޸�main.exe�ĵ�ַ�õ�Dictionary.txt�ĵ�ַ
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