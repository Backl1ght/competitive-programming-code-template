//最长双倍回文串长度
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 5e5 + 5;

struct Palindromic_Automaton{
	//0偶根 1奇根  range[2-tot]
	int s[N << 1],now;
	int next[N << 1][26], fail[N << 1], len[N << 1], last, tot;
	int cnt[N << 1]; //状态i表示的回文串数目 
	
    // extend
	int trans[N << 1];
	
	void init(){
		s[0]=len[1]=-1;
		fail[0]=tot=now=1;
		last=len[0]=0;
		memset(next[0],0,sizeof(next[0]));
		memset(next[1],0,sizeof(next[1]));
	}
	int newnode(){
		tot++;
		memset(next[tot],0,sizeof(next[tot]));
		fail[tot]=cnt[tot]=len[tot]=0;
		return tot;
	}
	int getfail(int x){
		while(s[now-len[x]-2]!=s[now-1])x=fail[x]; 
		return x;
	}
	void extend(int c){
		s[now++]=c;
		int cur=getfail(last);
		if(!next[cur][c]){
			int p=newnode();len[p]=len[cur]+2;
			fail[p]=next[getfail(fail[cur])][c];
			next[cur][c]=p;
			
			if(len[p]<=2)trans[p]=fail[p];
			else{
				int tmp=trans[cur];
				while(s[now-len[tmp]-2] != s[now-1] || (len[tmp]+2)*2>len[p])tmp=fail[tmp];
				trans[p]=next[tmp][c];
			}	
		}
		last=next[cur][c];
		cnt[last]++;
	}
	int count(){return tot-1;}
	void calc(){
		for(int i=tot;i>=2;--i)	cnt[fail[i]]+=cnt[i];
		cnt[0]=cnt[1]=0;
	}
	int getans(){
		int ans=0;
		for(int i=2;i<=tot;i++){
			if(len[i]>ans && len[trans[i]]*2==len[i] && len[trans[i]]%2==0)ans=len[i];
		}
		return ans;
	}
}pam;

char t[N];

int main()
{
	int n;
	scanf("%d",&n);
	scanf("%s",t);
	pam.init();
	for(int i=0;i<n;++i){
		pam.extend(t[i]-'a');
	}
	printf("%d\n",pam.getans());
	return 0;
}