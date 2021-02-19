#include <iostream>
#include <vector>
using namespace std;


struct Node {
    int key, height, depth, level;
    Node* Left;
    Node* Right;

    explicit Node(int x) {
        key = x;
        Left = Right = nullptr;
        height = depth = level = -1;
    }
};
Node* root = nullptr;

struct Path {
    int sum;
    Node* center;

    Path(Node* c, int s) {
        center = c;
        sum = s;
    }
};
vector<Node*> nodes, leaves;
vector<Path> paths;

void insert(int value) {
    Node* T = root;
    while(T) {
        if (value < T->key) {
            if (!T->Left) {
                T->Left = new Node(value);
                return;
            }
            T = T->Left;
        }
        else if (value > T->key) {
            if (!T->Right) {
                T->Right = new Node(value);
                return;
            }
            T = T->Right;
        }
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

void setDepth(Node* T, Node* Prev) {
    if (!T)
        return;
    if (!Prev)
        T->depth = 0;
    else
        T->depth = Prev->depth + 1;
    Prev = T;
    nodes.push_back(T);
    setDepth(T->Left, Prev);
    setDepth(T->Right, Prev);
}
void setHeightAndLevel(Node* T) {
    if (!T)
        return;
    else {
        setHeightAndLevel(T->Left);
        setHeightAndLevel(T->Right);
        if (!T->Left && !T->Right) {
            T->height = 0;
            leaves.push_back(T);
        }
        else if (!T->Right)
            T->height = T->Left->height + 1;
        else if (!T->Left)
            T->height = T->Right->height + 1;
        else {
            if (T->Left->height < T->Right->height)
                T->height = 1 + T->Right->height;
            else
                T->height = 1 + T->Left->height;
        }
        T->level = T->height - T->depth;
    }
}

void rotateRight(Node* Grandad, Node* Father, Node* Son) {
    Father->Right = Son->Left;
    Son->Left = Father;
    if (!Grandad)
        root = Son;
    else {
        if (Grandad->Left == Father)
            Grandad->Left = Son;
        else
            Grandad->Right = Son;
    }
}
void rotateLeft(Node* Grandad, Node* Father, Node* Son) {
    Father->Left = Son->Right;
    Son->Right = Father;
    if (!Grandad)
        root = Son;
    else {
        if (Grandad->Left == Father)
            Grandad->Left = Son;
        else
            Grandad->Right = Son;
    }
}
void rotation(Node* T) {
    while(T && T != root) {
        Node *Son = root, *Father = nullptr, *Grandad;
        while (Son != T) {
            Grandad = Father;
            Father = Son;
            if (T->key < Son->key)
                Son = Son->Left;
            else
                Son = Son->Right;
        }
        if (Father && Father->Left == Son)
            rotateLeft(Grandad, Father, Son);
        else if (Father && Father->Right == Son)
            rotateRight(Grandad, Father, Son);
    }
}

void processOneLeaf() {
    cout << root->key + leaves.at(0)->key << '\n';
    if (!(leaves.at(0)->depth % 2)) {
        Node* center = leaves.at(0);
        for (auto & node : nodes)
            if (node->depth == (leaves.at(0)->depth / 2)) {
                center = node;
                break;
            }
        rotation(center);
    }
    preOrderTraversal();
}
int findMaxRootLeafPath(int& maxPathIndex) {
    vector<Node*> path;
    Node* T;
    int maxLength = 0;
    for(auto & leaf : leaves) {
        path.clear();
        T = root;
        while (T) {
            path.push_back(T);
            if(leaf->key < T->key)
                T = T->Left;
            else if(leaf->key > T->key)
                T = T->Right;
            else
                break;
        }
        if(path.size() > maxLength) {
            maxPathIndex = paths.size();
            maxLength = path.size();
        }

        if(path.size() % 2 == 1)
            paths.emplace_back(path.at(path.size() / 2), root->key + leaf->key);
        else
            paths.emplace_back(nullptr, root->key + leaf->key);
    }
    return maxLength;
}

Node* findCenter(int k1, int k2, Node* begin, Node* end, Node* T) {
    Node* center = nullptr;
    if ((k1 + k2) % 2 == 0) {
        center = T;
        if (k1 < k2)
            while (k1 != k2) {
                if (end->key < center->key)
                    center = center->Left;
                else if (end->key > center->key)
                    center = center->Right;
                else
                    break;
                k1++;
                k2--;
            }
        else if (k1 > k2)
            while (k1 != k2) {
                if (begin->key < center->key)
                    center = center->Left;
                else if (begin->key > center->key)
                    center = center->Right;
                else
                    break;
                k1--;
                k2++;
            }
    }
    return center;
}

void findMaxLeafLeafPath(int& maxPathIndex, int maxLength) {
    Node *leaf1, *leaf2, *T;
    int k1(0), k2(0), path_len;
    for (int i = 0; i < leaves.size() - 1; i++) {
        for (int j = i + 1; j < leaves.size(); j++) {
            path_len = 0;
            leaf1 = leaves.at(i);
            leaf2 = leaves.at(j);
            T = root;
            while(T) {
                if(leaf1->key > T ->key && leaf2->key > T->key)
                    T = T->Right;
                else if(leaf1->key < T->key && leaf2->key < T->key)
                    T=T->Left;
                else {
                    Node *T1(T), *T2(T), *father1, *father2;
                    while (T1) {
                        if(leaf1->key == T1->key)
                            break;
                        father1 = T1;
                        path_len++;
                        k1++;
                        if (leaf1->key < T1->key)
                            T1 = T1->Left;
                        else
                            T1 = T1->Right;
                    }
                    while (T2) {
                        if(leaf2->key == T2->key)
                            break;
                        father2 = T2;
                        path_len++;
                        k2++;
                        if (leaf2->key < T2->key)
                            T2 = T2->Left;
                        else
                            T2 = T2->Right;
                    }
                    if(leaf1->level == leaf2->level) {
                        path_len--;
                        if (leaf1->key + father2->key <= leaf2->key + father2->key) {
                            k2--;
                            paths.emplace_back(findCenter(k1, k2, leaf1, father2, T), leaf1->key + father2->key);
                        }
                        else {
                            k1--;
                            paths.emplace_back(findCenter(k1, k2, father1, leaf2, T), father1->key + leaf2->key);
                        }
                    }
                    else
                        paths.emplace_back(findCenter(k1, k2, leaf1, leaf2, T), leaf1->key + leaf2->key);
                    if (path_len > maxLength) {
                        maxPathIndex = (int) paths.size() - 1;
                        maxLength = path_len;
                    }
                    break;
                }
            }
        }
    }
}

void processTree() {
    setDepth(root, nullptr);
    setHeightAndLevel(root);
    if (leaves.size() == 1) {
        processOneLeaf();
        return;
    }
    int maxPathIndex = 0;
    findMaxLeafLeafPath(maxPathIndex, findMaxRootLeafPath(maxPathIndex));

    cout << paths[maxPathIndex].sum << '\n';
    rotation(paths[maxPathIndex].center);
    preOrderTraversal();
}

int main() {
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/in.txt", "r", stdin);
    freopen("/home/akim/Other/C++/clion-workspace/Algorithms/tests/out.txt", "w", stdout);
    int node;
    cin >> node;
    root = new Node(node);
    while(cin >> node)
        insert(node);

    processTree();
    return 0;
}
