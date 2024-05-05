#include <vector>
#include <utility>

#include "tq_term_canvas.h"
#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_text.h"

namespace canvas_test {
	using CC = termiq::canvas::CharCell<wchar_t>;

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

DESCRIBE("canvas", {
	DESCRIBE("20x25 on [3;5] instance of MyCanvas is created.", {
		canvas_test::MyCanvas canvas;

		BEFORE_EACH({
			canvas = canvas_test::MyCanvas(20, 25, 3, 5);
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
			auto pieces = termiq::canvas::Text<canvas_test::CC>(L"test").build();
			canvas.draw(3, 4, std::move(pieces));

			EXPECT(canvas_test::drawn_called_times).toBe(1);
		});

		IT("should call drawn protected function once when draw is called with lvalue ref", {
			auto pieces = termiq::canvas::Text<canvas_test::CC>(L"test").build();
			canvas.draw(3, 4, pieces);

			EXPECT(canvas_test::drawn_called_times).toBe(1);
		});

		IT("should correctly draw pieces", {
			// TODO
		});

		IT("should call paint_row for every row in the canvas", {
			// TODO
		});

		DESCRIBE("canvas has been moved and resized", {
			BEFORE_EACH({
				canvas.move(10, 15);
				canvas.resize(30, 40);
			});

			IT("should correctly draw pieces with complex offsets", {
				// TODO
			});

			IT("should call paint_row for every row in the canvas", {
				// TODO
			});
		});
	});
});

SCENARIO_END
