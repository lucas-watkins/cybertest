//
// Created by lucas on 4/23/26.
//

#ifndef CYBERTEST_VALIDATION_HPP
#define CYBERTEST_VALIDATION_HPP

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

class Validation {
public:
    static std::optional<int> parse_int(const std::string&);

    template <typename T, typename U>
    static bool vector_contains(const std::vector<T>& vec, const U& val) {
        return std::find(vec.begin(), vec.end(), val) != vec.end();
    }
};

#endif //CYBERTEST_VALIDATION_HPP