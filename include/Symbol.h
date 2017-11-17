//
// Created by wuliZXF on 2017/11/17.
//

#ifndef YACC_SYMBOL_H
#define YACC_SYMBOL_H

#include <string>

struct Symbol {
    enum class Type {
        TERMINAL,
        NON_TERMINAL
    };
    Type type;
    std::string symbolName;
    Symbol(Type type, std::string &&str);
    Symbol(Type type, const std::string &str);
};


#endif //YACC_SYMBOL_H
