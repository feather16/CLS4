#pragma once

#include "Token.h"

using namespace std;

class TokenList{
    public:
    vector<Token> tokens;
    TokenList(){}
    TokenList(const string&);

    Token current() const;
    Token next(const int i = 1) const;
    bool has_current() const;
    bool has_next(const int i = 1) const;
    void advance();
    size_t size() const;

    bool is_statement_start() const;

    friend ostream& operator<<(ostream& os, const TokenList& token_list){
        os << "[";
        for(const Token& token : token_list){
            os << endl << "  " << token;
        }
        os << endl << "]";
        return os;
    }

    Token& operator[](const int);

    using iterator = vector<Token>::const_iterator;
    iterator begin() const noexcept;
    iterator end() const noexcept;
    private:

    void add(string, int, int);

    Token find_token(string&, int, int) const;

    int index = 0;
};