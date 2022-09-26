#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

namespace debug{
    void print();
    void println();

    template<class T>
    void print(const T& arg){
        cout << arg;
    }

    template<class T>
    void print(const vector<T>& arg){
        cout << "[";
        bool is_first = true;
        for(const T& e : arg){
            if(!is_first) cout << ", ";
            else is_first = false;
            print(e);
        }
        cout << "]";
    }

    template<class K, class V>
    void print(const unordered_map<K, V>& arg){
        cout << "{";
        bool is_first = true;
        for(const pair<K, V>& p : arg){
            if(!is_first) cout << ", ";
            else is_first = false;
            print(p.first);
            cout << ": ";
            print(p.second);
        }
        cout << "}";
    }

    template<class T>
    void print(const initializer_list<T>& arg){
        print(vector<T>(arg));
    }
    
    template<class T>
    void println(const T& arg){
        print(arg);
        cout << endl;
    }
}