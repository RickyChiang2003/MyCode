#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;

int DATA_SIZE = 1e6;
string charset {"abcdefghijklmnopqrstuvwxyz"};	//26                                                
std::random_device seed;
std::mt19937 gen{seed()}; // seed the generator
std::uniform_int_distribution<> dist{0, 25}; // set min and max
std::uniform_int_distribution<> dist_len{5, 10};

string Generate_Random_Name(int len){
	string output;
	output.reserve(len);
	while(len){
	    output.push_back(charset[dist(gen)]);
	    len--;
	}
	return output;
}
int main(){
	string a, b;
	int len;
	ofstream out;
    out.open("query2.txt");
    if(out.fail()){		//check if file can be opened
        cout << "input file opening failed...";
        exit(1);
    }
    for(int i=0;i<DATA_SIZE;i++){
    	len = dist_len(gen);
    	a = Generate_Random_Name(len);
    	out << a << endl;
    }
}