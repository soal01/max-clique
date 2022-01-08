#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

//std::vector<std::set<int>>& g
std::pair<int, int> get_min_degree(std::vector<std::set<int>>& g, const std::set<int>& V) {
    int min_deg = g.size();
    std::pair<int, int> ans = {min_deg, 0};
    for (auto i : V) {
        auto u = g[i];
        if (min_deg > u.size()) {
            min_deg = u.size();
            ans = {min_deg, i};
        }
    }
    return ans;
}

std::pair<int, int> get_max_degree(std::vector<std::set<int>>& g, const std::set<int>& V) {
    int max_deg = 0;
    std::pair<int, int> ans = {max_deg, 0};
    for (auto i : V) {
        auto u = g[i];
        if (max_deg < u.size()) {
            max_deg = u.size();
            ans = {max_deg, i};
        }
    }
    return ans;
}

int get_deg(std::vector<std::set<int>>& g, const std::set<int>& V, int u) {
    std::set<int> ans;
    std::set_intersection(V.begin(), V.end(), g[u].begin(), g[u].end(),
                        std::inserter(ans, ans.begin()));
    return ans.size();
}

std::set<int> get_A(std::vector<std::set<int>>& g, const std::set<int>& V, int u) {
    std::set<int> ans;
    std::set_intersection(V.begin(), V.end(), g[u].begin(), g[u].end(),
                        std::inserter(ans, ans.begin()));
    return ans;
}

std::set<int> intersect(const std::set<int>& A, const std::set<int>& B) {
    std::set<int> ans;
    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),
                        std::inserter(ans, ans.begin()));
    return ans;
}

std::set<int> difference(const std::set<int>& A, const std::set<int>& B) {
    std::set<int> ans;
    std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
                        std::inserter(ans, ans.begin()));
    return ans;
}

bool is_connected(std::vector<std::set<int>>& g, int u, int v) {
    return g[u].find(v) != g[u].end();
}

bool is_connected2(std::vector<std::set<int>>& g, const std::pair<int, int>& p) {
    return is_connected(g, p.first, p.second);
}

std::pair<int, int> find2(std::vector<std::set<int>>& g, const std::set<int>& V) {
    for (auto v : V) {
        auto A = get_A(g, V, v);
        for (auto w : A) {
            if (get_deg(g, V, v) == 2 && get_deg(g, V, w) >= 3) {
                return {v, w};
            }
        }
    }
}

int statement_1(std::vector<std::set<int>>& g, const std::set<int>& V, int v) {
    int w = *g[v].begin();
    std::set<int> new_V;
    std::set<int> used_set = {v, w};
    std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                    std::inserter(new_V, new_V.begin()));
    return 1 + maxset(g, new_V);
}

int statement_2(std::vector<std::set<int>>& g, const std::set<int>& V) {
    if (get_max_degree(g, V).first == 2) {
        return V.size() / 2;
    } else {
        auto p = find2(g, V);
        int v = p.first;
        int w1 = p.second;
        auto A = get_A(g, V, v);
        int w2 = 0;
        for (auto w : A) {
            if (w != w1) {
                w2 = w;
                break;
            }
        }
        auto A_w1 = get_A(g, V, w1);
        if (A_w1.find(w2) != A_w1.end()) {
            std::set<int> new_V;
            std::set<int> used_set = {v, w1, w2};
            std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                    std::inserter(new_V, new_V.begin()));
            return 1 + maxset(g, new_V);
        } else {
            std::set<int> new_V;
            std::set<int> used_set = {v, w1, w2};
            std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                    std::inserter(new_V, new_V.begin()));
            int maxset1 = 1 + maxset(g, new_V);
            new_V.clear();
            used_set.clear();
            auto A_w2 = get_A(g, V, w2);
            std::set_union(A_w1.begin(), A_w1.end(), A_w2.begin(), A_w2.end(),
                    std::inserter(used_set, used_set.begin()));
            std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                    std::inserter(new_V, new_V.begin()));
            int maxset2 = 2 + maxset(g, new_V);
            return std::max(maxset1, maxset2);
        }
    }
}


int statement_3_4_3_4(std::vector<std::set<int>>& g, const std::set<int>& V, std::set<int>& W,
                    std::set<int>& A1, std::set<int>& A2, std::set<int>& A3) {
    auto p = difference(intersect(A2, A3), A1);
    auto iter = p.begin();
    int u11 = *iter; ++iter;
    int u12 = *iter;
    
    auto p = difference(intersect(A1, A3), A2);
    auto iter = p.begin();
    int u21 = *iter; ++iter;
    int u22 = *iter;

    auto p = difference(intersect(A1, A2), A3);
    auto iter = p.begin();
    int u31 = *iter; ++iter;
    int u32 = *iter;
    int size = intersect(A1, intersect(A2, A3)).size();

    if (intersect(A1, A2).size() == size + 2 && intersect(A1, A3).size() == size + 2 &&
        intersect(A3, A2).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        if (is_connected(g, u11, u12)) {
            int maxset3 = 2 + maxset(g, intersect(A1, A2));
            int maxset4 = 2 + maxset(g, intersect(A1, A3));
            return std::max({maxset1, maxset2, maxset3, maxset4});
        }
        if (is_connected(g, u21, u22)) {
            int maxset3 = 2 + maxset(g, intersect(A1, A2));
            int maxset4 = 2 + maxset(g, intersect(A2, A3));
            return std::max({maxset1, maxset2, maxset3, maxset4});
        }
        if (is_connected(g, u31, u32)) {
            int maxset3 = 2 + maxset(g, intersect(A1, A3));
            int maxset4 = 2 + maxset(g, intersect(A2, A3));
            return std::max({maxset1, maxset2, maxset3, maxset4});
        }
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u11)), get_A(g, V, u12)));
        int maxset4 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u21)), get_A(g, V, u22)));
        int maxset5 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u31)), get_A(g, V, u32)));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    if (intersect(A1, A2).size() == size + 2 && intersect(A1, A3).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u31)), get_A(g, V, u32)));
        int maxset4 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u21)), get_A(g, V, u22)));
        int maxset5 = 2 + maxset(g, intersect(A2, A3));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    if (intersect(A2, A3).size() == size + 2 && intersect(A1, A2).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u31)), get_A(g, V, u32)));
        int maxset4 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u11)), get_A(g, V, u12)));
        int maxset5 = 2 + maxset(g, intersect(A1, A3));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    if (intersect(A2, A3).size() == size + 2 && intersect(A1, A3).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u21)), get_A(g, V, u22)));
        int maxset4 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u11)), get_A(g, V, u12)));
        int maxset5 = 2 + maxset(g, intersect(A1, A2));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    if (intersect(A1, A2).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u31)), get_A(g, V, u32)));
        int maxset4 = 2 + maxset(g, intersect(A1, A3));
        int maxset5 = 2 + maxset(g, intersect(A2, A3));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    if (intersect(A1, A3).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u21)), get_A(g, V, u22)));
        int maxset4 = 2 + maxset(g, intersect(A1, A2));
        int maxset5 = 2 + maxset(g, intersect(A2, A3));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    if (intersect(A2, A3).size() == size + 2) {
        int maxset1 = 1 + maxset(g, difference(V, W));
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        auto A_un = intersect(A1, intersect(A2, A3));
        int maxset3 = 4 + maxset(g, difference(difference(A_un, get_A(g, V, u11)), get_A(g, V, u12)));
        int maxset4 = 2 + maxset(g, intersect(A1, A2));
        int maxset5 = 2 + maxset(g, intersect(A1, A3));
        return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
    }

    int maxset1 = 1 + maxset(g, difference(V, W));
    int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
    int maxset3 = 2 + maxset(g, intersect(A1, A2));
    int maxset4 = 2 + maxset(g, intersect(A1, A3));
    int maxset5 = 2 + maxset(g, intersect(A2, A3));
    return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
}

int statement_3_4(std::vector<std::set<int>>& g, const std::set<int>& V, std::set<int>& W,
                    std::set<int>& A1, std::set<int>& A2, std::set<int>& A3) {
    int maxset1 = 1 + maxset(g, difference(V, W));
    if (intersect(A1, intersect(A2, A3)).size() >= V.size() - 7) {
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        return std::max(maxset1, maxset2);
    }
    int size = intersect(A1, intersect(A2, A3)).size();
    
    if (size == V.size() - 8 || size == V.size() - 9) {
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        if (intersect(A1, A2).size() >= size + 2) {
            int maxset3 = 2 + maxset(g, intersect(A1, A2));
            return std::max(maxset1, std::max(maxset2, maxset3));
        }
        if (intersect(A1, A3).size() >= size + 2) {
            int maxset3 = 2 + maxset(g, intersect(A1, A3));
            return std::max(maxset1, std::max(maxset2, maxset3));
        }
        if (intersect(A2, A3).size() >= size + 2) {
            int maxset3 = 2 + maxset(g, intersect(A2, A3));
            return std::max(maxset1, std::max(maxset2, maxset3));
        }
        return std::max(maxset1, maxset2);
    }

    if (size <= V.size() - 10) {
        int maxset2 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        if (intersect(A1, A2).size() <= size + 1 && intersect(A1, A3).size() <= size + 1 && 
            intersect(A3, A2).size() <= size + 1) {
            return std::max(maxset1, maxset2);
        }

        if (intersect(A1, A2).size() >= size + 2 && intersect(A1, A3).size() >= size + 2 && 
            intersect(A3, A2).size() >= size + 2) {
            return statement_3_4_3_4(g, V, W, A1, A2, A3);
        }

        if (intersect(A1, A2).size() >= size + 2) {
            int maxset3 = 2 + maxset(g, intersect(A1, A2));
            return std::max(maxset1, std::max(maxset2, maxset3));
        }
        if (intersect(A1, A3).size() >= size + 2) {
            int maxset3 = 2 + maxset(g, intersect(A1, A3));
            return std::max(maxset1, std::max(maxset2, maxset3));
        }
        if (intersect(A2, A3).size() >= size + 2) {
            int maxset3 = 2 + maxset(g, intersect(A2, A3));
            return std::max(maxset1, std::max(maxset2, maxset3));
        }
    }


}


int statement_3(std::vector<std::set<int>>& g, const std::set<int>& V, int v) {
    auto A = get_A(g, V, v);
    auto iter = A.begin();
    int w1 = *iter; iter++;
    int w2 = *iter; iter++;
    int w3 = *iter;
    int maxset1 = 0;
    std::set<int> new_V;
    std::set<int> used_set = {v, w1, w2, w3};
    std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
        std::inserter(new_V, new_V.begin()));
    maxset1 = 1 + maxset(g, new_V);
    if (is_connected(g, w1, w2) && is_connected(g, w3, w2) && is_connected(g, w1, w3)) {
        return maxset1;
    }
    if (is_connected(g, w1, w2) && is_connected(g, w1, w3)) {
        auto A_w2 = get_A(g, V, w2);
        auto A_w3 = get_A(g, V, w3);
        std::set<int> used_set, new_V;
        std::set_union(A_w3.begin(), A_w3.end(), A_w2.begin(), A_w2.end(),
                std::inserter(used_set, used_set.begin()));
        std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                std::inserter(new_V, new_V.begin()));
        int maxset2 = 2 + maxset(g, new_V);
        return std::max(maxset1, maxset2);
    }
    if (is_connected(g, w1, w2) && is_connected(g, w2, w3)) {
        auto A_w1 = get_A(g, V, w1);
        auto A_w3 = get_A(g, V, w3);
        std::set<int> used_set, new_V;
        std::set_union(A_w3.begin(), A_w3.end(), A_w1.begin(), A_w1.end(),
                std::inserter(used_set, used_set.begin()));
        std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                std::inserter(new_V, new_V.begin()));
        int maxset2 = 2 + maxset(g, new_V);
        return std::max(maxset1, maxset2);
    }
    if (is_connected(g, w1, w3) && is_connected(g, w2, w3)) {
        auto A_w1 = get_A(g, V, w1);
        auto A_w2 = get_A(g, V, w2);
        std::set<int> used_set, new_V;
        std::set_union(A_w2.begin(), A_w2.end(), A_w1.begin(), A_w1.end(),
                std::inserter(used_set, used_set.begin()));
        std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
                std::inserter(new_V, new_V.begin()));
        int maxset2 = 2 + maxset(g, new_V);
        return std::max(maxset1, maxset2);
    }
    std::set<int> W = {w1, w2, w3};
    auto A_w1 = get_A(g, V, w1);
    auto A_w2 = get_A(g, V, w2);
    auto A_w3 = get_A(g, V, w2);
    std::set<int> A1, A2, A3;
    std::set<int> used_set;
    std::set_union(W.begin(), W.end(), A_w1.begin(), A_w1.end(),
            std::inserter(used_set, used_set.begin()));
    std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
            std::inserter(A1, A1.begin()));
    used_set.clear();
    std::set_union(W.begin(), W.end(), A_w2.begin(), A_w2.end(),
            std::inserter(used_set, used_set.begin()));
    std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
            std::inserter(A2, A2.begin()));
    used_set.clear();
    std::set_union(W.begin(), W.end(), A_w3.begin(), A_w3.end(),
            std::inserter(used_set, used_set.begin()));
    std::set_difference(V.begin(), V.end(), used_set.begin(), used_set.end(),
            std::inserter(A3, A3.begin()));
    used_set.clear();
    if (is_connected(g, w1, w2)) {
        if (intersect(A3, A2).size() <= V.size() - 7 &&
            intersect(A1, A3).size() <= V.size() - 7) {
            int maxset2 = 2 + maxset(g, intersect(A1, A3));
            int maxset3 = 2 + maxset(g, intersect(A3, A2));
            return std::max(maxset1, std::max(maxset2, maxset3));
        } else {
            int maxset2 = 2 + maxset(g, A3);
            return std::max(maxset1, maxset2);
        }
    }
    if (is_connected(g, w1, w3)) {
        if (intersect(A1, A2).size() <= V.size() - 7 &&
            intersect(A2, A3).size() <= V.size() - 7) {
            int maxset2 = 2 + maxset(g, intersect(A1, A2));
            int maxset3 = 2 + maxset(g, intersect(A3, A2));
            return std::max(maxset1, std::max(maxset2, maxset3));
        } else {
            int maxset2 = 2 + maxset(g, A2);
            return std::max(maxset1, maxset2);
        }
    }
    if (is_connected(g, w2, w3)) {
        if (intersect(A1, A3).size() <= V.size() - 7 &&
            intersect(A2, A1).size() <= V.size() - 7) {
            int maxset2 = 2 + maxset(g, intersect(A1, A3));
            int maxset3 = 2 + maxset(g, intersect(A1, A2));
            return std::max(maxset1, std::max(maxset2, maxset3));
        } else {
            int maxset2 = 2 + maxset(g, A1);
            return std::max(maxset1, maxset2);
        }
    }
    W.insert(v);
    return statement_3_4(g, V, W, A1, A2, A3);
}

std::pair<int, int> find_4_1(std::vector<std::set<int>>& g, const std::set<int>& V) {
    int v = -1, w = -1;
    bool flag = false;
    for (auto u1 : V) {
        for (auto u2 : V) {
            if (u1 != u2) {
                if (!is_connected(g, u1, u2) && intersect(get_A(g, V, u1), get_A(g, V, u2)).size() >= 2 &&
                    get_deg(g, V, u1) == 4) {
                    flag = true;
                    v = u1;
                    w = u2;
                    break;
                }
            }
        }
        if (flag) {
            break;
        }
    }
    return {v, w};
}

std::pair<int, int> find_4_1_1_1(std::vector<std::set<int>>& g, const std::set<int>& V) {
    int v = -1, w = -1;
    bool flag = false;
    for (auto u1 : V) {
        for (auto u2 : V) {
            if (u1 != u2) {
                if (!is_connected(g, u1, u2) && intersect(get_A(g, V, u1), get_A(g, V, u2)).size() >= 3 &&
                    get_deg(g, V, u1) == 4) {
                    flag = true;
                    v = u1;
                    w = u2;
                    break;
                }
            }
        }
        if (flag) {
            break;
        }
    }
    return {v, w};
}

int statement_4_1_1_2(std::vector<std::set<int>>& g, const std::set<int>& V, int v, int w) {
    auto A = difference(get_A(g, V, v), get_A(g, V, w));
    auto iter = A.begin();
    int x = *iter; ++iter;
    int y = *iter;

    A = difference(get_A(g, V, w), get_A(g, V, v));
    iter = A.begin();
    int q = *iter; ++iter;
    int r = *iter;

    auto A_v = difference(difference(V, std::set<int>({v})), get_A(g, V, v));
    auto A_w = difference(difference(V, std::set<int>({w})), get_A(g, V, w));
    auto A_x = difference(difference(V, std::set<int>({x})), get_A(g, V, x));
    auto A_y = difference(difference(V, std::set<int>({y})), get_A(g, V, y));
    auto A_q = difference(difference(V, std::set<int>({q})), get_A(g, V, q));
    auto A_r = difference(difference(V, std::set<int>({r})), get_A(g, V, r));

    if (is_connected(g, x, y) && is_connected(g, q, r)) {
        int maxset1 = 2 + maxset(g, intersect(A_v, A_w));
        int maxset2 = maxset(g, difference(V, std::set<int>({w, v})));
        return std::max(maxset1, maxset2);
    }
    if (is_connected(g, x, y) && !is_connected(g, q, r)) {
        int maxset1 = 2 + maxset(g, intersect(A_v, A_w));
        int maxset2 = 3 + maxset(g, intersect(intersect(A_v, A_w), intersect(A_q, A_r)));
        int maxset3 = maxset(g, difference(V, std::set<int>({w, v})));
        return std::max({maxset1, maxset2, maxset3});
    }

    if (!is_connected(g, x, y) && is_connected(g, q, r)) {
        int maxset1 = 2 + maxset(g, intersect(A_v, A_w));
        int maxset2 = 3 + maxset(g, intersect(intersect(A_v, A_w), intersect(A_x, A_y)));
        int maxset3 = maxset(g, difference(V, std::set<int>({w, v})));
        return std::max({maxset1, maxset2, maxset3});
    }

    if (intersect(intersect(A_v, A_w), intersect(A_x, A_y)).size() >= V.size() - 9 ||
        intersect(intersect(A_v, A_w), intersect(A_r, A_q)).size() >= V.size() - 9 ) {
        int maxset1 = 3 + maxset(g, intersect(intersect(A_v, A_w), intersect(A_x, A_y)));
        int maxset2 = 3 + maxset(g, intersect(intersect(A_v, A_w), intersect(A_q, A_r)));
        int maxset3 = maxset(g, difference(V, std::set<int>({w, v})));
        return std::max({maxset1, maxset2, maxset3});
    } else {
        int maxset1 = 2 + maxset(g, intersect(A_v, A_w));
        int maxset2 = 3 + maxset(g, intersect(intersect(A_v, A_w), intersect(A_x, A_y)));
        int maxset3 = 3 + maxset(g, intersect(intersect(A_v, A_w), intersect(A_q, A_r)));
        int maxset4 = maxset(g, difference(V, std::set<int>({w, v})));
        return std::max({maxset1, maxset2, maxset3, maxset4});
    }

}

int statement_4_1_2_2(std::vector<std::set<int>>& g, const std::set<int>& V, int v, 
                    const std::set<int>& set1, const std::set<int>& set2, const std::set<int>& set3) {
    int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
    int maxset2 = 2 + maxset(g, set1);
    int maxset3 = 2 + maxset(g, set2);
    int maxset4 = 2 + maxset(g, set3);
    return std::max({maxset1, maxset2, maxset3, maxset4});
}

int statement_4_1_2_3(std::vector<std::set<int>>& g, const std::set<int>& V, int v, const std::set<int>& set1,
                     const std::set<int>& set2, const std::set<int>& set3, const std::set<int>& set4) {
    int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
    int maxset2 = 2 + maxset(g, set1);
    int maxset3 = 2 + maxset(g, set2);
    int maxset4 = 2 + maxset(g, set3);
    int maxset5 = 2 + maxset(g, set4);
    return std::max({maxset1, maxset2, maxset3, maxset4, maxset5});
}


int statement_4_1_2(std::vector<std::set<int>>& g, const std::set<int>& V, int v) {
    auto A = get_A(g, V, v);
    auto iter = A.begin();
    int w1 = *iter; ++iter; 
    int w2 = *iter; ++iter;
    int w3 = *iter; ++iter;
    int w4 = *iter;
    auto A1 = difference(difference(V, get_A(g, V, v)), get_A(g, V, w1));
    auto A2 = difference(difference(V, get_A(g, V, v)), get_A(g, V, w2));
    auto A3 = difference(difference(V, get_A(g, V, v)), get_A(g, V, w3));
    auto A4 = difference(difference(V, get_A(g, V, v)), get_A(g, V, w4));
    if ((is_connected(g, w1, w2) && is_connected(g, w1, w3) && is_connected(g, w1, w4)) || 
        (is_connected(g, w2, w1) && is_connected(g, w2, w3) && is_connected(g, w2, w4)) ||
        (is_connected(g, w3, w1) && is_connected(g, w3, w2) && is_connected(g, w3, w4)) ||
        (is_connected(g, w4, w1) && is_connected(g, w4, w2) && is_connected(g, w4, w3))) {
            return 1;
        }
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::vector<std::pair<int, int>> edges = {{w1, w2}, {w1, w3}, {w1, w4}, {w2, w3}, {w2, w4}, {w3, w4}};
    std::vector<std::set<int>> set_A = {intersect(A1, A2), intersect(A1, A3), intersect(A1, A4), intersect(A2, A3),
                                            intersect(A2, A4), intersect(A3, A4)};

    do {
        if (is_connected2(g, edges[numbers[0]]) && is_connected2(g, edges[numbers[1]]) && is_connected2(g, edges[numbers[2]]) &&
            !is_connected2(g, edges[numbers[3]]) && !is_connected2(g, edges[numbers[4]]) && !is_connected2(g, edges[numbers[5]])) {
            return statement_4_1_2_2(g, V, v, set_A[numbers[3]], set_A[numbers[4]], set_A[numbers[5]]);
        }
    } while (std::next_permutation(numbers.begin(), numbers.end()));

    do {
        if (is_connected2(g, edges[numbers[0]]) && is_connected2(g, edges[numbers[1]]) && !is_connected2(g, edges[numbers[2]]) &&
            !is_connected2(g, edges[numbers[3]]) && !is_connected2(g, edges[numbers[4]]) && !is_connected2(g, edges[numbers[5]])) {
            return statement_4_1_2_3(g, V, v, set_A[numbers[2]], set_A[numbers[3]], set_A[numbers[4]], set_A[numbers[5]]);
        }
    } while (std::next_permutation(numbers.begin(), numbers.end()));

    if (is_connected(g, w1, w2)) {
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
        int maxset2 = 2 + maxset(g, intersect(A1, A3));
        int maxset3 = 2 + maxset(g, intersect(A1, A4));
        int maxset4 = 2 + maxset(g, intersect(A2, A3));
        int maxset5 = 2 + maxset(g, intersect(A2, A4));
        int maxset6 = 2 + maxset(g, intersect(A3, A4));
        int maxset7 = 3 + maxset(g, intersect(A1, intersect(A3, A4)));
        int maxset8 = 3 + maxset(g, intersect(A2, intersect(A3, A4)));
        return std::max({maxset1, maxset2, maxset3, maxset4,
                        maxset5, maxset6, maxset7, maxset8});
    }
    if (is_connected(g, w1, w3)) {
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
        int maxset2 = 2 + maxset(g, intersect(A1, A2));
        int maxset3 = 2 + maxset(g, intersect(A1, A4));
        int maxset4 = 2 + maxset(g, intersect(A2, A3));
        int maxset5 = 2 + maxset(g, intersect(A2, A4));
        int maxset6 = 2 + maxset(g, intersect(A3, A4));
        int maxset7 = 3 + maxset(g, intersect(A1, intersect(A2, A4)));
        int maxset8 = 3 + maxset(g, intersect(A3, intersect(A2, A4)));
        return std::max({maxset1, maxset2, maxset3, maxset4,
                        maxset5, maxset6, maxset7, maxset8});
    }
    if (is_connected(g, w1, w4)) {
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
        int maxset2 = 2 + maxset(g, intersect(A1, A3));
        int maxset3 = 2 + maxset(g, intersect(A1, A2));
        int maxset4 = 2 + maxset(g, intersect(A2, A3));
        int maxset5 = 2 + maxset(g, intersect(A2, A4));
        int maxset6 = 2 + maxset(g, intersect(A3, A4));
        int maxset7 = 3 + maxset(g, intersect(A1, intersect(A2, A3)));
        int maxset8 = 3 + maxset(g, intersect(A4, intersect(A2, A3)));
        return std::max({maxset1, maxset2, maxset3, maxset4,
                        maxset5, maxset6, maxset7, maxset8});
    }
    if (is_connected(g, w2, w3)) {
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
        int maxset2 = 2 + maxset(g, intersect(A1, A3));
        int maxset3 = 2 + maxset(g, intersect(A1, A4));
        int maxset4 = 2 + maxset(g, intersect(A1, A2));
        int maxset5 = 2 + maxset(g, intersect(A2, A4));
        int maxset6 = 2 + maxset(g, intersect(A3, A4));
        int maxset7 = 3 + maxset(g, intersect(A2, intersect(A1, A4)));
        int maxset8 = 3 + maxset(g, intersect(A3, intersect(A1, A4)));
        return std::max({maxset1, maxset2, maxset3, maxset4,
                        maxset5, maxset6, maxset7, maxset8});
    }
    if (is_connected(g, w2, w4)) {
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
        int maxset2 = 2 + maxset(g, intersect(A1, A3));
        int maxset3 = 2 + maxset(g, intersect(A1, A4));
        int maxset4 = 2 + maxset(g, intersect(A2, A3));
        int maxset5 = 2 + maxset(g, intersect(A1, A2));
        int maxset6 = 2 + maxset(g, intersect(A3, A4));
        int maxset7 = 3 + maxset(g, intersect(A2, intersect(A1, A3)));
        int maxset8 = 3 + maxset(g, intersect(A4, intersect(A1, A3)));
        return std::max({maxset1, maxset2, maxset3, maxset4,
                        maxset5, maxset6, maxset7, maxset8});
    }
    if (is_connected(g, w3, w4)) {
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
        int maxset2 = 2 + maxset(g, intersect(A1, A3));
        int maxset3 = 2 + maxset(g, intersect(A1, A4));
        int maxset4 = 2 + maxset(g, intersect(A2, A3));
        int maxset5 = 2 + maxset(g, intersect(A2, A4));
        int maxset6 = 2 + maxset(g, intersect(A1, A2));
        int maxset7 = 3 + maxset(g, intersect(A3, intersect(A1, A2)));
        int maxset8 = 3 + maxset(g, intersect(A4, intersect(A1, A2)));
        return std::max({maxset1, maxset2, maxset3, maxset4,
                        maxset5, maxset6, maxset7, maxset8});
    }
    int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({v})), get_A(g, V, v))); 
    int maxset2 = 2 + maxset(g, intersect(A1, A3));
    int maxset3 = 2 + maxset(g, intersect(A1, A4));
    int maxset4 = 2 + maxset(g, intersect(A2, A3));
    int maxset5 = 2 + maxset(g, intersect(A2, A4));
    int maxset6 = 2 + maxset(g, intersect(A1, A2));
    int maxset7 = 2 + maxset(g, intersect(A3, A4));
    int maxset8 = 3 + maxset(g, intersect(A3, intersect(A1, A2)));
    int maxset9 = 3 + maxset(g, intersect(A4, intersect(A1, A2)));
    int maxset10 = 3 + maxset(g, intersect(A4, intersect(A1, A3)));
    int maxset11 = 3 + maxset(g, intersect(A4, intersect(A3, A2)));
    int maxset12 = 3 + maxset(g, intersect(intersect(A4, A1), intersect(A3, A2)));
    return std::max({maxset1, maxset2, maxset3, maxset4, maxset5, maxset6,
                     maxset7, maxset8, maxset9, maxset10, maxset11, maxset12});

}

int statement_4(std::vector<std::set<int>>& g, const std::set<int>& V, int v0) {
    auto p = get_max_degree(g, V);
    if (p.first == 4) {
        auto p = find_4_1(g, V);
        int v = p.first;
        int w = p.second;
        if (v != -1) {
            auto p1 = find_4_1_1_1(g, V);
            int v1 = p.first;
            int w1 = p.second;
            if (v1 != -1) {
                std::set<int> W = {v, w};
                int maxset1 = 2 + maxset(g, difference(difference(difference(V, W), get_A(g, V, v)), get_A(g, V, w)));
                int maxset2 = maxset(g, difference(V, W));
                return std::max(maxset1, maxset2);
            } else {
                return statement_4_1_1_2(g, V, v, w);
            }
        } else {
            return statement_4_1_2(g, V, v0);
        }
    } else {
        int w;
        for (auto u : g[v0]) {
            if (V.find(u) != V.end() && get_deg(g, V, u) >= 5) {
                w = u;
                break;
            }
        }
        int maxset1 = 1 + maxset(g, difference(difference(V, std::set<int>({w})), get_A(g, V, w)));
        int maxset2 = maxset(g, difference(V, std::set<int>({w})));
        return std::max(maxset1, maxset2);
    }

}



int maxset(std::vector<std::set<int>>& g, const std::set<int>& V) {
    auto p = get_min_degree(g, V);
    int d = p.first;
    int v = p.second;
    if (d == 1) {
        return statement_1(g, V, v);
    }
    if (d == 2) {
        return statement_2(g, V);
    }
    if (d == 3) {
        return statement_3(g, V, v);
    }
    if (d == 4) {
        return statement_4(g, V);
    }
}

int tarjan_trojanowski(std::vector<std::set<int>>& g) {
    std::set<int> v;
    for (int i = 0; i < g.size(); ++i) {
        v.insert(i);
    }
    return maxset(g, v);
}

int main() {




    return 0;
}