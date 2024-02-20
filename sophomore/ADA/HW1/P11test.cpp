#include <iostream>
#include <fstream>
#include <random>
using namespace std;
# define SIZE 1000
//longlong to 9.22*(10^18)

int main(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-10000,10000);
    
    ofstream out("P11.in");
    out<<SIZE<<'\n';
    for(int i = 0; i < SIZE; i++)
	out<<dist(gen)<<'\n';
    out.close();

    return 0;
}
