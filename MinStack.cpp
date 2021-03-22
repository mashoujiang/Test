#include <stack>
#include <iostream>
#include <limits.h>

class MinStack
{
public:
    /** initialize your data structure here. */
    MinStack()
    {
        miniStack.push(INT_MAX);
    }

    void push(int val)
    {
        realStack.push(val);
        miniStack.push(std::min(miniStack.top(), val));
    }

    void pop()
    {
        realStack.pop();
        miniStack.pop();
    }

    int top()
    {
        return realStack.top();
    }

    int getMin()
    {
        return miniStack.top();
    }

private:
    std::stack<int> miniStack;
    std::stack<int> realStack;
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

int main()
{
    MinStack minStack{};
    minStack.push(-2);
    minStack.push(0);
    minStack.push(-3);
    std::cout << minStack.getMin() << std::endl;
    // -- > 返回 - 3.
    // minStack.pop();
    std::cout << minStack.top() << std::endl;
    // -- > 返回 0.
    std::cout << minStack.getMin() << std::endl;
    // -- > 返回 - 2.
}