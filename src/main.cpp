#include <iostream>
#include <OpenXLSX.hpp>
#include <crow.h>
#include <optional>
#include <validation.hpp>
#include <string_view>
#include <filesystem>
#include <questiongenerator.hpp>

using namespace std::string_literals;
using Question = QuestionGenerator::Question;

int main() {
    crow::SimpleApp app{};

    // adjust template directory globally to match repository
    crow::mustache::set_global_base("web/templates");

    if (!std::filesystem::exists("web/apcyberquestions.xlsx")) {
        std::cerr << "Failure to open web/apcyberquestions.xlsx\n";
        return 0;
    }

    const QuestionGenerator question_gen{"web/apcyberquestions.xlsx"};

    CROW_ROUTE(app, "/")([](const crow::request& req, crow::response& res) {
        res.set_static_file_info("web/index.html");
        res.end();
    });

    CROW_ROUTE(app, "/question").methods(crow::HTTPMethod::POST)([&question_gen](const crow::request& req) {
        const crow::query_string params{req.get_body_params()};
        const auto& keys{params.keys()};

        if (!Validation::vector_contains(keys, "correct")
            || !Validation::vector_contains(keys, "incorrect")
            || !Validation::vector_contains(keys, "questionNum")) {

            return crow::mustache::compile("Malformed").render();
        }

        const std::optional question_num{Validation::parse_int(params.get("questionNum"))};
        const std::optional incorrect{Validation::parse_int(params.get("incorrect"))};
        const std::optional correct{Validation::parse_int(params.get("correct"))};

        if (!question_num || !incorrect || !correct || *question_num < 0 || *incorrect < 0 || *correct < 0) {
            return crow::mustache::compile("Malformed").render();
        }

        std::optional question{question_gen.next()};

        if (!question) {
            return crow::mustache::compile("Server Error: Failure to get question...").render();
        }

        question->shuffle();

        crow::mustache::context ctx{};
        ctx["questionNum"] = *question_num;
        ctx["incorrect"] = *incorrect;
        ctx["correct"] = *correct;
        ctx["question"] = question->question;
        ctx["answerA"] = question->answer_a;
        ctx["answerB"] = question->answer_b;
        ctx["answerC"] = question->answer_c;
        ctx["answerD"] = question->answer_d;
        ctx["correctAnswer"] = question->correct_answer;

        const crow::mustache::template_t page{crow::mustache::load("question.html")};

        return page.render(ctx);
    });

    app.bindaddr("0.0.0.0");
    app.port(4576);
    auto _{app.run_async()};

    return 0;
}
