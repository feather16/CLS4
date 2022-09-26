#include "Object.h"
#include "Error.h"

#include <any>
#include <iostream>

using namespace std;

namespace Program{
    Object::Object(const Type type){
        this->type = type;
        if(type == Type::sint){
            value = int();
        }
        else if(type == Type::sstring){
            value = string();
        }
        else if(type == Type::sbool){
            value = bool();
        }
    }
    Object::Object(const Type type, const string& value_str) : Object(type){
        if(type == Type::sint){
            value = stoi(value_str);
        }
        else if(type == Type::sstring){
            value = value_str.substr(1, value_str.size() - 2);
        }
        else if(type == Type::sbool){
            value = value_str == "true";
        }
    }

    Object Object::operator+(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            Program::Object result = Program::Object(Program::Type::sint);
            result.value = any_cast<int>(value) + any_cast<int>(rhs.value);
            return result;
        }
        else if(type == Program::Type::sstring && rhs.type == Program::Type::sstring){
            Program::Object result = Program::Object(Program::Type::sstring);
            result.value = any_cast<string>(value) + any_cast<string>(rhs.value);
            return result;
        }
        else{
            throw TypeError();
        }
    }
    Object Object::operator-(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            Program::Object result = Program::Object(Program::Type::sint);
            result.value = any_cast<int>(value) - any_cast<int>(rhs.value);
            return result;
        }
        else{
            throw TypeError();
        }
    }
    Object Object::operator*(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            Program::Object result = Program::Object(Program::Type::sint);
            result.value = any_cast<int>(value) * any_cast<int>(rhs.value);
            return result;
        }
        else{
            throw TypeError();
        }
    }
    Object Object::operator/(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            if(any_cast<int>(rhs.value) == 0){
                throw ZeroDivisionError();
            }
            Program::Object result = Program::Object(Program::Type::sint);
            result.value = any_cast<int>(value) / any_cast<int>(rhs.value);
            return result;
        }
        else{
            throw TypeError();
        }
    }
    Object Object::operator%(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            if(any_cast<int>(rhs.value) == 0){
                throw ZeroDivisionError();
            }
            Program::Object result = Program::Object(Program::Type::sint);
            result.value = any_cast<int>(value) % any_cast<int>(rhs.value);
            return result;
        }
        else{
            throw TypeError();
        }
    }
    Object& Object::operator+=(const Object& rhs){
        return *this = *this + rhs;
    }
    Object& Object::operator-=(const Object& rhs){
        return *this = *this - rhs;
    }
    Object& Object::operator*=(const Object& rhs){
        return *this = *this * rhs;
    }
    Object& Object::operator/=(const Object& rhs){
        return *this = *this / rhs;
    }
    Object& Object::operator%=(const Object& rhs){
        return *this = *this % rhs;
    }
    bool Object::operator==(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            return any_cast<int>(value) == any_cast<int>(rhs.value);
        }
        else if(type == Program::Type::sstring && rhs.type == Program::Type::sstring){
            return any_cast<string>(value) == any_cast<string>(rhs.value);
        }
        else if(type == Program::Type::sbool && rhs.type == Program::Type::sbool){
            return any_cast<bool>(value) == any_cast<bool>(rhs.value);
        }
        else{
            throw TypeError();
        }
    }
    bool Object::operator!=(const Object& rhs) const{
        return !((*this) == rhs);
    }
    bool Object::operator<(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            return any_cast<int>(value) < any_cast<int>(rhs.value);
        }
        else if(type == Program::Type::sstring && rhs.type == Program::Type::sstring){
            return any_cast<string>(value) < any_cast<string>(rhs.value);
        }
        else{
            throw TypeError();
        }
    }
    bool Object::operator>(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            return any_cast<int>(value) > any_cast<int>(rhs.value);
        }
        else if(type == Program::Type::sstring && rhs.type == Program::Type::sstring){
            return any_cast<string>(value) > any_cast<string>(rhs.value);
        }
        else{
            throw TypeError();
        }
    }
    bool Object::operator<=(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            return any_cast<int>(value) <= any_cast<int>(rhs.value);
        }
        else if(type == Program::Type::sstring && rhs.type == Program::Type::sstring){
            return any_cast<string>(value) <= any_cast<string>(rhs.value);
        }
        else{
            throw TypeError();
        }
    }
    bool Object::operator>=(const Object& rhs) const{
        if(type == Program::Type::sint && rhs.type == Program::Type::sint){
            return any_cast<int>(value) >= any_cast<int>(rhs.value);
        }
        else if(type == Program::Type::sstring && rhs.type == Program::Type::sstring){
            return any_cast<string>(value) >= any_cast<string>(rhs.value);
        }
        else{
            throw TypeError();
        }
    }
}