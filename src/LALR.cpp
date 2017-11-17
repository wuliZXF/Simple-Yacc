//
// Created by wuliZXF on 2017/11/17.
//
#include "LALR.h"
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <queue>
#include <algorithm>

namespace {

    std::map<std::vector<Symbol>, std::set<Symbol>> first;

    struct Item {
        Production *production;
        std::size_t pos;
        std::vector<Symbol> look;
        inline bool operator ==(const Item& a) { return this->production == a.production && this->pos == a.pos; }
    };

    typedef std::vector<Item> State;

    /**
     * 求FIRST(I)
     * @param grammar
     * @param a
     * @return
     */
    std::set<Symbol> findFirst(const Grammar &grammar, const std::vector<Symbol> &a) {
        auto &t = first[a];
        if (!t.empty())
            return t;
        if (a.size() == 1) {
            if (a[0].type == Symbol::Type::TERMINAL) {
                auto &tmp = (first[a] = std::set<Symbol>{a[0]});
                return tmp;
            }
            std::set<Symbol> result;
            for (const auto &p: grammar) {
                if (p[0] != a[0])
                    continue;
                for (int i = 1; i < p.size(); ++i) {
                    auto x = findFirst(grammar, std::vector<Symbol>{p[i]});
                    result.insert(x.begin(), x.end());
                    if (x.find(EPSILON) == x.end())
                        break;
                }
            }
            if (result.empty())
                throw std::string("No FIRST of <") + std::string(a[0].type == Symbol::Type::NON_TERMINAL ? "non-terminal" : "terminal") + std::string(", ") + a[0].symbolName + std::string(">");
            first[a] = result;
            return result;
        }
    }


    /**
     * 求闭包
     * @param state
     * @param G 增广文法
     * @return
     */
    void _closure(State &state, const Grammar &G) {
        std::queue<Item> q;
        for (const auto &item: state) {
            q.push(item);
        }
        while (!q.empty()) {
            auto front = q.front();
            q.pop();
            for (const auto &production: G) {

            }
        }
    }

    /**
     * 求GOTO
     * @param state
     * @param symbol
     * @return
     */
    State _goto(const State &state, const Symbol &symbol, const Grammar &G);

    /**
     * 求LR(1)项集族
     * @param production
     * @return
     */
    std::vector<State> generateLRItems(const Grammar &G);
}

LALR::LALR(const Grammar &grammar) {
    // 增广文法 G'
    std::vector<Production> extendedProd{Production{Symbol(Symbol::Type::NON_TERMINAL, ""), grammar[0][0]}};   // S' -> S
    extendedProd.insert(extendedProd.end(), grammar.begin(), grammar.end());

}


