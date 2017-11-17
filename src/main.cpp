#include <iostream>
#include "Symbol.h"
#include "Production.h"
using namespace std;

int main() {
    Production production{Symbol(Symbol::Type::NON_TERMINAL, "1")};
    production[0].symbolName = "3";
    cout << production[0].symbolName << endl;
    return 0;
}