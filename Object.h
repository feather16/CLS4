#pragma once

#include "Type.h"

#include <any>
#include <iostream>

using namespace std;

namespace Program{
    class Object{
        public:
        Type type = Type::svoid;
        any value = 0;
        Object(){}
        Object(const Type);
        Object(const Type, const string&);
        Object(const string& type_str)
            : Object(str_to_type[type_str]){}
        Object(const string& type_str, const string& value_str)
            : Object(str_to_type[type_str], value_str){}

        Object operator+(const Object&) const;
        Object operator-(const Object&) const;
        Object operator*(const Object&) const;
        Object operator/(const Object&) const;
        Object operator%(const Object&) const;
        Object& operator+=(const Object&);
        Object& operator-=(const Object&);
        Object& operator*=(const Object&);
        Object& operator/=(const Object&);
        Object& operator%=(const Object&);
        bool operator==(const Object&) const;
        bool operator!=(const Object&) const;
        bool operator<(const Object&) const;
        bool operator>(const Object&) const;
        bool operator<=(const Object&) const;
        bool operator>=(const Object&) const;

        friend ostream& operator<<(ostream& os, const Object& object){
            #define PROGRAM_OBJECT_TO_STREAM(type_str)\
            if(object.type==Type::s ## type_str)os<<any_cast<type_str>(object.value);

            if(object.type == Type::svoid){
                os << "void";
            }
            else if(object.type == Type::sbool){
                os << (any_cast<bool>(object.value) ? "true" : "false");
            }

            PROGRAM_OBJECT_TO_STREAM(int)
            PROGRAM_OBJECT_TO_STREAM(string)

            return os;
        }

        friend istream& operator>>(istream& is, Object& object){
            if(object.type == Type::sint){
                int value;
                is >> value;
                object.value = value;
            }
            else if(object.type == Type::sstring){
                string value;
                is >> value;
                object.value = value;
            }
            return is;
        }
    };
}