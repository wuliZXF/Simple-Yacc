//
// Created by wuliZXF on 2017/11/17.
//

#ifndef YACC_PRODUCTION_H
#define YACC_PRODUCTION_H

#include "Symbol.h"
#include <vector>

class Production {
private:
    std::vector<Symbol> seq;
public:
    Production(std::initializer_list<Symbol> symbolSeq);
    auto size() -> decltype(seq.size());
    auto operator[](std::size_t n) -> decltype(seq[n]);
    const auto operator[](std::size_t n) const -> decltype(seq[n]);
};

#endif //YACC_PRODUCTION_H
