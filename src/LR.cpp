//
// Created by wuliZXF on 2017/11/17.
//
#include "LR.h"
#include <set>
#include <queue>

namespace {

    std::map<Symbol, std::set<Symbol>> first;

    struct Item {
        const Production *production;
        std::size_t pos;
        std::set<Symbol> look;

        Item(const Production &p, std::size_t pos, const std::set<Symbol> &look): production(&p), pos(pos), look(look){}
        Item(const Production &p, std::size_t pos, std::set<Symbol> &&look): production(&p), pos(pos), look(std::move(look)){}
    };

    bool operator ==(const Item &a, const Item &b) {
        return a.production == b.production && a.pos == b.pos && a.look == b.look;
    }

    bool operator !=(const Item &a, const Item &b) {
        return !(a == b);
    }

    bool operator <(const Item &a, const Item &b) {
        return *(a.production) < *(b.production) || a.pos < b.pos || a.look < b.look;
    }

    std::ostream& operator << (std::ostream &out, const Item &item) {
        out << '[' << (*item.production)[0].symbolName << " ->";
        for (int i = 1; i < item.production->size(); ++i) {
            if (i - 1 == item.pos)
                out << " .";
            out << ' ' << (*item.production)[i].symbolName;
        }
        if (item.pos == item.production->size() - 1)
            out << " .";
        out << ", ";
        for (auto it = item.look.begin(); it != item.look.end(); ++it) {
            if (it != item.look.begin())
                out << '/';
            out << it->symbolName;
        }
        out << ']';
    }

    typedef std::set<Item> State;

    /**
     * 求FIRST(I)
     * @param grammar
     * @param a
     * @return
     */
    std::set<Symbol> findFirst(const Grammar &grammar, const std::vector<Symbol> &a) {
        if (a.size() == 1 && a[0].type == Symbol::Type::TERMINAL)
            return std::set<Symbol>{a[0]};
        std::set<Symbol> result;
        if (a.size() == 1) {
            result = first[a[0]];
            if (!result.empty())
                return result;
            for (const auto &p: grammar)
                if (p[0] == a[0]) {
                    for (auto it = p.begin() + 1; it != p.end(); ++it) {
                        std::set<Symbol> t(findFirst(grammar, std::vector<Symbol>{*it}));
                        result.insert(t.begin(), t.end());
                        if (t.find(EPSILON) == t.end())
                            break;
                    }
                }
            first[a[0]] = result;
        } else {
            for (const auto &syb : a) {
                std::set<Symbol> t(findFirst(grammar, std::vector<Symbol>{syb}));
                result.insert(t.begin(), t.end());
                if (t.find(EPSILON) == t.end())
                    break;
            }
        }
        if (result.empty())
            throw "Wrong grammar.";
        return result;
    }


    /**
     * 求闭包
     * @param state
     * @param G 增广文法
     * @return
     */
    void _closure(State &state, const Grammar &G) {
        if (state.empty())
            return;
        std::queue<Item> q;
        // 将每个可以增广的产生式 [A -> α·Bβ, a]加入队列
        for (const auto &i: state)
            if (i.pos + 1 < i.production->size() && (*i.production)[i.pos + 1].type == Symbol::Type::NON_TERMINAL)
                q.push(i);
        while (!q.empty()) {
            auto i = q.front();
            q.pop();
            for (const auto &p: G)
                // G 中每个产生式B -> γ
                if (p[0] == (*i.production)[i.pos + 1]) {
                    std::set<Symbol> term;
                    std::vector<Symbol> foo(i.production->begin() + i.pos + 2, i.production->end());
                    for (const auto &look: i.look) {
                        std::vector<Symbol> tail(foo.begin(), foo.end());
                        tail.push_back(look);
                        std::set<Symbol> fir = findFirst(G, tail);
                        term.insert(fir.begin(), fir.end());
                    }
                    Item item(p, 0, term);
                    bool insert = true;
                    for (auto it = state.begin(); it != state.end(); ++it) {
                        if (it->production == item.production && it->pos == item.pos) {
                            if (it->look != term) {
                                item.look.insert(it->look.begin(), it->look.end());
                                if (item.look == it->look)
                                    insert = false;
                                else
                                    state.erase(it);
                            } else
                                insert = false;
                            break;
                        }
                    }
                    if (insert) {
                        state.insert(item);
                        q.push(item);
                    }
                }
        }

    }

    /**
     * 求GOTO
     * @param state
     * @param symbol
     * @return
     */
    State _goto(const State &state, const Symbol &symbol, const Grammar &G) {
        State nextState;
        for (const auto &item: state)
            if (item.pos + 1 < item.production->size() && symbol == (*item.production)[item.pos + 1])
                nextState.insert(Item(*item.production, item.pos + 1, item.look));
        _closure(nextState, G);
        return nextState;
    }

    /**
     * 求LR(1)项集族，同时构造ACTION表和GOTO表
     * @param production
     * @return
     */
    std::vector<State> _items(const Grammar &G,
                              std::vector<std::map<Symbol, std::pair<char, std::size_t>>> &actionTable,
                              std::vector<std::map<Symbol, size_t>> &gotoTable) {
        std::set<Symbol> symbols;
        for (const auto &p: G)
            for (const auto &s: p)
                symbols.insert(s);
        State initial{Item(G[0], 0, std::set<Symbol>{TAILED})};
        _closure(initial, G);
        std::vector<State> items{initial};
        std::queue<size_t> q;
        q.push(0);
        while (!q.empty()) {
            auto i = q.front();
            q.pop();
            if (actionTable.size() <= i) {
                actionTable.resize(i + 1);
                gotoTable.resize(i + 1);
            }
            for (const auto &item: items[i]) {
                if (item.pos == item.production->size() - 1) {
                    for (const auto &symb: item.look) {
                        if (!actionTable[i].count(symb)) {
                            actionTable[i][symb].first = 'r';
                            actionTable[i][symb].second = item.production - &G[0];
                        } else {
                            auto &now = actionTable[i][symb];
                            if (now.second == item.production - &G[0] || G[now.second].prec > item.production->prec)
                                continue;
                            if (G[now.second].prec < item.production->prec || now.second > item.production - &G[0])
                                now.second = item.production - &G[0];
                        }
                    }
                }
            }
            for (const auto &x: symbols) {
                int j = -1;
                auto go = _goto(items[i], x, G);
                if (!go.empty()) {
                    bool exist = false;
                    for (int it = 0; it < items.size(); ++it)
                        if (items[it] == go) {
                            j = it;
                            exist = true;
                            break;
                        }
                    if (!exist) {
                        items.push_back(go);
                        q.push(items.size() - 1);
                        j = (int)items.size() - 1;
                    }
                }
                if (j != -1)
                    if (x.type == Symbol::Type::NON_TERMINAL)
                        gotoTable[i][x] = (size_t)j;
                    else {
                        if (!actionTable[i].count(x)) {
                            actionTable[i][x].first = 'S';
                            actionTable[i][x].second = (size_t)j;
                        } else {
                            auto &t = actionTable[i][x];
                            if (t.first == 'S')
                                throw "Not a DFA!";
                            const Production &r_p = G[t.second];
                            if (G[t.second].prec < x.prec
                                || (G[t.second].prec == x.prec && G[t.second].associative == 'l' && x.associative == 'r')) {
                                t.first = 'S';
                                t.second = (size_t)j;
                            }
                        }
                    }
            }
        }
        return items;
    }
}

LR::LR(const Grammar &grammar) {
    extendedGrammar = Grammar{Production{START, grammar[0][0]}};   // S' -> S
    extendedGrammar.insert(extendedGrammar.end(), grammar.begin(), grammar.end());
    _items(extendedGrammar, actionTable, gotoTable);
}


