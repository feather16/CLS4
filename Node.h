#pragma once

#include "Token.h"

#include <unordered_map>
#include <string>

using namespace std;

class Node{
    public:
    enum class Id{
        error,
        program,
        func_decl,
        statement_list,
        statement,
        basic_statement,
        if_statement,
        while_statement,
        var_decl,
        assignment_like,
        expression,
        type_name,
        func_name,
        var_name,
        func_call,
        return_statement,
        built_in_func,
        built_in_procedure,
        literal,
        uint_literal,
        string_literal,
    };

    Id id = Id::error;
    
    string name;
    Token token;
    vector<Node> leaves;
    Node(){}
    Node(const string&);
    Node(const string&, const Token&);
    void add_leaf(const Node&);
    Token get_first_token() const;
    string to_string(const int indent = 0) const;
    int size() const;
    friend ostream& operator<<(ostream& os, const Node& node){
        os << node.to_string();
        return os;
    }
    Node& operator[](const int n);

    vector<Node>::const_iterator begin() const noexcept;
    vector<Node>::const_iterator end() const noexcept;

    // 文字列をNodeのIDに変換
    static unordered_map<string, Id> str_to_id;

    // NodeのIDを文字列に変換
    static unordered_map<Id, string> id_to_str;
};

