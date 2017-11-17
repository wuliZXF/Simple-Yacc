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
    std::size_t priority;
    std::string symbolName;

    Symbol(Type type, std::string &&str, std::size_t priority=0): type(type), symbolName(std::move(str)), priority(priority) {}
    Symbol(Type type, const std::string &str, std::size_t priority=0): type(type), symbolName(str), priority(priority) {}
};

// 终止符ε
static const Symbol EPSILON = Symbol(Symbol::Type::TERMINAL, "");

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
