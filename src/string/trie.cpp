namespace Trie{
    // 1 based
    int sz;
    int ch[N][26];
    int val[N];// 记录某些特性，如以当前字符串为结尾的串的个数
    void init()	{
        sz=1;
        memset(ch[0], 0, sizeof(ch[0]));
        memset(val, 0, sizeof(val));
    }
    inline int idx(char c){return c-'a';}
    void add(char* s) {
        int u=0, len=strlen(s+1);
        for(int i=1; i<=len; i++){
            int c = idx(s[i]);
            if(!ch[u][c]) {
                memset(ch[sz], 0, sizeof(ch[sz]));
                val[sz] = 0;
                ch[u][c] = sz++;
            }
            u = ch[u][c];
            val[u]++;
        }
    }
    int query(char* s) {
        int u=0, len=strlen(s+1);
        for(int i=1; i<=len; i++){
            int c = idx(s[i]);
            if(!ch[u][c]) return 0;
            u = ch[u][c];
        }
        return val[u];
    }
}