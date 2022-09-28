#include <unordered_map>
#include <vector>

#include "Object.h"

using namespace std;

class SymbolTable{
    public:
    SymbolTable();
    void in_scope();
    void out_scope();
    void in_func();
    void out_func();
    bool has_var(string&) const;
    Program::Object get_var(string&) const;
    void set_new_var(string&, const Program::Object&);
    void set_var(string&, const Program::Object&);
    int get_func_depth() const;
    int get_scope_depth() const;
    private:
    int func_depth = 0;
    int scope_depth = 0;
    vector<vector<unordered_map<string, Program::Object>>> table;
};