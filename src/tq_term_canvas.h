#ifndef TQ_TERM_CANVAS_H_
#define TQ_TERM_CANVAS_H_

#include "tq_term_canvas_utils.h"

#include <vector>
#include <memory>

namespace termiq {
	namespace canvas {
		template<typename CC, typename Container = CanvasContainer<CC>>
		class Canvas {
			public:
				using Cell = CC;
				using Char = typename CC::Char;
				using Style = typename CC::Style;

			public:
				Canvas() = default;
				Canvas(uint32_t h, uint32_t w, int32_t r=0, int32_t c=0);
				virtual ~Canvas() = default;

				void resize(uint32_t height, uint32_t width);
				void move(int32_t row, int32_t col);
				template<typename Piece>
				void draw(uint32_t row, uint32_t col, Piece&& pieces);
				void paint();

				uint32_t get_width();
				uint32_t get_height();
				int32_t get_col();
				int32_t get_row();
				const Container& get_canvas();

			protected:
				virtual void moved();
				virtual void resized();
				virtual void drawn();
				virtual void prepaint();
				virtual void painted();
				virtual void paint_row(uint32_t index);

				virtual void move_cursor(uint32_t row, uint32_t col) = 0;
				virtual void set_paint_state(const Style& state) = 0;
				virtual void paint_cell(const Cell& cell) = 0;

			private:
				template<typename Layer>
				void draw_layer(uint32_t row, uint32_t col, const Layer& piece);

				unsigned int height_;
				unsigned int width_;
				unsigned int row_;
				unsigned int col_;
				Container canvas_;
		};

		template<typename CC, typename Container = CanvasContainer<CC>>
		class SECanvas : public Canvas<CC, Container> {
			public:
				using Cell = CC;
				using Style = Canvas<Cell, Container>::Style;

				SECanvas(SequenceExecutor* se, uint32_t h, uint32_t w, int32_t r=0, int32_t c=0);

			protected:
				void move_cursor(uint32_t row, uint32_t col) override;
				void set_paint_state(const Style& state) override;
				void paint_cell(const Cell& cell) override;

			private:
				SequenceExecutor* se_;
		};
	}
}

#include "tq_term_canvas.hpp"

#endif // TQ_TERM_CANVAS_H_
