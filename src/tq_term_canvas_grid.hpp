#include "tq_term_unicode_draws.h"

template<typename CC>
termiq::canvas::Grid<CC>::Grid(unsigned int rows, unsigned int cols) : Content<CC>(), _rows(rows), _cols(cols), _grid(_rows, std::vector<GridCellState>(_cols)) {}

template<typename CC>
void termiq::canvas::Grid<CC>::select_cell(size_t c) {
	_current_cell_c = c;
};

template<typename CC>
void termiq::canvas::Grid<CC>::select_cell(size_t r, size_t c) {
	_current_cell_r = r;
	_current_cell_c = c;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_background_color(termiq::color_t color) {
	for (size_t r=0;r<_rows;r++) {
		for (size_t c=0;c<_cols;c++) {
			_grid[r][c].background = color;
		}
	}
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_border_foreground_color(termiq::color_t color) {
	_border_foreground_color = color;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_border_background_color(termiq::color_t color) {
	_border_background_color = color;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_background_color(termiq::color_t color) {
	get_current_cell().background = color;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_content(Content<CC>* content) {
	auto &cell = get_current_cell();
	cell.content = content;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_width(unsigned int width) {
	_width = width;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_height(unsigned int height) {
	_height = height;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_width(unsigned int width) {
	auto &cell = get_current_cell();
	cell.width = width;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_height(unsigned int height) {
	auto &cell = get_current_cell();
	cell.height = height;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_border_type(BorderType type) {
	_border_type = type;
	invalidate_calc();
}

template<typename CC>
typename termiq::canvas::CanvasPieces<CC> termiq::canvas::Grid<CC>::build() {
	// Calc column and rows sizes.
	lazy_calc();

	const auto& cols_widths = _cols_widths;
	const auto& rows_heights = _rows_heights;

	GridBorders border = get_border(_border_type);
	unsigned int width = get_width();
	unsigned int height = get_height();
	std::vector<std::vector<CC>> canvas(height, std::vector<CC>(width));

	// Fill in borders.
	if (_border_type != BorderType::NONE) {
		auto cs = std::make_shared<CharState>();
		cs->foreground(_border_foreground_color);
		cs->background(_border_background_color);

		for (size_t r=1;r<height-1;++r) {
			for (size_t l=0,c=0;l<=_cols;c+=cols_widths[l++]+1) {
				canvas[r][c] = {char_type{&*border.C_V.begin(), &*border.C_V.end()}, cs};
				if (l == _cols) break;
			}
		}
		for (size_t c=1;c<width-1;++c) {
			for (size_t l=0,r=0;l<=_rows;r+=rows_heights[l++]+1) {
				canvas[r][c] = {char_type{&*border.C_H.begin(), &*border.C_H.end()}, cs};
				if (l == _rows) break;
			}
		}

		for (size_t i=0,r=0;i<=_rows;r+=rows_heights[i++]+1) {
			for (size_t j=0,c=0;j<=_cols;c+=cols_widths[j++]+1) {
				char_type ct = border.C_X;
				if (i == 0) ct = border.C_HB;
				if (i == _rows) ct = border.C_HT;
				if (j == 0) ct = border.C_VR;
				if (j == _cols) ct = border.C_VL;
				if (i == 0 && j == 0) ct = border.C_BR;
				if (i == 0 && j == _cols) ct = border.C_BL;
				if (i == _rows && j == 0) ct = border.C_TR;
				if (i == _rows && j == _cols) ct = border.C_TL;
				canvas[r][c] = {ct, cs};
				if (j == _cols) break;
			}
			if (i == _rows) break;
		}
	}

	// Fill cell backgrounds and text.
	size_t border_size = _border_type == BorderType::NONE ? 0 : 1;
	for (size_t i=0,r=border_size;r<height;r+=rows_heights[i++]+border_size) {
		for (size_t j=0,c=border_size;c<width;c+=cols_widths[j++]+border_size) {
			GridCellState& cell = _grid[i][j];
			auto cs = std::make_shared<CharState>();
			cs->background(cell.background);
			bool has_content = cell.content != nullptr;
			auto built_text = has_content ? cell.content->build() : CanvasPieces<CC>();
			for (size_t ri=r,rr=0;rr<rows_heights[i];ri++,++rr) {
				for (size_t ci=c,cc=0;cc<cols_widths[j];ci++,++cc) {
					canvas[ri][ci] = {EMPTY_SPACE, cs};
				}
			}
			for (auto &piece : built_text.pieces) {
				for (size_t ri=0,rr=piece.offset_rows;ri<piece.rows;ri++,++rr) {
					if (rr >= rows_heights[i]) break;
					for (size_t ci=0,cc=piece.offset_cols;ci<piece.cols;ci++,++cc) {
						if (cc >= cols_widths[j]) break;
						auto &cell = canvas[r+rr][c+cc];
						cell = piece.canvas[ri][ci];
						if (!termiq::style::is_color_defined(cell.state->background())) {
							cell.state->background(cs->background());
						}
					}
				}
			}
		}
	}

	CanvasPieces<CC> pieces{{{canvas, height, width}}};
	this->built(pieces);

	return pieces;
}

template<typename CC>
unsigned int termiq::canvas::Grid<CC>::get_width() {
	lazy_calc();
	unsigned int width = 0;
	for (auto c : _cols_widths) {
		width += c;
	}
	if (_border_type == BorderType::NONE) {
		return width;
	}
	return width + _cols + 1;
}

template<typename CC>
unsigned int termiq::canvas::Grid<CC>::get_height() {
	lazy_calc();
	unsigned int height = 0;
	for (auto c : _rows_heights) {
		height += c;
	}
	if (_border_type == BorderType::NONE) {
		return height;
	}
	return height + _rows + 1;
}

template<typename CC>
unsigned int termiq::canvas::Grid<CC>::min_width() {
	unsigned int width = 0;
	for (size_t c=0;c<_cols;++c) {
		width += get_column_defined_width(c);
	}
	if (_border_type != BorderType::NONE) {
		width += _cols + 1;
	}
	if (_width) {
		width = std::max(_width, width);
	}
	return width;
}

template<typename CC>
unsigned int termiq::canvas::Grid<CC>::min_height() {
	unsigned int height = 0;
	for (size_t r=0;r<_rows;r++) {
		height += get_row_defined_height(r);
	}
	if (_border_type != BorderType::NONE) {
		height += _rows + 1;
	}
	if (_height) {
		height = std::max(_height, height);
	}
	return height;
}

template<typename CC>
std::vector<unsigned int> termiq::canvas::Grid<CC>::calculate_column_sizes() {
	std::vector<unsigned int> widths(_cols, 0);
	unsigned int sum = 0;
	unsigned int added = 0;
	size_t zeros = 0;
	for (size_t c=0;c<_cols;c++) {
		widths[c] = get_column_defined_width(c);
		sum += widths[c];
		if (!widths[c]) {
			++zeros;
		}
	}
	if (_border_type != BorderType::NONE) {
		sum += _cols + 1;
	}
	if (_width && _width > sum) {
		unsigned int divs = zeros ? zeros : _cols;
		unsigned int rest = _width - sum;
		for (auto &w : widths) {
			if (zeros > 0 && w > 0) continue;
			w += rest/divs;
			added += rest/divs;
		}
		rest -= added;
		for (auto &w : widths) {
			if (rest-- == 0) break;
			++w;
		}
	}
	return widths;
}

template<typename CC>
std::vector<unsigned int> termiq::canvas::Grid<CC>::calculate_row_sizes() {
	std::vector<unsigned int> heights(_rows, 0);
	unsigned int sum = 0;
	unsigned int added = 0;
	size_t zeros = 0;
	for (size_t c=0;c<_rows;c++) {
		heights[c] = get_row_defined_height(c);
		sum += heights[c];
		if (!heights[c]) {
			++zeros;
		}
	}
	if (_border_type != BorderType::NONE) {
		sum += _rows + 1;
	}
	if (_height && _height > sum) {
		unsigned int divs = zeros ? zeros : _rows;
		unsigned int rest = _height - sum;
		for (auto &h : heights) {
			if (zeros > 0 && h > 0) continue;
			h += rest/divs;
			added += rest/divs;
		}
		rest -= added;
		for (auto &h : heights) {
			if (rest-- == 0) break;
			++h;
		}
	}
	return heights;
}

template<typename CC>
typename termiq::canvas::Grid<CC>::GridCellState& termiq::canvas::Grid<CC>::get_current_cell() {
	return _grid[_current_cell_r][_current_cell_c];
}

template<typename CC>
unsigned int termiq::canvas::Grid<CC>::get_column_defined_width(size_t index) {
	unsigned int width = 0;
	iterate_column(index, [&width](auto &cell){
		width = std::max(width, cell.width);
	});
	return width;
}

template<typename CC>
unsigned int termiq::canvas::Grid<CC>::get_row_defined_height(size_t index) {
	unsigned int height = 0;
	iterate_row(index, [&height](auto &cell){
		height = std::max(height, cell.height);
	});
	return height;
}

template<typename CC>
void termiq::canvas::Grid<CC>::calc() {
	_cols_widths = calculate_column_sizes();
	_rows_heights = calculate_row_sizes();
}

template<typename CC>
void termiq::canvas::Grid<CC>::lazy_calc() {
	if (_valid_calc) return;
	calc();
	_valid_calc = true;
}

template<typename CC>
void termiq::canvas::Grid<CC>::invalidate_calc() {
	_valid_calc = false;
}

template<typename CC>
bool termiq::canvas::Grid<CC>::is_valid_calc() {
	return _valid_calc;
}

template<typename CC>
void termiq::canvas::Grid<CC>::iterate_column(size_t index, std::function<void(GridCellState&)> callback) {
	for (size_t r=0;r<_rows;++r) {
		callback(_grid[r][index]);
	}
}

template<typename CC>
void termiq::canvas::Grid<CC>::iterate_row(size_t index, std::function<void(GridCellState&)> callback) {
	for (size_t c=0;c<_cols;c++) {
		callback(_grid[index][c]);
	}
}

template<typename CC>
typename termiq::canvas::Grid<CC>::GridBorders termiq::canvas::Grid<CC>::get_border(BorderType type) {
	using namespace draw_chars;
	switch(type) {
		case BorderType::SINGLE:
			return GridBorders{
				char_type(C_H.begin(), C_H.end()), char_type(C_V.begin(), C_V.end()), char_type(C_X.begin(), C_X.end()),
				char_type(C_HT.begin(), C_HT.end()), char_type(C_HB.begin(), C_HB.end()), char_type(C_VL.begin(), C_VL.end()),
				char_type(C_VR.begin(), C_VR.end()), char_type(C_TL.begin(), C_TL.end()), char_type(C_TR.begin(), C_TR.end()),
				char_type(C_BL.begin(), C_BL.end()), char_type(C_BR.begin(), C_BR.end())
			};
		case BorderType::DOUBLE:
			return GridBorders{
				char_type(C_H_DOUBLE.begin(), C_H_DOUBLE.end()), char_type(C_V_DOUBLE.begin(), C_V_DOUBLE.end()),
				char_type(C_X_DOUBLE.begin(), C_X_DOUBLE.end()), char_type(C_HT_DOUBLE.begin(), C_HT_DOUBLE.end()),
				char_type(C_HB_DOUBLE.begin(), C_HB_DOUBLE.end()), char_type(C_VL_DOUBLE.begin(), C_VL_DOUBLE.end()),
				char_type(C_VR_DOUBLE.begin(), C_VR_DOUBLE.end()), char_type(C_TL_DOUBLE.begin(), C_TL_DOUBLE.end()),
				char_type(C_TR_DOUBLE.begin(), C_TR_DOUBLE.end()), char_type(C_BL_DOUBLE.begin(), C_BL_DOUBLE.end()),
				char_type(C_BR_DOUBLE.begin(), C_BR_DOUBLE.end())
			};
		case BorderType::BOLD:
			return GridBorders{
				char_type(C_H_BOLD.begin(), C_H_BOLD.end()), char_type(C_V_BOLD.begin(), C_V_BOLD.end()),
				char_type(C_X_BOLD.begin(), C_X_BOLD.end()), char_type(C_HT_BOLD.begin(), C_HT_BOLD.end()),
				char_type(C_HB_BOLD.begin(), C_HB_BOLD.end()), char_type(C_VL_BOLD.begin(), C_VL_BOLD.end()),
				char_type(C_VR_BOLD.begin(), C_VR_BOLD.end()), char_type(C_TL_BOLD.begin(), C_TL_BOLD.end()),
				char_type(C_TR_BOLD.begin(), C_TR_BOLD.end()), char_type(C_BL_BOLD.begin(), C_BL_BOLD.end()),
				char_type(C_BR_BOLD.begin(), C_BR_BOLD.end())
			};
		case BorderType::ROUND:
			return GridBorders{
				char_type(C_H.begin(), C_H.end()), char_type(C_V.begin(), C_V.end()), char_type(C_X.begin(), C_X.end()),
				char_type(C_HT.begin(), C_HT.end()), char_type(C_HB.begin(), C_HB.end()), char_type(C_VL.begin(), C_VL.end()),
				char_type(C_VR.begin(), C_VR.end()), char_type(C_TL_ROUND.begin(), C_TL_ROUND.end()),
				char_type(C_TR_ROUND.begin(), C_TR_ROUND.end()), char_type(C_BL_ROUND.begin(), C_BL_ROUND.end()),
				char_type(C_BR_ROUND.begin(), C_BR_ROUND.end())
			};
		case BorderType::EMPTY:
		{
			std::basic_string<unsigned char> invis_ch{' '};
			char_type invis_char{invis_ch.begin(), invis_ch.end()};
			return GridBorders{invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char};
		}
		default:
			return GridBorders{
				char_type(C_H.begin(), C_H.end()), char_type(C_V.begin(), C_V.end()), char_type(C_X.begin(), C_X.end()),
				char_type(C_HT.begin(), C_HT.end()), char_type(C_HB.begin(), C_HB.end()), char_type(C_VL.begin(), C_VL.end()),
				char_type(C_VR.begin(), C_VR.end()), char_type(C_TL.begin(), C_TL.end()), char_type(C_TR.begin(), C_TR.end()),
				char_type(C_BL.begin(), C_BL.end()), char_type(C_BR.begin(), C_BR.end())
			};
	}
}
