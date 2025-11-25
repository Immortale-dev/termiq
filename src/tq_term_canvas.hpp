#include <cstdio>
#include <iostream>
#include <algorithm>
#include <functional>

#include "tq_term_adv.h"
#include "tq_term_canvas_utils.h"

template<typename CC>
termiq::canvas::Canvas<CC>::Canvas(unsigned int h, unsigned int w, unsigned int r, unsigned int c) : _height(h), _width(w), _row(r), _col(c), _canvas(h, container<CC>(w)) {}

template<typename CC>
void termiq::canvas::Canvas<CC>::resize(unsigned int height, unsigned int width) {
	_width = width;
	_height = height;
	resized();
}

template<typename CC>
void termiq::canvas::Canvas<CC>::move(unsigned int row, unsigned int col) {
	_row = row;
	_col = col;
	moved();
}

template<typename CC>
void termiq::canvas::Canvas<CC>::draw(unsigned int row, unsigned int col, const CanvasPiece<CC> &piece) {
	for(size_t pr=0;pr<piece.rows;pr++) {
		for(size_t pc=0;pc<piece.cols;pc++) {
			size_t r = pr + row + piece.offset_rows;
			size_t c = pc + col + piece.offset_cols;
			if (r >= _height || c >= _width || !piece.canvas[pr][pc].state) continue;
			_canvas[r][c] = piece.canvas[pr][pc];
		}
	}
}


template<typename CC>
void termiq::canvas::Canvas<CC>::draw(unsigned int row, unsigned int col, const CanvasPieces<CC> &pieces) {
	for (auto& piece : pieces.pieces) {
		draw(row, col, piece);
	}
	drawn();
}

template<typename CC>
void termiq::canvas::Canvas<CC>::draw(unsigned int row, unsigned int col, const CanvasPieces<CC> &&pieces) {
	draw(row, col, pieces);
}

template<typename CC>
void termiq::canvas::Canvas<CC>::paint() {
	for(size_t cr=0;cr<_height;cr++) {
		paint_row(cr);
	}
}

template<typename CC>
unsigned int termiq::canvas::Canvas<CC>::get_width() {
	return _width;
}

template<typename CC>
unsigned int termiq::canvas::Canvas<CC>::get_height() {
	return _height;
}

template<typename CC>
unsigned int termiq::canvas::Canvas<CC>::get_col() {
	return _col;
}

template<typename CC>
unsigned int termiq::canvas::Canvas<CC>::get_row() {
	return _row;
}

template<typename CC>
const typename termiq::canvas::Canvas<CC>::container<typename termiq::canvas::Canvas<CC>::container<CC>>& termiq::canvas::Canvas<CC>::get_canvas() {
	return _canvas;
}

template<typename CC>
void termiq::canvas::Canvas<CC>::paint_row(size_t index) {
	move_cursor(_row + index, _col);
	for (size_t i=0;i<_width;i++) {
		paint_cell(_canvas[index][i]);
	}
}

template<typename CC>
void termiq::canvas::Canvas<CC>::resized() {
	_canvas.resize(_height);
	for(auto &c : _canvas) {
		c.resize(_width);
	}
}

template<typename CC>
void termiq::canvas::Canvas<CC>::moved() {}

template<typename CC>
void termiq::canvas::Canvas<CC>::drawn() {}

// SECanvas

template<typename CC>
termiq::canvas::SECanvas<CC>::SECanvas(SequenceExecutor* se, unsigned int h, unsigned int w, unsigned int r, unsigned int c)
	: Canvas<CC>(h,w,r,c), se_(se) {}

template<typename CC>
void termiq::canvas::SECanvas<CC>::move_cursor(unsigned int row, unsigned int col) {
	se_->execute<termiq::se::move>(row, col);
}

template<typename CC>
void termiq::canvas::SECanvas<CC>::set_paint_state(CharState* state) {
	if (!state) {
		termiq::style::style_reset();
		return;
	}
	termiq::style::style({
		.foreground = state->foreground(),
		.background = state->background(),
		.bold = state->bold(),
		.dim = state->dim(),
		.italic = state->italic(),
		.underline = state->underline(),
		.inverse = state->inverse(),
	});
}

template<typename CC>
void termiq::canvas::SECanvas<CC>::paint_cell(CC &cell) {
	set_paint_state(cell.state ? cell.state.get() : nullptr);
	se_->writer()->write(cell.symbol.begin(), cell.symbol.size());
}
