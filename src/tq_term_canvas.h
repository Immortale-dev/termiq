#ifndef _TQ_TERM_CANVAS
#define _TQ_TERM_CANVAS

#include "tq_term_canvas_utils.h"

#include <vector>
#include <memory>

namespace termiq {
	// CC - char cell type.
	namespace canvas {
		template<typename CC>
		class Canvas {
			public:
				template<typename T>
				using container = std::vector<T>;
				using char_type = typename CC::char_type;
				using CharState = typename CC::char_state_type;

			public:
				Canvas() = default;
				Canvas(unsigned int h, unsigned int w, unsigned int r=0, unsigned int c=0);
				virtual ~Canvas() = default;

				void resize(unsigned int height, unsigned int width);
				void move(unsigned int row, unsigned int col);
				void draw(unsigned int row, unsigned int col, const CanvasPieces<CC>&& pieces);
				void draw(unsigned int row, unsigned int col, const CanvasPieces<CC>& pieces);
				void paint();

				unsigned int get_width();
				unsigned int get_height();
				unsigned int get_col();
				unsigned int get_row();
				const container<container<CC>>& get_canvas();

			protected:
				virtual void moved();
				virtual void resized();
				virtual void drawn();
				virtual void paint_row(size_t index);

				virtual void move_cursor(unsigned int row, unsigned int col) = 0;
				virtual void set_paint_state(CharState* state) = 0;
				virtual void paint_cell(const CC &cell) = 0;

			private:
				void draw(unsigned int row, unsigned int col, const CanvasPiece<CC>& piece);

				unsigned int _height;
				unsigned int _width;
				unsigned int _row;
				unsigned int _col;
				container<container<CC>> _canvas;
		};

		template<typename CC>
		class SECanvas : public Canvas<CC> {
			public:
				SECanvas(SequenceExecutor* se, unsigned int h, unsigned int w, unsigned int r=0, unsigned int c=0);

			protected:
				void move_cursor(unsigned int row, unsigned int col) override;
				void set_paint_state(CharState* state) override;
				void paint_cell(CC &cell) override;

			private:
				SequenceExecutor* se_;
		};
	}
}

#include "tq_term_canvas.hpp"

#endif // _TQ_TERM_CANVAS
