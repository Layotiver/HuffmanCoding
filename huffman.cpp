#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <cctype>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct node
{
    char c;
    int cnt, left, right, number;
    bool operator <(const node b) const{
        return cnt>b.cnt;
    }
} nd[610]; //in fact, 512 is enough

char faddress[100];  //要压缩的文件地址
char ch[114514];     //用于存储读入的字节
int keynum;          //读入字节的种数
int root;            //根节点的编号
string huffman[256]; //各字节对应的huffman编码
int bitnum[256];     //各字节对应的huffman编码所需bit数

ofstream fout("zip.ch", ios::binary);

bool cmp(node a, node b)
{
    return a.cnt > b.cnt;
}

void buildtree()
{
    priority_queue<node> q;
    int i, p = keynum - 1;
    node a, b;

    for (i = 0; i < keynum; i++)
    {
        nd[i].number = i;
        q.push(nd[i]);
    }
    while (1)
    {
        a = q.top();
        q.pop();
        if (q.empty())
        {
            root = p;
            break;
        }

        b = q.top();
        q.pop();

        nd[++p].left = a.number;
        nd[p].right = b.number;
        nd[p].cnt = a.cnt + b.cnt;
        nd[p].number=p;
        q.push(nd[p]);
    }
    return;
}

void printtree(int ndnum,int depth)
{
    
}

void buildmap(int ndnum, string code, int depth)
{
    if (ndnum < keynum)
    {
        huffman[nd[ndnum].c] = keynum == 1 ? "0" : code;
        bitnum[nd[ndnum].c] = keynum == 1 ? 1 : depth;
        return;
    }
    //左子树为0，右子树为1
    buildmap(nd[ndnum].left, code + "0", depth + 1);
    buildmap(nd[ndnum].right, code + "1", depth + 1);
    return;
}

void printmap()
{
    int i;
    for(i=0;i<keynum;i++)
    {
        printf("%c ",nd[i].c);
        printf("%s",huffman[nd[i].c].c_str());
        printf("\n");
    }
    return ;
}

void Read()
{
    int i;
    printf("Please input the address of the file to be zipped\n");
    scanf("%s", faddress);
    ifstream fin1(faddress, ios::binary);
    while (!fin1.eof())
    {
        fin1.read(ch, 114514);
        for (i = 0; i < fin1.gcount(); i++)
        {
            nd[ch[i]].cnt++;
        }
    }
    fin1.close();
    return;
}

void count()
{
    int i;
    for (i = 0; i <= 255; i++)
    {
        if (nd[i].cnt)
        {
            nd[i].c = i;
            keynum++;
        }
    }
    sort(nd, nd + 256, cmp);
    /*for (i = 0; i < keynum; i++)
    {
        printf("%c %d\n", nd[i].c, nd[i].cnt);
    }*/
    return;
}

void Write(int a)
{
    if (a == 0) //a为0的时候，写入一个全为0的字节
    {
        fout.write((char *)&a, 1);
        return;
    }
    fout.write((char *)&a, sizeof((char *)&a));
    return;
}

void write_filename()
{
    int i, len;
    for (i = strlen(faddress) - 1; i >= 0; i--)
    {
        if (faddress[i] == '/')
            break;
    }
    len = strlen(faddress) - i - 1;
    Write(len);
    for (i = strlen(faddress) - len; i <= strlen(faddress) - 1; i++)
    {
        Write(faddress[i]);
    }
    return;
}

void Write_str(string s)
{
    int i, j, x;
    if (s.length() / 8)
    {
        for (i = 1; i <= 8 - s.length() % 8; i++)
        {
            s.insert(s.begin(), '0');
        }
    }
    for (i = 0; i <= s.length(); i += 8)
    {
        x = 0;
        for (j = 0; j <= 7; j++)
        {
            x <<= 1;
            x |= s[i + j] - '0';
        }
        Write(x);
    }
    return;
}

void write_huffmap()
{
    //键值对存储方式：key(1byte)+值所用比特数(1byte)+值(n bytes)
    int i, x, y;

    Write(keynum);
    for (i = 0; i <= 255; i++)
    {
        if (huffman[i].length())
        {
            Write(i);
            Write(bitnum[i]);
            Write_str(huffman[i]);
        }
    }
    return;
}

void write_filebit()
{
    int i, j;
    int fbitcnt = 0; //计算整个文件的字节转换成huffman编码要多少比特
    int x, y = 0;
    ifstream fin2(faddress, ios::binary);
    while (!fin2.eof())
    {
        fin2.read(ch, 114514);
        for (i = 0; i < fin2.gcount(); i++)
        {
            fbitcnt += bitnum[ch[i]];
        }
    }
    Write((8 - fbitcnt % 8) % 8); //文件末尾补0情况
    fin2.close();

    ifstream fin3(faddress, ios::binary);
    while (!fin3.eof())
    {
        fin3.read(ch, 114514);
        for (i = 0; i < fin3.gcount(); i++)
        {
            for (j = 0; j < huffman[ch[i]].length(); j++)
            {
                y++;
                x <<= 1;
                x |= huffman[ch[i]][j];
                if (y == 8)
                {
                    Write(x);
                    x = 0;
                    y = 0;
                }
            }
            if (y)
            {
                x <<= 8 - y;
                Write(x);
            }
        }
    }
    fin3.close();
    return;
}

int main()
{
    Read();
    count();

    buildtree();
    buildmap(root, "", 0);
    printmap();

    //write_filename();
    //write_huffmap();
    //write_filebit();

    fout.close();
    return 0;
}