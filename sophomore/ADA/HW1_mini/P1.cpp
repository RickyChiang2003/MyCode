#include <iostream>
using namespace std;


# define SIZE 300001
# define MIN -1000001


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    // useless when using <iostream> and <stdio.h> simultaniously

    long long n;
    cin >> n;
    long long arr[SIZE] = {0};
    long long id_arr = 0;
    long long max_neg = MIN;
    
    // store and optimize the data into positive and negative staggered
    for(long long i = 0, input, type; i < n; i++){
	cin >> input;
	if(input <= 0)
	    max_neg = max(input, max_neg);
	
	if(i == 0){
	    arr[0] = input;
	    type = (input > 0) ? 1 : -1;
	}
	else if((input >= 0 && type > 0) || (input <= 0 && type < 0)){
	    arr[id_arr] += input;
	}
	else{
	    id_arr++;
	    arr[id_arr] = input;
	    type  = (type == 1) ? -1 : 1;
	}
    }

    // compute ans
    long long ans = 0;
    long long id_ans = 0;
    
    while(id_ans <= id_arr && arr[id_ans] <= 0)
	id_ans++;

    if(id_arr == 0 && arr[0] <= 0)
	ans = max_neg;
    else{
	ans = arr[id_ans];
	id_ans++;
    }

    long long ans_max = ans;
    while(id_ans+1 <= id_arr){
	long long tmp1 = ans+arr[id_ans]+arr[id_ans+1];
	long long tmp2 = arr[id_ans+1];
	ans = max(tmp1, tmp2);
	ans_max = max(ans_max, ans);
	id_ans += 2;
    }


    cout << ans_max << endl;



    return 0;
}
