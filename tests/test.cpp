#include<bits/stdc++.h>
using namespace std;
class Prueba{
public:
    vector<int> A;
    Prueba(){
        A.clear();
    }
    void do_insert(){
        A.push_back(1);
        cout<<A[0]<<endl;
    }
};
int main(){
    Prueba p;
    p.do_insert(); 
}