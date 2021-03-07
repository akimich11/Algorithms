//
// Created by akim on 06.03.2021.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;

struct node {
    long long key;
    int height, depth, level, msl;
    node *Left, *Right, *Father;

    explicit node(long long x) {
        key = x;
        Left = Right = Father = nullptr;
        height = depth = level = msl = -1;
    }
};
node* root = nullptr, *minRoot = nullptr;
int maxMSL = -1;

void insert(long long value) {
    node* T = root;
    while(T) {
        if(value < T -> key) {
            if(T -> Left)
                T = T -> Left;
            else {
                T -> Left = new node(value);
                T -> Left -> Father = T;
                return;
            }
        }
        else if (value > T -> key) {
            if (T -> Right)
                T = T -> Right;
            else {
                T -> Right = new node(value);
                T -> Right -> Father = T;
                return;
            }
        }
        else
            break;
    }
}
void preOrderTraversal(ofstream& out, node*& T = root) {
    if (!T)
        return;
    else {
        out << T->key << '\n';
        preOrderTraversal(out, T->Left);
        preOrderTraversal(out, T->Right);
    }
}

void setDepth(node* T) {
    if (!T)
        return;
    if (!T->Father)
        T->depth = 0;
    else
        T->depth = T->Father->depth + 1;
    setDepth(T->Left);
    setDepth(T->Right);
}
void setHeightAndMSL(node* T) {
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
void setLevel(node* T) {
    if (!T)
        return;
    else {
        setLevel(T->Left);
        setLevel(T->Right);
        T->level = root->height - T->depth;
    }
}

void rotateLeft(node* Son) {
    node *Dad = Son->Father, *Grandad = Son->Father->Father;
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
void rotateRight(node* Son) {
    node *Dad = Son->Father, *Grandad = Son->Father->Father;
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
void rotation(node* T) {
    node *Son = root;
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

node* findCenter(int& l1, int& l2, node* left, node* right, node* T) {
    node* center = nullptr;
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
node* findEdge(node* T, int& length) {
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


void findEdges(int& l1, int& l2, node* &leftEdge, node* &rightEdge, node* T) {
    if(!T->Left)
        leftEdge = T;
    else
        leftEdge = findEdge(T->Left, ++l1);
    if(!T->Right)
        rightEdge = T;
    else
        rightEdge = findEdge(T->Right, ++l2);

    if(leftEdge->level == rightEdge->level) {
        /*есть 4 варианта:
             * левый брат отца правого края + левый край
             * левый брат отца левого края + правый край
             * отец правого + левый край
             * отец левого + правый край
             * надо выбрать минимальный
        */
        long long sum1, sum2, sum3 = INT64_MAX, sum4 = INT64_MAX, minimum;
        sum1 = leftEdge->key + rightEdge->Father->key;
        sum2 = rightEdge->key + leftEdge->Father->key;

        if(leftEdge->Father->Father && rightEdge->Father->Father &&
            leftEdge->Father->Father->Left && rightEdge->Father->Father->Left) {
            sum3 = leftEdge->key + rightEdge->Father->Father->Left->key;
            sum4 = rightEdge->key + leftEdge->Father->Father->Left->key;
            minimum = min4(sum1, sum3, sum2, sum4);
        }
        else if(leftEdge->Father->Father && leftEdge->Father->Father->Left) {
            sum4 = rightEdge->key + leftEdge->Father->Father->Left->key;
            minimum = min3(sum1, sum2, sum4);
        }
        else if(rightEdge->Father->Father && rightEdge->Father->Father->Left) {
            sum3 = leftEdge->key + rightEdge->Father->Father->Left->key;
            minimum = min3(sum1, sum2, sum3);
        }
        else
            minimum = min(sum1, sum2);

        if(minimum == sum1 || minimum == sum3) {
            if (minimum == sum1)
                rightEdge = rightEdge->Father;
            else if (minimum == sum3)
                rightEdge = rightEdge->Father->Father->Left;
            l2--;
        }
        else {
            if (minimum == sum2)
                leftEdge = leftEdge->Father;
            else
                leftEdge = leftEdge->Father->Father->Left;
            l1--;
        }
    }
}

bool processTree(ofstream& out) {
    setDepth(root);
    setHeightAndMSL(root);
    setLevel(root);

    int l1(0), l2(0);
    node* leftEdge = nullptr, *rightEdge = nullptr, *center;

    findEdges(l1, l2, leftEdge, rightEdge, minRoot);
    center = findCenter(l1, l2, leftEdge, rightEdge, minRoot);
    out << leftEdge->key + rightEdge->key << '\n';
    rotation(center);
    preOrderTraversal(out);
    return minRoot->msl == l1 + l2;
}

bool DeleteIfSingle(node* & node) {
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
node* Leftmost(node* node) {
    if (node == nullptr)
        return nullptr;
    if (node -> Left != nullptr)
        return Leftmost(node -> Left);
    return node;
}
void Delete(node* nod) {
    if (nod == nullptr)
        return;
    if(!DeleteIfSingle(nod)) {
        node* newNode = Leftmost(nod->Right);
        nod -> key = newNode -> key;
        DeleteIfSingle(newNode);
        return;
    }
}
void clearTree(node* T) {
    if(T) {
        Delete(T->Left);
        Delete(T->Right);
        delete T;
    }
}







class Tree;
class Node
{
private:
    int data;
    Node *left;
    Node *right;

    int height;
    int depth;
    int level;
    int max_halfpath_length;

public:
    friend Tree;
    Node() { data = 0; left = 0; right = 0; height = -1; depth = -1; level = -1; max_halfpath_length = -1; }
    Node(int x) : data(x), left(0), right(0) { height = -1; depth = -1; level = -1; max_halfpath_length = -1; }
    int getdata() { return data; }
    Node *getleft() { return left; }
    Node *getright() { return right; }
};

vector<Node*> lists;
vector<pair<vector<Node*>, int>> all_pathes;
vector<Node*> all_vertexes;
vector<Node*> end_vertex;

class Tree
{
    Node *root;
public:
    Tree() :root(0) {}



    bool insert(int x)
    {

        bool b = true;
        Node** ppPrev = &root;
        Node* p = root;


        while (p)
        {
            if (x < p->data)
            {
                ppPrev = &(p->left);
                p = p->left;
            }
            else
            {
                if (x > p->data)
                {
                    ppPrev = &(p->right);
                    p = p->right;
                }
                else
                {
                    b = false;
                    return b;
                }
            }

        }


        if (b == true)
        {
            *ppPrev = new Node(x);
        }



        return b;
    }




    void remove_node(int x) {



        Node* prev = nullptr;
        Node* cur = root;

        while (cur)
        {



            if (x < cur->data)
            {
                prev = cur;
                cur = cur->left;
            }





            else if (x > cur->data)
            {
                prev = cur;
                cur = cur->right;
            }








            else
            {

                if (cur->right == nullptr && cur->left == nullptr)
                {
                    if (prev)
                    {
                        if (prev->left == cur) prev->left = nullptr;
                        else prev->right = nullptr;
                        return;
                    }
                    else { root = nullptr; return; }
                }

                else if (cur->right == nullptr)
                {
                    if (prev)
                    {
                        if (prev->left == cur) prev->left = cur->left;
                        else prev->right = cur->left;
                        return;
                    }
                    else
                    {
                        root = cur->left; return;
                    }
                }

                else if (cur->left == nullptr)
                {
                    if (prev)
                    {
                        if (prev->left == cur) prev->left = cur->right;
                        else prev->right = cur->right;
                        return;
                    }
                    else
                    {
                        root = cur->right; return;
                    }
                }


                else
                {



                    if (cur->right->left == nullptr)
                    {
                        cur->data = cur->right->data;
                        cur->right = cur->right->right; return;
                    }
                    else
                    {
                        Node* temp = cur;
                        Node* predok = cur->right;
                        cur = cur->right->left;

                        while (cur)
                        {
                            if (cur->left)
                                predok = predok->left;
                            prev = cur;
                            cur = cur->left;
                        }


                        temp->data = prev->data;
                        predok->left = prev->right;

                    }



                }



            }





        }

        return;

    }



    /////////////////////////////////////////////////
    //              Ðåêóðñèâíûé îòñîðòèðîâàííûé âûâîä (inorder)
    void inorder_rec(ofstream& fo)
    {
        inorder_rec2(root, fo);
    }

    void inorder_rec2(Node *t, ofstream& fo)
    {
        if (t == nullptr)
        {
            return;
        }
        else
        {
            inorder_rec2(t->left, fo);
            fo << t->data << "\n";
            inorder_rec2(t->right, fo);
        }
    }




    /////////////////////////////////////////////////
    //                       Ðåêóðñèâíûé ïðÿìîé âûâîä (preorder)
    void preorder_rec(ofstream& fo)
    {
        preorder_rec2(root, fo);
    }

    void preorder_rec2(Node *t, ofstream& fo)
    {
        if (t == nullptr)
        {
            return;
        }
        else
        {


            fo << t->data << endl;
            preorder_rec2(t->left, fo);
            preorder_rec2(t->right, fo);
        }
    }




    /////////////////////////////////////////////////
    //                     Ðåêóðñèâíûé îáðàòíûé âûâîä (postorder)
    void postorder_rec(ofstream& fo)
    {
        postorder_rec2(root, fo);
    }

    void postorder_rec2(Node *t, ofstream& fo)
    {
        if (t == nullptr)
        {
            return;
        }
        else
        {
            postorder_rec2(t->left, fo);
            postorder_rec2(t->right, fo);
            fo << t->data << endl;
        }
    }








    void put_depth(Node* t, Node* prevt) {

        if (t == nullptr) return;

        if (!prevt)
        {
            t->depth = 0;
        }

        else
        {
            t->depth = prevt->depth + 1;
        }

        prevt = t;
        all_vertexes.push_back(t);
        put_depth(t->left, prevt);
        put_depth(t->right, prevt);


    }

    void put_depth()
    {
        put_depth(root, nullptr);
    }






    void put_numerics(Node* t) {


        Node* cur = t;
        if (cur == nullptr) return;


        else
        {
            put_numerics(cur->left);
            put_numerics(cur->right);
            if (cur->left == nullptr && cur->right == nullptr)
            {
                cur->height = 0;
                cur->max_halfpath_length = 0;
                lists.push_back(cur);
            }
            else if (cur->left == nullptr)
            {
                cur->height = cur->right->height + 1;
                cur->max_halfpath_length = cur->height;
            }
            else if (cur->right == nullptr)
            {
                cur->height = cur->left->height + 1;
                cur->max_halfpath_length = cur->height;
            }
            else
            {
                cur->height = (cur->left->height > cur->right->height) ? 1 + cur->left->height : 1 + cur->right->height;
                cur->max_halfpath_length = 2 + cur->left->height + cur->right->height;
            }
            cur->level = cur->height - cur->depth;

        }


    }

    void put_numerics()
    {
        put_numerics(root);
    }






    void pp(ofstream& fo) {

        put_depth();
        put_numerics();




        vector<Node*> path;
        vector<pair<vector<Node*>, int>> pathes_l;
        int k1 = 0;
        int k2 = 0;
        int max_l = 0;
        int s = 0;






        if (lists.size() > 1) {





            if (root->left == nullptr || root->right == nullptr)
            {


                Node* z = root;
                Node* cur = nullptr;
                Node* central = nullptr;

                max_l = lists.at(0)->depth;
                s = lists.at(0)->data + root->data;


                for (int i = 0; i < lists.size(); i++)
                {
                    path.clear();
                    path.push_back(z);
                    cur = lists.at(i);
                    z = root;
                    while (z)
                    {
                        if (cur->data < z->data)
                        {
                            z = z->left;
                            path.push_back(z);
                            if (cur->depth % 2 == 0 && z->depth == (cur->depth / 2)) central = z;
                        }
                        else if (cur->data > z->data)
                        {
                            z = z->right;
                            path.push_back(z);
                            if (cur->depth % 2 == 0 && z->depth == (cur->depth / 2)) central = z;
                        }
                        else if (cur->data == z->data)
                        {
                            break;
                        }
                    }

                    if (central) { path.push_back(central); path.push_back(central); }
                    all_pathes.push_back(make_pair(path, root->data + cur->data));

                    if (path.at(path.size() - 1) == path.at(path.size() - 2))
                    {
                        if (path.size() - 2 > max_l) max_l = path.size() - 2;
                    }
                    else
                    {
                        if (path.size() > max_l) max_l = path.size();
                    }

                }


            }






            for (int i = 0; i < lists.size() - 1; i++)
            {
                for (int j = i + 1; j < lists.size(); j++)
                {

                    path.clear();
                    k1 = 0; k2 = 0; s = 0;
                    Node* cur1 = lists.at(i);
                    Node* cur2 = lists.at(j);
                    Node* central = nullptr;






                    if (cur1->level != cur2->level)
                    {
                        s = cur1->data + cur2->data;
                        Node* cur = root;
                        while (cur)
                        {
                            if (cur1->data < cur->data && cur2->data > cur->data)
                            {

                                path.push_back(cur);
                                Node* cur3 = cur;
                                Node* cur4 = cur;

                                while (cur3)
                                {
                                    if (cur1->data < cur3->data) {
                                        cur3 = cur3->left; k1++;
                                        path.push_back(cur3);
                                    }
                                    else if (cur1->data > cur3->data) {
                                        cur3 = cur3->right; k1++;
                                        path.push_back(cur3);
                                    }
                                    else if (cur1->data == cur3->data) {
                                        break;
                                    }
                                }


                                while (cur4)
                                {
                                    if (cur2->data < cur4->data) {
                                        cur4 = cur4->left; k2++;
                                        path.push_back(cur4);
                                    }
                                    else if (cur2->data > cur4->data) {
                                        cur4 = cur4->right; k2++;
                                        path.push_back(cur4);
                                    }
                                    else if (cur2->data == cur4->data) {
                                        break;
                                    }
                                }



                                if ((k1 + k2) % 2 == 0)
                                {
                                    central = cur;
                                    if (k1 < k2)
                                    {
                                        central = cur;

                                        while (k1 != k2)
                                        {
                                            if (cur2->data < central->data)
                                            {
                                                central = central->left;
                                                k1++; k2--;
                                            }
                                            else if (cur2->data > central->data)
                                            {
                                                central = central->right;
                                                k1++; k2--;
                                            }
                                            else break;
                                        }
                                    }

                                    else if (k1 > k2)
                                    {
                                        central = cur;

                                        while (k1 != k2)
                                        {
                                            if (cur1->data < central->data)
                                            {
                                                central = central->left;
                                                k1--; k2++;
                                            }
                                            else if (cur1->data > central->data)
                                            {
                                                central = central->left;
                                                k1--; k2++;
                                            }
                                            else break;
                                        }
                                    }
                                    path.push_back(central); path.push_back(central);

                                }


                                all_pathes.push_back(make_pair(path, s));


                                if (path.at(path.size() - 1) == path.at(path.size() - 2))
                                {
                                    if (path.size() - 2 > max_l) max_l = path.size() - 2;
                                }
                                else
                                {
                                    if (path.size() > max_l) max_l = path.size();
                                }



                                break;
                            }


                            else if (cur1->data < cur->data && cur2->data < cur->data)
                            {
                                cur = cur->left;
                            }
                            else if (cur1->data > cur->data && cur2->data > cur->data)
                            {
                                cur = cur->right;
                            }


                        }

                    }







                    else
                    {
                        s = 0;
                        Node* prev1 = nullptr;
                        Node* prev2 = nullptr;
                        Node* prev = nullptr;
                        Node* cur = root;
                        while (cur)
                        {
                            if (cur1->data < cur->data && cur2->data > cur->data)
                            {

                                path.push_back(cur);
                                Node* cur3 = cur;
                                Node* cur4 = cur;

                                while (cur3)
                                {
                                    if (cur1->data < cur3->data) {
                                        prev2 = cur3;
                                        cur3 = cur3->left; k1++;
                                        if (cur3 != cur1)
                                            path.push_back(cur3);
                                    }
                                    else if (cur1->data > cur3->data) {
                                        prev2 = cur3;
                                        cur3 = cur3->right; k1++;
                                        if (cur3 != cur1)
                                            path.push_back(cur3);
                                    }
                                    else if (cur1->data == cur3->data) {
                                        break;
                                    }
                                }


                                while (cur4)
                                {
                                    if (cur2->data < cur4->data) {
                                        prev1 = cur4;
                                        cur4 = cur4->left; k2++;
                                        if (cur4 != cur2)
                                            path.push_back(cur4);
                                    }
                                    else if (cur2->data > cur4->data) {
                                        prev1 = cur4;
                                        cur4 = cur4->right; k2++;
                                        if (cur4 != cur2)
                                            path.push_back(cur4);
                                    }
                                    else if (cur2->data == cur4->data) {
                                        break;
                                    }
                                }

                                if (cur1->data + prev1->data <= cur2->data + prev2->data)
                                {
                                    s = cur1->data + prev1->data;
                                    prev = prev1;
                                    path.push_back(cur1);
                                    k2--;
                                }
                                else {
                                    s = cur2->data + prev2->data;
                                    prev = prev2;
                                    path.push_back(cur2);
                                    k1--;
                                }

                                if ((k1 + k2) % 2 == 0)
                                {
                                    central = cur;
                                    if (k1 < k2)
                                    {
                                        central = cur;

                                        while (k1 != k2)
                                        {
                                            if (prev->data < central->data)
                                            {
                                                central = central->left;
                                                k1++; k2--;
                                            }
                                            else if (prev->data > central->data)
                                            {
                                                central = central->right;
                                                k1++; k2--;
                                            }
                                            else break;
                                        }
                                    }

                                    else if (k1 > k2)
                                    {
                                        central = cur;

                                        while (k1 != k2)
                                        {
                                            if (cur1->data < central->data)
                                            {
                                                central = central->left;
                                                k1--; k2++;
                                            }
                                            else if (cur1->data > central->data)
                                            {
                                                central = central->left;
                                                k1--; k2++;
                                            }
                                            else break;
                                        }
                                    }



                                    path.push_back(central); path.push_back(central);

                                }



                                if (find(end_vertex.begin(), end_vertex.end(), prev1) == end_vertex.end()) {
                                    all_pathes.push_back(make_pair(path, s));
                                    end_vertex.push_back(prev1);
                                }



                                if (path.at(path.size() - 1) == path.at(path.size() - 2))
                                {
                                    if (path.size() - 2 > max_l) max_l = path.size() - 2;
                                }
                                else
                                {
                                    if (path.size() > max_l) max_l = path.size();
                                }


                                break;
                            }


                            else if (cur1->data < cur->data && cur2->data < cur->data)
                            {
                                cur = cur->left;
                            }
                            else if (cur1->data > cur->data && cur2->data > cur->data)
                            {
                                cur = cur->right;
                            }


                        }
                    }




                }

            }





        }









            ///////////////////1 list
        else {

            if (lists.at(0)->depth % 2 == 0)
            {
                Node* nod = lists.at(0);
                fo << root->data + nod->data << endl;
                for (int i = 0; i < all_vertexes.size(); ++i)
                {
                    if (all_vertexes.at(i)->depth == (lists.at(0)->depth / 2))  nod = all_vertexes.at(i);
                }


                make_root(nod);
                preorder_rec(fo);
                return;

            }
            else { fo << root->data + lists.at(0)->data << endl; preorder_rec(fo); return; }
        }







        int q = 0;
        for (int i = 0; i < all_pathes.size(); ++i)
        {
            q = all_pathes.at(i).first.size();
            if (all_pathes.at(i).first.at(q - 1) == all_pathes.at(i).first.at(q - 2))
            {
                if (all_pathes.at(i).first.size() - 2 == max_l) pathes_l.push_back(all_pathes.at(i));
            }
            else if (all_pathes.at(i).first.size() == max_l) pathes_l.push_back(all_pathes.at(i));
        }







        if (!pathes_l.empty())
        {
            vector<Node*> we_need = pathes_l.at(0).first;
            Node* centr = nullptr;
            Node* p1; Node* p2;
            int sum = 0;
            int min_q = pathes_l.at(0).second;

            for (int i = 0; i < pathes_l.size(); ++i)
            {
                if (pathes_l.at(i).second < min_q)
                {
                    min_q = pathes_l.at(i).second;
                    we_need = pathes_l.at(i).first;
                }
            }



            if (we_need.at(we_need.size() - 1) == we_need.at(we_need.size() - 2))
                make_root(we_need.at(we_need.size() - 1));


            fo << min_q << endl;

        }



        preorder_rec(fo);

    }







    void change(Node* prevparent, Node* parent, Node* cur)
    {


        if (parent->left == cur) {

            parent->left = cur->right;
            cur->right = parent;


            if (prevparent == nullptr)
            {
                root = cur;
            }
            else
            {
                if (prevparent->left == parent)
                {
                    prevparent->left = cur;
                }
                else prevparent->right = cur;
            }

        }


        if (parent->right == cur) {

            parent->right = cur->left;
            cur->left = parent;


            if (prevparent == nullptr)
            {
                root = cur;
            }
            else
            {
                if (prevparent->left == parent)
                {
                    prevparent->left = cur;
                }
                else prevparent->right = cur;
            }



        }



    }


    void make_root(Node* cur)
    {
        if (cur == nullptr || cur == root) return;
        Node* temp = root;
        Node* prevtemp = nullptr;
        Node* prprtemp = nullptr;
        while (temp->data != cur->data)
        {
            if (cur->data < temp->data)
            {
                prprtemp = prevtemp;
                prevtemp = temp;
                temp = temp->left;
            }
            else if (cur->data > temp->data)
            {
                prprtemp = prevtemp;
                prevtemp = temp;
                temp = temp->right;
            }
        }
        change(prprtemp, prevtemp, temp);

        make_root(cur);

    }










    void out_depth(Node* t)
    {
        if (t == nullptr) return;
        cout << t->data << "   " << t->depth << endl;
        out_depth(t->left);
        out_depth(t->right);
    }
    void out_depth() { out_depth(root); }



    void out_height(Node* t)
    {
        if (t == nullptr) return;
        cout << t->data << "   " << t->height << endl;
        out_height(t->left);
        out_height(t->right);
    }
    void out_height() { out_height(root); }



    void out_level(Node* t)
    {
        if (t == nullptr) return;
        cout << t->data << "   " << t->level << endl;
        out_level(t->left);
        out_level(t->right);
    }
    void out_level() { out_level(root); }


    void out_path(Node* t)
    {
        if (t == nullptr) return;
        cout << t->data << "   " << t->max_halfpath_length << endl;
        out_path(t->left);
        out_path(t->right);
    }
    void out_path() { out_path(root); }


};










bool compareFiles() {
    ifstream fs1("/home/akim/Other/C++/clion-workspace/Algorithms/tests/my_solution_out.txt"),
    fs2("/home/akim/Other/C++/clion-workspace/Algorithms/tests/github_solution_out.txt");
    int sum1, sum2;
    return !(fs1 >> sum1 && fs2 >> sum2) || sum1 == sum2;
}

int main() {
    srand(time(nullptr));
    int iteration = 0;
    Tree tree;
    const int NODES_NUM = 10;
    long long nod;
    bool answer = true;
    ofstream out1, out2;
    ifstream fin;
    vector<int> n;
    n.reserve(10);

    out1.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/my_solution_out.txt");
    out2.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/github_solution_out.txt");
    out1.close();
    out2.close();

    while(compareFiles() && iteration < 500) {
        root = nullptr, minRoot = nullptr;
        maxMSL = -1;
        iteration++;
//        out1.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/in.txt");
//        for (int i = 0; i < NODES_NUM; ++i)
//            n.push_back(rand() % 1000);
//        shuffle(n.begin(), n.end(), std::mt19937(std::random_device()()));
//        for (int i = 0; i < NODES_NUM; ++i)
//            out1 << n[i] << endl;
//        out1.close();

        fin.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/in.txt");
        out1.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/my_solution_out.txt");
        out2.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/github_solution_out.txt");

        fin >> nod;
        root = new node(nod);
        tree.insert(nod);
        while (fin >> nod) {
            insert(nod);
            tree.insert(nod);
        }
        fin.close();

        processTree(out1);
        tree.pp(out2);
        out1.close();
        out2.close();

        fin.open("/home/akim/Other/C++/clion-workspace/Algorithms/tests/in.txt");
        while (fin >> nod)
            tree.remove_node(nod);
        fin.close();

        clearTree(root);
        n.clear();
        all_vertexes.clear();
        all_pathes.clear();
        lists.clear();
        end_vertex.clear();
    }
    cout << iteration << " tests passed" << endl;
    return 0;
}