#include <vector>
#include <utility>

#include "tq_term_canvas.h"
#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_text.h"

namespace canvas_test {
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
			void paint_row(size_t index) {
				paint_row_called.push_back(index);
			}
	};
}

SCENARIO_START

using namespace canvas_test;

DESCRIBE("canvas", {
	DESCRIBE("20x25 on [3;5] instance of MyCanvas is created.", {
		canvas_test::MyCanvas canvas;

		std::istringstream input_sstream;
		std::ostringstream output_sstream;
		std::unique_ptr<std::istream> input_stream;
		std::unique_ptr<std::ostream> output_stream;
		std::unique_ptr<termiq::Reader> reader;
		std::unique_ptr<termiq::Writer> writer;
		std::unique_ptr<termiq::SequenceExecutor> se;
		BEFORE_EACH({
			input_sstream = std::istringstream();
			output_sstream = std::ostringstream();
			input_stream = std::make_unique<std::istream>(input_sstream.rdbuf());
			output_stream = std::make_unique<std::ostream>(output_sstream.rdbuf());
			reader = std::make_unique<termiq::StreamReader>(input_stream.get());
			writer = std::make_unique<termiq::StreamWriter>(output_stream.get());
			se = std::make_unique<termiq::SequenceExecutor>(reader.get(), writer.get());
			termiq::style::init(se.get());
		});

		BEFORE_EACH({
			canvas = canvas_test::MyCanvas(se.get(), 20, 25, 3, 5);
		});

		AFTER_EACH({
			canvas_test::test_clear();
		});

		IT("should create correct canvas", {
			EXPECT(canvas.get_width()).toBe(25);
			EXPECT(canvas.get_height()).toBe(20);
			EXPECT(canvas.get_col()).toBe(5);
			EXPECT(canvas.get_row()).toBe(3);
			EXPECT(canvas_test::size_on_resized.size()).toBe(0);
			EXPECT(canvas_test::position_on_moved.size()).toBe(0);
			EXPECT(canvas_test::drawn_called_times).toBe(0);
			EXPECT(canvas_test::paint_row_called.size()).toBe(0);
		});

		IT("should call resized protected function once when resize is called", {
			canvas.resize(15, 18);

			EXPECT(canvas_test::size_on_resized.size()).toBe(1);
			EXPECT(canvas_test::size_on_resized[0]).toBe({15, 18});
			EXPECT(canvas.get_width()).toBe(18);
			EXPECT(canvas.get_height()).toBe(15);
		});

		IT("should call moved protected function once when move is called", {
			canvas.move(10, 15);

			EXPECT(canvas_test::position_on_moved.size()).toBe(1);
			EXPECT(canvas_test::position_on_moved[0]).toBe({10, 15});
			EXPECT(canvas.get_col()).toBe(15);
			EXPECT(canvas.get_row()).toBe(10);
		});

		IT("should call drawn protected function once when draw is called with rvalue", {
			auto pieces = termiq::canvas::Text<canvas_test::CC>(build_text<CC,char16_t>(u"test")).build();
			canvas.draw(3, 4, std::move(pieces));

			EXPECT(canvas_test::drawn_called_times).toBe(1);
		});

		IT("should call drawn protected function once when draw is called with lvalue ref", {
			auto pieces = termiq::canvas::Text<canvas_test::CC>(build_text<CC,char16_t>(u"test")).build();
			canvas.draw(3, 4, pieces);

			EXPECT(canvas_test::drawn_called_times).toBe(1);
		});

		IT("should correctly draw pieces", {
			auto pieces = termiq::canvas::Text<canvas_test::CC>(build_text<CC,char16_t>(u"Hello\nWorld!"));
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
			auto text_lines = grid_to_text<canvas_test::CC,char16_t>(matrix);
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

			int i=0;
			for (auto index : canvas_test::paint_row_called) {
				EXPECT(index).toBe(i++);
			}
			EXPECT(i).toBe(20);
		});

		DESCRIBE("canvas has been moved and resized", {
			BEFORE_EACH({
				canvas.move(10, 15);
				canvas.resize(30, 40);
			});

			IT("should correctly draw pieces with complex offsets", {
				auto piece_state = std::make_shared<termiq::canvas::CharState>();
				const termiq::canvas::CanvasPieces<canvas_test::CC> built_pieces{
					{
						termiq::canvas::CanvasPiece<canvas_test::CC>{
							{
								{
									{'T', piece_state},
									{'T', piece_state},
									{'T', piece_state}
								},
							},
							1, 3, -2, 0
						},
						termiq::canvas::CanvasPiece<canvas_test::CC>{
							{
								{
									{'B', piece_state},
									{'B', piece_state},
									{'B', piece_state}
								},
							},
							1, 3, 0, 0
						},
						termiq::canvas::CanvasPiece<canvas_test::CC>{
							{
								{{'L', piece_state}},
							},
							1, 1, -1, 0
						},
						termiq::canvas::CanvasPiece<canvas_test::CC>{
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
				auto text_lines = grid_to_text<canvas_test::CC,char16_t>(matrix);
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

				int i=0;
				for (auto index : canvas_test::paint_row_called) {
					EXPECT(index).toBe(i++);
				}
				EXPECT(i).toBe(30);
			});
		});
	});
});

SCENARIO_END
