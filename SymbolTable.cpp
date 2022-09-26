#include "SymbolTable.h"

SymbolTable::SymbolTable(){
    table.push_back({{}});
}
void SymbolTable::in_scope(){
    table[func_depth].push_back({});
    scope_depth++;
}
void SymbolTable::out_scope(){
    table[func_depth].erase(table[func_depth].begin() + scope_depth);
    scope_depth--;
}
void SymbolTable::in_func(){
    table.push_back({{}});
    func_depth++;
}
void SymbolTable::out_func(){
    table.erase(table.begin() + func_depth);
    func_depth--;
}

bool SymbolTable::has_var(string& var_name) const{
    for(int i = scope_depth; i >= 0; i--){
        if(table[func_depth][i].find(var_name) != table[func_depth][i].end()){
            return true;
        }
    }
    if(func_depth > 0){
        return table[0][0].find(var_name) != table[0][0].end();
    }
    return false;
}
Program::Object SymbolTable::get_var(string& var_name){
    for(int i = scope_depth; i >= 0; i--){
        if(table[func_depth][i].find(var_name) != table[func_depth][i].end()){
            return table[func_depth][i][var_name];
        }
    }
    if(func_depth > 0){
        // グローバル変数
        return table[0][0][var_name];
    }
    return Program::Object();
}
void SymbolTable::set_new_var(string& var_name, const Program::Object& value){
    table[func_depth][scope_depth][var_name] = value;
}
void SymbolTable::set_var(string& var_name, const Program::Object& value){
    for(int i = scope_depth; i >= 0; i--){
        if(table[func_depth][i].find(var_name) != table[func_depth][i].end()){
            table[func_depth][i][var_name] = value;
            return;
        }
    }
    if(func_depth > 0){
        // グローバル変数
        table[0][0][var_name] = value;
    }
}

int SymbolTable::get_func_depth() const{
    return func_depth;
}
int SymbolTable::get_scope_depth() const{
    return scope_depth;
}