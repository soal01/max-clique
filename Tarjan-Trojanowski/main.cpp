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


int statement_3_4(std::vector<std::set<int>>& g, const std::set<int>& V, int v) {

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
    return statement_3_4(g, V, v);
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