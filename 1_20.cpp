//
// Created by akim on 06.03.2021.
//
#include <iostream>

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
Node* findEdge(Node* T, int& length) {
    while(T->Left || T->Right) {
        length++;
        if(!T->Left || (T->Right && T->Right->height > T->Left->height))
            T = T->Right;
        else
            T = T->Left;
    }
    return T;
}

long long min4(long long  a, long long b, long long c, long long d) {
    return min(a, min(b, min(c, d)));
}

long long min3(long long a, long long b, long long c) {
    return min(a, min(b, c));
}


void findEdges(int& l1, int& l2, Node* &leftEdge, Node* &rightEdge, Node* T) {
    if(!T->Left)
        leftEdge = T;
    else
        leftEdge = findEdge(T->Left, ++l1);
    if(!T->Right)
        rightEdge = T;
    else
        rightEdge = findEdge(T->Right, ++l2);

    if(leftEdge->level == rightEdge->level) {
        long long sum1, sum2, sum3 = INT64_MAX, sum4 = INT64_MAX, minimum;
        sum1 = leftEdge->key + rightEdge->Father->key;
        sum2 = rightEdge->key + leftEdge->Father->key;
        Node* grandadLeft = leftEdge->Father->Father, *grandadRight = rightEdge->Father->Father;

        if(grandadLeft && grandadRight && grandadLeft->Left && grandadRight->Left) {
            sum3 = leftEdge->key + grandadRight->Left->key;
            sum4 = rightEdge->key + grandadLeft->Left->key;
            minimum = min4(sum1, sum2, sum3, sum4);
        }
        else if(grandadLeft && grandadLeft->Left) {
            sum4 = rightEdge->key + grandadLeft->Left->key;
            minimum = min3(sum1, sum2, sum4);
        }
        else if(grandadRight && grandadRight->Left) {
            sum3 = leftEdge->key + grandadRight->Left->key;
            minimum = min3(sum1, sum2, sum3);
        }
        else
            minimum = min(sum1, sum2);

        if(minimum == sum1 || minimum == sum3) {
            if (minimum == sum1)
                rightEdge = rightEdge->Father;
            else
                rightEdge = grandadRight->Left;
            l2--;
        }
        else {
            if (minimum == sum2)
                leftEdge = leftEdge->Father;
            else
                leftEdge = grandadLeft->Left;
            l1--;
        }
    }
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
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/in.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/out.txt", "w", stdout);
    long long node;
    cin >> node;
    root = new Node(node);
    while (cin >> node)
        insert(node);

    processTree();
    return 0;
}