#pragma once

#include <string>
#include <ostream>
#include <unordered_map>
#include <regex>

#include "Type.h"

using namespace std;

class Token{
    public:
    enum class Id{
        error,

        svoid,
        sint,
        sstring,
        sbool,
        strue,
        sfalse,
        sif,
        selse,
        swhile,
        sbreak,
        scontinue,
        sreturn,
        
        sdot,
        scomma,
        scolon,
        ssemicolon,
        
        slparen,
        srparen,
        slbrace,
        srbrace,

        sassign,
        splus,
        sminus,
        sstar,
        sslash,
        spercent,
        spluseq,
        sminuseq,
        sstareq,
        sslasheq,
        spercenteq,
        samp,
        spipe,
        shat,
        stilde,
        sexc,
        seq,
        sne,
        slt,
        sgt,
        sle,
        sge,

        sident,
        suint_literal,
        sstring_literal,
        schar_literal,
    };

    Id id = Id::error;
    string name;
    string str; // tokenが表す文字列
    int line = -1, count = -1;

    Token();
    Token(const string&);

    friend ostream& operator<<(ostream& os, const Token& token){
        os
        << "{str: '" << token.str << "'"
        << ", id: " << static_cast<int>(token.id)
        << ", name: '" << token.name << "'"
        << ", line: " << token.line
        << ", count: " << token.count
        << "}";
        return os;
    }

    bool is_type_name() const;
    bool is_literal() const;
    bool is_additive_operator() const;
    bool is_multiplicative_operator() const;
    bool is_relational_operator() const;
    bool is_assignment_like() const;
    bool is_control() const;
    bool is_loop_control() const;
    bool is_built_in_func() const;
    bool is_built_in_procedure() const;

    Program::Type to_program_type() const;

    private:
    static bool is_string_literal(string);
    static const regex identifier_re;
    static const regex uint_literal_re;
    static const regex char_literal_re;
    static unordered_map<string, Id> token_str_to_id;
};