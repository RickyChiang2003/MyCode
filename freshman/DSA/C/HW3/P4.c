# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
# define ll long long
# define SIZE 1000002
# define prime1 9999991
# define prime2 9999973
// hash: 'A' ~ 'Z' <=> 65 ~ 90

ll T[SIZE] = {0};  // start from T[1]
ll P[SIZE] = {0};  // start from P[1]
int hash1[prime1] = {0};
int hash2[prime2] = {0};


int readchar(){
    static int N = 1 << 20;
    static char buf[1 << 20];
    static char *p = buf, *end = buf;
    if (p == end){
        if ((end = buf + fread(buf, 1, N, stdin)) == buf)
            return EOF;
        p = buf;
    }
    return *p++;
}


int main(){
    int N,M,ans = 0;
    scanf("%d%d", &N, &M);
    for(int i = 1; i <= N; i++){
	int in = readchar();
	if(in >= 'A' && in <= 'Z') T[i] = in-'A';
	else i--;
    }
    for(int i = 1; i <= M; i++){
	int in = readchar();
	if(in >= 'A' && in <= 'Z') P[i] = in-'A';
	else i--;
    }	
    
    
    ll p1 = 0, p2 = 0, t1 = 0, t2 = 0, k1 = 1, k2 = 1;  // k means 26^(m) (mod prime)
    for(int i = 1; i <= M; i++){  // find first hashvalue of p and t, and find hashvalue of 26^m
	p1 = p1*26 + P[i]; while(p1 >= prime1) p1 -= prime1; while(p1 < 0) p1 += prime1;
	p2 = p2*26 + P[i]; while(p2 >= prime2) p2 -= prime2; while(p2 < 0) p2 += prime2;
	t1 = t1*26 + T[i]; while(t1 >= prime1) t1 -= prime1; while(t1 < 0) t1 += prime1;
	t2 = t2*26 + T[i]; while(t2 >= prime2) t2 -= prime2; while(t2 < 0) t2 += prime2;
	k1 *= 26;	   while(k1 >= prime1) k1 -= prime1; while(k1 < 0) k1 += prime1;
	k2 *= 26;	   while(k2 >= prime2) k2 -= prime2; while(k2 < 0) k2 += prime2;
    }

    for(int i = 1; i <= M; i++){
//        printf("init hash: i = %d, p1 = %lld, p2 = %lld, P[i] = %lld, k1 = %lld, k2 = %lld\n", i, p1, p2, P[i], k1, k2);
	hash1[p1] = i;
	hash2[p2] = i;
	p1 = p1*26 - k1*P[i] + P[i]; while(p1 >= prime1) p1 -= prime1; while(p1 < 0) p1 += prime1;
	p2 = p2*26 - k2*P[i] + P[i]; while(p2 >= prime2) p2 -= prime2; while(p2 < 0) p2 += prime2;
    }
    
    for(int i = 0; i <= N-M; i++){
	if(hash1[t1] != 0 && hash1[t1] == hash2[t2])
	    ans++;
	if(i < N-M){
	    t1 = t1*26 - k1*T[i+1] + T[i+1+M]; while(t1 >= prime1) t1 -= prime1; while(t1 < 0) t1 += prime1;
	    t2 = t2*26 - k2*T[i+1] + T[i+1+M]; while(t2 >= prime2) t2 -= prime2; while(t2 < 0) t2 += prime2;

	}
    }

    printf("%d\n", ans);


    return 0;
}
