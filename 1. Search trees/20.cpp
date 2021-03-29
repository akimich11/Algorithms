//
// Created by akim on 06.03.2021.
//
#include <iostream>
#include <vector>

using namespace std;

struct Node {
    long long key;
    int height, depth, level, msl;
    Node *Left, *Right, *Father;

    explicit Node(long long x) {
        key = x;
        Left = Right = Father = nullptr;
        height = depth = level = msl = -1;
    }
};
Node* root = nullptr, *minRoot = nullptr;
vector<Node*> leftLeaves, rightLeaves;
int maxMSL = -1;

void insert(long long value) {
    Node* T = root;
    while(T) {
        if(value < T -> key) {
            if(T -> Left)
                T = T -> Left;
            else {
                T -> Left = new Node(value);
                T -> Left -> Father = T;
                return;
            }
        }
        else if (value > T -> key) {
            if (T -> Right)
                T = T -> Right;
            else {
                T -> Right = new Node(value);
                T -> Right -> Father = T;
                return;
            }
        }
        else
            break;
    }
}
void preOrderTraversal(Node*& T = root) {
    if (!T)
        return;
    else {
        cout << T->key << '\n';
        preOrderTraversal(T->Left);
        preOrderTraversal(T->Right);
    }
}

void setDepth(Node* T) {
    if (!T)
        return;
    if (!T->Father)
        T->depth = 0;
    else
        T->depth = T->Father->depth + 1;
    setDepth(T->Left);
    setDepth(T->Right);
}
void setHeightAndMSL(Node* T) {
    if (!T)
        return;
    else {
        setHeightAndMSL(T->Left);
        setHeightAndMSL(T->Right);
        if (!T->Left && !T->Right)
            T->height = T->msl = 0;
        else if (!T->Right)
            T->height = T->msl = 1 + T->Left->height;
        else if (!T->Left)
            T->height = T->msl = 1 + T->Right->height;
        else {
            T->height = 1 + max(T->Right->height, T->Left->height);
            T->msl = 2 + T->Left->height + T->Right->height;
            if(T->Right->height == T->Left->height)
                T->msl--;
        }
        if(T->msl >= maxMSL) {
            if(T->msl > maxMSL || (T->msl == maxMSL && (!minRoot || T->key < minRoot->key)))
                minRoot = T;
            maxMSL = T->msl;
        }
    }
}
void setLevel(Node* T) {
    if (!T)
        return;
    else {
        setLevel(T->Left);
        setLevel(T->Right);
        T->level = root->height - T->depth;
    }
}

void rotateLeft(Node* Son) {
    Node *Dad = Son->Father, *Grandad = Son->Father->Father;
    if(Son->Left)
        Son->Left->Father = Dad;
    Dad->Right = Son->Left;
    Dad->Father = Son;
    Son->Left = Dad;
    if (!Grandad) {
        Son->Father = root;
        root = Son;
    }
    else {
        Son->Father = Grandad;
        if (Grandad->Left == Dad)
            Grandad->Left = Son;
        else
            Grandad->Right = Son;
    }
}
void rotateRight(Node* Son) {
    Node *Dad = Son->Father, *Grandad = Son->Father->Father;
    if(Son->Right)
        Son->Right->Father = Dad;
    Dad->Left = Son->Right;
    Dad->Father = Son;
    Son->Right = Dad;
    if (!Grandad) {
        Son->Father = root;
        root = Son;
    }
    else {
        Son->Father = Grandad;
        if (Grandad->Left == Dad)
            Grandad->Left = Son;
        else
            Grandad->Right = Son;
    }
}
void rotation(Node* T) {
    Node *Son = root;
    while(T && T != root) {
        while (Son != T) {
            if (T->key < Son->key)
                Son = Son->Left;
            else
                Son = Son->Right;
        }
        if (Son->Father->Left == Son)
            rotateRight(Son);
        else if (Son->Father->Right == Son)
            rotateLeft(Son);
    }
}

Node* findCenter(int& l1, int& l2, Node* left, Node* right, Node* T) {
    Node* center = nullptr;
    if (T->msl % 2 == 0) {
        center = T;
        if (l1 < l2)
            while (l1 != l2) {
                if (right->key < center->key)
                    center = center->Left;
                else if (right->key > center->key)
                    center = center->Right;
                else
                    break;
                l1++;
                l2--;
            }
        else
            while (l1 != l2) {
                if (left->key < center->key)
                    center = center->Left;
                else if (left->key > center->key)
                    center = center->Right;
                else
                    break;
                l1--;
                l2++;
            }
    }
    return center;
}

void findLeaves(Node* T, vector<Node*>& leaves) {
    if(!T)
        return;
    if(!T->Left && !T->Right)
        leaves.push_back(T);
    findLeaves(T->Left, leaves);
    findLeaves(T->Right, leaves);
}

void findEdges(int& l1, int& l2, Node* &leftEdge, Node* &rightEdge, Node* T) {
    if(!T->Left)
        leftLeaves.push_back(T);
    else
        findLeaves(T->Left, leftLeaves);
    if(!T->Right)
        rightLeaves.push_back(T);
    else
        findLeaves(T->Right, rightLeaves);

    long long minSum = INT64_MAX;
    for(auto & lLeaf : leftLeaves) {
        for(auto & rLeaf : rightLeaves) {
            l1 = lLeaf->depth - T->depth;
            l2 = rLeaf->depth - T->depth;

            if(lLeaf->level != rLeaf->level && lLeaf->key + rLeaf->key < minSum && l1 + l2 == maxMSL) {
                minSum = lLeaf->key + rLeaf->key;
                leftEdge = lLeaf, rightEdge = rLeaf;
            }
            else if(lLeaf->level == rLeaf->level && l1 + l2 - 1 == maxMSL) {
                if (lLeaf->Father->key + rLeaf->key < rLeaf->Father->key + lLeaf->key) {
                    if (rLeaf->key + lLeaf->Father->key < minSum) {
                        minSum = rLeaf->key + lLeaf->Father->key;
                        leftEdge = lLeaf->Father;
                        rightEdge = rLeaf;
                    }
                }
                else if (lLeaf->key + rLeaf->Father->key < minSum) {
                    minSum = lLeaf->key + rLeaf->Father->key;
                    rightEdge = rLeaf->Father;
                    leftEdge = lLeaf;
                }
            }
        }
    }
    l1 = leftEdge->depth - T->depth;
    l2 = rightEdge->depth - T->depth;
}

void processTree() {
    setDepth(root);
    setHeightAndMSL(root);
    setLevel(root);

    int l1(0), l2(0);
    Node* leftEdge = nullptr, *rightEdge = nullptr, *center;

    findEdges(l1, l2, leftEdge, rightEdge, minRoot);
    center = findCenter(l1, l2, leftEdge, rightEdge, minRoot);
    cout << leftEdge->key + rightEdge->key << '\n';
    rotation(center);
    preOrderTraversal();
}

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/in.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/0. tests/out.txt", "w", stdout);
    long long node;
    cin >> node;
    root = new Node(node);
    while (cin >> node)
        insert(node);

    processTree();
    return 0;
}