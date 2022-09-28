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
    extern unordered_map<string, Type> str_to_type;

    // 型を文字列に変換
    extern unordered_map<Type, string> type_to_str;
}