# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
# define ll long long
# define SIZE 1000002
# define prime1 33290011
# define prime2 33000007
//(N+Q)*M <= 1e6
//33000007 33290011
//100000007 100000037


ll P[SIZE] = {0};
typedef struct hashvalue{
    int hash2_id;
    int num;
} hashvalue;
hashvalue hash[prime1] = {0};


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


void input(int* len){
    *len = 0;
    P[0] = readchar();
    while(P[0] < 'a') P[0] = readchar();
    P[0] = P[0]-'a';
    for(int i = 1; i < SIZE; i++){
	char in = readchar();
	if(in >= 'a' && in <= 'z'){
	    P[i] = in-'a';
	    (*len)++;
	}
	else break;
    }
}


void get_hash(int len, ll* p1, ll* p2, ll* k1, ll* k2){
    *p1 = 0, *p2 = 0, *k1 = 1, *k2 = 1;
    for(int i = 0; i <= len; i++){  
	// find first hashvalue of p and t, and find hashvalue of 26^m
	*p1 = ((*p1)*26 + P[i]) % prime1; 
	*p2 = ((*p2)*26 + P[i]) % prime2; 
	*k1 = ((*k1)*26) % prime1;	   
	*k2 = ((*k2)*26) % prime2;	   
    }
}


ll get_hash2_d(ll p1){
    ll d = p1;
    while(d > prime1-1) d -= prime1-1;
    return d;
}


void adjust_ans(ll* ans, int num, int d){
    if(d == 1)
	*ans += num;
    else if(num > 1)
	*ans -= (num-1);
}


void find_string(int len, int* found, ll* ansptr, int type, 
		 ll p1, ll p2, ll k1, ll k2){
    *found = 0;
    for(int i = 0; i <= len && !(*found); i++){
	ll flag = p1;
	ll d = get_hash2_d(p1);
	for(ll j = 0; j < prime1 && hash[flag].num > 0; j++){
	    ll t = (ll)hash[flag].hash2_id;
	    if(t == p2){
		adjust_ans(ansptr, hash[flag].num, type);
		hash[flag].num += type;
		*found = 1;
		break;
	    }
	    flag = (flag + d) % prime1;
	}
	p1 = (p1*26 - k1*P[i] + P[i]) % prime1;
	p2 = (p2*26 - k2*P[i] + P[i]) % prime2; 
	while(p1 < 0) p1 += prime1;
	while(p2 < 0) p2 += prime2;
    }
}


void put_hash(ll p1, ll p2){
    ll flag = p1;
    ll d = get_hash2_d(p1);
    while(hash[flag].num > 0) flag = (flag + d) % prime1;
    hash[flag].hash2_id = (int)p2;
    hash[flag].num = 1;
}


int main(){
    int N, Q, len = 0;
    ll ans = 0;
    ll p1, p2, k1, k2;  // k means 26^(m) (mod prime)
    int found;
    scanf("%d%d", &N, &Q);

    for(int cnt = 0; cnt < N; cnt++){
	input(&len);

	get_hash(len, &p1, &p2, &k1, &k2);
	find_string(len, &found, &ans, 1, p1, p2, k1, k2);  
	// adjust ans if found
	if(!found)
	    put_hash(p1, p2);
    }
    printf("%lld\n", ans);

    for(int cnt = 0; cnt < Q; cnt++){
	char c = readchar();
	while(c < '0') c = readchar();
	input(&len);

	get_hash(len, &p1, &p2, &k1, &k2);
	switch(c){
	    case '1':
		find_string(len, &found, &ans, 1, p1, p2, k1, k2);
		// adjust ans if found
		if(!found)
		    put_hash(p1, p2);
		break;
	    case '2':
		find_string(len, &found, &ans, -1, p1, p2, k1, k2);
		// adjust ans if found
		if(!found) 
		    printf("You fucked up in delete.\n"), exit(-1);
		break;
	    default:
		printf("You fucked up in input and switch\n"), exit(-1);
		break;
	}
	printf("%lld\n", ans);
    }

    return 0;
}
