#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

//std::vector<std::set<int>>& g


bool is_connected(std::vector<std::set<int>>& g, int u, int v) {
    return g[u].find(v) != g[u].end();
}


std::set<int> get_set(std::vector<int>& mask) {
    std::set<int> ans;
    for (int i = 0; i < mask.size(); ++i) {
        if (mask[i]) {
            ans.insert(i);
        }
    }
    return ans;
}

bool is_clique(std::vector<std::set<int>>& g, std::set<int>& s) {
    for (auto u : s) {
        for (auto w : s) {
            if (u != w && !is_connected(g, u, w)) {
                return false;
            }
        }
    }
    return true;
}

int naive_algorithm(std::vector<std::set<int>>& g) {
    int ans = 1;
    for (int kol = 2; kol <= g.size(); ++kol) {
        std::vector<int> mask(g.size(), 0);
        for (int i = 0; i < kol; ++i) {
            mask[i] = 1;
        }
        bool flag = false;
        do {
            auto pot_clique = get_set(mask);
            if (is_clique(g, pot_clique)) {
                ans = kol;
                flag = true;
                continue;
            }
        } while (std::next_permutation(mask.begin(), mask.end()));
        if (!flag) {
            break;
        }
    }
    return ans;
}

int main() {




    return 0;
}