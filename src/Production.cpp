//
// Created by wuliZXF on 2017/11/17.
//
#include "Production.h"

Production::Production(std::initializer_list<Symbol> symbolSeq): seq(symbolSeq) {}
auto Production::size() -> decltype(seq.size()) {
    return this->seq.size();
}

auto Production::operator[](std::size_t n) -> decltype(seq[n]) {
    return this->seq[n];
}

const auto Production::operator[](std::size_t n) const -> decltype(seq[n]) {
    return this->seq[n];
}
