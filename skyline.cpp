// skyline.cpp

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
