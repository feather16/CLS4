import re
import os
from enum import Enum, auto

class Token:

    class Id(Enum):
        error = auto()

        sequal = auto()
        slparen = auto()
        srparen = auto()
        sor = auto()
        sstar = auto()
        ssemicolon = auto()

        sident = auto()
        sstring_literal = auto()
        sregex_literal = auto()

    # constants
    identifier_re = re.compile(r"[a-zA-Z_]\w*")
    string_literal_re = re.compile(r'"[^"]*"')
    regex_literal_re = re.compile(r"/*/")
    token_str_to_id: dict = {
        "=": Id.sequal,
        "(": Id.slparen,
        ")": Id.srparen,
        "|": Id.sor,
        "*": Id.sstar,
        ";": Id.ssemicolon,
    }

    id: Id = Id.error
    name: str = ""

    def __init__(self, string: str):
        self.string = string
        if string in self.token_str_to_id: # 記号
            self.id = self.token_str_to_id[string]
            self.name = string
        elif self.identifier_re.fullmatch(string): # 名前
            self.id = self.Id.sident
            self.name = "ident"
        elif self.string_literal_re.fullmatch(string): # 文字列リテラル
            self.id = self.Id.sstring_literal
            self.name = "string_literal"
        elif self.regex_literal_re.fullmatch(string): # 正規表現リテラル
            self.id = self.Id.sregex_literal
            self.name = "regex_literal"

    def __str__(self):
        return "Token " + str({"string": self.string, "id": self.id, "name": self.name})

    def __repr__(self):
        return "Token(" + repr(self.string) + ")"

def tokenize(buf: str) -> list[Token]:
    tokens = []
    while True:
        # 先頭の空白を削除
        while len(buf) > 0 and buf[0] == " ":
            buf = buf[1:]

        error = False
        for pos in range(len(buf) - 1, -1, -1): # pos = len(buf) - 1, ..., 1, 0
            token = Token(buf[: pos + 1])
            if token.id != Token.Id.error:
                buf = buf[pos + 1 :]
                tokens.append(token)
                parsed = True
                break
            if pos == 0:
                print("Can't parse '" + buf + "'")
                error = True
        if error:
            break
        if len(buf) == 0:
            break
    return tokens

current_dir = os.path.dirname(__file__)
input_file_path = f"{current_dir}/../grammer.txt"
output_file_path = f"{current_dir}/parse-output.h"
output_class_name = "SyntaxTree"
node_class_name = "Node"

# 文法ファイルを読み込む
with open(input_file_path, encoding="utf-8") as f:
    lines = [line.rstrip() for line in f.readlines() if len(line) > 0]

definition_map: dict[Token, list[Token]] = {}

for line in lines:
    tokens = tokenize(line)
    print(tokens)
    if tokens[1].id != Token.Id.sequal:
        print("Missing '=' in " + str(tokens))
    definition_map[tokens[0]] = tokens[2:]

#print(definition_map)

HEADER = '''\
#pragma once

#include "Node.h"

using namespace std;
'''

with open(output_file_path, mode="w") as f:
    f.write(HEADER)
    f.write('\n')
    f.write("class %s{\n" % output_class_name)
    f.write("\tprivate:\n")
    for key, value in definition_map.items():
        #indent: int = 1
        f.write("\tvoid %s(const %s& self){\n" % (key.string, node_class_name))
        
        for token in value:
            if token.id == Token.Id.sident and token.string != "none":
                f.write("\t\t%s(self);\n" % token.string)

        f.write("\t}\n")
    f.write("}")