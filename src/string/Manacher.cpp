#include<bits/stdc++.h>
using namespace std;
const int maxn = 11000009;
char s[maxn];

int len[maxn<<1],N;
char ch[maxn<<1];
void init(char *s){
	int n = strlen(s+1);//s:range[1,n]
	ch[n*2 +1] = '#';
	ch[0] = '@';
	ch[n*2 +2] = '\0';
	for (int i=n;i>=1;i--)ch[i*2] = s[i], ch[i*2 -1] = '#';
	N = 2* n +1;
}
void manacher(){
	int ma=0, k=1;
	len[1]=1;
	for (int i=2;i<=N;i++){
		int p = k+len[k]-1;
		if (i<=p)len[i]=min(len[2*k-i],p-i+1);
		else len[i]=1;  
		while (ch[i+len[i]]==ch[i-len[i]])len[i]++;
		if (i+len[i]>k+len[k])k=i;
		ma=max(ma,len[i]);
	}
	printf("%d\n",ma-1);
}
int main(){
	scanf("%s",s+1);
	init(s);
	manacher();
	return 0;
}