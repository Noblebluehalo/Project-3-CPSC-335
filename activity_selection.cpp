// CPSC 335 - Project 3
// Algorithm 2: Activity Selection Problem
// Author: Joshua Zamora
// Email: JoshuaMZamora@csu.fullerton.edu

#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Activity = std::pair<int, int>; // (start, finish)

// Stable insertion sort 
static void insertion_sort_by_finish(std::vector<Activity>& a) {
    for (size_t i = 1; i < a.size(); ++i) {
        Activity key = a[i];
        size_t j = i;
        // while a[j-1] > key 
        while (j > 0) {
            bool greater = false;
            if (a[j - 1].second > key.second) greater = true;
            else if (a[j - 1].second == key.second && a[j - 1].first > key.first) greater = true;

            if (!greater) break;
            a[j] = a[j - 1];
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
        if (s[i] == '-' && !innum) {
            neg = true;
            innum = true;
            num = 0;
        }
        else if (std::isdigit(uc)) {
            if (!innum) {
                innum = true;
                num = 0;
                neg = false;
            }
            num = num * 10 + (s[i] - '0');
        }
        else {
            if (innum) {
                nums.push_back(neg ? static_cast<int>(-num) : static_cast<int>(num));
                innum = false;
                neg = false;
            }
        }
    }
    if (innum) nums.push_back(neg ? static_cast<int>(-num) : static_cast<int>(num));

    std::vector<Activity> acts;
    for (size_t i = 0; i + 1 < nums.size(); i += 2)
        acts.emplace_back(nums[i], nums[i + 1]);
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

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string a;

    std::cout << "Enter activities as pairs of start and finish times.\n";
    std::cout << "Examples:";
    std::cout << "(x,y) (x,y) (x,y)\n";
    std::cout << "Input: ";

    std::getline(std::cin, a);

    if (a.empty()) {
        std::cerr << "No activities entered. Exiting.\n";
        return 1;
    }

    std::vector<Activity> acts = parse_activities(a);

    if (acts.empty()) {
        std::cerr << "Could not parse any activities from input. Exiting.\n";
        return 1;
    }

    std::vector<Activity> sel = activity_selection(acts);


    std::cout << "\nOutput: ";
    for (size_t i = 0; i < sel.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << '(' << sel[i].first << ',' << sel[i].second << ')';
    }
    std::cout << "\n";

    return 0;
}

