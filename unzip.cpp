#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <cctype>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

char fname[100];
ofstream fout("chenhao.txt", ios::binary);
char ch[114514];
char unzipname[100];
string map[256];

void judgewrite(string s)
{
    int i;
    for (i = 0; i <= 255; i++)
    {
        if (map[i] == s)
        {
            fout.write((char *)&i, 1);
            break;
        }
    }
    return;
}

void readname()
{
    int i, j;
    scanf("%s", fname);
    ifstream fin(fname, ios::binary);

    fin.read(ch, 1);
    fin.read(unzipname, ch[0]);

    char keynum, key, bitnum;
    char x;
    int y = 0;
    fin.read(&keynum, 1);
    for (i = 1; i <= keynum; i++)
    {
        fin.read(&key, 1);
        fin.read(&bitnum, 1);
        for (j = 1; j <= bitnum; j++)
        {
            if (!y)
            {
                fin.read(&x, 1);
                y = 8;
            }
            map[key].push_back((x & 128) / 128 + '0');
            x <<= 1;
            y--;
        }
    }

    char zero;
    fin.read(&zero, 1);
    string s;

    while (1)
    {
        fin.read(&x, 1);
        if (fin.eof())
        {
            for (i = 1; i <= 8 - zero; i++)
            {
                s.push_back((x & 128) / 128 + '0');
                x <<= 1;
                judgewrite(s);
            }
            break;
        }
        for (i = 1; i <= 8; i++)
        {
            s.push_back((x & 128) / 128 + '0');
            x<<=1;
            judgewrite(s);
        }
    }

    return;
}

void Readmap()
{
    char c;
}

int main()
{
    readname();
    Readmap();
}