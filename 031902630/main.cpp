#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
#include<map>
using namespace std;


class TreeNode		//ǰ׺���ڵ���
{
public:
    bool is_End = false;		//�Ƿ�Ϊ��β

    void setEnd(bool end, string s)
    {
        is_End = end;
        allMgc = s;
    }       //����Ϊ��β

    map<char, int> sonNodes;		//�ӽڵ���±�
    string allMgc;      //���д�
};
TreeNode rootNode;
TreeNode treeNode[3000];
int nodeNum = 0;

void createTree(string s)
{
    TreeNode* tempNode = &rootNode;       //��ʱ�ڵ�
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


void readMgc()    //�������д�
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

void readFile()		//���������ı�
{
    ifstream inFile("org.txt");
    if (!inFile.is_open())
        cout << "can not open org.txt" << endl;




    inFile.close();
}

void writeFile()		//�����
{
    ofstream outFile("ans.txt", ios::trunc);

    outFile << 12678;

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