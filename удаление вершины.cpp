#pragma comment(linker, "/STACK:16777216")
#include <fstream>

using namespace std;

struct Node
{
    int key;
    Node* Left;
    Node* Right;
    Node* Father;
};
Node* root = nullptr;

void insert(int c, Node* &T=root)
{
    if(!T)
    {
        T = new Node;
        T -> key = c;
        T -> Left = T -> Right = nullptr;
    }
    else if(c < T -> key)
    {
        if(T -> Left)
        {
            T -> Left -> Father = T;
            Insert(c, T -> Left);
        }
        else
        {
            T -> Left = new Node;
            T -> Left -> Father = T;
            T -> Left -> key = c;
            T -> Left -> Left = T -> Left -> Right = nullptr;
        }
    }
    else if (c > T -> key)
    {
        if (T -> Right)
        {
            T -> Right -> Father = T;
            Insert(c, T -> Right);
        }
        else
        {
            T -> Right = new Node;
            T -> Right -> Father = T;
            T -> Right -> key = c;
            T -> Right -> Left = T -> Right -> Right = nullptr;
        }
    }
}

Node* Leftmost(Node* node)
{
    if (node == nullptr)
        return nullptr;
    if (node -> Left != nullptr)
        return Leftmost(node -> Left);
    return node;
}

bool DeleteIfSingle(Node* & node) {
    //корень
    if(node -> Left == nullptr && node -> Right == nullptr && node -> Father == nullptr) {
        delete node;
        root = nullptr;
        return true;
    }
    else if(node -> Left == nullptr && node -> Father == nullptr) {
        root = node -> Right;
        delete node;
        return true;
    }
    else if(node -> Right == nullptr && node -> Father == nullptr) {
        root = node -> Left;
        delete node;
        return true;
    }
    //лист
    if(node -> Left == nullptr && node -> Right == nullptr)
    {
        if (node -> Father -> Left == node)
            node -> Father -> Left = nullptr;
        else
            node -> Father -> Right = nullptr;
        delete node;
        return true;
    }
    //не лист и не корень
    else if(node -> Left == nullptr) {
        if (node -> Father -> Left == node)
            node -> Father -> Left = node -> Right;
        else
            node -> Father -> Right = node -> Right;
        delete node;
        return true;
    }
    else if(node -> Right == nullptr) {
        if (node -> Father -> Left == node)
            node -> Father -> Left = node -> Left;
        else
            node -> Father -> Right = node -> Left;
        delete node;
        return true;
    }
    return false;
}

void Delete(Node* node, int value)
{
    if (node == nullptr)
        return;
    if (value < node -> key)
        Delete(node -> Left, value);
    else if(value > node -> key)
        Delete(node -> Right, value);
    else
    {
        if(!DeleteIfSingle(node)) {
            Node* newNode = Leftmost(node->Right);
            node -> key = newNode -> key;
            DeleteIfSingle(newNode);
            return;
        }
    }
}

void preorderPrint(Node *Tree, ofstream& out)
{
    if (Tree == nullptr)
        return;
    out << Tree -> key << endl;
    preorderPrint(Tree -> Left, out);
    preorderPrint(Tree -> Right, out);
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    int toDelete, node;
    in >> toDelete;
    while(in.peek() != EOF) {
        in >> node;
        Insert(node);
    }
    in.close();
    Delete(root, toDelete);
    preorderPrint(root, out);
    out.close();
    return 0;
}