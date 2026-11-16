#include <cstdio>
#include <iostream>
#include <algorithm>
#include <functional>

#include "tq_term_adv.h"
#include "tq_term_canvas_utils.h"

template<typename CC, typename Container>
termiq::canvas::Canvas<CC,Container>::Canvas(uint32_t h, uint32_t w, int32_t r, int32_t c)
	: height_(h), width_(w), row_(r), col_(c), canvas_(h, w) {}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::resize(uint32_t height, uint32_t width) {
	width_ = width;
	height_ = height;
	canvas_.resize(height_, width_);
	resized();
}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::move(int32_t row, int32_t col) {
	row_ = row;
	col_ = col;
	canvas_.move(row_, col_);
	moved();
}

template<typename CC, typename Container>
template<typename Layer>
void termiq::canvas::Canvas<CC,Container>::draw_layer(uint32_t row, uint32_t col, const Layer& layer) {
	for(size_t pr=0;pr<layer.rows();pr++) {
		for(size_t pc=0;pc<layer.cols();pc++) {
			size_t r = pr + row + layer.row();
			size_t c = pc + col + layer.col();
			if (r >= height_ || c >= width_ || layer.cell(pr,pc).transparent()) continue;
			canvas_.set(r,c, layer.cell(pr,pc));
		}
	}
}


template<typename CC, typename Container>
template<typename Piece>
void termiq::canvas::Canvas<CC,Container>::draw(uint32_t row, uint32_t col, Piece&& piece) {
	for (auto& layer : piece.layers()) {
		draw_layer(row, col, layer);
	}
	drawn();
}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::paint() {
	prepaint();
	for(size_t cr=0;cr<height_;cr++) {
		paint_row(cr);
	}
	painted();
}

template<typename CC, typename Container>
uint32_t termiq::canvas::Canvas<CC,Container>::get_width() {
	return width_;
}

template<typename CC, typename Container>
uint32_t termiq::canvas::Canvas<CC,Container>::get_height() {
	return height_;
}

template<typename CC, typename Container>
int32_t termiq::canvas::Canvas<CC,Container>::get_col() {
	return col_;
}

template<typename CC, typename Container>
int32_t termiq::canvas::Canvas<CC,Container>::get_row() {
	return row_;
}

template<typename CC, typename Container>
const Container& termiq::canvas::Canvas<CC,Container>::get_canvas() {
	return canvas_;
}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::paint_row(uint32_t index) {
	move_cursor(row_ + index, col_);
	for (size_t i=0;i<width_;i++) {
		paint_cell(canvas_.get(index, i));
	}
}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::resized() {}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::moved() {}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::drawn() {}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::painted() {}

template<typename CC, typename Container>
void termiq::canvas::Canvas<CC,Container>::prepaint() {}

// SECanvas

template<typename CC, typename Container>
termiq::canvas::SECanvas<CC,Container>::SECanvas(SequenceExecutor* se, uint32_t h, uint32_t w, int32_t r, int32_t c)
	: Canvas<CC,Container>(h,w,r,c), se_(se) {}

template<typename CC, typename Container>
void termiq::canvas::SECanvas<CC,Container>::move_cursor(uint32_t row, uint32_t col) {
	se_->execute<termiq::se::move>(row, col);
}

template<typename CC, typename Container>
void termiq::canvas::SECanvas<CC,Container>::set_paint_state(const Style& state) {
	termiq::style::style({
		.foreground = state.foreground(),
		.background = state.background(),
		.bold = state.bold(),
		.dim = state.dim(),
		.italic = state.italic(),
		.underline = state.underline(),
		.inverse = state.inverse(),
	});
}

template<typename CC, typename Container>
void termiq::canvas::SECanvas<CC,Container>::paint_cell(const Cell& cell) {
	set_paint_state(cell.style());
	se_->writer()->write(cell.symbol().begin(), cell.symbol().size());
}
