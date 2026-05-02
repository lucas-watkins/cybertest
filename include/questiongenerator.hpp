#ifndef CYBERTEST_QUESTIONGENERATOR_HPP
#define CYBERTEST_QUESTIONGENERATOR_HPP

#include <string>
#include <random>
#include <optional>
#include <OpenXLSX.hpp>

class QuestionGenerator {
    inline static std::random_device rd{};
    inline static std::seed_seq seed_seq{rd(), rd(), rd(), rd(), rd()};
    inline static std::mt19937 mt_engine{seed_seq};

    std::string xl_file;

public:
    struct Question {
        std::string question;
        std::string correct_answer;
        std::string answer_a;
        std::string answer_b;
        std::string answer_c;
        std::string answer_d;

        void shuffle();
    };

    QuestionGenerator(std::string file) : xl_file{std::move(file)} {};

    ~QuestionGenerator() = default;

    std::optional<Question> next() const;
};

#endif //CYBERTEST_QUESTIONGENERATOR_HPP

