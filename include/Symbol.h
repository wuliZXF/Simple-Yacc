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
    // 非终止符可能有优先级
    std::size_t prec;
    // l-左结合, r-右结合
    char associative;
    std::string symbolName;

    Symbol(Type type, std::string &&str, std::size_t prec=0, char associative='l')
            : type(type), symbolName(std::move(str)), prec(prec), associative(associative) {}
    Symbol(Type type, const std::string &str, std::size_t prec=0, char associative='l')
            : type(type), symbolName(str), prec(prec), associative(associative) {}
};

static const Symbol START = Symbol(Symbol::Type::NON_TERMINAL, "_S_");
// 终止符ε
static const Symbol EPSILON = Symbol(Symbol::Type::TERMINAL, "");
// 终结符号
static const Symbol TAILED = Symbol(Symbol::Type::TERMINAL, "$");

inline bool operator ==(const Symbol &a, const Symbol &b) {
    return a.symbolName == b.symbolName && a.type == b.type;
}

inline bool operator !=(const Symbol &a, const Symbol &b) {
    return !(a == b);
}

inline bool operator <(const Symbol &a, const Symbol &b) {
    return a.symbolName < b.symbolName;
}

#endif //YACC_SYMBOL_H
