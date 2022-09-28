#include "Token.h"

unordered_map<string, Token::Id> Token::token_str_to_id{
    {"void", Token::Id::svoid},
    {"int", Token::Id::sint},
    {"string", Token::Id::sstring},
    {"bool", Token::Id::sbool},
    {"true", Token::Id::strue},
    {"false", Token::Id::sfalse},
    {"if", Token::Id::sif},
    {"else", Token::Id::selse},
    {"while", Token::Id::swhile},
    {"break", Token::Id::sbreak},
    {"continue", Token::Id::scontinue},
    {"return", Token::Id::sreturn},
    {".", Token::Id::sdot},
    {",", Token::Id::scomma},
    {":", Token::Id::scolon},
    {";", Token::Id::ssemicolon},

    {"(", Token::Id::slparen},
    {")", Token::Id::srparen},
    {"{", Token::Id::slbrace},
    {"}", Token::Id::srbrace},

    {"=", Token::Id::sassign},
    {"+", Token::Id::splus},
    {"-", Token::Id::sminus},
    {"*", Token::Id::sstar},
    {"/", Token::Id::sslash},
    {"%", Token::Id::spercent},
    {"+=", Token::Id::spluseq},
    {"-=", Token::Id::sminuseq},
    {"*=", Token::Id::sstareq},
    {"/=", Token::Id::sslasheq},
    {"%=", Token::Id::spercenteq},
    {"&", Token::Id::samp},
    {"|", Token::Id::spipe},
    {"^", Token::Id::shat},
    {"~", Token::Id::stilde},
    {"!", Token::Id::sexc},
    {"==", Token::Id::seq},
    {"!=", Token::Id::sne},
    {"<", Token::Id::slt},
    {">", Token::Id::sgt},
    {"<=", Token::Id::sle},
    {">=", Token::Id::sge},
};

Token::Token(){}
Token::Token(const string& str){
    this->str = str;

    if(token_str_to_id.find(str) != token_str_to_id.end()){ // found
        id = token_str_to_id[str];
        name = str;
    }
    else if(regex_match(str, identifier_re)){
        id = Id::sident;
        name = "ident";
    }
    else if(regex_match(str, uint_literal_re)){
        id = Id::suint_literal;
        name = "uint_literal";
    }
    else if(is_string_literal(str)){
        id = Id::sstring_literal;
        name = "string_literal";
    }
    else if(regex_match(str, char_literal_re)){
        id = Id::schar_literal;
        name = "char_literal";
    }
}

bool Token::is_type_name() const{
    return id == Id::svoid || id == Id::sint || id == Id::sstring || id == Id::sbool;
}
bool Token::is_literal() const{
    return 
        id == Id::suint_literal || id == Id::sstring_literal || id == Id::schar_literal ||
        id == Id::strue || id == Id::sfalse;
}
bool Token::is_additive_operator() const{
    return id == Id::splus || id == Id::sminus;
}
bool Token::is_multiplicative_operator() const{
    return id == Id::sstar || id == Id::sslash || id == Id::spercent;
}
bool Token::is_relational_operator() const{
    return 
        id == Id::seq || id == Id::sne ||
        id == Id::slt || id == Id::sgt ||
        id == Id::sle || id == Id::sge;
}
bool Token::is_assignment_like() const{
    return 
        id == Id::sassign || id == Id::spluseq ||
        id == Id::sminuseq || id == Id::sstareq ||
        id == Id::sslasheq || id == Id::spercenteq;
}
bool Token::is_control() const{
    return id == Id::sif || id == Id::swhile;
}
bool Token::is_loop_control() const{
    return id == Id::sbreak || id == Id::scontinue;
}
bool Token::is_built_in_func() const{
    return str == "input_int";
}
bool Token::is_built_in_procedure() const{
    return str == "print" || str == "println";
}

Program::Type Token::to_program_type() const{
    if(id == Id::sint){
        return Program::Type::sint;
    }
    else if(id == Id::sstring){
        return Program::Type::sstring;
    }
    else if(id == Id::sbool){
        return Program::Type::sbool;
    }
    else{
        return Program::Type::svoid;
    }
}

bool Token::is_string_literal(string text){
    int i = 0;
    while(i < text.size()){
        // エスケープ
        if(text[i] == '\\' && i + 1 < text.size()){
            text = text.erase(i, 2);
        }
        else{
            i++;
        }
    }

    // 前後にダブルクォーテーションが無い場合
    if(text[0] != '"' || text[text.size() - 1] != '"'){
        return false;
    }

    // 前後のダブルクォーテーションを削除
    text = text.substr(1, text.size() - 2);

    // 他にダブルクォーテーションがある場合はfalse
    if(text.find('"') != string::npos){
        return false;
    }

    return true;
}

const regex Token::identifier_re(R"([a-zA-Z_]\w*)");
const regex Token::uint_literal_re(R"(\d+)");
const regex Token::char_literal_re(R"('.')");