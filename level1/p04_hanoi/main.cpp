#include<iostream>
using namespace std;
//真的输出的完么2^64-1 ???

void hanoi(int n, char from, char sto, char to) {
    if (n == 1) {
        cout << from << "-->" << to << '\n';
        return;
    }
    hanoi(n - 1, from, to, sto);      
    cout << from << "-->" << to << '\n'; 
    hanoi(n - 1, sto, from, to);      
}


int main(){
    hanoi( 64 ,'A','B' ,'C' );
}




