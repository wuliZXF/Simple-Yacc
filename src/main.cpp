#include <iostream>
#include "Production.h"
#include "LALR.h"
using namespace std;

int main() {
    // S -> CC, C -> cC | d.
    Grammar test{
            Production{Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::NON_TERMINAL, "C"), Symbol(Symbol::Type::NON_TERMINAL, "C")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "C"), Symbol(Symbol::Type::TERMINAL, "c"), Symbol(Symbol::Type::NON_TERMINAL, "C")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "C"), Symbol(Symbol::Type::TERMINAL, "d")}
    };
    LALR table(test);

    return 0;
}