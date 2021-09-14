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
    bool is_NULL = true;        //�Ƿ�Ϊ��

    void addSonNode(char key, TreeNode node)
    {
        sonNodes.insert(pair<int, TreeNode>(key, node));
        is_NULL = false;
    }       //�����ӽڵ�

    TreeNode getSonNode(int key)
    {
        map<int, TreeNode>::iterator iter = sonNodes.find(key);
        return iter->second;


        if (iter != sonNodes.end())
            return iter->second;
        else
        {
            cout << "Do not Find" << endl;
            TreeNode sonNode;
        }
    }       //Ѱ���ӽڵ�

    void setEnd(bool end, string s)
    {
        is_End = end;
        allMgc = s;
    }       //����Ϊ��β

private:
    map<int, TreeNode> sonNodes;		//�ӽڵ�
    string allMgc;      //���д�
};
TreeNode rootNode;
TreeNode treeNode[3000];

void createTree(string s)
{
    TreeNode tempNode = rootNode;       //��ʱ�ڵ�

    for (int i = 0; i < s.size(); i++)
    {
        int c = s[i];

        TreeNode node = tempNode.getSonNode(c);
        if (node.is_NULL == true)
        {
            tempNode.addSonNode(c, node);
        }

        tempNode = node;
        if (i == s.size() - 1)
        {
            tempNode.setEnd(true, s);
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
	ofstream outFile("ans1.txt", ios::trunc);
	
    outFile << 12345678;

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