#pragma comment(linker, "/STACK:167772160")
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
using namespace std;

struct Node {
    long Key;
    Node* Left;
    Node* Right;
};

Node* Rightmost(Node* T) {
    Node* Q = T;
    while (Q->Right != nullptr)
        Q = Q->Right;
    return Q;
}

Node* Leftmost(Node* T) {
    Node* Q = T;
    while (Q->Left != nullptr)
        Q = Q->Left;
    return Q;
}

bool isBST(Node* T) {
    if (T == nullptr)
        return true;
    if (T->Right != nullptr && Leftmost(T->Right)->Key < T->Key)
        return false;
    if (T->Left != nullptr && Rightmost(T->Left)->Key >= T->Key)
        return false;
    if (!isBST(T->Left) || !isBST(T->Right))
        return false;
    return true;
}

bool processTree() {
    int nodes_num, str_num;
    long node, root_key;
    char letter;
    scanf("%d", &nodes_num);
    if (nodes_num != 0) {
        Node** nodes = new Node*[nodes_num];
        scanf("%ld", &root_key);
        Node* root = new Node;
        root->Key = root_key;
        root->Left = root->Right = nullptr;
        nodes[0] = root;

        for (int i = 1; i < nodes_num; ++i) {
            scanf("%ld %d %c", &node, &str_num, &letter);
            Node* T = new Node;
            T->Key = node;
            T->Left = T->Right = nullptr;
            Node* father = nodes[str_num - 1];
            if (letter == 'L') {
                if (father->Key == node)
                    return false;
                father->Left = T;
            }
            else
                father->Right = T;
            nodes[i] = T;
        }
        return isBST(root);
    }
    return true;
}

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/bst.in", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/bst.out", "w", stdout);

    if (processTree())
        printf("YES");
    else
        printf("NO");
    return 0;
}