#include "SyntaxTree.h"

#include <iostream>

using namespace std;

SyntaxTree::SyntaxTree(){
    root = Node("program");
}
void SyntaxTree::parse(const TokenList& token_list){
    this->tokens = token_list;
    program(root);
}
void SyntaxTree::program(Node& self){
    Node leaf; 
    while(tokens.has_current()){
        Token token = tokens.current();

        // 関数定義
        if(
            token.is_type_name() &&
            tokens.next().id == Token::Id::sident &&
            tokens.next(2).id == Token::Id::slparen
        ){
            leaf = Node("func_decl");
            func_decl(leaf);
            self.add_leaf(leaf);
        }

        // 文
        else if(tokens.is_statement_start()){
            leaf = Node("statement");
            statement(leaf);
            self.add_leaf(leaf);
        }

        else{
            break;
        }
    }
}
void SyntaxTree::func_decl(Node& self){
    Node leaf;

    leaf = Node("type_name");
    type_name(leaf);
    self.add_leaf(leaf);

    leaf = Node("func_name");
    func_name(leaf);
    self.add_leaf(leaf);

    if(tokens.current().id != Token::Id::slparen){
        throw_syntax_error("Expected (");
    }
    tokens.advance();

    if(tokens.current().id != Token::Id::srparen){
        throw_syntax_error("Expected )");
    }
    tokens.advance();

    if(tokens.current().id != Token::Id::slbrace){
        throw_syntax_error("Expected {");
    }
    tokens.advance();

    leaf = Node("statement_list");
    statement_list(leaf);
    self.add_leaf(leaf);

    if(tokens.current().id != Token::Id::srbrace){
        throw_syntax_error("Expected }");
    }
    tokens.advance();
}
void SyntaxTree::statement_list(Node& self){
    Node leaf; 
    while(tokens.has_current()){
        Token token = tokens.current();
        if(tokens.is_statement_start()){
            leaf = Node("statement");
            statement(leaf);
            self.add_leaf(leaf);
        }
        else{
            break;
        }
    }
}
void SyntaxTree::statement(Node& self){
    Node leaf; 

    // if文
    if(tokens.current().id == Token::Id::sif){
        leaf = Node("if_statement");
        if_statement(leaf);
        self.add_leaf(leaf);
    }
    // while文
    else if(tokens.current().id == Token::Id::swhile){
        leaf = Node("while_statement");
        while_statement(leaf);
        self.add_leaf(leaf);
    }
    // 基本文
    else{
        leaf = Node("basic_statement");
        basic_statement(leaf);
        self.add_leaf(leaf);
    }
}
void SyntaxTree::basic_statement(Node& self){
    Node leaf; 
    
    // 変数宣言文
    if(tokens.current().is_type_name()){ 
        leaf = Node("var_decl");
        var_decl(leaf);
        self.add_leaf(leaf);
    }
    // 代入文
    else if(tokens.next().is_assignment_like()){ 
        leaf = Node("assignment_like");
        assignment_like(leaf);
        self.add_leaf(leaf);
    }
    // 組み込みプロシージャ
    else if(tokens.current().is_built_in_procedure()){
        leaf = Node("built_in_procedure");
        built_in_procedure(leaf);
        self.add_leaf(leaf);
    }
    // ループ制御
    else if(tokens.current().is_loop_control()){
        self.token = tokens.current();
        tokens.advance();
    }
    // return
    else if(tokens.current().id == Token::Id::sreturn){
        leaf = Node("return_statement");
        return_statement(leaf);
        self.add_leaf(leaf);
    }
    // 式
    else{
        leaf = Node("expression");
        expression(leaf);
        self.add_leaf(leaf);
    }

    if(tokens.current().id != Token::Id::ssemicolon){
        throw_syntax_error("Expected ';'");
    }
    tokens.advance();
}
void SyntaxTree::if_statement(Node& self){
    Node leaf;
    tokens.advance();
    if(tokens.current().id != Token::Id::slparen){
        throw_syntax_error("Expected (");
    }
    tokens.advance();
    leaf = Node("expression");
    expression(leaf);
    self.add_leaf(leaf);
    if(tokens.current().id != Token::Id::srparen){
        throw_syntax_error("Expected )");
    }
    tokens.advance();

    // {}がある場合
    if(tokens.current().id == Token::Id::slbrace){
        tokens.advance();
        leaf = Node("statement_list");
        statement_list(leaf);
        self.add_leaf(leaf);
        if(tokens.current().id != Token::Id::srbrace){
            throw_syntax_error("Expected }");
        }
        tokens.advance();
    }

    // {}がない場合
    else{
        leaf = Node("statement");
        statement(leaf);
        self.add_leaf(leaf);
    }

    // else節がある場合
    if(tokens.current().id == Token::Id::selse){
        tokens.advance();

        // {}がある場合
        if(tokens.current().id == Token::Id::slbrace){
            tokens.advance();
            leaf = Node("statement_list");
            statement_list(leaf);
            self.add_leaf(leaf);
            if(tokens.current().id != Token::Id::srbrace){
                throw_syntax_error("Expected }");
            }
            tokens.advance();
        }

        // {}がない場合
        else{
            leaf = Node("statement");
            statement(leaf);
            self.add_leaf(leaf);
        }
    }
}
void SyntaxTree::while_statement(Node& self){
    Node leaf;
    tokens.advance();
    if(tokens.current().id != Token::Id::slparen){
        throw_syntax_error("Expected (");
    }
    tokens.advance();
    leaf = Node("expression");
    expression(leaf);
    self.add_leaf(leaf);
    if(tokens.current().id != Token::Id::srparen){
        throw_syntax_error("Expected )");
    }
    tokens.advance();

    // {}がある場合
    if(tokens.current().id == Token::Id::slbrace){
        tokens.advance();
        leaf = Node("statement_list");
        statement_list(leaf);
        self.add_leaf(leaf);
        if(tokens.current().id != Token::Id::srbrace){
            throw_syntax_error("Expected }");
        }
        tokens.advance();
    }

    // {}がない場合
    else{
        leaf = Node("statement");
        statement(leaf);
        self.add_leaf(leaf);
    }
}
void SyntaxTree::var_decl(Node& self){
    Node leaf; 
    leaf = Node("type_name");
    type_name(leaf);
    self.add_leaf(leaf);
    while(true){
        leaf = Node("var_name");
        var_name(leaf);
        self.add_leaf(leaf);
        if(tokens.current().id == Token::Id::sassign){
            tokens.advance();
            leaf = Node("expression");
            expression(leaf);
            self.add_leaf(leaf);
        }

        // ,があれば
        if(tokens.current().id == Token::Id::scomma){
            tokens.advance();
        }
        else{
            break;
        }
    }
}
void SyntaxTree::assignment_like(Node& self){
    Node leaf; 
    leaf = Node("var_name");
    var_name(leaf);
    self.add_leaf(leaf);
    if(!tokens.current().is_assignment_like()){
        throw_syntax_error("Expected assignment");
    }
    self.token = tokens.current();
    tokens.advance();
    leaf = Node("expression");
    expression(leaf);
    self.add_leaf(leaf);
}
void SyntaxTree::expression(Node& self){
    Node leaf;
    leaf = Node("simple_expression");
    simple_expression(leaf);
    self.add_leaf(leaf);
    while(tokens.current().is_relational_operator()){
        leaf = Node("relational_operator");
        relational_operator(leaf);
        self.add_leaf(leaf);
        leaf = Node("simple_expression");
        simple_expression(leaf);
        self.add_leaf(leaf);
    }
}
void SyntaxTree::simple_expression(Node& self){
    Node leaf;
    leaf = Node("term");
    term(leaf);
    self.add_leaf(leaf);
    while(tokens.current().is_additive_operator()){
        leaf = Node("additive_operator");
        additive_operator(leaf);
        self.add_leaf(leaf);
        leaf = Node("term");
        term(leaf);
        self.add_leaf(leaf);
    }
}
void SyntaxTree::term(Node& self){
    Node leaf;
    leaf = Node("factor");
    factor(leaf);
    self.add_leaf(leaf);
    while(tokens.current().is_multiplicative_operator()){
        leaf = Node("multiplicative_operator");
        multiplicative_operator(leaf);
        self.add_leaf(leaf);
        leaf = Node("factor");
        factor(leaf);
        self.add_leaf(leaf);
    }
}
void SyntaxTree::factor(Node& self){
    Node leaf;
    // 組み込み関数
    if(tokens.current().is_built_in_func()){
        leaf = Node("built_in_func");
        built_in_func(leaf);
        self.add_leaf(leaf);
    }
    // 関数呼び出し
    else if(
        tokens.current().id == Token::Id::sident &&
        tokens.next().id == Token::Id::slparen
        ){
        leaf = Node("func_call");
        func_call(leaf);
        self.add_leaf(leaf);
    }
    // 変数名
    else if(tokens.current().id == Token::Id::sident){ 
        leaf = Node("var_name");
        var_name(leaf);
        self.add_leaf(leaf);
    }
    // リテラル
    else{
        leaf = Node("literal");
        literal(leaf);
        self.add_leaf(leaf);
    }
}
void SyntaxTree::additive_operator(Node& self){
    if(!tokens.current().is_additive_operator()){
        throw_syntax_error("Expected additive operator");
    }
    self.token = tokens.current();
    tokens.advance();
}
void SyntaxTree::multiplicative_operator(Node& self){
    if(!tokens.current().is_multiplicative_operator()){
        throw_syntax_error("Expected multiplicative operator");
    }
    self.token = tokens.current();
    tokens.advance();
}    
void SyntaxTree::relational_operator(Node& self){
    if(!tokens.current().is_relational_operator()){
        throw_syntax_error("Expected relational operator");
    }
    self.token = tokens.current();
    tokens.advance();
}    
void SyntaxTree::func_call(Node& self){
    Node leaf;
    leaf = Node("func_name");
    func_name(leaf);
    self.add_leaf(leaf);
    if(tokens.current().id != Token::Id::slparen){
        throw_syntax_error("Expected '('");
    }
    tokens.advance();
    if(tokens.current().id != Token::Id::srparen){
        throw_syntax_error("Expected ')'");
    }
    tokens.advance();
}
void SyntaxTree::return_statement(Node& self){
    if(tokens.current().id != Token::Id::sreturn){
        throw_syntax_error("Expected 'return'");
    }
    self.token = tokens.current();
    tokens.advance();
}
void SyntaxTree::built_in_func(Node& self){
    Node leaf;
    self.token = tokens.current();
    if(tokens.current().str == "input_int"){
        tokens.advance();
        if(tokens.current().id != Token::Id::slparen){
            throw_syntax_error("Expected '('");
        }
        tokens.advance();
        if(tokens.current().id != Token::Id::srparen){
            throw_syntax_error("Expected ')'");
        }
        tokens.advance();
    }
    else{
        throw_syntax_error("Expected built-in function");
    }
}
void SyntaxTree::built_in_procedure(Node& self){
    Node leaf;
    self.token = tokens.current();
    if(tokens.current().str == "print" || tokens.current().str == "println"){
        tokens.advance();
        if(tokens.current().id != Token::Id::slparen){
            throw_syntax_error("Expected '('");
        }
        tokens.advance();

        if(tokens.current().id != Token::Id::srparen){
            while(true){
                leaf = Node("expression");
                expression(leaf);
                self.add_leaf(leaf);

                // , があれば続ける
                if(tokens.current().id == Token::Id::scomma){
                    tokens.advance();
                    continue;
                }

                // ) で終了
                if(tokens.current().id == Token::Id::srparen){
                    break;
                }
                else{
                    throw_syntax_error("Expected ')' or ','");
                }
            }
        }

        tokens.advance();
    }
    else{
        throw_syntax_error("Expected built-in procedure");
    }
}
void SyntaxTree::type_name(Node& self){
    if(!tokens.current().is_type_name()){
        throw_syntax_error("Expected type name");
    }
    self.token = tokens.current();
    tokens.advance();
}
void SyntaxTree::func_name(Node& self){
    if(tokens.current().id != Token::Id::sident){
        throw_syntax_error("Expected identifier");
    }
    self.token = tokens.current();
    tokens.advance();
}
void SyntaxTree::var_name(Node& self){
    if(tokens.current().id != Token::Id::sident){
        throw_syntax_error("Expected identifier");
    }
    self.token = tokens.current();
    tokens.advance();
}
void SyntaxTree::literal(Node& self){
    Node leaf;
    if(tokens.current().id == Token::Id::suint_literal){
        leaf = Node("uint_literal");
        uint_literal(leaf);
        self.add_leaf(leaf);
    }
    else if(tokens.current().id == Token::Id::sstring_literal){
        leaf = Node("string_literal");
        string_literal(leaf);
        self.add_leaf(leaf);
    }
    else if(tokens.current().id == Token::Id::strue){
        self.token = tokens.current();
        tokens.advance();
    }
    else if(tokens.current().id == Token::Id::sfalse){
        self.token = tokens.current();
        tokens.advance();
    }
    else{
        throw_syntax_error("Expected literal, got '" + tokens.current().str + "'");
    }
}
void SyntaxTree::uint_literal(Node& self){
    if(tokens.current().id != Token::Id::suint_literal){
        throw_syntax_error("Expected unsigned int literal");
    }
    self.token = tokens.current();
    tokens.advance();
}
void SyntaxTree::string_literal(Node& self){
    if(tokens.current().id != Token::Id::sstring_literal){
        throw_syntax_error("Expected string literal");
    }
    self.token = tokens.current();
    tokens.advance();
}

void SyntaxTree::throw_syntax_error(const string& msg){
    Token token = tokens.current();
    throw SyntaxError(msg, token.line, token.count);
}