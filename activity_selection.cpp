// CPSC 335 - Project 3
// Algorithm 2: Activity Selection Problem
// Author: Joshua Zamora
// Email: JoshuaMZamora@csu.fullerton.edu
// activity_selection.cpp
// Greedy Activity Selection with a hand-written stable insertion sort by finish time.
// No <algorithm> used.
//
// Build: g++ -std=c++17 -O2 -o activity_selection activity_selection.cpp
// Run:   ./activity_selection --activities "[(1,3),(2,5),(4,6),(6,7),(5,9),(8,9)]"

#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Activity = std::pair<int,int>; // (start, finish)

// Stable insertion sort by (finish asc, then start asc)
static void insertion_sort_by_finish(std::vector<Activity>& a) {
    for (size_t i = 1; i < a.size(); ++i) {
        Activity key = a[i];
        size_t j = i;
        // while a[j-1] > key (by finish, then start)
        while (j > 0) {
            bool greater = false;
            if (a[j-1].second > key.second) greater = true;
            else if (a[j-1].second == key.second && a[j-1].first > key.first) greater = true;

            if (!greater) break;
            a[j] = a[j-1];
            --j;
        }
        a[j] = key;
    }
}

static std::vector<Activity> parse_activities(const std::string& s) {
    std::vector<int> nums;
    long long num = 0;
    bool innum = false, neg = false;
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char uc = static_cast<unsigned char>(s[i]);
        if (s[i] == '-' && !innum) { neg = true; innum = true; num = 0; }
        else if (std::isdigit(uc)) {
            if (!innum) { innum = true; num = 0; neg = false; }
            num = num * 10 + (s[i] - '0');
        } else {
            if (innum) { nums.push_back(neg ? static_cast<int>(-num) : static_cast<int>(num)); innum = false; neg = false; }
        }
    }
    if (innum) nums.push_back(neg ? static_cast<int>(-num) : static_cast<int>(num));

    std::vector<Activity> acts;
    for (size_t i = 0; i + 1 < nums.size(); i += 2) acts.emplace_back(nums[i], nums[i+1]);
    return acts;
}

static std::vector<Activity> activity_selection(std::vector<Activity> acts) {
    // sort by earliest finish (stable)
    insertion_sort_by_finish(acts);

    std::vector<Activity> result;
    bool first = true;
    int last_finish = 0; // unused before first pick

    for (size_t i = 0; i < acts.size(); ++i) {
        int s = acts[i].first, f = acts[i].second;
        if (first || s >= last_finish) {
            result.push_back(acts[i]);
            last_finish = f;
            first = false;
        }
    }
    return result;
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string a = "[(1,3),(2,5),(4,6),(6,7),(5,9),(8,9)]";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--activities" && i + 1 < argc) a = argv[++i];
    }

    std::vector<Activity> acts = parse_activities(a);
    std::vector<Activity> sel = activity_selection(acts);

    std::cout << "Input activities: ";
    for (size_t i = 0; i < acts.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << '(' << acts[i].first << ',' << acts[i].second << ')';
    }
    std::cout << "\nSelected (max non-overlapping): ";
    for (size_t i = 0; i < sel.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << '(' << sel[i].first << ',' << sel[i].second << ')';
    }
    std::cout << "\n";
    return 0;
}
