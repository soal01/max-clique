#include <iostream>
#include <vector>
#include <set>

bool check_wrong(std::vector<std::set<int>>& g, std::set<int>& candidates, std::set<int>& wrong) {
    for (auto v : wrong) {
        bool flag = false;
        for (auto u : candidates) {
            if (g[v].find(u) == g[v].end()) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return true;
        }

    }
    return false;
}


void extend(std::vector<std::set<int>>& g, std::set<int>& compsub, std::set<int>& candidates,
             std::set<int>& wrong, std::vector<int>& res) {
    while (!candidates.empty() && check_wrong(g, candidates, wrong)) {
        int v = *candidates.begin();
        compsub.insert(v);
        std::set<int> new_candidates, new_wrong;
        for (auto u : candidates) {
            if (g[u].find(v) != g[u].end()) {
                new_candidates.insert(u);
            }
        }
        for (auto u : wrong) {
            if (g[u].find(v) != g[u].end()) {
                new_wrong.insert(u);
            }
        }
        if (new_candidates.empty() && new_wrong.empty()) {
            // compsub - клика
            if (compsub.size() > res.size()) {
                res = std::vector<int>(compsub.begin(), compsub.end());
            }
            return;
        } else {
            extend(g, compsub, new_candidates, new_wrong, res);
        }
        compsub.erase(v);
        candidates.erase(v);
        wrong.insert(v);
    }
}

std::vector<int> bron_kerbosch(std::vector<std::set<int>>& g) {
    std::vector<int> ans;
    std::set<int> candidates, compsub, wrong;
    for (int i = 0; i < g.size(); ++i) {
        candidates.insert(i);
    }
    extend(g, compsub, candidates, wrong, ans);
    return ans;
}



int main() {




    return 0;
}