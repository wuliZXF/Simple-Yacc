//
// Created by wuliZXF on 2017/11/17.
//

#ifndef YACC_PRODUCTION_H
#define YACC_PRODUCTION_H

#include "Symbol.h"
#include <iostream>
#include <vector>

class Production {
private:
    std::vector<Symbol> expr;
public:
    // 优先级
    std::size_t prec;
    // 最后终结符的结合性
    char associative;

    Production(std::initializer_list<Symbol> list): expr(list){}

    const Symbol& operator[](std::size_t n) const {
        return expr[n];
    }

    Symbol& operator[](std::size_t n) {
        return expr[n];
    }

    std::size_t size() const {
        return expr.size();
    }

    auto begin() const noexcept -> decltype(expr.begin()) {
        return expr.begin();
    }

    auto end() const noexcept -> decltype(expr.end()) {
        return expr.end();
    }

    friend bool operator <(const Production &a, const Production &b);
    friend bool operator ==(const Production &a, const Production &b);
    friend bool operator !=(const Production &a, const Production &b);
};


inline std::ostream& operator << (std::ostream &out, const Production &production) {
    out << production[0].symbolName << " ->";
    for (auto it = production.begin() + 1; it != production.end(); ++it)
        out << ' ' << it->symbolName;
    return out;
}

inline bool operator <(const Production &a, const Production &b) {
    return a.expr < b.expr;
}

inline bool operator ==(const Production &a, const Production &b) {
    return a.expr == b.expr;
}

inline bool operator !=(const Production &a, const Production &b) {
    return !(a.expr == b.expr);
}


typedef std::vector<Production> Grammar;

#endif //YACC_PRODUCTION_H
