#include "Stack.h"

bool Stack::push(int x){
    if (top >= (MAX - 1)) return false;
    else {
        a[++top] = x;
        return true;
    }
}
 
int Stack::pop(){
    if (top < 0) return 0;
    else {
        int x = a[top--];
        return x;
    }
}
 
bool Stack::isEmpty(){
    return (top < 0);
}
