#include "Type.h"

#include <string>
#include <vector>
#include <utility>

using namespace std;

namespace Program{
    #define STR_TYPE_ARRANGE(type_str)\
    {#type_str, Type::s ## type_str},
    const vector<pair<string, Type>> STR_AND_TYPE{
        STR_TYPE_ARRANGE(void)
        STR_TYPE_ARRANGE(int)
        STR_TYPE_ARRANGE(string)
        STR_TYPE_ARRANGE(bool)
    };

    // 文字列を型に変換
    unordered_map<string, Type> get_str_to_type(){
        unordered_map<string, Type> str_to_type;
        for(const pair<string, Type>& p : STR_AND_TYPE){
            str_to_type[get<0>(p)] = get<1>(p);
        }
        return str_to_type;
    };
    unordered_map<string, Type> str_to_type = get_str_to_type();

    // 型を文字列に変換
    unordered_map<Type, string> get_type_to_str(){
        unordered_map<Type, string> type_to_str;
        for(const pair<string, Type>& p : STR_AND_TYPE){
            type_to_str[get<1>(p)] = get<0>(p);
        }
        return type_to_str;
    };
    unordered_map<Type, string> type_to_str = get_type_to_str();
}