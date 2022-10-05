#include "Argument.h"

Argument::Argument(int argc, char* argv[]){
    args = vector<string>(argv, argv + argc);
    for(int i = 0; i < argc; i++){
        if(i == 0){
            interpreter = argv[0];
        }
        else if(i == 1){
            filename = argv[1];
        }
    }
    
    if(argc < 3){
        return;
    }

    auto itr = args.begin() + 2;
    while(itr != args.end()){
        if(is_option(*itr)){
            const string option = *itr;
            ++itr;
            if(option == "--tree"){
                if(itr == args.end() || *itr == "" || is_option(*itr)){
                    throw_error(itr, "Not specified filename");
                }
                tree_filename = *itr;
            }
            else{
                throw_error(itr - 1, "Unrecognized command line option '" + option + "'");
            }
            ++itr;
        }
        else{
            throw_error(itr, "Expected option");
        }
    }
}

void Argument::throw_error(const vector<string>::iterator& itr, const string& message) const{
    throw ArgumentError(args, itr - args.begin(), message);
}

bool Argument::is_option(const string& arg){
    return arg.size() > 0 && arg[0] == '-';
}

ArgumentError::ArgumentError(const vector<string>& args, const int index, const string& message){
    what_str = "";
    vector<string> args_tmp = args;
    int error_point = -1;
    args_tmp.push_back("");

    // 1行目：コマンドライン引数
    for(int i = 0; i < args_tmp.size(); i++){
        if(i != 0){
            what_str += ' ';
        }
        if(i == index){
            error_point = what_str.size();
        }
        what_str += args_tmp[i];
    }

    // 2行目：エラー箇所
    what_str += '\n';
    for(int i = 0; i < error_point; i++){
        what_str += ' ';
    }
    what_str += '^';

    // 3行目：エラー内容
    what_str += '\n';
    what_str += "ArgumentError: " + message;
}

const char* ArgumentError::what() const throw(){
    return what_str.c_str();
}