#include "Interpreter.h"

using namespace std;

class LoopControl{
    public:
    Token token;
    LoopControl(const Token& token){
        this->token = token;
    }
};

class Return{
    public:
    Token token;
    Return(const Token& token){
        this->token = token;
    }
};

void Interpreter::run(const SyntaxTree& tree){
    Node program_node = tree.root;
    try{
        program(program_node);
    }
    catch(const LoopControl& loop_control){
        throw SyntaxError(
            loop_control.token.str + " outside loop",
            loop_control.token
        );
    }
    catch(const Return& ret){
        throw SyntaxError(
            ret.token.str + " outside function",
            ret.token
        );
    }
}
void Interpreter::program(Node& self){
    for(Node statement_node : self){
        if(statement_node.id == Node::Id::statement){
            statement(statement_node);
        }
        else{
            func_decl(statement_node);
        }
    }
}
void Interpreter::func_decl(Node& self){
    Program::Type return_type = self[0].token.to_program_type();
    string func_name = self[1].token.str;
    Program::Function function = Program::Function(
        return_type,
        func_name,
        self[2]
    );
    func_table[func_name] = function;
}
void Interpreter::statement_list(Node& self){
    for(Node statement_node : self){
        statement(statement_node);
    }
}
void Interpreter::statement(Node& self){
    if(self[0].id == Node::Id::basic_statement){
        basic_statement(self[0]);
    }
    else if(self[0].id == Node::Id::if_statement){
        if_statement(self[0]);
    }
    else if(self[0].id == Node::Id::while_statement){
        while_statement(self[0]);
    }
}
void Interpreter::basic_statement(Node& self){
    if(self.size() == 0){
        // ループ制御
        if(self.token.is_loop_control()){
            throw LoopControl(self.token);
        }
    }

    if(self[0].id == Node::Id::var_decl){
        var_decl(self[0]);
    }
    else if(self[0].id == Node::Id::assignment_like){
        assignment_like(self[0]);
    }
    else if(self[0].id == Node::Id::expression){
        expression(self[0]);
    }
    else if(self[0].id == Node::Id::built_in_procedure){
        built_in_procedure(self[0]);
    }
    else if(self[0].id == Node::Id::return_statement){
        return_statement(self[0]);
    }
    else{
        fatal_error("in basic_statement");
    }
}
void Interpreter::if_statement(Node& self){
    expression(self[0]);
    
    Program::Object condition_object = stack.top();
    stack.pop();

    // 条件式がboolではない
    if(condition_object.type != Program::Type::sbool){
        throw_type_condition_not_bool_error(self[0].get_first_token(), condition_object.type);
    }

    bool condition = any_cast<bool>(condition_object.value);

    if(condition){
        // if文の内部を実行
        if(self[1].id == Node::Id::statement_list){
            statement_list(self[1]);
        }
        else{
            statement(self[1]);
        }
    }
    else{
        // else節がある場合
        if(self.size() == 3){
            // else節の内部を実行
            if(self[2].id == Node::Id::statement_list){
                statement_list(self[2]);
            }
            else{
                statement(self[2]);
            }
        }
    }
}
void Interpreter::while_statement(Node& self){
    while(true){
        expression(self[0]);
        
        Program::Object condition_object = stack.top();
        stack.pop();

        // 条件式がboolではない
        if(condition_object.type != Program::Type::sbool){
            throw_type_condition_not_bool_error(self[0].get_first_token(), condition_object.type);
        }

        bool condition = any_cast<bool>(condition_object.value);
        if(!condition) return;

        // 内部を実行
        symbol_table.in_scope();
        try{
            if(self[1].id == Node::Id::statement_list){
                statement_list(self[1]);
            }
            else{
                statement(self[1]);
            }
        }
        catch(const LoopControl& loop_control){
            Token::Id loop_control_id = loop_control.token.id;
            if(loop_control_id == Token::Id::sbreak){
                symbol_table.out_scope();
                break;
            }
        }
        symbol_table.out_scope();
    }
}
void Interpreter::var_decl(Node& self){
    string type_name = self[0].token.str;
    int i = 1;
    while(i < self.size()){
        string var_name = self[i].token.str;
        Program::Object var(type_name);
        
        // 初期値がある場合
        if(i + 1 < self.size() && self[i + 1].id == Node::Id::expression){
            i++;
            expression(self[i]);
            Program::Object init_value_object = stack.top();
            stack.pop();

            // 型が異なる場合はエラー
            if(init_value_object.type != var.type){
                throw_type_assignment_error(self[i].get_first_token(), init_value_object.type, var.type);
            }

            var = init_value_object;
        }

        symbol_table.set_new_var(var_name, var);

        i++;
    }
}
void Interpreter::assignment_like(Node& self){
    string var_name = self[0].token.str;
    expression(self[1]);
    Program::Object lhs = symbol_table.get_var(var_name);
    Program::Object rhs = stack.top();
    Program::Type original_type = lhs.type;
    stack.pop();

    Token::Id op = self.token.id;

    if(op == Token::Id::sassign){
        lhs = rhs;
    }
    else if(op == Token::Id::spluseq){
        lhs += rhs;
    }
    else if(op == Token::Id::sminuseq){
        lhs -= rhs;
    }
    else if(op == Token::Id::sstareq){
        lhs *= rhs;
    }
    else if(op == Token::Id::sslasheq){
        lhs /= rhs;
    }
    else if(op == Token::Id::spercenteq){
        lhs %= rhs;
    }

    // 型が異なる場合はエラー
    if(lhs.type != original_type){
        throw_type_assignment_error(self[1].get_first_token(), lhs.type, original_type);
    }

    symbol_table.set_var(var_name, lhs);
}
void Interpreter::expression(Node& self){
    simple_expression(self[0]);
    for(int i = 1; i < self.size(); i += 2){
        simple_expression(self[i + 1]);
    }
    for(int i = 1; i < self.size(); i += 2){
        relational_operator(self[i]);
    }
}
void Interpreter::simple_expression(Node& self){
    term(self[0]);
    for(int i = 1; i < self.size(); i += 2){
        term(self[i + 1]);
    }
    for(int i = 1; i < self.size(); i += 2){
        additive_operator(self[i]);
    }
}
void Interpreter::term(Node& self){
    factor(self[0]);
    for(int i = 1; i < self.size(); i += 2){
        factor(self[i + 1]);
    }
    for(int i = 1; i < self.size(); i += 2){
        multiplicative_operator(self[i]);
    }
}
void Interpreter::factor(Node& self){
    if(self[0].id == Node::Id::literal){
        literal(self[0]);
    }
    else if(self[0].id == Node::Id::var_name){
        string var_name = self[0].token.str; 

        // 未定義変数
        if(!symbol_table.has_var(var_name)){
            throw_name_error(self[0].token, var_name);
        }

        stack.push(symbol_table.get_var(var_name));
    }
    else if(self[0].id == Node::Id::built_in_func){
        built_in_func(self[0]);
    }
    else if(self[0].id == Node::Id::func_call){
        func_call(self[0]);
    }
    else{
        fatal_error("in factor self[0].id = " + Node::id_to_str[self[0].id]);
    }
}
void Interpreter::additive_operator(Node& self){
    Program::Object operand2 = stack.top();
    stack.pop();
    Program::Object operand1 = stack.top();
    stack.pop();

    try{
        Program::Object result;
        if(self.token.id == Token::Id::splus){
            result = operand1 + operand2;
        }
        else if(self.token.id == Token::Id::sminus){
            result = operand1 - operand2;
        }
        stack.push(result);
    }
    catch(const TypeError&){
        throw_type_unsupported_error(self.token, operand1.type, operand2.type);
    }
}
void Interpreter::multiplicative_operator(Node& self){
    Program::Object operand2 = stack.top();
    stack.pop();
    Program::Object operand1 = stack.top();
    stack.pop();
    
    try{
        Program::Object result;
        if(self.token.id == Token::Id::sstar){
            result = operand1 * operand2;
        }
        else if(self.token.id == Token::Id::sslash){
            result = operand1 / operand2;
        }
        else if(self.token.id == Token::Id::spercent){
            result = operand1 % operand2;
        }
        stack.push(result);
    }
    catch(const TypeError&){
        throw_type_unsupported_error(self.token, operand1.type, operand2.type);
    }
    catch(const ZeroDivisionError&){
        throw ZeroDivisionError("Division by zero", self.token);
    }
}
void Interpreter::relational_operator(Node& self){
    Program::Object operand2 = stack.top();
    stack.pop();
    Program::Object operand1 = stack.top();
    stack.pop();

    bool result;
    try{
        if(self.token.id == Token::Id::seq){
            result = operand1 == operand2;
        } 
        else if(self.token.id == Token::Id::sne){
            result = operand1 != operand2;
        }
        else if(self.token.id == Token::Id::slt){
            result = operand1 < operand2;
        }
        else if(self.token.id == Token::Id::sgt){
            result = operand1 > operand2;
        }
        else if(self.token.id == Token::Id::sle){
            result = operand1 <= operand2;
        }
        else if(self.token.id == Token::Id::sge){
            result = operand1 >= operand2;
        }
    }
    catch(const TypeError&){
        throw_type_unsupported_error(self.token, operand1.type, operand2.type);
    }
    
    Program::Object result_object = Program::Object(Program::Type::sbool);
    result_object.value = result;
    stack.push(result_object);
}
void Interpreter::func_call(Node& self){
    string func_name = self[0].token.str;

    // 未定義関数の呼び出し
    if(func_table.find(func_name) == func_table.end()){
        throw_name_error(self[0].token, func_name);
    }

    // 関数呼び出し
    symbol_table.in_func();
    try{
        statement_list(func_table[func_name].content);
    }
    catch(const Return& ret){
        //
    }
    symbol_table.out_func();
}
void Interpreter::return_statement(Node& self){
    throw Return(self.token);
}
void Interpreter::built_in_func(Node& self){
    string func_name = self.token.str;
    if(func_name == "input_int"){
        Program::Object object = Program::Object(Program::Type::sint);
        std_in >> object;
        stack.push(object);
    }
}
void Interpreter::built_in_procedure(Node& self){
    vector<Program::Object> objects(self.size());
    for(int i = 0; i < self.size(); i++){
        expression(self[i]);
        objects[i] = stack.top();
        stack.pop();
    }

    string func_name = self.token.str;

    if(func_name == "print"){
        for(const Program::Object& object : objects){
            std_out << object;
        }
    }
    else if(func_name == "println"){
        for(const Program::Object& object : objects){
            std_out << object;
        }
        std_out << endl;
    }    
}
void Interpreter::literal(Node& self){
    // trueかfalse
    if(
        self.token.id == Token::Id::strue || 
        self.token.id == Token::Id::sfalse
        ){
        Program::Object object(Program::Type::sbool, self.token.str);
        stack.push(object);
    }
    else if(self[0].id == Node::Id::uint_literal){
        uint_literal(self[0]);
    }
    else if(self[0].id == Node::Id::string_literal){
        string_literal(self[0]);
    }
    else{
        fatal_error("in literal");
    }
}
void Interpreter::uint_literal(Node& self){
    Program::Object object(Program::Type::sint, self.token.str);
    stack.push(object);
}
void Interpreter::string_literal(Node& self){
    Program::Object object(Program::Type::sstring, self.token.str);
    stack.push(object);
}
void Interpreter::fatal_error(const string& msg = "") const{
    std_err << "Fatal Error: " << msg << endl;
}
void Interpreter::throw_name_error(const Token& token, const string& name){
    throw NameError(
        "'" + name + "' is not declared in this scope",
        token
    );
}
void Interpreter::throw_type_unsupported_error(const Token& token, const Program::Type& type1, const Program::Type& type2){
    throw TypeError(
        "Unsupported operand type(s) for " + token.str + ": '" + Program::type_to_str[type1] + "' and '" + Program::type_to_str[type2] + "'", 
        token
    );
}
void Interpreter::throw_type_assignment_error(const Token& token, const Program::Type& src_type, const Program::Type& dst_type){
    throw TypeError(
        "Cannot assign '" + Program::type_to_str[src_type] + "' to '" + Program::type_to_str[dst_type] + "'", 
        token
    );
}
void Interpreter::throw_type_condition_not_bool_error(const Token& token, const Program::Type& type){
    throw TypeError(
        "Conditional expressions can be only bool, not " + Program::type_to_str[type],
        token
    );
}