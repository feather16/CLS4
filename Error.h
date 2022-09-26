#pragma once

#include <string>

#include "Token.h"

using namespace std;

class Error: public exception{
    public:
    string msg, what_str;
    int line, count;
    Error(){}
    Error(const string&, const int, const int);
    Error(const string& msg) :
        Error(msg, -1, -1){}
    Error(const string& msg, const Token& token) :
        Error(msg, token.line, token.count){}
    void set_name(const string &);
    const char* what() const throw() override;
};

#define DEFINE_ERROR_CLASS(name) \
class name : public Error{ \
    public: \
    name() : Error(){} \
    name(const string&, const int, const int); \
    name(const string& msg) : \
        name(msg, -1, -1){} \
    name(const string& msg, const Token& token) : \
        name(msg, token.line, token.count){} \
};

DEFINE_ERROR_CLASS(SyntaxError)
DEFINE_ERROR_CLASS(FileNotFoundError)
DEFINE_ERROR_CLASS(TypeError)
DEFINE_ERROR_CLASS(NameError)
DEFINE_ERROR_CLASS(ZeroDivisionError)