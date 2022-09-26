#pragma once

#include "Error.h"
#include "Node.h"
#include "TokenList.h"

using namespace std;

class SyntaxTree{
    public:
    Node root;
    SyntaxTree();
    void parse(const TokenList&);
    private:
    TokenList tokens;
	void program(Node&);
    void func_decl(Node&);
    void statement_list(Node&);
    void statement(Node&);
    void basic_statement(Node&);
    void if_statement(Node&);
    void while_statement(Node&);
    void var_decl(Node&);
    void assignment_like(Node&);
    void expression(Node&);
    void simple_expression(Node&);
    void term(Node&);
    void factor(Node&);
    void additive_operator(Node&);
    void multiplicative_operator(Node&);
    void func_call(Node&);
    void return_statement(Node&);
    void built_in_func(Node&);
    void built_in_procedure(Node&);
    void relational_operator(Node&);
    void type_name(Node&);
    void func_name(Node&);
    void var_name(Node&);
    void literal(Node&);
    void uint_literal(Node&);
    void string_literal(Node&);

    void throw_syntax_error(const string&);
    friend ostream& operator<<(ostream& os, const SyntaxTree& tree){
        os << tree.root.to_string();
        return os;
    }
};