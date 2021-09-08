#ifndef STACK_H
#define STACK_H

#define MAX 100					//MAX Stack size...

/*Using stack for saving vaules predicted by system
 and modify that value if any prediction goes wrong*/
class Stack {
    int top;
public:
    int a[MAX]; // Maximum size of Stack
    Stack() {top = -1;}
    bool push(int x);
    int pop();
    bool isEmpty();
};

#endif
