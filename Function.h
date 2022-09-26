#pragma once

#include <string>
#include "Node.h"
#include "Type.h"

using namespace std;

namespace Program{
    class Function{
        public:
        Type return_type;
        string name;
        Node content;
        Function(){
            this->return_type = Type::svoid;
            this->name = "";
            this->content = Node();
        }
        Function(const Type& return_type, const string& name, const Node& content){
            this->return_type = return_type;
            this->name = name;
            this->content = content;
        }
    };
}