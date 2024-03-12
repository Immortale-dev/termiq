#include <cstdio>

template<typename CC>
termiq::Canvas<CC>::Canvas(unsigned int w, unsigned int h, unsigned int r, unsigned int c): _width(w), _height(h), _row(r), _col(c), _canvas(h, container<CC>(w, {})) {}

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
	for(size_t pr=0;pr<piece.height;pr++) {
		for(size_t pc=0;pc<piece.width;pr++) {
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
	const char_sz = sizeof(typename CC::char_type);
	set_paint_state(cell.state ? cell.state.get() : nullptr);
	printf(char_sz == 1 ? "%c" : "%lc", cell.symbol);
}

template<typename CC>
void termiq::Canvas<CC>::set_paint_state(CharState* state) {
	if (!state) {
		termiq::style::clear();
	}
	termiq::style::foreground(state->foreground);
	termiq::style::background(state->background);
	termiq::style::bold(state->bold);
	termiq::style::italic(state->italic);
	termiq::style::dim(state->dim);
	termiq::style::underline(state->underline);
	termiq::style::inverse(state->inverse);
}

template<typename CC>
termiq::Canvas<CC>::TextBuilder Termiq::Canvas::text(typename CC::char_type *txt) {
	return TextBuilder(txt);
}

template<typename CC>
termiq::Canvas<CC>::GridBuilder Termiq::Canvas::grid(unsigned int rows, unsigned int cols) {
	return GridBuilder(); // TODO: implement.
}


template<typename CC>
termiq::Canvas<CC>::CanvasPiece termiq::Canvas<CC>::TextBuilder::Build() {
	container<CC> line;
	std::shared_ptr<CharState> cs;
	cs->foreground = _foreground;
	cs->background = _background;
	cs->bold = _bold;
	cs->italic = _italic;
	cs->dim = _dim;
	cs->underline = _underline;
	cs->inverse = _inverse;
	int w = 0;
	for(size_t i=0;txt[i]!='\0';++i) {
		line.push_back({txt[i], cs});
		++w;
	}
	return {w,1,container(1, line)};
}
