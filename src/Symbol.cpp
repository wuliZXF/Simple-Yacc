//
// Created by wuliZXF on 2017/11/17.
//

#include "Symbol.h"
Symbol::Symbol(Type type, std::string &&str): type(type), symbolName(move(str)) {}
Symbol::Symbol(Type type, const std::string &str): type(type), symbolName(str) {}