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
Answer answer[10000];
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

    map<char, int> sonNodes;		//�ӽڵ���±�
    string allMgc;      //���д�
};
TreeNode rootNode;
TreeNode treeNode[3000];
int nodeNum = 0;

void createTree(string s)       //������
{
    TreeNode* tempNode = &rootNode;       //��ʱ�ڵ�
    for (int i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (c >= 'a' && c <= 'z')
            c -= 32;

        map<char, int>::iterator iter = tempNode->sonNodes.find(c);
        if (iter == tempNode->sonNodes.end())
        {
            tempNode->sonNodes.insert(map<char, int>::value_type(c, nodeNum));      //����ӽڵ�
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

void searchMgc(string s, int line)        //�������д�
{
    TreeNode* tempNode = &rootNode;     //��ʱ�ڵ�
    int begin = 0;      //��ʼλ��
    int position = 0;       //��ǰλ��

    for (; begin < s.size();)
    {
        position = begin;
        tempNode = &rootNode;
        char c = s[begin];
        if (c >= 'a' && c <= 'z')
            c -= 32;

        if (c < 0)      //��������
        {
            map<char, int>::iterator iter = tempNode->sonNodes.find(c);
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
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))      //Ӣ������
        {
            map<char, int>::iterator iter = tempNode->sonNodes.find(c);
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
        else        //�ַ�����
            begin++;
    }
}

void readMgc()    //�������д�
{
    string s;
    ifstream inMgc("words.txt");
    if (!inMgc.is_open())
        cout << "can not open words.txt" << endl;

    while (inMgc >> s)
    {
        createTree(s);      //������
    }

    inMgc.close();
}

void readFile()		//���������ı�
{
    string s;
    int line = 1;
    ifstream inFile("org.txt");
    if (!inFile.is_open())
        cout << "can not open org.txt" << endl;

    while (getline(inFile, s))
    {
        searchMgc(s, line);       //�������д�
        line++;
    }

    inFile.close();
}

void writeFile()		//�����
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
    SetConsoleOutputCP(65001);		//��cmd����Ϊutf-8����

    readMgc();

    readFile();

    writeFile();

    return 0;
}