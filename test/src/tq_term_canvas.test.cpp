#include "qtest.hpp"

#include <vector>
#include <utility>

#include "tq_term_canvas.h"
#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_text.h"
#include "test/src/helpers.h"

namespace {
	using CC = termiq::canvas::CharCell<termiq::canvas::WCharType, termiq::canvas::CharState>;

	std::vector<std::pair<unsigned int, unsigned int>> size_on_resized;
	std::vector<std::pair<unsigned int, unsigned int>> position_on_moved;
	int drawn_called_times = 0;
	std::vector<size_t> paint_row_called;

	void test_clear() {
		size_on_resized.resize(0);
		position_on_moved.resize(0);
		drawn_called_times = 0;
		paint_row_called.resize(0);
	}

	class MyCanvas : public termiq::canvas::Canvas<CC> {
		public:
			using termiq::canvas::Canvas<CC>::Canvas;

		protected:
			void moved() override {
				position_on_moved.push_back({get_row(), get_col()});
			}
			void resized() override {
				termiq::canvas::Canvas<CC>::resized();
				size_on_resized.push_back({get_height(), get_width()});
			}
			void drawn() override {
				++drawn_called_times;
			}
			void paint_row(size_t index) override {
				paint_row_called.push_back(index);
			}
			void paint_cell([[maybe_unused]] const CC &cell) override {}
			void move_cursor([[maybe_unused]] unsigned int row, [[maybe_unused]] unsigned int col) override {}
			void set_paint_state([[maybe_unused]] const char_state_type* state) override {}
	};
}

SCENARIO_START

DESCRIBE("canvas", {
	DESCRIBE("20x25 on [3;5] instance of MyCanvas is created.", {
		MyCanvas canvas;

		BEFORE_EACH({
			canvas = MyCanvas(20, 25, 3, 5);
		});

		AFTER_EACH({
			test_clear();
		});

		IT("should create correct canvas", {
			EXPECT(canvas.get_width()).toBe(25ull);
			EXPECT(canvas.get_height()).toBe(20ull);
			EXPECT(canvas.get_col()).toBe(5u);
			EXPECT(canvas.get_row()).toBe(3u);
			EXPECT(size_on_resized.size()).toBe(0ull);
			EXPECT(position_on_moved.size()).toBe(0ull);
			EXPECT(drawn_called_times).toBe(0);
			EXPECT(paint_row_called.size()).toBe(0ull);
		});

		IT("should call resized protected function once when resize is called", {
			canvas.resize(15, 18);

			EXPECT(size_on_resized.size()).toBe(1ull);
			EXPECT(size_on_resized[0]).toBe({15, 18});
			EXPECT(canvas.get_width()).toBe(18u);
			EXPECT(canvas.get_height()).toBe(15u);
		});

		IT("should call moved protected function once when move is called", {
			canvas.move(10, 15);

			EXPECT(position_on_moved.size()).toBe(1ull);
			EXPECT(position_on_moved[0]).toBe({10, 15});
			EXPECT(canvas.get_col()).toBe(15u);
			EXPECT(canvas.get_row()).toBe(10u);
		});

		IT("should call drawn protected function once when draw is called with rvalue", {
			auto pieces = termiq::canvas::Text<CC>(build_text<CC,char16_t>(u"test")).build();
			canvas.draw(3, 4, std::move(pieces));

			EXPECT(drawn_called_times).toBe(1);
		});

		IT("should call drawn protected function once when draw is called with lvalue ref", {
			auto pieces = termiq::canvas::Text<CC>(build_text<CC,char16_t>(u"test")).build();
			canvas.draw(3, 4, pieces);

			EXPECT(drawn_called_times).toBe(1);
		});

		IT("should correctly draw pieces", {
			auto pieces = termiq::canvas::Text<CC>(build_text<CC,char16_t>(u"Hello\nWorld!"));
			canvas.draw(3, 4, pieces.build());

			int filled_cells = 0;
			auto matrix = canvas.get_canvas();
			for (auto &l : matrix) {
				for (auto &c : l) {
					if (!c.is_transparent()) {
						++filled_cells;
					}
				}
			}
			auto text_lines = grid_to_text<CC,char16_t>(matrix);
			int text_cells = 0;
			for (auto &l : text_lines) {
				for (auto c : l) {
					if (c != ' ') {
						++text_cells;
					}
				}
			}
			std::vector<std::basic_string<char16_t>> part;
			for (int i=0;i<2;i++) {
				std::basic_string<char16_t> str;
				for(int j=0;j<6;j++) {
					str.push_back(text_lines[3+i][4+j]);
				}
				part.push_back(str);
			}

			EXPECT(part).toBe({
				u"Hello ",
				u"World!",
			});

			EXPECT(filled_cells).toBe(11);
			EXPECT(text_cells).toBe(11);
		});

		IT("should call paint_row for every row in the canvas", {
			canvas.paint();

			size_t i=0;
			for (auto index : paint_row_called) {
				EXPECT(index).toBe(i++);
			}
			EXPECT(i).toBe(20ull);
		});

		DESCRIBE("canvas has been moved and resized", {
			BEFORE_EACH({
				canvas.move(10, 15);
				canvas.resize(30, 40);
			});

			IT("should correctly draw pieces with complex offsets", {
				auto piece_state = termiq::canvas::CharState({});
				const termiq::canvas::CanvasPieces<CC> built_pieces{
					{
						termiq::canvas::CanvasPiece<CC>{
							{
								{
									{'T', piece_state},
									{'T', piece_state},
									{'T', piece_state}
								},
							},
							1, 3, -2, 0
						},
						termiq::canvas::CanvasPiece<CC>{
							{
								{
									{'B', piece_state},
									{'B', piece_state},
									{'B', piece_state}
								},
							},
							1, 3, 0, 0
						},
						termiq::canvas::CanvasPiece<CC>{
							{
								{{'L', piece_state}},
							},
							1, 1, -1, 0
						},
						termiq::canvas::CanvasPiece<CC>{
							{
								{{'R', piece_state}},
							},
							1, 1, -1, 2
						},
					}
				};

				canvas.draw(5, 6, built_pieces);

				int filled_cells = 0;
				auto matrix = canvas.get_canvas();
				for (auto &l : matrix) {
					for (auto &c : l) {
						if (!c.is_transparent()) {
							++filled_cells;
						}
					}
				}
				auto text_lines = grid_to_text<CC,char16_t>(matrix);
				int text_cells = 0;
				for (auto &l : text_lines) {
					for (auto c : l) {
						if (c != ' ') {
							++text_cells;
						}
					}
				}
				std::vector<std::basic_string<char16_t>> part;
				for (int i=0;i<3;i++) {
					std::basic_string<char16_t> str;
					for(int j=0;j<3;j++) {
						str.push_back(text_lines[3+i][6+j]);
					}
					part.push_back(str);
				}

				EXPECT(part).toBe({{
					u"TTT",
					u"L R",
					u"BBB",
				}});

				EXPECT(filled_cells).toBe(8);
				EXPECT(text_cells).toBe(8);
			});

			IT("should call paint_row for every row in the canvas", {
				canvas.paint();

				size_t i=0;
				for (auto index : paint_row_called) {
					EXPECT(index).toBe(i++);
				}
				EXPECT(i).toBe(30ull);
			});
		});
	});
});

SCENARIO_END
