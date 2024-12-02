#include "threadpool.h"

void funcx(int a){
    for(int i = 0; i < 100000; ++i){
        a++;
        cout << a << endl;
    }
}

int main(){
    ThreadPool pool(4);
    
    return 0;
}