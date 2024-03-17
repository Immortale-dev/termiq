#include <cstdio>
#include <iostream>
#include <algorithm>
#include <functional>

template<typename CC>
termiq::Canvas<CC>::Canvas(unsigned int h, unsigned int w, unsigned int r, unsigned int c): _height(h), _width(w), _row(r), _col(c), _canvas(h, container<CC>(w)) {}

template<typename CC>
void termiq::Canvas<CC>::resize(unsigned int width, unsigned int height) {
	_width = width;
	_height = height;
	resized();
}

template<typename CC>
void termiq::Canvas<CC>::move(unsigned int row, unsigned int col) {
	_row = row;
	_col = col;
	moved();
}

template<typename CC>
void termiq::Canvas<CC>::draw(unsigned int row, unsigned int col, CanvasPiece&& piece) {
	for(size_t pr=0;pr<piece.rows;pr++) {
		for(size_t pc=0;pc<piece.cols;pc++) {
			size_t r = pr + row;
			size_t c = pc + col;
			if (r >= _height || c >= _width) continue;
			_canvas[r][c] = piece.canvas[pr][pc];
		}
	}
	drawn();
}

template<typename CC>
void termiq::Canvas<CC>::paint() {
	for(size_t cr=0;cr<_height;cr++) {
		paint_row(cr);
	}
}

template<typename CC>
void termiq::Canvas<CC>::paint_row(size_t index) {
	move_cursor(_row + index, _col);
	for (size_t i=0;i<_width;i++) {
		paint_cell(_canvas[index][i]);
	}
}

template<typename CC>
void termiq::Canvas<CC>::resized() {
	_canvas.resize(_height);
	for(auto &c : _canvas) {
		c.resize(_width);
	}
}

template<typename CC>
void termiq::Canvas<CC>::moved() {}

template<typename CC>
void termiq::Canvas<CC>::drawn() {}

template<typename CC>
void termiq::Canvas<CC>::paint_cell(CC &cell) {
	const size_t char_sz = sizeof(typename CC::char_type);
	set_paint_state(cell.state ? cell.state.get() : nullptr);
	printf(char_sz == 1 ? "%c" : "%lc", cell.symbol);
}

template<typename CC>
void termiq::Canvas<CC>::move_cursor(unsigned int row, unsigned int col) {
	termiq::move(col, row);
}

template<typename CC>
void termiq::Canvas<CC>::set_paint_state(CharState* state) {
	if (!state) {
		termiq::style::clear();
		return;
	}
	termiq::style::style(state->foreground, state->background, state->bold, state->italic, state->dim, state->underline, state->inverse);
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder termiq::Canvas<CC>::text(const typename CC::char_type *txt) {
	return TextBuilder(txt);
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder termiq::Canvas<CC>::grid(unsigned int rows, unsigned int cols) {
	return GridBuilder(rows, cols);
}

// GridBuilder implementation
