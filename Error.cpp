#include "Error.h"

Error::Error(const string& msg, const int line = -1, const int count = -1){
    this->msg = msg;
    this->line = line;
    this->count = count;
    set_name("Error");
}
void Error::set_name(const string &name){
    what_str = "";
    if(line != -1 && count != -1){
        what_str += std::to_string(line) + ":" + std::to_string(count) + ": ";
    }
    what_str += name + ": " + msg;
}
const char* Error::what() const throw(){
    return what_str.c_str();
}

#define DEFINE_ERROR_CLASS_CONSTRUCTER(name) \
name::name(const string& msg, const int line, const int count) : Error(msg, line, count){ \
    set_name(#name); \
}

DEFINE_ERROR_CLASS_CONSTRUCTER(SyntaxError)
DEFINE_ERROR_CLASS_CONSTRUCTER(FileNotFoundError)
DEFINE_ERROR_CLASS_CONSTRUCTER(TypeError)
DEFINE_ERROR_CLASS_CONSTRUCTER(NameError)
DEFINE_ERROR_CLASS_CONSTRUCTER(ZeroDivisionError)