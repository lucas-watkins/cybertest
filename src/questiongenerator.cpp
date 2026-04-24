//
// Created by lucas on 4/24/26.
//

#include <questiongenerator.hpp>
#include <cstdint>
#include <random>
#include <ranges>

std::optional<QuestionGenerator::Question> QuestionGenerator::next() const {
    try {
        OpenXLSX::XLWorkbook workbook {xl_doc.workbook()};

        const std::uint16_t sheets{static_cast<std::uint16_t>(workbook.worksheetCount())};
        std::uniform_int_distribution<std::uint16_t> sheets_possible{1U, sheets};

        const OpenXLSX::XLWorksheet sheet {workbook.worksheet(sheets_possible(mt_engine))};

        OpenXLSX::XLColumn first_col{sheet.column(1)};

        std::uint32_t question_cell{0};

        for (std::uint32_t i{1}; i < sheet.rowCount(); ++i) {
            if (auto cell{sheet.cell(i, 1)}; cell.getString().empty()) {
                question_cell = i - 1;
                break;
            }
        }

        std::uint16_t col_start{0};
        for (std::uint16_t i{1}; i < sheet.columnCount(); ++i) {
            if (auto cell{sheet.cell(1, i)}; cell.getString() == "Question") {
                col_start = i;
                break;
            }
        }

        std::uniform_int_distribution rows_possible{2U, question_cell};

        const std::uint32_t row{rows_possible(mt_engine)};

        return Question {
            sheet.cell(row, col_start).getString(),
            sheet.cell(row, col_start + 1).getString(),
            sheet.cell(row, col_start + 1).getString(),
            sheet.cell(row, col_start + 2).getString(),
            sheet.cell(row, col_start + 3).getString(),
            sheet.cell(row, col_start + 4).getString(),
        };

    } catch (const OpenXLSX::XLException& ex) {
        std::cerr << "XLException: " << ex.what() << '\n';
        return std::nullopt;
    }
}

void QuestionGenerator::Question::shuffle() {
    std::vector answers{answer_a, answer_b, answer_c, answer_d};
    std::ranges::shuffle(answers, mt_engine);

    answer_a = answers[0];
    answer_b = answers[1];
    answer_c = answers[2];
    answer_d = answers[3];

    for (std::size_t i{0}; i < answers.size(); ++i) {
        if (answers[i] == correct_answer) {
            switch (i) {
                case 0:
                correct_answer = "answerA";
                break;

                case 1:
                correct_answer = "answerB";
                break;

                case 2:
                correct_answer = "answerC";
                break;

                case 3:
                correct_answer = "answerD";
                break;

                default:
                throw std::runtime_error("Shuffling answer choices failed");
            }
        }
    }
}
