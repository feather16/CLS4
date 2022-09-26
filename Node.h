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
    #define NODE_ID_ARRANGE(str)\
    {#str, Node::Id:: str},
    static inline unordered_map<string, Id> str_to_id{
        NODE_ID_ARRANGE(program)
        NODE_ID_ARRANGE(func_decl)
        NODE_ID_ARRANGE(statement_list)
        NODE_ID_ARRANGE(statement)
        NODE_ID_ARRANGE(basic_statement)
        NODE_ID_ARRANGE(if_statement)
        NODE_ID_ARRANGE(while_statement)
        NODE_ID_ARRANGE(var_decl)
        NODE_ID_ARRANGE(assignment_like)
        NODE_ID_ARRANGE(expression)
        NODE_ID_ARRANGE(type_name)
        NODE_ID_ARRANGE(func_name)
        NODE_ID_ARRANGE(var_name)
        NODE_ID_ARRANGE(func_call)
        NODE_ID_ARRANGE(built_in_func)
        NODE_ID_ARRANGE(built_in_procedure)
        NODE_ID_ARRANGE(literal)
        NODE_ID_ARRANGE(uint_literal)
        NODE_ID_ARRANGE(string_literal)
    };

    // NodeのIDを文字列に変換
    #define NODE_ID_ARRANGE_R(str)\
    {Node::Id:: str, #str},
    static inline unordered_map<Id, string> id_to_str{
        NODE_ID_ARRANGE_R(program)
        NODE_ID_ARRANGE_R(func_decl)
        NODE_ID_ARRANGE_R(statement_list)
        NODE_ID_ARRANGE_R(statement)
        NODE_ID_ARRANGE_R(basic_statement)
        NODE_ID_ARRANGE_R(if_statement)
        NODE_ID_ARRANGE_R(while_statement)
        NODE_ID_ARRANGE_R(var_decl)
        NODE_ID_ARRANGE_R(assignment_like)
        NODE_ID_ARRANGE_R(expression)
        NODE_ID_ARRANGE_R(type_name)
        NODE_ID_ARRANGE_R(func_name)
        NODE_ID_ARRANGE_R(var_name)
        NODE_ID_ARRANGE_R(func_call)
        NODE_ID_ARRANGE_R(built_in_func)
        NODE_ID_ARRANGE_R(built_in_procedure)
        NODE_ID_ARRANGE_R(literal)
        NODE_ID_ARRANGE_R(uint_literal)
        NODE_ID_ARRANGE_R(string_literal)
    };
};

