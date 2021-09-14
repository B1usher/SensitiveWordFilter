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
    bool is_NULL = true;        //是否为空

    void addSonNode(char key, TreeNode node)
    {
        sonNodes.insert(pair<int, TreeNode>(key, node));
        is_NULL = false;
    }       //插入子节点

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
    }       //寻找子节点

    void setEnd(bool end, string s)
    {
        is_End = end;
        allMgc = s;
    }       //设置为结尾

private:
    map<int, TreeNode> sonNodes;		//子节点
    string allMgc;      //敏感词
};
TreeNode rootNode;
TreeNode treeNode[3000];

void createTree(string s)
{
    TreeNode tempNode = rootNode;       //临时节点

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
	ofstream outFile("ans1.txt", ios::trunc);
	
    outFile << 12345678;

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