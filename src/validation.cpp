//
// Created by lucas on 4/23/26.
//

#include "../include/validation.hpp"

#include <stdexcept>

std::optional<int> Validation::parse_int(const std::string& s) {
    if (s.find_first_not_of("0123456789+-") == std::string::npos) {
        try {
            return std::stoi(s);
        } catch (const std::out_of_range& ex) {
            return std::nullopt;
        }
    }

    return std::nullopt;
}
