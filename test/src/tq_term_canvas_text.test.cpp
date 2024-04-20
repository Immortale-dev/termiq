#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_text.h"

SCENARIO_START

DESCRIBE_ONLY("Canvas", {
	DESCRIBE("Text", {
		using CC = termiq::canvas::CharCell<char>;
		termiq::canvas::Text<CC>* text;
		DESCRIBE("'lorem ipsum' char instance is created", {
			BEFORE_EACH({
				text = new termiq::canvas::Text<CC>("lorem ipsum");
			});
			AFTER_EACH({
				delete text;
			});

			IT("should share style state among the whole grid", {
				text->set_width(3);
				text->set_bold();

				auto grid = pieces_to_grid(text->build());
				auto first_state = grid[0][0].state;

				EXPECT(first_state).NOT().toBe(nullptr);
				for (auto &row : grid) {
					for (auto &cell : row) {
						if (cell.is_transparent()) continue;
						EXPECT(cell.state).toBe(first_state);
					}
				}
			});

			IT("should apply bold style", {
				text->set_bold();

				auto state = pieces_to_grid(text->build())[0][0].state;

				termiq::canvas::CharState compare;
				compare.bold = true;
				EXPECT(*state).toBe(compare);
			});

			IT("should build the not styled text", {
				auto built = text->build();
				auto text_result = pieces_to_text(built);

				EXPECT(text_result).toBe({
					{"lorem ipsum"}
				});
			});
		});
	});
});

SCENARIO_END
