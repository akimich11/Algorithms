//
// Created by akim on 15.02.2021.
//
#include <iostream>
using namespace std;

int binarySearch(int left, int right, const int array[], int x) {
    int mid;
    while(right > left) {
        mid = (left + right) / 2;
        if(x == array[mid])
            return 1;
        else if(x > array[mid])
            left = mid + 1;
        else if(x < array[mid])
            right = mid;
    }
    return 0;
}

int lowerBound(int left, int right, const int array[], int x) {
    int mid;
    while(right > left) {
        mid = (left + right) / 2;
        if(x > array[mid])
            left = mid + 1;
        else if(x <= array[mid])
            right = mid;
    }
    return left;
}

int upperBound(int left, int right, const int array[], int x) {
    int mid;
    while(right > left) {
        mid = (left + right) / 2;
        if(x >= array[mid])
            left = mid + 1;
        else if(x < array[mid])
            right = mid;
    }
    return left;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int length, req_num, x;
    cin >> length;
    int* array = new int[length];
    for (int i = 0; i < length; ++i)
        cin >> array[i];
    cin >> req_num;
    for (int i = 0; i < req_num; ++i) {
        cin >> x;
        cout << binarySearch(0, length, array, x) << " " <<
        lowerBound(0, length, array, x) << " " << upperBound(0, length, array, x) << '\n';
    }
    delete[] array;
}
