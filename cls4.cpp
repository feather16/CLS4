#include "Error.h"
#include "Token.h"
#include "TokenList.h"
#include "Node.h"
#include "SyntaxTree.h"
#include "Type.h"
#include "Object.h"
#include "Interpreter.h"
#include "Argument.h"

#include <fstream>
#include <iostream>
#include <string>

#include <unistd.h>

using namespace std;

string shape_path(string path){
    for(int i = 0; i < path.size(); i++){
        if(path[i] == '\\') path[i] = '/';
    }
    return path;
}

string tolower(string text){
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c);});
    return text;
}

const string get_current_directory(){
    constexpr int BUF_SIZE = 1024;
    char buf[BUF_SIZE];
    getcwd(buf, BUF_SIZE);
    return buf;
}

string get_base_name(string path){
    path = shape_path(path);
    string current_path = shape_path(get_current_directory());
    
    // 絶対パスの場合に相対パスに変換
    if(
        path.size() > current_path.size() && 
        tolower(path.substr(0, current_path.size())) == tolower(current_path)
        ){
        path = path.substr(current_path.size());
        while(path[0] == '/'){
            path = path.substr(1);
        }
    }

    return path;
}

int main(int argc, char* argv[]){
    Argument argument(argc, argv);

    TokenList tokens;
    SyntaxTree tree;
    Interpreter interpreter;
    try{
        tokens = TokenList(argument.filename); // 字句解析
        tree.parse(tokens); // 構文解析
        if(argument.tree_filename != ""){
            ofstream tree_file(argument.tree_filename);
            tree_file << tree;
        }
        interpreter.run(tree); // 実行
    }
    catch(const Error& e){
        const string base_file_name = get_base_name(argument.filename);
        if(base_file_name.size() > 0){
            cout << base_file_name << ":";
        }
        cout << e.what() << endl;
    }
}
