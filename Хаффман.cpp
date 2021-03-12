//
// Created by akim on 10/03/2021.
//
#include <iostream>
#include <queue>
using namespace std;

struct Node {
    unsigned long long key;
    Node *Left, *Right;

    explicit Node(unsigned long long x) {
        key = x;
        Left = Right = nullptr;
    }
};

bool comparator(Node* a, Node* b) {
    return a->key >= b->key;
}

void preorderTraversal(Node* T, unsigned long long& sum) {
    if(!T)
        return;
    if(T->Left || T->Right)
        sum += T->key;
    preorderTraversal(T->Left, sum);
    preorderTraversal(T->Right, sum);
}

int main() {
    freopen("huffman.in", "r", stdin);
    freopen("huffman.out", "w", stdout);
    priority_queue<Node*, vector<Node*>, decltype(&comparator)> huffman(comparator);
    int n, el;
    cin >> n;
    Node *T, *Q, *P;
    unsigned long long sum = 0;
    for (int i = 0; i < n; ++i) {
        cin >> el;
        huffman.push(new Node(el));
    }
    while (huffman.size() != 1) {
        T = huffman.top();
        huffman.pop();
        Q = huffman.top();
        huffman.pop();
        P = new Node(T->key + Q->key);
        P->Left = T;
        P->Right = Q;
        huffman.push(P);
    }
    preorderTraversal(huffman.top(), sum);

    cout << sum;
    return 0;
}