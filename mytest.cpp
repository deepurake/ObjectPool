#include <iostream>

template<typename T>
class A{
    T i;
    A(T x):i(x){}
};
class B:public A<std::string>{};
int main(){
    //B b;
    int x = 4,*y;
    y = &x;
    (*y)++;
    std::cout << *y << std::endl;
    return 0;
}