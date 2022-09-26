#pragma once

#include <unordered_map>
#include <string>

using namespace std;

namespace Program{
    enum class Type{
        svoid,
        sint,
        sstring,
        sbool,
    };

    // 文字列を型に変換
    #define PROGRAM_TYPE_ARRANGE(type_str)\
    {#type_str, Type::s ## type_str},
    static inline unordered_map<string, Type> str_to_type{
        PROGRAM_TYPE_ARRANGE(void)
        PROGRAM_TYPE_ARRANGE(int)
        PROGRAM_TYPE_ARRANGE(string)
        PROGRAM_TYPE_ARRANGE(bool)
    };

    // 型を文字列に変換
    #define PROGRAM_TYPE_ARRANGE_R(type_str)\
    {Type::s ## type_str, #type_str},
    static inline unordered_map<Type, string> type_to_str{
        PROGRAM_TYPE_ARRANGE_R(void)
        PROGRAM_TYPE_ARRANGE_R(int)
        PROGRAM_TYPE_ARRANGE_R(string)
        PROGRAM_TYPE_ARRANGE_R(bool)
    };
}