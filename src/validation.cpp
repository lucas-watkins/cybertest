//
// Created by lucas on 4/23/26.
//

#include "../include/validation.hpp"

#include <stdexcept>
#include <iostream>

std::optional<int> Validation::parse_int(const std::string& s) {
    if (s.find_first_not_of("0123456789+-") == std::string::npos) {
        try {
            return std::stoi(s);
        } catch (const std::exception& ex) {
            std::cerr << "Invalid Number: " << ex.what() << '\n';
            return std::nullopt;
        }
    }

    return std::nullopt;
}
