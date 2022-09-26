#include "Token.h"
#include "Node.h"

#include <string>

using namespace std;

Node::Node(const string& name){
    this->name = name;
    this->id = str_to_id[name];
}
Node::Node(const string& name, const Token& token){
    this->name = name;
    this->token = token;
    this->id = str_to_id[name];
}
void Node::add_leaf(const Node& leaf){
    leaves.push_back(leaf);
}
Token Node::get_first_token() const{
    Node node = *this;
    while(node.size() > 0){
        node = node[0];
    }
    return node.token;
}
string Node::to_string(const int indent) const{
    string ret;
    if(indent != 0){
        ret += "\n";
    }
    for(int i = 0; i < indent; i++) ret += ' ';
    ret += name + " -> ";
    if(leaves.size() > 0){
        bool first = true;
        for(Node leaf : leaves){
            if(!first) ret += " ";
            first = false;
            ret += leaf.name;
        }
    }
    else{
        ret += (token.id != Token::Id::error ? "'" + token.str + "'" : "NULL");
    }
    for(Node leaf : leaves){
        ret += leaf.to_string(indent + 2);
    }
    return ret;
}
int Node::size() const{
    return leaves.size();
}
Node& Node::operator[](const int n){
    return leaves[n];
}

vector<Node>::const_iterator Node::begin() const noexcept{
    return leaves.begin();
}
vector<Node>::const_iterator Node::end() const noexcept{
    return leaves.end();
}