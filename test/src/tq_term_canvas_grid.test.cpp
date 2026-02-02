#include "qtest.hpp"

#include <string>
#include <vector>
#include <memory>

#include "tq_term_canvas_text.h"
#include "tq_term_canvas_grid.h"
#include "tq_term_style.h"
#include "test/src/helpers.h"

SCENARIO_START

DESCRIBE("Canvas", {
	DESCRIBE("Grid", {
		using CC_CHAR = termiq::canvas::CharCell<termiq::canvas::CharType, termiq::canvas::CharState>;
		using CC = termiq::canvas::CharCell<termiq::canvas::WCharType, termiq::canvas::CharState>;
		termiq::canvas::Grid<CC>* grid;

		DESCRIBE_SKIP("a 2x2 instance of char grid has been created", {
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
				auto result = pieces_to_text<CC_CHAR, char>(char_grid->build());

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
				auto result = pieces_to_text<CC, char16_t>(grid->build());

				EXPECT(result).toBe({
					{u"┌┬┬┐",
					 u"├┼┼┤",
					 u"└┴┴┘"}
				});
			});

			IT("should render grid with texts", {
				termiq::canvas::Text<CC> t1(build_text<CC,char16_t>(u"Hello"));
				termiq::canvas::Text<CC> t2(build_text<CC,char16_t>(u"World"));
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
				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"┌┬───┬──┐",
					 u"││Hel│  │",
					 u"││   │  │",
					 u"├┼───┼──┤",
					 u"││   │Wo│",
					 u"││   │  │",
					 u"└┴───┴──┘",
				 }});
			});

			IT("should distribute space correctly", {
				termiq::canvas::Text<CC> t1(build_text<CC,char16_t>(u"Hello"));
				grid->select_cell(1);
				grid->set_cell_content(&t1);
				grid->set_cell_width(5);
				grid->set_cell_height(1);
				grid->set_width(15);
				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"┌───┬─────┬───┐",
					 u"│   │Hello│   │",
					 u"├───┼─────┼───┤",
					 u"└───┴─────┴───┘",
				 }});
			});
		});

		DESCRIBE("an 2x2 instance of wchar_t with text has been created", {
			termiq::canvas::Text<CC>* txt;
			BEFORE_EACH({
				grid = new termiq::canvas::Grid<CC>(2,2);
				txt = new termiq::canvas::Text<CC>(build_text<CC,char16_t>(u"Hello W!"));
				grid->select_cell(0,0);
				grid->set_cell_content(txt);
				grid->set_cell_width(10);
				grid->set_cell_height(1);
				grid->set_width(15);
			});
			AFTER_EACH({
				delete grid;
				delete txt;
			});

			IT("should display empty border", {
				grid->set_border_type(termiq::canvas::BorderType::EMPTY);

				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"               ",
					 u" Hello W!      ",
					 u"               ",
					 u"               ",
				 }});
			});

			IT("should not display border", {
				grid->set_border_type(termiq::canvas::BorderType::NONE);

				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"Hello W!       ",
				 }});
			});

			IT("should display double border", {
				grid->set_border_type(termiq::canvas::BorderType::DOUBLE);

				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"╔══════════╦══╗",
					 u"║Hello W!  ║  ║",
					 u"╠══════════╬══╣",
					 u"╚══════════╩══╝",
				 }});
			});

			IT("should display bold border", {
				grid->set_border_type(termiq::canvas::BorderType::BOLD);

				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"┏━━━━━━━━━━┳━━┓",
					 u"┃Hello W!  ┃  ┃",
					 u"┣━━━━━━━━━━╋━━┫",
					 u"┗━━━━━━━━━━┻━━┛",
				 }});
			});

			IT("should display rounded border", {
				grid->set_border_type(termiq::canvas::BorderType::ROUND);

				auto result = pieces_to_text<CC,char16_t>(grid->build());

				EXPECT(result).toBe({{
					 u"╭──────────┬──╮",
					 u"│Hello W!  │  │",
					 u"├──────────┼──┤",
					 u"╰──────────┴──╯",
				 }});
			});
		});

		DESCRIBE("an 1x1 instance of wchar_t with text has been created", {
			termiq::canvas::Text<CC>* txt;
			BEFORE_EACH({
				grid = new termiq::canvas::Grid<CC>(1,1);
				txt = new termiq::canvas::Text<CC>(build_text<CC,char16_t>(u"¯\\_(ツ)_/¯"));
				grid->select_cell(0,0);
				grid->set_border_type(termiq::canvas::BorderType::SINGLE);
				grid->set_cell_content(txt);
				grid->set_cell_width(9);
				grid->set_cell_height(1);
			});
			AFTER_EACH({
				delete grid;
				delete txt;
			});

			IT("should share style state among the text and borders", {
				auto state = pieces_to_grid(grid->build());
				std::vector<termiq::canvas::CharState*> borders;
				std::vector<termiq::canvas::CharState*> text;

				for (size_t i=0;i<state[0].size();i++) {
					borders.push_back(&state[0][i].state);
					borders.push_back(&state[2][i].state);
				}
				borders.push_back(&state[1][0].state);
				borders.push_back(&state[1][state[0].size()-1].state);
				for (size_t i=1;i<state[0].size()-1;i++) {
					text.push_back(&state[1][i].state);
				}

				auto b_state = borders[0];
				auto t_state = text[0];

				for (auto &b : borders) {
					EXPECT(*b).toBe(*b_state);
				}
				for (auto &t : text) {
					EXPECT(*t).toBe(*t_state);
				}
				EXPECT(*b_state).toBe(*t_state);
			});

			IT("should not be stylized", {
				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto expected = termiq::canvas::CharState({
					termiq::Color::NONE,
					termiq::Color::NONE,
					false, false, false, false, false
				});

				EXPECT(b_state).toBe(expected);
				EXPECT(t_state).toBe(expected);
			});

			IT("should style border color", {
				grid->set_border_foreground_color(termiq::Color{100, 100, 100});

				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto e_text = termiq::canvas::CharState({});
				auto e_border = termiq::canvas::CharState({
					termiq::Color{100, 100, 100}
				});

				EXPECT(t_state).toBe(e_text);
				EXPECT(b_state).toBe(e_border);
			});

			IT("should style border background color", {
				grid->set_border_background_color(termiq::Color{100, 100, 100});

				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto e_text = termiq::canvas::CharState({});
				auto e_border = termiq::canvas::CharState({
					termiq::Color::NONE,
					termiq::Color{100, 100, 100},
				});

				EXPECT(t_state).toBe(e_text);
				EXPECT(b_state).toBe(e_border);
			});

			IT("should style cell background color", {
				grid->set_background_color(termiq::Color{100, 100, 100});

				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto e_text = termiq::canvas::CharState({
					termiq::Color::NONE,
					termiq::Color{100, 100, 100}
				});
				auto e_border = termiq::canvas::CharState({});

				EXPECT(t_state).toBe(e_text);
				EXPECT(b_state).toBe(e_border);
			});
		});
	});
});

SCENARIO_END
