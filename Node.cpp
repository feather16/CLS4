#include "Token.h"
#include "Node.h"

#include <string>

using namespace std;

#define STR_ID_ARRANGE(str)\
{#str, Node::Id::str},
const vector<pair<string, Node::Id>> STR_AND_ID{
    STR_ID_ARRANGE(program)
    STR_ID_ARRANGE(func_decl)
    STR_ID_ARRANGE(statement_list)
    STR_ID_ARRANGE(statement)
    STR_ID_ARRANGE(basic_statement)
    STR_ID_ARRANGE(if_statement)
    STR_ID_ARRANGE(while_statement)
    STR_ID_ARRANGE(var_decl)
    STR_ID_ARRANGE(assignment_like)
    STR_ID_ARRANGE(expression)
    STR_ID_ARRANGE(type_name)
    STR_ID_ARRANGE(func_name)
    STR_ID_ARRANGE(var_name)
    STR_ID_ARRANGE(func_call)
    STR_ID_ARRANGE(return_statement)
    STR_ID_ARRANGE(built_in_func)
    STR_ID_ARRANGE(built_in_procedure)
    STR_ID_ARRANGE(literal)
    STR_ID_ARRANGE(uint_literal)
    STR_ID_ARRANGE(string_literal)
};

// 文字列をNodeのIDに変換
unordered_map<string, Node::Id> get_str_to_id(){
    unordered_map<string, Node::Id> str_to_id;
    for(const pair<string, Node::Id>& p : STR_AND_ID){
        str_to_id[get<0>(p)] = get<1>(p);
    }
    return str_to_id;
};
unordered_map<string, Node::Id> Node::str_to_id = get_str_to_id();

// NodeのIDを文字列に変換
unordered_map<Node::Id, string> get_id_to_str(){
    unordered_map<Node::Id, string> id_to_str;
    for(const pair<string, Node::Id>& p : STR_AND_ID){
        id_to_str[get<1>(p)] = get<0>(p);
    }
    return id_to_str;
};
unordered_map<Node::Id, string> Node::id_to_str = get_id_to_str();

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