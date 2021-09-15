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

    map<string, int> sonNodes;		//�ӽڵ���±�
    string allMgc;      //���д�
};
TreeNode rootNode;
TreeNode treeNode[3000];
int nodeNum = 0;

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