//
// Created by lucas on 4/24/26.
//

#include <questiongenerator.hpp>
#include <cstdint>
#include <random>

std::optional<QuestionGenerator::Question> QuestionGenerator::next() const {
    try {
        OpenXLSX::XLWorkbook workbook {xl_doc.workbook()};

        const std::uint16_t sheets{static_cast<std::uint16_t>(workbook.worksheetCount())};
        std::uniform_int_distribution<std::uint16_t> sheets_possible{1U, sheets};

        OpenXLSX::XLWorksheet sheet {workbook.worksheet(sheets_possible(mt_engine))};

        OpenXLSX::XLColumn first_col{sheet.column(1)};

        std::uint32_t question_cell{0};

        for (std::uint32_t i{1}; i < sheet.rowCount(); ++i) {
            if (auto cell{sheet.cell(i, 1)}; cell.getString().empty()) {
                question_cell = i - 1;
                break;
            }
        }

        std::uniform_int_distribution rows_possible{2U, question_cell};

        const std::uint32_t row{rows_possible(mt_engine)};

        return Question {
            sheet.cell(row, 4).getString(),
            sheet.cell(row, 5).getString(),
            sheet.cell(row, 5).getString(),
            sheet.cell(row, 6).getString(),
            sheet.cell(row, 7).getString(),
            sheet.cell(row, 8).getString(),
        };

    } catch (const OpenXLSX::XLException& ex) {
        std::cerr << "XLException: " << ex.what() << '\n';
        return std::nullopt;
    }
}
