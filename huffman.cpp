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
using namespace std;

struct node
{
    char c;
    int cnt, left, right, number;
    bool operator<(const node &b)
    {
        return cnt < b.cnt;
    }
} nd[610]; //in fact, 512 is enough

char faddress[100]; //要压缩的文件地址
char ch[114514];    //用于存储读入的字节
int keynum;         //读入字节的种数
int root;           //根节点的编号
int huffman[256];   //各字节对应的huffman编码，转换成10进制用int存储
int bytenum[256];   //各字节对应的huffman编码所需字节数

ifstream fin(faddress, ios::binary);
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

    for (i = 1; i <= keynum; i++)
    {
        q.push(nd[i]);
        nd[i].number = i;
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

        nd[p++].left = a.number;
        nd[p].right = b.number;
        nd[p].cnt = a.cnt + b.cnt;
        q.push(nd[p]);
    }
    return;
}

void buildmap(int ndnum, int code)
{
    if (ndnum < keynum)
    {
        huffman[nd[ndnum].c] = code;
        return;
    }
    //左子树为0，右子树为1
    buildmap(nd[ndnum].left, code << 1);
    buildmap(nd[ndnum].right, (code << 1) + 1);
    return;
}

void Read()
{
    int i;
    printf("Please enter the address of the file to be zipped\n");
    scanf("%s", faddress);
    while (!fin.eof())
    {
        fin.read(ch, 114514);
        for (i = 0; i < fin.gcount(); i++)
        {
            nd[ch[i]].cnt++;
        }
    }
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
    sort(nd, nd + keynum, cmp);
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

void Write_huffmap()
{
    //键值对存储方式：key(1byte)+值所用字节数(1byte)+补0情况(1byte)+值(n bytes)
    int i, x, y;

    Write(keynum);
    for (i = 0; i <= 255; i++)
    {
        if (huffman[i] != -1)
        {
            Write(i);

            //huffman编码为0（最左叶子节点），用特判
            if (huffman[i] == 0)
            {
                Write(1);
                Write(0);
                Write(0);
                bytenum[i] = 1;
                continue;
            }

            x = huffman[i];
            y = 0;
            while (x > 127)
            {
                y++;
                x >>= 8;
            }
            if (x) //x还有剩余，需要加一个字节，并在结尾补0
            {
                Write(y + 1);
                bytenum[i] = y + 1;
                y = 8;
                while (x)
                {
                    y--;
                    x >>= 1;
                }
                Write(y);
                Write(huffman[i] << y);
            }
            else
            {
                Write(y);
                bytenum[i] = y;
                Write(0);
                Write(huffman[i]);
            }
        }
    }
    return;
}

int main()
{
    memset(huffman, -1, sizeof(huffman));
    Read();
    count();

    /*for(i=0;i<=keynum;i++)
    {
        printf("%d\n",nd[i].cnt);
    }*/

    buildtree();
    buildmap(root, 0);

    fin.close();
    fout.close();
    return 0;
}