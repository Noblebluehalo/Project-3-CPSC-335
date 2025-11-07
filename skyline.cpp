// CPSC 335 - Project 3
// Algorithm 1: Skyline Visibility Problem
// Author: Joshua Zamora
// Email: JoshuaMZamora@csu.fullerton.edu

#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using List = std::vector<int>;

static std::pair<List, int> visible_left_dc(const std::vector<int>& h, int lo, int hi) {
    if (hi - lo == 0) return {List{}, std::numeric_limits<int>::min()};
    if (hi - lo == 1) return {List{lo}, h[lo]};
    int mid = (lo + hi) / 2;

    auto L = visible_left_dc(h, lo, mid);
    auto R = visible_left_dc(h, mid, hi);

    const List& Llist = L.first;
    const List& Rlist = R.first;
    int Lmax = L.second;
    int Rmax = R.second;

    // filter right record-highs by left max
    List filtered_right;
    filtered_right.reserve(Rlist.size());
    for (size_t i = 0; i < Rlist.size(); ++i) {
        int idx = Rlist[i];
        if (h[idx] > Lmax) filtered_right.push_back(idx);
    }
  // merge (append)
    List merged;
    merged.reserve(Llist.size() + filtered_right.size());
    for (size_t i = 0; i < Llist.size(); ++i) merged.push_back(Llist[i]);
    for (size_t i = 0; i < filtered_right.size(); ++i) merged.push_back(filtered_right[i]);

    // manual max
    int segmax = (Lmax > Rmax) ? Lmax : Rmax;
    return {merged, segmax};
}

static List visible_from_left(const std::vector<int>& h) {
    return visible_left_dc(h, 0, static_cast<int>(h.size())).first;
}

static List visible_from_right(const std::vector<int>& h) {
    int n = static_cast<int>(h.size());
    // reverse manually
    std::vector<int> rev; rev.resize(n);
    for (int i = 0; i < n; ++i) rev[i] = h[n - 1 - i];

    List rev_ans = visible_left_dc(rev, 0, n).first;

    // map back
    List mapped; mapped.reserve(rev_ans.size());
    for (size_t k = 0; k < rev_ans.size(); ++k) mapped.push_back(n - 1 - rev_ans[k]);
    return mapped;
}

static std::vector<int> parse_heights(const std::string& s) {
    std::vector<int> a;
    long long num = 0;
    bool innum = false, neg = false;
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char uc = static_cast<unsigned char>(s[i]);
        if (s[i] == '-' && !innum) { neg = true; innum = true; num = 0; }
        else if (std::isdigit(uc)) {
            if (!innum) { innum = true; num = 0; neg = false; }
            num = num * 10 + (s[i] - '0');
        } else {
            if (innum) { a.push_back(neg ? static_cast<int>(-num) : static_cast<int>(num)); innum = false; neg = false; }
        }
    }
    if (innum) a.push_back(neg ? static_cast<int>(-num) : static_cast<int>(num));
    return a;
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string heights = "3,7,8,3,6,1";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--heights" && i + 1 < argc) heights = argv[++i];
    }

    std::vector<int> h = parse_heights(heights);
    List L = visible_from_left(h);
    List R = visible_from_right(h);

    std::cout << "Input heights: ";
    for (size_t i = 0; i < h.size(); ++i) { if (i) std::cout << ' '; std::cout << h[i]; }
    std::cout << "\nVisible from left (indices): ";
    for (size_t i = 0; i < L.size(); ++i) { if (i) std::cout << ' '; std::cout << L[i]; }
    std::cout << "\nVisible from right (indices): ";
    for (size_t i = 0; i < R.size(); ++i) { if (i) std::cout << ' '; std::cout << R[i]; }
    std::cout << "\n";
    return 0;
}
