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
        else if(i == 2){
            tree_filename = argv[2];
        }
    }
}