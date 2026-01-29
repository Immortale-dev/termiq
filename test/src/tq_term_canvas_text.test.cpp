#include "qtest.hpp"

#include <vector>

#include "test/src/helpers.h"
#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_text.h"

SCENARIO_START

DESCRIBE("Canvas", {
	DESCRIBE("Text", {
		using CC = termiq::canvas::CharCell<termiq::canvas::CharType, termiq::canvas::CharState>;
		termiq::canvas::Text<CC>* text;
		DESCRIBE("'lorem ipsum' char instance is created", {
			BEFORE_EACH({
				text = new termiq::canvas::Text<CC>(build_text<CC, char>("lorem ipsum"));
			});
			AFTER_EACH({
				delete text;
			});

			IT("should share style state among the whole grid", {
				text->set_width(3);
				text->set_bold();

				auto grid = pieces_to_grid(text->build());
				auto first_state = grid[0][0].state;
				int transparent_count = 0;

				EXPECT(first_state).NOT().toBe(nullptr);
				for (auto &row : grid) {
					for (auto &cell : row) {
						if (cell.is_transparent()){
							transparent_count++;
							 continue;
						}
						EXPECT(cell.state).toBe(first_state);
					}
				}
				EXPECT(transparent_count).toBe(1);
			});

			IT("should apply bold style", {
				text->set_bold();

				auto state = pieces_to_grid(text->build())[0][0].state;

				termiq::canvas::CharState compare;
				compare.bold(true);
				EXPECT(*state).toBe(compare);
			});
			IT("should apply italic style", {
				text->set_italic();
				auto state = pieces_to_grid(text->build())[0][0].state;
				termiq::canvas::CharState compare;
				compare.italic(true);

				EXPECT(*state).toBe(compare);
			});

			IT("should apply dim style", {
				text->set_dim();
				auto state = pieces_to_grid(text->build())[0][0].state;
				termiq::canvas::CharState compare;
				compare.dim(true);

				EXPECT(*state).toBe(compare);
			});

			IT("should apply inverse style", {
				text->set_inverse();
				auto state = pieces_to_grid(text->build())[0][0].state;
				termiq::canvas::CharState compare;
				compare.inverse(true);

				EXPECT(*state).toBe(compare);
			});

			IT("should apply foreground color", {
				text->set_foreground_color(termiq::Color{100, 100, 100});
				auto state = pieces_to_grid(text->build())[0][0].state;
				termiq::canvas::CharState compare;
				compare.foreground(termiq::Color{100, 100, 100});

				EXPECT(*state).toBe(compare);
			});

			IT("should apply background color", {
				text->set_background_color(termiq::Color{200, 200, 200});
				auto state = pieces_to_grid(text->build())[0][0].state;
				termiq::canvas::CharState compare;
				compare.background(termiq::Color{200, 200, 200});

				EXPECT(*state).toBe(compare);
			});

			IT("should mix colors and styles", {
				text->set_background_color(termiq::Color{100, 120, 150});
				text->set_bold();
				text->set_inverse();

				auto state = pieces_to_grid(text->build())[0][0].state;
				termiq::canvas::CharState compare({.background=termiq::Color{100,120,150}, .bold=true, .inverse=true});

				EXPECT(*state).toBe(compare);
			});

			IT("should build one liner text", {
				auto built = text->build();
				auto text_result = pieces_to_text<CC, char>(built);

				EXPECT(text_result).toBe({
					{"lorem ipsum"}
				});
			});

			IT("should build multiline text", {
				text->set_width(3);

				auto text_result = pieces_to_text<CC, char>(text->build());

				EXPECT(text_result).toBe({
					{"lor", "em ", "ips", "um "}
				});
			});
		});

		DESCRIBE("'Lorem ipsum<br>dolor sit amet' char instance is created", {
			BEFORE_EACH({
				text = new termiq::canvas::Text<CC>(build_text<CC,char>("lorem ipsum\ndolor sit amet"));
			});
			AFTER_EACH({
				delete text;
			});

			IT("should show 2 lines of text", {
				auto text_result = pieces_to_text<CC,char>(text->build());

				EXPECT(text_result.size()).toBe(2ull);
				EXPECT(text_result[0]).toBe("lorem ipsum   ");
				EXPECT(text_result[1]).toBe("dolor sit amet");
			});

			IT("should leave only first 2 workds", {
				text->set_width(6);
				text->set_height(2);

				auto text_result = pieces_to_text<CC,char>(text->build());

				EXPECT(text_result).toBe({
					"lorem ", "ipsum "
				});
			});
		});
	});
});

SCENARIO_END
