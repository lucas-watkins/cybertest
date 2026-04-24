#include <iostream>
#include <OpenXLSX.hpp>
#include <crow.h>
#include <optional>
#include <validation.hpp>
#include <string_view>

using namespace std::string_literals;

int main() {
    crow::SimpleApp app{};

    // adjust template directory globally to match repository
    crow::mustache::set_global_base("web/templates");

    CROW_ROUTE(app, "/")([](const crow::request& req, crow::response& res) {
        res.set_static_file_info("web/index.html");
        res.end();
    });

    CROW_ROUTE(app, "/question").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
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

        crow::mustache::context ctx{};
        ctx["questionNum"] = *question_num;
        ctx["incorrect"] = *incorrect;
        ctx["correct"] = *correct;
        ctx["question"] = "Example Question";
        ctx["answerA"] = "Answer Choice A";
        ctx["answerB"] = "Answer Choice B";
        ctx["answerC"] = "Answer Choice C";
        ctx["answerD"] = "Answer Choice D";
        ctx["correctAnswer"] = "answerA";
        ctx["answerExplanation"] = "Answer Explanation";

        const crow::mustache::template_t page{crow::mustache::load("question.html")};

        return page.render(ctx);
    });

    app.bindaddr("0.0.0.0");
    app.port(4576);
    auto _{app.run_async()};

    return 0;
}
