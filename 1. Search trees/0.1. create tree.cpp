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

void Insert(int c, Node* &T=root)
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
    int node;
    while(in.peek() != EOF) {
        in >> node;
        Insert(node);
    }
    in.close();
    preorderPrint(root, out);
    out.close();
    return 0;
}