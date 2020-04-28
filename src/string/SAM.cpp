//广义后缀自动机：insert后重新将last赋1	
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const int maxn=1e6+5;

char s[maxn];
struct Suffix_Automaton
{
	//初始状态为0,range[0...tot-1]
	struct state{
		int len,link;
		map<char,int>next;
	}st[maxn<<1];
	int last,tot;
	
	void init(){
		st[0].len=0;st[0].link=-1;
		tot++;
		last=0;
	}
	
	void extend(char c){
		int cur=tot++;
		st[cur].len=st[last].len+1;
		int p=last;
		while(p!=-1 && !st[p].next.count(c)){
			st[p].next[c]=cur;
			p=st[p].link;
		}
		if(p==-1)st[cur].link=0;
		else{
			int q=st[p].next[c];
			if(st[p].len+1==st[q].len)st[cur].link=q;
			else{
				int clone=tot++;
				st[clone].len=st[p].len+1;
				st[clone].next=st[q].next;
				st[clone].link=st[q].link;
				while(p!=-1 && st[p].next[c]==q){
					st[p].next[c]=clone;
					p=st[p].link;
				}
				st[q].link=st[cur].link=clone;
			}
		}
		last=cur;
	}
	
	ll count(){
		ll res=0;
		for(int i=0;i<tot;i++)res+=st[i].len-st[st[i].link].len;
		return res;
	}
} sam;

int main()
{
	scanf("%s",s);
	sam.init();
	for(int i=0;s[i]!=0;i++)sam.extend(s[i]);
	printf("%lld\n",sam.count());
	return 0;
}