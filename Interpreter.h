#pragma once

#include "SyntaxTree.h"
#include "Object.h"
#include "Function.h"
#include "Type.h"
#include "SymbolTable.h"

#include <iostream>
#include <stack>

using namespace std;

class Interpreter{
    public:
    istream& std_in = cin;
    ostream& std_out = cout;
    ostream& std_err = cerr;
    std::stack<Program::Object> stack;
    void run(const SyntaxTree&);
    private:
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
    void relational_operator(Node&);
    void func_call(Node&);
    void built_in_func(Node&);
    void built_in_procedure(Node&);
    void literal(Node&);
    void uint_literal(Node&);
    void string_literal(Node&);
    void fatal_error(const string&) const;
    void throw_name_error(const Token&, const string&);
    void throw_type_unsupported_error(const Token&, const Program::Type&, const Program::Type&);
    void throw_type_assignment_error(const Token&, const Program::Type&, const Program::Type&);
    void throw_type_condition_not_bool_error(const Token&, const Program::Type&);
    private:
    SymbolTable symbol_table;
    unordered_map<string, Program::Function> func_table;
};