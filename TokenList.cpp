#include "TokenList.h"
#include "Error.h"

#include <fstream>
#include <iostream>

TokenList::TokenList(const string& filename){
    ifstream ifs(filename);
    if(ifs.fail()){
        throw FileNotFoundError("Can't open source file");
    }

    char c;
    string buf;
    bool line_comment_mode = false;
    int line = 1, count = 1;
    while(ifs.get(c)){
        // 改行文字があれば1行コメント終了
        if(c == '\n'){
            line_comment_mode = false;
        }

        if(line_comment_mode) goto end_of_while;

        if(c == '\n'){
            add(buf, line, count);
            buf = "";
        }
        else{
            buf += c;
        }

        // コメント
        if(buf == "//"){
            buf = "";
            line_comment_mode = true;
        }

        end_of_while:
        if(c == '\n'){
            line++;
            count = 1;
        }
        else{
            count++;
        }
    }
    add(buf, line, count);
}

Token TokenList::current() const{
    if(has_current())
        return tokens[index];
    else
        return Token();
}

Token TokenList::next(const int i) const{
    if(has_next(i))
        return tokens[index + i];
    else
        return Token();
}

bool TokenList::has_current() const{
    return 0 <= index && index < tokens.size();
}

bool TokenList::has_next(const int i) const{
    return 0 <= index + i && index + i < tokens.size();
}

void TokenList::advance(){
    if(index < tokens.size()){
        index++;
    }
}

size_t TokenList::size() const{
    return tokens.size();
}

bool TokenList::is_statement_start() const{
    Token token = current();
    return
        token.id == Token::Id::sident || 
        token.is_type_name() || 
        token.is_literal() || 
        token.is_control() || 
        token.is_loop_control() ||
        token.id == Token::Id::sreturn;
}

Token& TokenList::operator[](const int n){
    return tokens[n];
}

TokenList::iterator TokenList::begin() const noexcept{
    return tokens.begin();
}
TokenList::iterator TokenList::end() const noexcept{
    return tokens.end();
}

void TokenList::add(string buf, int line, int count){
    while(true){
        Token token = find_token(buf, line, count);
        if(token.id != Token::Id::error){
            tokens.push_back(token);
        }
        else if(buf != ""){
            // 字句解析エラー
            cout << "Tokenize Error: '" << buf << "'" << endl;
            return;
        }
        else{
            break;
        }
    }
}

Token TokenList::find_token(string& str, int line, int count) const{
    // 先頭の空白は読み飛ばす
    while(str.size() > 0 && str[0] == ' '){
        str = str.substr(1);
    }

    for(int pos = str.size() - 1; pos >= 0; pos--){
        Token token(str.substr(0, pos + 1));
        if(token.id != Token::Id::error){
            str = str.substr(pos + 1);
            token.line = line;
            token.count = count - str.size() - token.str.size();
            return token;
        }
    }
    return Token("");
}