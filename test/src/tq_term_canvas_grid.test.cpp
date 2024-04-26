#include <string>
#include <vector>
#include <memory>

#include "tq_term_canvas_grid.h"
#include "tq_term_style.h"

SCENARIO_START

DESCRIBE("Canvas", {
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
			termiq::canvas::Text<CC>* txt;
			BEFORE_EACH({
				grid = new termiq::canvas::Grid<CC>(2,2);
				txt = new termiq::canvas::Text<CC>(L"Hello W!");
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

			IT("should display invisible border", {
				grid->set_border_type(termiq::canvas::BorderType::INVISIBLE);

				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"               ",
					 L" Hello W!      ",
					 L"               ",
					 L"               ",
				 }});
			});

			IT("should not display border", {
				grid->set_border_type(termiq::canvas::BorderType::NONE);

				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"Hello W!       ",
				 }});
			});

			IT("should display double border", {
				grid->set_border_type(termiq::canvas::BorderType::DOUBLE);

				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"╔══════════╦══╗",
					 L"║Hello W!  ║  ║",
					 L"╠══════════╬══╣",
					 L"╚══════════╩══╝",
				 }});
			});

			IT("should display bold border", {
				grid->set_border_type(termiq::canvas::BorderType::BOLD);

				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"┏━━━━━━━━━━┳━━┓",
					 L"┃Hello W!  ┃  ┃",
					 L"┣━━━━━━━━━━╋━━┫",
					 L"┗━━━━━━━━━━┻━━┛",
				 }});
			});

			IT("should display rounded border", {
				grid->set_border_type(termiq::canvas::BorderType::ROUND);

				auto result = pieces_to_text(grid->build());

				EXPECT(result).toBe({{
					 L"╭──────────┬──╮",
					 L"│Hello W!  │  │",
					 L"├──────────┼──┤",
					 L"╰──────────┴──╯",
				 }});
			});
		});

		DESCRIBE("an 1x1 instance of wchar_t with text has been created", {
			termiq::canvas::Text<CC>* txt;
			BEFORE_EACH({
				grid = new termiq::canvas::Grid<CC>(1,1);
				txt = new termiq::canvas::Text<CC>(L"¯\\_(ツ)_/¯");
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
				std::vector<std::shared_ptr<termiq::canvas::CharState>> borders;
				std::vector<std::shared_ptr<termiq::canvas::CharState>> text;

				for (size_t i=0;i<state[0].size();i++) {
					borders.push_back(state[0][i].state);
					borders.push_back(state[2][i].state);
				}
				borders.push_back(state[1][0].state);
				borders.push_back(state[1][state[0].size()-1].state);
				for (size_t i=1;i<state[0].size()-1;i++) {
					text.push_back(state[1][i].state);
				}

				auto b_state = borders[0];
				auto t_state = text[0];

				for (auto &b : borders) {
					EXPECT(b).toBe(b_state);
				}
				for (auto &t : text) {
					EXPECT(t).toBe(t_state);
				}
				EXPECT(b_state).NOT().toBe(t_state);
			});

			IT("should not be stylized", {
				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto expected = termiq::canvas::CharState{
					termiq::style::Color::UNDEFINED,
					termiq::style::Color::UNDEFINED,
					false, false, false, false, false, false
				};

				EXPECT(*b_state).toBe(expected);
				EXPECT(*t_state).toBe(expected);
			});

			IT("should style border color", {
				grid->set_border_foreground_color({100, 100, 100});

				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto e_text = termiq::canvas::CharState{};
				auto e_border = termiq::canvas::CharState{
					{100, 100, 100},
				};

				EXPECT(*t_state).toBe(e_text);
				EXPECT(*b_state).toBe(e_border);
			});

			IT("should style border background color", {
				grid->set_border_background_color({100, 100, 100});

				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto e_text = termiq::canvas::CharState{};
				auto e_border = termiq::canvas::CharState{
					termiq::style::Color::UNDEFINED,
					{100, 100, 100},
				};

				EXPECT(*t_state).toBe(e_text);
				EXPECT(*b_state).toBe(e_border);
			});

			IT("should style cell background color", {
				grid->set_background_color({100, 100, 100});

				auto state = pieces_to_grid(grid->build());

				auto b_state = state[0][0].state;
				auto t_state = state[1][1].state;

				auto e_text = termiq::canvas::CharState{
					termiq::style::Color::UNDEFINED,
					{100, 100, 100}
				};
				auto e_border = termiq::canvas::CharState{};

				EXPECT(*t_state).toBe(e_text);
				EXPECT(*b_state).toBe(e_border);
			});
		});
	});
});

SCENARIO_END
