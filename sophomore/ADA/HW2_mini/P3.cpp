#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
# define pii pair<int,int>
# define ll long long
# define MAX_n 51


int nextidx[200][2] = {0};  // nextidx and output
int flag = MAX_n;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, input;
    cin >> n;
    priority_queue<pii, vector<pii>, greater<pii>> pq;  // amount
    for(int i = 0; i < n; i++){
	cin >> input;
	pq.push({input,i});
    }

    for(int i = 0; i < n; i++){
	if(n-i > 2){
	    pii tmp1 = pq.top();
	    pq.pop();
	    pii tmp2 = pq.top();
	    pq.pop();
	    int idx1 = tmp1.second;
	    int idx2 = tmp2.second;
	    nextidx[idx1][0] = flag;
	    nextidx[idx2][0] = flag;
	    nextidx[idx1][1] = 0;
	    nextidx[idx2][1] = 1;
	    pq.push({tmp1.first + tmp2.first, flag});
	    flag++;
	}
	else if(n-i == 2){
	    pii tmp1 = pq.top();
	    pq.pop();
	    pii tmp2 = pq.top();
	    pq.pop();
	    int idx1 = tmp1.second;
	    int idx2 = tmp2.second;
	    nextidx[idx1][0] = flag;
	    nextidx[idx2][0] = flag;
	    nextidx[idx1][1] = 0;
	    nextidx[idx2][1] = 1;
	    break;
	}
	else if(n-i == 1){
	    pii tmp = pq.top();
	    pq.pop();
	    int idx = tmp.second;
	    nextidx[idx][0] = flag;
	    nextidx[idx][1] = 0;
	}
    }

    for(int i = 0; i < n; i++){
	char name[MAX_n] = {0};
	int id = i;
	int nid = 0;
	while(id != flag){
	    name[nid] = '0'+nextidx[id][1];
	    id = nextidx[id][0];
	    nid++;
	}
	for(int j = nid-1; j >= 0; j--)
	    putchar(name[j]);
	putchar('\n');
    }



    return 0;
}
