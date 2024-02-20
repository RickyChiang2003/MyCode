#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <algorithm>
#include <chrono>
#include <ratio>
#include <vector>
using namespace std;

vector<string> query;
set<string> myset;

void RangeSearch_Set(string& pre){
    string next = pre;
    next[next.length()-1]++;

	auto itlow = myset.lower_bound(pre);
  	auto itup = myset.upper_bound(next); 
 
    //cout<<"Search Suggestions for ["<<pre<<"]:\n";
  	//cout<<"  -> Found: "<<distance(myset.begin(), itup) 
    //    - distance(myset.begin(), itlow)<<" Matching Results\n";
    for(auto it = itlow;it != itup;it++){
        //cout<<"  - "<<(*it)<<"\n";
    }
    //cout<<"\n";
    //cout<<"---------------------------------------------\n";
    
}

int main(){
	ifstream in;
	string pre;
	// setup query
	in.open("query.txt");
    while(in >> pre){
    	query.push_back(pre);
    }
    in.close();

    // setup database 
    in.open("test_data.txt");
    string str;
    while(in >> str){
    	// set version
    	myset.insert(str);
    }
    in.close();

    auto start_time = std::chrono::steady_clock::now();

    for(unsigned i=0;i<query.size();i++){
    	RangeSearch_Set(query[i]);
    }

    auto end_time = std::chrono::steady_clock::now();
    auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    
    cout << "Set: elapsed time: " << nano.count() << "ns\n";
    return 0;
}