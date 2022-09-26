#pragma once

#include "Node.h"

using namespace std;

class SyntaxTree{
	private:
	void program(const Node& self){
		statement(self);
	}
	void statement(const Node& self){
		basic_statement(self);
		if_statement(self);
		while_statement(self);
	}
	void basic_statement(const Node& self){
	}
	void if_statement(const Node& self){
		expression(self);
		program(self);
	}
	void while_statement(const Node& self){
		expression(self);
		program(self);
	}
	void var_decl(const Node& self){
		type_name(self);
		var_name(self);
	}
	void assignment(const Node& self){
		var_name(self);
		expression(self);
	}
	void expression(const Node& self){
		simple_expression(self);
	}
	void simple_expression(const Node& self){
		term(self);
	}
	void term(const Node& self){
		factor(self);
	}
	void factor(const Node& self){
		literal(self);
		var_name(self);
	}
	void additive_operator(const Node& self){
	}
	void multiplicative_operator(const Node& self){
	}
	void relational_operator(const Node& self){
	}
	void type_name(const Node& self){
	}
	void var_name(const Node& self){
		regex(self);
	}
	void console_println(const Node& self){
		expression(self);
	}
	void literal(const Node& self){
		uint_literal(self);
		string_literal(self);
	}
	void uint_literal(const Node& self){
		regex(self);
	}
	void string_literal(const Node& self){
		regex(self);
	}
}