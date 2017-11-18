#include <iostream>
#include "Production.h"
#include "LR.h"
using namespace std;

int main() {
    // S -> if S else S | if S | S ; S | a
    Grammar test{
            Production{Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::TERMINAL, "if"), Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::TERMINAL, "else"), Symbol(Symbol::Type::NON_TERMINAL, "S")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::TERMINAL, "if"), Symbol(Symbol::Type::NON_TERMINAL, "S")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::TERMINAL, ";"), Symbol(Symbol::Type::NON_TERMINAL, "S")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "S"), Symbol(Symbol::Type::TERMINAL, "a")}
    };

    // E -> E + E | E * E | ( E ) | id
    Grammar test1 {
            Production{Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::TERMINAL, "+"), Symbol(Symbol::Type::NON_TERMINAL, "E")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::TERMINAL, "*"), Symbol(Symbol::Type::NON_TERMINAL, "E")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::TERMINAL, "("), Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::TERMINAL, ")")},
            Production{Symbol(Symbol::Type::NON_TERMINAL, "E"), Symbol(Symbol::Type::TERMINAL, "id")}
    };
    LR table(test);

    return 0;
}