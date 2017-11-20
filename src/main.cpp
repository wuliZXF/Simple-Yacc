#include "Production.h"
#include "LR.h"
#include <fstream>

using namespace std;

void generateAnalyzer(ofstream &out, LR &lr);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Source file(*.y) is needed." << endl;
        return 1;
    }
    if (argc > 2)
        cout << "The redundant argument(s) will be ignored." << endl;
    string lexFile(argv[1]);
    ifstream ifs(lexFile, ifstream::in);
    if (!ifs) {
        cout << "No such file." << endl;
        return 1;
    }
    map<string, Symbol> tokens;
    // flags
    bool rules = false;
    size_t upperPrec = 0;
    size_t prec = 0;
    char asso = 'l';

    Symbol emptySymbol, left = emptySymbol;
    vector<Symbol> right;
    size_t productionPrec = 0;
    Grammar grammar;
    string str;
    while (ifs >> str) {
        if (!rules) {
            if (str == "%token") {
                prec = 0;
                asso = 'l';
            } else if (str == "%left") {
                prec = ++upperPrec;
                asso = 'l';
            } else if (str == "%right") {
                prec = ++upperPrec;
                asso = 'r';
            } else if (str == "%%")
                rules = true;
            else tokens[str] = Symbol(Symbol::Type::TERMINAL, str, prec, asso);
        } else {
            if (str == ":")
                continue;
            else if (str == "%prec") {
                ifs >> str;
                productionPrec = tokens[str].prec;
            } else if (str == "|" || str == ";") {
                vector<Symbol> temp{left};
                temp.insert(temp.end(), right.begin(), right.end());
                grammar.push_back(Production(move(temp), productionPrec));
                productionPrec = 0;
                right.clear();
                if (str == ";")
                    left = emptySymbol;
            } else {
                if (left == emptySymbol)
                    left = Symbol(Symbol::Type::NON_TERMINAL, str);
                else {
                    if (tokens.count(str))
                        right.push_back(tokens[str]);
                    else if (str[0] == '\'' && str[str.length() - 1] == '\'')
                        right.emplace_back(Symbol::Type::TERMINAL, str);
                    else
                        right.emplace_back(Symbol::Type::NON_TERMINAL, str);
                }
            }
        }
    }

    try {
        LR lrTable(grammar);
        auto pos = lexFile.find_last_of('/') == -1 ? 0 : lexFile.find_last_of('/') + 1;
        auto n = lexFile.find_last_of('.') == -1 ? (lexFile.length() - pos - 1) : (lexFile.find_last_of('.') - pos);
        string name(lexFile, pos, n);
        ofstream ofs(name + ".tab.cpp", ofstream::out);
        generateAnalyzer(ofs, lrTable);
    } catch (const char *s) {
        cerr << s << endl;
    }
    return 0;
}

void generateAnalyzer(ofstream &out, LR &lr) {
    static string type[2] = {"TERMINAL", "NON_TERMINAL"};
    out << "#include <iostream>\n"
            "#include <vector>\n"
            "#include <map>\n"
            "#include <cctype>\n"
            "#include <fstream>\n"
            "#include <stack>\n"
            "using namespace std;\n"
            "struct Symbol {\n"
            "    enum class Type {\n"
            "        TERMINAL,\n"
            "        NON_TERMINAL\n"
            "    };\n"
            "    Type type;\n"
            "    size_t prec;\n"
            "    char associative;\n"
            "    string symbolName;\n"
            "    Symbol(){}\n"
            "    Symbol(Type type, string &&str, size_t prec=0, char associative='l')\n"
            "            : type(type), symbolName(move(str)), prec(prec), associative(associative) {}\n"
            "    Symbol(Type type, const string &str, size_t prec=0, char associative='l')\n"
            "            : type(type), symbolName(str), prec(prec), associative(associative) {}\n"
            "};\n"
            "static const Symbol START = Symbol(Symbol::Type::NON_TERMINAL, \"_S_\");\n"
            "static const Symbol EPSILON = Symbol(Symbol::Type::TERMINAL, \"ε\");\n"
            "static const Symbol TAILED = Symbol(Symbol::Type::TERMINAL, \"$\");\n"
            "inline bool operator ==(const Symbol &a, const Symbol &b) {\n"
            "    return a.symbolName == b.symbolName && a.type == b.type;\n"
            "}\n"
            "inline bool operator !=(const Symbol &a, const Symbol &b) {\n"
            "    return !(a == b);\n"
            "}\n"
            "inline bool operator <(const Symbol &a, const Symbol &b) {\n"
            "    return a.symbolName < b.symbolName;\n"
            "}\n"
            "typedef pair<char, size_t> Action;\n"
            "class Production {\n"
            "private:\n"
            "    std::vector<Symbol> expr;\n"
            "public:\n"
            "    std::size_t prec;\n"
            "    char associative;\n"
            "    Production(std::initializer_list<Symbol> list): expr(list), prec(0), associative('l') {}\n"
            "    Production(std::vector<Symbol> &&expr, size_t prec = 0, char associative = 'l'): expr(std::move(expr)), prec(prec), associative(associative) {}\n"
            "    Production(const std::vector<Symbol> &expr, size_t prec = 0, char associative = 'l'): expr(expr), prec(prec), associative(associative) {}\n"
            "    const Symbol& operator[](std::size_t n) const {\n"
            "        return expr[n];\n"
            "    }\n"
            "    Symbol& operator[](std::size_t n) {\n"
            "        return expr[n];\n"
            "    }\n"
            "    std::size_t size() const {\n"
            "        return expr.size();\n"
            "    }\n"
            "    auto begin() const noexcept -> decltype(expr.begin()) {\n"
            "        return expr.begin();\n"
            "    }\n"
            "    auto end() const noexcept -> decltype(expr.end()) {\n"
            "        return expr.end();\n"
            "    }\n"
            "    auto rbegin() const noexcept -> decltype(expr.rbegin()) {\n"
            "        return expr.rbegin();\n"
            "    }\n"
            "    auto rend() const noexcept -> decltype(expr.rend()) {\n"
            "        return expr.rend();\n"
            "    }\n"
            "    friend bool operator <(const Production &a, const Production &b);\n"
            "    friend bool operator ==(const Production &a, const Production &b);\n"
            "    friend bool operator !=(const Production &a, const Production &b);\n"
            "};\n"
            "inline std::ostream& operator << (std::ostream &out, const Production &production) {\n"
            "    out << production[0].symbolName << \" ->\";\n"
            "    for (auto it = production.begin() + 1; it != production.end(); ++it)\n"
            "        out << ' ' << it->symbolName;\n"
            "    return out;\n"
            "}\n"
            "inline bool operator <(const Production &a, const Production &b) {\n"
            "    return a.expr < b.expr;\n"
            "}\n"
            "inline bool operator ==(const Production &a, const Production &b) {\n"
            "    return a.expr == b.expr;\n"
            "}\n"
            "inline bool operator !=(const Production &a, const Production &b) {\n"
            "    return !(a.expr == b.expr);\n"
            "}\n"
            "typedef std::vector<Production> Grammar;\n"
            "static vector<map<Symbol, Action>> actionTable{";
    const auto &actionTable = lr.actionTable;
    const auto &gotoTable = lr.gotoTable;
    const auto &extendedGrammar = lr.extendedGrammar;
    for (auto it = actionTable.begin(); it != actionTable.end(); ++it) {
        if (it != actionTable.begin())
            out << ',';
        out << "map<Symbol, Action>{";
        for (auto mapIt = it->begin(); mapIt != it->end(); ++mapIt) {
            const auto &symbol = mapIt->first;
            const auto &action = mapIt->second;
            if (mapIt != it->begin())
                out << ',';
            out << "make_pair(Symbol(Symbol::Type::" << type[(int)symbol.type] << ",\""
                << symbol.symbolName << "\"," << symbol.prec << ",'" << symbol.associative
                << "'),make_pair('" << action.first << "'," << action.second << "))";
        }
        out << '}';
    }
    out << "};\nstatic vector<map<Symbol, size_t>> gotoTable{";
    for (auto it = gotoTable.begin(); it != gotoTable.end(); ++it) {
        if (it != gotoTable.begin())
            out << ',';
        out << "map<Symbol, size_t>{";
        for (auto mapIt = it->begin(); mapIt != it->end(); ++mapIt) {
            const auto &symbol = mapIt->first;
            if (mapIt != it->begin())
                out << ',';
            out << "make_pair(Symbol(Symbol::Type::" << type[(int)symbol.type] << ",\""
                << symbol.symbolName << "\"," << symbol.prec << ",'" << symbol.associative
                << "')," << mapIt->second << ")";
        }
        out << '}';
    }
    out << "};\nstatic Grammar extendedGrammar{";
    for (auto it = extendedGrammar.begin(); it != extendedGrammar.end(); ++it) {
        if (it != extendedGrammar.begin())
            out << ',';
        out << "{";
        for (auto symIt = it->begin(); symIt != it->end(); ++symIt) {
            if (symIt != it->begin())
                out << ',';
            out << "Symbol(Symbol::Type::" << type[(int)symIt->type] << ",\""
                << symIt->symbolName << "\"," << symIt->prec << ",'" << symIt->associative << "')";
        }
        out << "}";
    }
    out << "};\nint main(int argc, char* argv[]) {\n"
            "    if (argc < 2) {\n"
            "        cout << \"Token-stream file is needed.\" << endl;\n"
            "        return 1;\n"
            "    }\n"
            "    if (argc > 2)\n"
            "        cout << \"The redundant argument(s) will be ignored.\" << endl;\n"
            "    string src(argv[1]);\n"
            "    ifstream ifs(src, ifstream::in);\n"
            "    if (!ifs) {\n"
            "        cout << \"No such file.\" << endl;\n"
            "        return 1;\n"
            "    }\n"
            "\tvector<pair<string, string>> tokens;\n"
            "\ttry {\n"
            "\t\twhile (ifs.good()) {\n"
            "\t\t\tchar c = ifs.get();\n"
            "\t\t\tif (c == EOF)\n"
            "\t\t\t\tbreak;\n"
            "\t\t\tif (isspace(c))\n"
            "\t\t\t\tcontinue;\n"
            "\t\t\tstring tokenName, str;\n"
            "\t\t\tswitch (c) {\n"
            "\t\t\tcase '<':\n"
            "\t\t\t\twhile ((c = ifs.get()) != ',')\n"
            "\t\t\t\t\ttokenName += c;\n"
            "\t\t\t\twhile ((c = ifs.get()) != '\"');\n"
            "\t\t\t\twhile ((c = ifs.get()) != '\"')\n"
            "\t\t\t\t\tstr += c;\n"
            "\t\t\t\ttokens.emplace_back(move(tokenName), move(str));\n"
            "\t\t\t\twhile ((c = ifs.get()) != '>');\n"
            "\t\t\t\tbreak;\n"
            "\t\t\tdefault:\n"
            "\t\t\t\tcerr << \"Error.\" << endl;\n"
            "\t\t\t\treturn 1;\n"
            "\t\t\t}\n"
            "\t\t}\n"
            "\t} catch (...) {\n"
            "\t\tcerr << \"Error.\" << endl;\n"
            "\t\treturn 1;\n"
            "\t}\n"
            "\tcout << \"Start analyzing the tokens...\\n===========\\n\";\n"
            "\ttokens.emplace_back(\"$\", \"$\");\n"
            "\tstack<size_t> states;\n"
            "\tstates.push(0);\n"
            "\tfor (auto it = tokens.begin(); it != tokens.end(); ) {\n"
            "\t\tconst auto &token = *it;\n"
            "\t\tint state = states.top();\n"
            "\t\tSymbol sb;\n"
            "\t\tAction action;\n"
            "\t\tif (actionTable[state].count(sb = Symbol(Symbol::Type::TERMINAL, \"'\" + token.second + \"'\")))\n"
            "\t\t\taction = actionTable[state][sb];\n"
            "\t\telse if (actionTable[state].count(sb = Symbol(Symbol::Type::TERMINAL, token.first)))\n"
            "\t\t\taction = actionTable[state][sb];\n"
            "\t\telse {\n"
            "\t\t\tcerr << \"Invalid statement.(0)\" << endl;\n"
            "\t\t\treturn 1;\n"
            "\t\t}\n"
            "\t\tif (action.first == 'S') {\n"
            "\t\t\tstates.push(action.second);\n"
            "\t\t\t++it;\n"
            "\t\t} else if (action.first == 'r' && action.second) {\n"
            "\t\t\tcout << extendedGrammar[action.second] << endl;\n"
            "\t\t\tfor (int i = 1; i < extendedGrammar[action.second].size(); ++i)\n"
            "\t\t\t\tstates.pop();\n"
            "\t\t\tif (gotoTable[states.top()].count(extendedGrammar[action.second][0]))\n"
            "\t\t\t\tstates.push(gotoTable[states.top()][extendedGrammar[action.second][0]]);\n"
            "\t\t\telse {\n"
            "\t\t\t\tcerr << \"Invalid statement.(1)\" << endl;\n"
            "\t\t\t\treturn 1;\n"
            "\t\t\t}\n"
            "\t\t} else if (action.first == 'r' && !action.second) {\n"
            "\t\t\tcout << \"Accepted.\" << endl;\n"
            "\t\t\treturn 0;\n"
            "\t\t}\n"
            "\t}\n"
            "\tcerr << \"Unknown error.\" << endl;\n"
            "\treturn 1;\n"
            "}";
}