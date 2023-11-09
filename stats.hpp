#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

class Statistics {
private:
    std::vector<std::pair<std::string, int>> counters;

    auto findCounter(const std::string& key) {
        return std::find_if(counters.begin(), counters.end(),
                            [&key](const auto& pair) { return pair.first == key; });
    }

public:
    void Increment(const std::string& key) {
        auto it = findCounter(key);
        if (it != counters.end()) {
            it->second++;
        } else {
            counters.emplace_back(key, 1);
        }
    }

    void AddMany(const std::string& key, const int val) {
        auto it = this->findCounter(key);
        if (it != this->counters.end()) {
            it->second += val;
        } else {
            this->counters.emplace_back(key, val);
        }
    }

    int GetCount(const std::string& key) {
        auto it = findCounter(key);
        return it != counters.end() ? it->second : 0;
    }

    void SetCount(const std::string& key, const int val) {
        auto it = findCounter(key);
        assert(it == counters.end());
        this->counters.emplace_back(key, val);
    }
};
