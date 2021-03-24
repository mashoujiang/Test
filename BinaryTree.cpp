#include <algorithm>
#include <iostream>
struct Node
{
    Node *left = nullptr;
    Node *right = nullptr;
    int data{0};
};

int findMaxDepth(Node *root, int &maxLen)
{
    if (!root)
    {
        return 0;
    }
    int leftDepth = findMaxDepth(root->left, maxLen) + 1;
    int rightDepth = findMaxDepth(root->right, maxLen) + 1;

    maxLen = std::max(maxLen, leftDepth + rightDepth - 2);

    int maxDepth = std::max(leftDepth, rightDepth);
    return maxDepth;
}

int findMaxLen(Node *root)
{
    if (root == nullptr)
        return 0;
    int maxLen{0};
    int maxDepth = findMaxDepth(root, maxLen);
    std::cout << "maxDepth = " << maxDepth << ", maxLen = " << maxLen << std::endl;
    return maxLen;
}

int main()
{
    /**
     *  binary tree
     *     1
     *    2   3
     *  4  5  6  7
     *   8      9
     *            10
     * 
     * */

    Node *node1 = new Node();
    Node *node2 = new Node();
    Node *node3 = new Node();
    Node *node4 = new Node();
    Node *node5 = new Node();
    Node *node6 = new Node();
    Node *node7 = new Node();
    Node *node8 = new Node();
    Node *node9 = new Node();
    Node *node10 = new Node();

    node1->left = node2;
    node1->right = node3;
    node2->left = node4;
    node2->right = node5;
    node3->left = node6;
    node3->right = node7;
    node4->right = node8;
    node7->right = node9;
    node9->right = node10;

    findMaxLen(node1);
    findMaxLen(node2);
}
