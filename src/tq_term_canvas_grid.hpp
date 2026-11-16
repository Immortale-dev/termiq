#include "tq_term_unicode_draws.h"

template<typename CC>
termiq::canvas::Grid<CC>::Grid(uint32_t rows, uint32_t cols) : Content<CC>(), _rows(rows), _cols(cols), _grid(_rows, std::vector<GridCellState>(_cols)) {}

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
void termiq::canvas::Grid<CC>::set_background_color(termiq::Color color) {
	for (size_t r=0;r<_rows;r++) {
		for (size_t c=0;c<_cols;c++) {
			_grid[r][c].background = color;
		}
	}
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_border_foreground_color(termiq::Color color) {
	_border_foreground_color = color;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_border_background_color(termiq::Color color) {
	_border_background_color = color;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_background_color(termiq::Color color) {
	get_current_cell().background = color;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_content(Content<CC>* content) {
	auto &cell = get_current_cell();
	cell.content = content;
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_width(uint32_t width) {
	_width = width;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_height(uint32_t height) {
	_height = height;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_width(uint32_t width) {
	auto &cell = get_current_cell();
	cell.width = width;
	invalidate_calc();
}

template<typename CC>
void termiq::canvas::Grid<CC>::set_cell_height(uint32_t height) {
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
termiq::canvas::CanvasPiece<CC> termiq::canvas::Grid<CC>::build() const {
	// Calc column and rows sizes.
	lazy_calc();

	const auto& cols_widths = _cols_widths;
	const auto& rows_heights = _rows_heights;

	GridBorders border = get_border(_border_type);
	uint32_t width = get_width();
	uint32_t height = get_height();
	std::vector<std::vector<CC>> canvas(height, std::vector<CC>(width));

	// Fill in borders.
	if (_border_type != BorderType::NONE) {
		Style cs;
		cs.foreground(_border_foreground_color);
		cs.background(_border_background_color);

		for (size_t r=1;r<height-1;++r) {
			for (size_t l=0,c=0;l<=_cols;c+=cols_widths[l++]+1) {
				canvas[r][c] = {Char{&*border.C_V.begin(), &*border.C_V.end()}, cs};
				if (l == _cols) break;
			}
		}
		for (size_t c=1;c<width-1;++c) {
			for (size_t l=0,r=0;l<=_rows;r+=rows_heights[l++]+1) {
				canvas[r][c] = {Char{&*border.C_H.begin(), &*border.C_H.end()}, cs};
				if (l == _rows) break;
			}
		}

		for (size_t i=0,r=0;i<=_rows;r+=rows_heights[i++]+1) {
			for (size_t j=0,c=0;j<=_cols;c+=cols_widths[j++]+1) {
				Char ct = border.C_X;
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
			const GridCellState& cell = _grid[i][j];
			Style cs;
			cs.background(cell.background);
			bool has_content = cell.content != nullptr;
			auto built_text = has_content ? cell.content->build() : CanvasPiece<CC>();
			for (size_t ri=r,rr=0;rr<rows_heights[i];ri++,++rr) {
				for (size_t ci=c,cc=0;cc<cols_widths[j];ci++,++cc) {
					canvas[ri][ci] = {EMPTY_SPACE, cs};
				}
			}
			for (auto &layer : built_text.layers()) {
				for (size_t ri=0,rr=layer.row();ri<layer.rows();ri++,++rr) {
					if (rr >= rows_heights[i]) break;
					for (size_t ci=0,cc=layer.col();ci<layer.cols();ci++,++cc) {
						if (cc >= cols_widths[j]) break;
						auto &cell = canvas[r+rr][c+cc];
						cell = layer.cell(ri,ci);
						if (!termiq::style::is_color_defined(cell.style().background())) {
							cell.style().background(cs.background());
						}
					}
				}
			}
		}
	}

	CanvasPiece<CC> pieces({CanvasLayer<CC>(std::move(canvas), height, width)});
	this->built(pieces);

	return pieces;
}

template<typename CC>
uint32_t termiq::canvas::Grid<CC>::get_width() const {
	lazy_calc();
	uint32_t width = 0;
	for (auto c : _cols_widths) {
		width += c;
	}
	if (_border_type == BorderType::NONE) {
		return width;
	}
	return width + _cols + 1;
}

template<typename CC>
uint32_t termiq::canvas::Grid<CC>::get_height() const {
	lazy_calc();
	uint32_t height = 0;
	for (auto c : _rows_heights) {
		height += c;
	}
	if (_border_type == BorderType::NONE) {
		return height;
	}
	return height + _rows + 1;
}

template<typename CC>
uint32_t termiq::canvas::Grid<CC>::min_width() const {
	uint32_t width = 0;
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
uint32_t termiq::canvas::Grid<CC>::min_height() const {
	uint32_t height = 0;
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
std::vector<uint32_t> termiq::canvas::Grid<CC>::calculate_column_sizes() const {
	std::vector<uint32_t> widths(_cols, 0);
	uint32_t sum = 0;
	uint32_t added = 0;
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
		uint32_t divs = zeros ? zeros : _cols;
		uint32_t rest = _width - sum;
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
std::vector<uint32_t> termiq::canvas::Grid<CC>::calculate_row_sizes() const {
	std::vector<uint32_t> heights(_rows, 0);
	uint32_t sum = 0;
	uint32_t added = 0;
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
		uint32_t divs = zeros ? zeros : _rows;
		uint32_t rest = _height - sum;
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
uint32_t termiq::canvas::Grid<CC>::get_column_defined_width(size_t index) const {
	uint32_t width = 0;
	iterate_column(index, [&width](auto &cell){
		width = std::max(width, cell.width);
	});
	return width;
}

template<typename CC>
uint32_t termiq::canvas::Grid<CC>::get_row_defined_height(size_t index) const {
	uint32_t height = 0;
	iterate_row(index, [&height](auto &cell){
		height = std::max(height, cell.height);
	});
	return height;
}

template<typename CC>
void termiq::canvas::Grid<CC>::calc() const {
	_cols_widths = calculate_column_sizes();
	_rows_heights = calculate_row_sizes();
}

template<typename CC>
void termiq::canvas::Grid<CC>::lazy_calc() const {
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
void termiq::canvas::Grid<CC>::iterate_column(size_t index, std::function<void(const GridCellState&)> callback) const {
	for (size_t r=0;r<_rows;++r) {
		callback(_grid[r][index]);
	}
}

template<typename CC>
void termiq::canvas::Grid<CC>::iterate_row(size_t index, std::function<void(const GridCellState&)> callback) const {
	for (size_t c=0;c<_cols;c++) {
		callback(_grid[index][c]);
	}
}

template<typename CC>
typename termiq::canvas::Grid<CC>::GridBorders termiq::canvas::Grid<CC>::get_border(BorderType type) const {
	using namespace draw_chars;
	switch(type) {
		case BorderType::SINGLE:
			return GridBorders{
				Char(C_H.begin(), C_H.end()), Char(C_V.begin(), C_V.end()), Char(C_X.begin(), C_X.end()),
				Char(C_HT.begin(), C_HT.end()), Char(C_HB.begin(), C_HB.end()), Char(C_VL.begin(), C_VL.end()),
				Char(C_VR.begin(), C_VR.end()), Char(C_TL.begin(), C_TL.end()), Char(C_TR.begin(), C_TR.end()),
				Char(C_BL.begin(), C_BL.end()), Char(C_BR.begin(), C_BR.end())
			};
		case BorderType::DOUBLE:
			return GridBorders{
				Char(C_H_DOUBLE.begin(), C_H_DOUBLE.end()), Char(C_V_DOUBLE.begin(), C_V_DOUBLE.end()),
				Char(C_X_DOUBLE.begin(), C_X_DOUBLE.end()), Char(C_HT_DOUBLE.begin(), C_HT_DOUBLE.end()),
				Char(C_HB_DOUBLE.begin(), C_HB_DOUBLE.end()), Char(C_VL_DOUBLE.begin(), C_VL_DOUBLE.end()),
				Char(C_VR_DOUBLE.begin(), C_VR_DOUBLE.end()), Char(C_TL_DOUBLE.begin(), C_TL_DOUBLE.end()),
				Char(C_TR_DOUBLE.begin(), C_TR_DOUBLE.end()), Char(C_BL_DOUBLE.begin(), C_BL_DOUBLE.end()),
				Char(C_BR_DOUBLE.begin(), C_BR_DOUBLE.end())
			};
		case BorderType::BOLD:
			return GridBorders{
				Char(C_H_BOLD.begin(), C_H_BOLD.end()), Char(C_V_BOLD.begin(), C_V_BOLD.end()),
				Char(C_X_BOLD.begin(), C_X_BOLD.end()), Char(C_HT_BOLD.begin(), C_HT_BOLD.end()),
				Char(C_HB_BOLD.begin(), C_HB_BOLD.end()), Char(C_VL_BOLD.begin(), C_VL_BOLD.end()),
				Char(C_VR_BOLD.begin(), C_VR_BOLD.end()), Char(C_TL_BOLD.begin(), C_TL_BOLD.end()),
				Char(C_TR_BOLD.begin(), C_TR_BOLD.end()), Char(C_BL_BOLD.begin(), C_BL_BOLD.end()),
				Char(C_BR_BOLD.begin(), C_BR_BOLD.end())
			};
		case BorderType::ROUND:
			return GridBorders{
				Char(C_H.begin(), C_H.end()), Char(C_V.begin(), C_V.end()), Char(C_X.begin(), C_X.end()),
				Char(C_HT.begin(), C_HT.end()), Char(C_HB.begin(), C_HB.end()), Char(C_VL.begin(), C_VL.end()),
				Char(C_VR.begin(), C_VR.end()), Char(C_TL_ROUND.begin(), C_TL_ROUND.end()),
				Char(C_TR_ROUND.begin(), C_TR_ROUND.end()), Char(C_BL_ROUND.begin(), C_BL_ROUND.end()),
				Char(C_BR_ROUND.begin(), C_BR_ROUND.end())
			};
		case BorderType::EMPTY:
		{
			std::basic_string<unsigned char> invis_ch{' '};
			Char invis_char{invis_ch.begin(), invis_ch.end()};
			return GridBorders{invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char};
		}
		default:
			return GridBorders{
				Char(C_H.begin(), C_H.end()), Char(C_V.begin(), C_V.end()), Char(C_X.begin(), C_X.end()),
				Char(C_HT.begin(), C_HT.end()), Char(C_HB.begin(), C_HB.end()), Char(C_VL.begin(), C_VL.end()),
				Char(C_VR.begin(), C_VR.end()), Char(C_TL.begin(), C_TL.end()), Char(C_TR.begin(), C_TR.end()),
				Char(C_BL.begin(), C_BL.end()), Char(C_BR.begin(), C_BR.end())
			};
	}
}
