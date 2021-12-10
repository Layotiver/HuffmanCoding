#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<cctype>
#include<cstring>
#include<cmath>
#include<vector>
#include<fstream>
using namespace std;

char faddress[100]; //要压缩的文件地址
char ch[114514];
int cnt[256]; //统计每个字节出现的次数



int main()
{
    int i;
    printf("请输入需要压缩的文件地址\n");
    scanf("%s",faddress);
    ifstream fin(faddress,ios::binary);
    while(!fin.eof())
    {
        fin.read(ch,114514);
        for(i=1;i<=fin.gcount();i++)
        {
            cnt[ch[i]]++;
        }
    }
    
}