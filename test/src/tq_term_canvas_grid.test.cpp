#include <string>

#include "tq_term_canvas_grid.h"

SCENARIO_START

DESCRIBE_ONLY("Canvas", {
	DESCRIBE("Grid", {
		using CC_CHAR = termiq::canvas::CharCell<char>;
		using CC = termiq::canvas::CharCell<wchar_t>;
		termiq::canvas::Grid<CC>* grid;

		DESCRIBE("a 2x2 instance of char grid has been created", {
			termiq::canvas::Grid<CC_CHAR>* char_grid;
			BEFORE_EACH({
				char_grid = new termiq::canvas::Grid<CC_CHAR>(2,2);
			});
			AFTER_EACH({
				delete char_grid;
			});

			IT("should render ascii char borders", {
				char_grid->set_width(5);
				char_grid->set_height(5);
				auto result = pieces_to_text(char_grid->build());

				EXPECT(result).toBe({
					{"lqwqk", "x x x", "tqnqu", "x x x", "mqvqj"}
				});
			});
		});

		DESCRIBE("an 2x3 instace of wchar_t has been created", {
			BEFORE_EACH({
				grid = new termiq::canvas::Grid<CC>(2,3);
				grid->set_border_type(termiq::canvas::BorderType::SINGLE);
			});
			AFTER_EACH({
				delete grid;
			});

			IT("should render empty grid", {
				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({
					{L"┌┬┬┐",
					 L"├┼┼┤",
					 L"└┴┴┘"}
				});
			});

			IT("should render grid with texts", {
				termiq::canvas::Text<CC> t1(L"Hello");
				termiq::canvas::Text<CC> t2(L"World");
				grid->select_cell(1);
				grid->set_cell_content(&t1);
				grid->set_cell_width(3);
				grid->set_cell_height(1);
				grid->select_cell(1,2);
				grid->set_cell_content(&t2);
				grid->set_cell_width(2);
				grid->set_cell_height(2);
				grid->set_width(4);
				grid->set_height(7);
				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"┌┬───┬──┐",
					 L"││Hel│  │",
					 L"││   │  │",
					 L"├┼───┼──┤",
					 L"││   │Wo│",
					 L"││   │  │",
					 L"└┴───┴──┘",
				 }});
			});

			IT("should distribute space correctly", {
				termiq::canvas::Text<CC> t1(L"Hello");
				grid->select_cell(1);
				grid->set_cell_content(&t1);
				grid->set_cell_width(5);
				grid->set_cell_height(1);
				grid->set_width(15);
				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"┌───┬─────┬───┐",
					 L"│   │Hello│   │",
					 L"├───┼─────┼───┤",
					 L"└───┴─────┴───┘",
				 }});
			});
		});

		DESCRIBE("an 2x2 instance of wchar_t with text has been created", {
			BEFORE_EACH({
				grid = new termiq::canvas::Grid<CC>(2,2);
				// TODO: add text
			});
			AFTER_EACH({
				delete grid;
			});

			IT("should display invisible border", {
				// TODO
			});

			IT("should not display border", {
				// TODO
			});

			IT("should display double border", {
				// TODO
			});

			IT("should display bold border", {
				// TODO
			});

			IT("should display rounded border", {
				// TODO
			});
		});

		DESCRIBE("an 1x1 instance of wchar_t with text has been created", {
			BEFORE_EACH({
				// TODO
			});
			AFTER_EACH({
				// TODO
			});

			IT("should not be stylized", {
				// TODO
			});

			IT("should style border color", {
				// TODO
			});

			IT("should style border background color", {
				// TODO
			});

			IT("should style cell background color", {
				// TODO
			});

			IT("should style background color", {
				// TODO
			});
		});
	});
});

SCENARIO_END
