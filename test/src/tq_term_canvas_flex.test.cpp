#include "tq_term_canvas_flexgrid.h"
#include "tq_term_canvas_flextext.h"

SCENARIO_START

DESCRIBE("Canvas", {
	DESCRIBE("FlexGrid", {
		using CC = termiq::canvas::CharCell<termiq::canvas::WCharType, termiq::canvas::CharState>;
		termiq::canvas::FlexGrid<CC>* grid;

		DESCRIBE("w_char 2x3 instance is created", {
			BEFORE_EACH({
				grid = new termiq::canvas::FlexGrid<CC>(2,3);
				grid->set_border_type(termiq::canvas::BorderType::SINGLE);
			});
			AFTER_EACH({
				delete grid;
			});

			DESCRIBE("FlexText instances are added to {0,1} and {1,2} cells.", {
				termiq::canvas::FlexText<CC> *txt1, *txt2;
				BEFORE_EACH({
					txt1 = new termiq::canvas::FlexText<CC>(build_text<CC,char16_t>(u"Hello"));
					txt2 = new termiq::canvas::FlexText<CC>(build_text<CC,char16_t>(u"Hello World!"));
					grid->select_cell(0, 1);
					grid->set_cell_content(txt1);
					grid->select_cell(1, 2);
					grid->set_cell_content(txt2);
				});
				AFTER_EACH({
					delete txt1;
					delete txt2;
				});

				DESCRIBE("Grid width is set to 11", {
					BEFORE_EACH({
						grid->set_width(11);
					});

					IT("should leave first column empty, and give more space to the last column", {
						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌┬───┬────┐",
							u"││Hel│    │",
							u"││lo │    │",
							u"├┼───┼────┤",
							u"││   │Hell│",
							u"││   │o Wo│",
							u"││   │rld!│",
							u"└┴───┴────┘",
						}});
					});

					IT("should cut out overflowed text when cell height is not high enough", {
						grid->select_cell(1, 2);
						grid->set_cell_height(2);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌┬───┬────┐",
							u"││Hel│    │",
							u"││lo │    │",
							u"├┼───┼────┤",
							u"││   │Hell│",
							u"││   │o Wo│",
							u"└┴───┴────┘",
						}});
					});

					IT("should cut out overflowed text when grid height is not high enough", {
						grid->set_height(6);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌┬───┬────┐",
							u"││Hel│    │",
							u"││lo │    │",
							u"├┼───┼────┤",
							u"││   │Hell│",
							u"└┴───┴────┘",
						}});
					});

					IT("should set second and third column width to 1", {
						grid->select_cell(0);
						grid->set_cell_width(10);
						grid->set_height(9);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌─────┬─┬─┐",
							u"│     │H│ │",
							u"│     │e│ │",
							u"│     │l│ │",
							u"├─────┼─┼─┤",
							u"│     │ │H│",
							u"│     │ │e│",
							u"│     │ │l│",
							u"└─────┴─┴─┘",
						}});
					});
				});

				DESCRIBE("Grid width is set to 30", {
					BEFORE_EACH({
						grid->set_width(30);
					});

					IT("should proportionally distribute space between columns", {
						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌─┬────────┬─────────────────┐",
							u"│ │Hello   │                 │",
							u"├─┼────────┼─────────────────┤",
							u"│ │        │Hello World!     │",
							u"└─┴────────┴─────────────────┘",
						}});
					});

					IT("should fill second column with maximum width", {
						grid->select_cell(0);
						grid->set_cell_width(2);
						grid->set_cell_height(2);
						grid->select_cell(1,2);
						grid->set_cell_width(6);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌──┬──────────────────┬──────┐",
							u"│  │Hello             │      │",
							u"├──┼──────────────────┼──────┤",
							u"│  │                  │Hello │",
							u"│  │                  │World!│",
							u"└──┴──────────────────┴──────┘",
						}});
					});

					IT("should render grid without borders", {
						grid->set_border_type(termiq::canvas::BorderType::NONE);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result.size()).toBe(2);
						EXPECT(result[0]).toBe(u" Hello                        ");
						EXPECT(result[1]).toBe(u"          Hello World!        ");
					});

					IT("should distribute space proportionally betwenn columns", {
						grid->select_cell(0);
						grid->set_cell_width(5);
						grid->select_cell(1);
						grid->set_cell_width(5);
						grid->select_cell(2);
						grid->set_cell_width(5);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌──────┬────────┬────────────┐",
							u"│      │Hello   │            │",
							u"├──────┼────────┼────────────┤",
							u"│      │        │Hello World!│",
							u"└──────┴────────┴────────────┘",
						}});
					});
				});

				DESCRIBE("Grid width is not set", {
					IT("should fit the content of all columns", {
						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌┬─────┬────────────┐",
							u"││Hello│            │",
							u"├┼─────┼────────────┤",
							u"││     │Hello World!│",
							u"└┴─────┴────────────┘",
						}});
					});

					IT("should respect all set columns and row sizes", {
						grid->select_cell(0,0);
						grid->set_cell_width(3);
						grid->set_cell_height(1);
						grid->select_cell(1,1);
						grid->set_cell_width(3);
						grid->set_cell_height(3);

						auto result = pieces_to_text<CC,char16_t>(grid->build());

						EXPECT(result).toBe({{
							u"┌───┬───┬────────────┐",
							u"│   │Hel│            │",
							u"├───┼───┼────────────┤",
							u"│   │   │Hello World!│",
							u"│   │   │            │",
							u"│   │   │            │",
							u"└───┴───┴────────────┘",
						}});
					});
				});
			});
			DESCRIBE("Grid instance with text is added to {1,0}", {
				termiq::canvas::FlexGrid<CC> *inner_grid;
				termiq::canvas::FlexText<CC> *txt1;

				BEFORE_EACH({
					inner_grid = new termiq::canvas::FlexGrid<CC>(1,2);
					txt1 = new termiq::canvas::FlexText<CC>(build_text<CC,char16_t>(u"I am inside!"));
					grid->select_cell(1,0);
					grid->set_cell_content(inner_grid);
					inner_grid->set_border_type(termiq::canvas::BorderType::SINGLE);
					inner_grid->select_cell(0);
					inner_grid->set_cell_content(txt1);
				});
				AFTER_EACH({
					delete txt1;
					delete inner_grid;
				});

				IT("should render grid correctly", {
					auto result = pieces_to_text<CC,char16_t>(grid->build());

					EXPECT(result).toBe({{
						u"┌───────────────┬┬┐",
						u"├───────────────┼┼┤",
						u"│┌────────────┬┐│││",
						u"││I am inside!│││││",
						u"│└────────────┴┘│││",
						u"└───────────────┴┴┘",
					}});
				});

				IT("should squize inner border", {
					grid->select_cell(0);
					grid->set_cell_width(7);

					auto result = pieces_to_text<CC,char16_t>(grid->build());

					EXPECT(result).toBe({{
						u"┌───────┬┬┐",
						u"├───────┼┼┤",
						u"│┌────┬┐│││",
						u"││I am│││││",
						u"││ ins│││││",
						u"││ide!│││││",
						u"│└────┴┘│││",
						u"└───────┴┴┘",
					}});
				});

				IT("should still squize inner grid", {
					grid->select_cell(0);
					grid->set_cell_width(7);
					inner_grid->set_width(20);

					auto result = pieces_to_text<CC,char16_t>(grid->build());

					EXPECT(result).toBe({{
						u"┌───────┬┬┐",
						u"├───────┼┼┤",
						u"│┌────┬┐│││",
						u"││I am│││││",
						u"││ ins│││││",
						u"││ide!│││││",
						u"│└────┴┘│││",
						u"└───────┴┴┘",
					}});
				});

				IT("should expand inner grid", {
					inner_grid->set_width(20);

					auto result = pieces_to_text<CC,char16_t>(grid->build());

					EXPECT(result).toBe({{
						u"┌────────────────────┬┬┐",
						u"├────────────────────┼┼┤",
						u"│┌─────────────────┬┐│││",
						u"││I am inside!     │││││",
						u"│└─────────────────┴┘│││",
						u"└────────────────────┴┴┘",
					}});
				});
			});
		});
	});
});

SCENARIO_END
