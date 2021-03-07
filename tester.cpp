//
// Created by akim on 20.02.2021.
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;


bool compareFiles() {
    ifstream fs1("/home/akim/Other/C++/clion-workspace/Algorithms/out1.txt"), fs2("/home/akim/Other/C++/clion-workspace/Algorithms/out2.txt");
    vector<int> n1, n2;
    int tmp;
    while (fs1 >> tmp)
        n1.push_back(tmp);
    fs1.close();
    while (fs2 >> tmp)
        n2.push_back(tmp);
    fs1.close();
    if(n1.size() != n2.size())
        return false;
    for(int i = 0; i < n1.size(); i++)
        if(n1[i] != n2[i])
            return false;
    return true;
}

int main() {
    if(CompareStringOfFile()) {
        cout << "ответы совпадают" << endl;
        freopen("/home/akim/Other/C++/clion-workspace/Algorithms/in.txt", "w", stdout);
        vector<int> n;
        n.reserve(10);
        for (int i = 0; i < 10; ++i)
            n.push_back(random() % 1000);
        shuffle(n.begin(), n.end(), std::mt19937(std::random_device()()));
        for (int i = 0; i < 10; ++i)
            cout << n.at(i) << endl;
    }
    else
        cout << "ответы не совпадают" << endl;
    return 0;
}