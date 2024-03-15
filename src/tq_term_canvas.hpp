#include <cstdio>
#include <iostream>

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

// TextBuilder Implementation

template<typename CC>
termiq::Canvas<CC>::TextBuilder::TextBuilder(const typename CC::char_type* txt) {
	for(size_t i=0;txt[i]!='\0';++i) {
		_txt.push_back(txt[i]);
	}
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_foreground_color(termiq::style::Color &&color) {
	_foreground = color;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_background_color(termiq::style::Color &&color) {
	_background = color;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_bold() {
	_bold = true;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_italic() {
	_italic = true;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_dim() {
	_dim = true;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_underline() {
	_underline = true;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_inverse() {
	_inverse = true;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::TextBuilder& termiq::Canvas<CC>::TextBuilder::set_width(unsigned int w) {
	_width = w;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::CanvasPiece termiq::Canvas<CC>::TextBuilder::build() {
	container<container<CC>> all_lines;
	auto cs = std::make_shared<CharState>();
	cs->foreground = _foreground;
	cs->background = _background;
	cs->bold = _bold;
	cs->italic = _italic;
	cs->dim = _dim;
	cs->underline = _underline;
	cs->inverse = _inverse;
	unsigned int width = _width ? _width : _txt.size();
	unsigned int lines_count = (_txt.size()-1) / width + 1;
	for (size_t l=0;l<lines_count;l++) {
		container<CC> line;
		for (size_t i=0;i<width;++i) {
			line.push_back({_txt[l*width+i], cs});
		}
		all_lines.push_back(line);
	}
	return {lines_count, width, all_lines};
}

// GridBuilder implementation

template<typename CC>
termiq::Canvas<CC>::GridBuilder::GridBuilder(unsigned int rows, unsigned int cols): _rows(rows), _cols(cols), _grid(_rows, std::vector<GridCellState>(_cols)) {}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::select_cell(size_t c) {
	_current_cell_c = c;
	return *this;
};

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::select_cell(size_t r, size_t c) {
	_current_cell_r = r;
	_current_cell_c = c;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_background_color(termiq::style::Color color) {
	for (size_t r=0;r<_rows;r++) {
		for (size_t c=0;c<_cols;c++) {
			_grid[r][c].background = color;
		}
	}
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_border_foreground_color(termiq::style::Color color) {
	_border_foreground_color = color;
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_border_background_color(termiq::style::Color color) {
	_border_background_color = color;
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_cell_background_color(termiq::style::Color color) {
	get_current_cell().background = color;
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_cell_padding(int top, int right, int bottom, int left) {
	auto &cell = get_current_cell();
	set_cell_padding_values(cell, top, right, bottom, left);
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_padding(int top, int right, int bottom, int left) {
	for (size_t r=0;r<_rows;r++) {
		for (size_t c=0;c<_cols;c++) {
			auto &cell = _grid[r][c];
			set_cell_padding_values(cell, top, right, bottom, left);
		}
	}
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_cell_text(TextBuilder text) {
	auto &cell = get_current_cell();
	cell.text = text;
	cell.has_text = true;
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_size(unsigned int width, unsigned int height) {
	_width = width;
	_height = height;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder& termiq::Canvas<CC>::GridBuilder::set_cell_size(unsigned int width, unsigned int height) {
	auto &cell = get_current_cell();
	cell.width = width;
	cell.height = height;
	return *this;
}

template<typename CC>
typename termiq::Canvas<CC>::CanvasPiece termiq::Canvas<CC>::GridBuilder::build() {
	
}

template<typename CC>
void termiq::Canvas<CC>::GridBuilder::set_cell_padding_values(typename termiq::Canvas<CC>::GridBuilder::GridCellState &cell, int top, int right, int bottom, int left) {
	if (top < 0) return;
	if (right < 0) {
		cell.padding = {top, top, top, top};
		return;
	}
	if (bottom < 0) {
		cell.padding = {top, right, top, right};
		return;
	}
	if (left < 0) {
		cell.padding = {top, right, bottom, right};
		return;
	}
	cell.padding = {top, right, bottom, left};
}

template<typename CC>
typename termiq::Canvas<CC>::GridBuilder::GridCellState& termiq::Canvas<CC>::GridBuilder::get_current_cell() {
	return _grid[_current_cell_r][_current_cell_c];
}
