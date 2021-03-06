//
// Created by wuliZXF on 2017/11/17.
//

#ifndef YACC_LR_H
#define YACC_LR_H

#include "Production.h"
#include <map>

struct LR {
    typedef typename std::pair<char, std::size_t> Action;

    std::vector<std::map<Symbol, Action>> actionTable;
    std::vector<std::map<Symbol, size_t>> gotoTable;

    LR(const Grammar &grammar);
    Grammar augmentedGrammar;
};

#endif //YACC_LR_H