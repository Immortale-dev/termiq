#include "tq_term_unicode_draws.h"

template<typename CC>
termiq::GridBuilder<CC>::GridBuilder(unsigned int rows, unsigned int cols): _rows(rows), _cols(cols), _grid(_rows, std::vector<GridCellState>(_cols)) {}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::select_cell(size_t c) {
	_current_cell_c = c;
	return *this;
};

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::select_cell(size_t r, size_t c) {
	_current_cell_r = r;
	_current_cell_c = c;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_background_color(termiq::style::Color color) {
	for (size_t r=0;r<_rows;r++) {
		for (size_t c=0;c<_cols;c++) {
			_grid[r][c].background = color;
		}
	}
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_border_foreground_color(termiq::style::Color color) {
	_border_foreground_color = color;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_border_background_color(termiq::style::Color color) {
	_border_background_color = color;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_background_color(termiq::style::Color color) {
	get_current_cell().background = color;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_text(TextBuilder<CC> text) {
	auto &cell = get_current_cell();
	cell.text = text;
	cell.has_text = true;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_width(unsigned int width) {
	_width = width;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_height(unsigned int height) {
	_height = height;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_width(unsigned int width) {
	auto &cell = get_current_cell();
	cell.width = width;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_height(unsigned int height) {
	auto &cell = get_current_cell();
	cell.height = height;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_border_type(BorderType type) {
	_border_type = type;
	return *this;
}

template<typename CC>
typename termiq::CanvasPieces<CC> termiq::GridBuilder<CC>::build() {
	// Reset cells text sizes.
	for (size_t r=0;r<_rows;++r) {
		for (size_t c=0;c<_cols;++c) {
			if (_grid[r][c].has_text) {
				_grid[r][c].text.set_width(0);
				_grid[r][c].text.set_height(0);
			}
		}
	}

	std::vector<unsigned int> cols_widths = calculate_column_sizes();
	// Define cells text widths.
	for (size_t r=0;r<_rows;++r) {
		for (size_t c=0;c<_cols;++c) {
			if (_grid[r][c].has_text) {
				_grid[r][c].text.set_width(cols_widths[c]);
			}
		}
	}

	std::vector<unsigned int> rows_heights = calculate_row_sizes();
	// Define cells text heights.
	for (size_t r=0;r<_rows;++r) {
		for (size_t c=0;c<_cols;++c) {
			if (_grid[r][c].has_text) {
				_grid[r][c].text.set_height(rows_heights[r]);
			}
		}
	}

	GridBorders border = get_border(_border_type);
	unsigned int width = summary(cols_widths) + _cols + 1;
	unsigned int height = summary(rows_heights) + _rows + 1;
	std::vector<std::vector<CC>> canvas(height, std::vector<CC>(width));

	// Fill in borders.
	auto cs = std::make_shared<CharState>();
	if (_border_type == BorderType::SINGLE_ASCII) {
		cs->special = true;
	}
	cs->foreground = _border_foreground_color;
	cs->background = _border_background_color;

	for (size_t r=1;r<height-1;++r) {
		for (size_t l=0,c=0;l<=_cols;c+=cols_widths[l++]+1) {
			canvas[r][c] = {border.C_V, cs};
			if (l == _cols) break;
		}
	}
	for (size_t c=1;c<width-1;++c) {
		for (size_t l=0,r=0;l<=_rows;r+=rows_heights[l++]+1) {
			canvas[r][c] = {border.C_H, cs};
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

	// Fill cell backgrounds and text.
	for (size_t i=0,r=1;r<height;r+=rows_heights[i++]+1) {
		for (size_t j=0,c=1;c<width;c+=cols_widths[j++]+1) {
			GridCellState& cell = _grid[i][j];
			auto cs = std::make_shared<CharState>();
			cs->background = cell.background;
			auto built_text = cell.text.build();
			if (built_text.pieces.size() && !termiq::style::is_color_defined(built_text.pieces[0].canvas[0][0].state->background)) {
				built_text.pieces[0].canvas[0][0].state->background = cs->background;
			}
			for (size_t ri=r,rr=0;rr<rows_heights[i];ri++,++rr) {
				for (size_t ci=c,cc=0;cc<cols_widths[j];ci++,++cc) {
					if (rr < built_text.pieces.size() && cc < built_text.pieces[i].cols) {
						canvas[ri][ci] = built_text.pieces[rr].canvas[0][cc];
						continue;
					}
					canvas[ri][ci] = {EMPTY_SPACE, cs};
				}
			}
		}
	}

	return {{{canvas, height, width}}};
}

template<typename CC>
std::vector<unsigned int> termiq::GridBuilder<CC>::get_optimal_cell_sizes(std::vector<unsigned int>& defined_sizes, std::vector<unsigned int>& text_sizes, unsigned int size) {
	size_t vals_count = defined_sizes.size();
	std::vector<unsigned int> cols_widths(vals_count);
	for (size_t c=0;c<vals_count;++c) {
		cols_widths[c] = defined_sizes[c] ? defined_sizes[c] : text_sizes[c];
	}
	if (!size) return cols_widths;

	std::vector<unsigned int*> fit_values;
	std::vector<unsigned int> fit_rates;

	int rest = size - summary(cols_widths);
	if (rest >= 0) {
		// The rest of table width is distributed proportionally to the amount of text between the columns which
		// width was not. If non of such columns exist, the width is equaly distributed between all of the columns.
		for (size_t c=0;c<vals_count;++c) {
			if (!defined_sizes[c]) {
				fit_values.push_back(&cols_widths[c]);
				fit_rates.push_back(text_sizes[c]+1);
			}
		}
		if (fit_values.empty()) {
			for (size_t c=0;c<vals_count;++c) {
				fit_values.push_back(&cols_widths[c]);
				fit_rates.push_back(text_sizes[c]+1);
			}
		}
		distribute_rated<unsigned int>(fit_values, fit_rates, rest);
		return cols_widths;
	}

	for (size_t c=0;c<_cols;c++) {
		if (defined_sizes[c]) {
			cols_widths[c] = std::min(defined_sizes[c], text_sizes[c]);
		}
	}
	rest = size - summary(cols_widths);
	if (rest > 0) {
		// Distribute between the columns which default width is defined proportionally to this defined width.
		for (size_t c=0;c<vals_count;++c) {
			if (defined_sizes[c]) {
				fit_values.push_back(&cols_widths[c]);
				fit_rates.push_back(text_sizes[c]+1);
			}
		}
		distribute_rated<unsigned int>(fit_values, fit_rates, rest);
		return cols_widths;
	}

	for (size_t c=0;c<_cols;c++) {
		cols_widths[c] = text_sizes[c] ? 1 : 0;
	}
	rest = size - summary(cols_widths);
	if (rest > 0) {
		// Distribute between all columns proportionally to defined width + text width.
		for (size_t c=0;c<vals_count;++c) {
			fit_values.push_back(&cols_widths[c]);
			fit_rates.push_back(defined_sizes[c] + text_sizes[c] + 1);
		}
		distribute_rated<unsigned int>(fit_values, fit_rates, rest);
	}
	return cols_widths;
}

template<typename CC>
std::vector<unsigned int> termiq::GridBuilder<CC>::calculate_column_sizes() {
	unsigned int max_width = _width ? _width - _cols - 1 : 0;
	std::vector<unsigned int> cols_defined_widths(_cols, 0);
	std::vector<unsigned int> cols_text_widths(_cols, 0);

	for (size_t c=0;c<_cols;++c) {
		cols_defined_widths[c] = get_column_defined_width(c);
		cols_text_widths[c] = get_column_text_width(c);
	}

	return get_optimal_cell_sizes(cols_defined_widths, cols_text_widths, max_width);
}

template<typename CC>
std::vector<unsigned int> termiq::GridBuilder<CC>::calculate_row_sizes() {
	unsigned int max_height = _height ? _height - _rows - 1 : 0;
	std::vector<unsigned int> rows_defined_heights(_rows, 0);
	std::vector<unsigned int> rows_text_heights(_rows, 0);

	for (size_t r=0;r<_rows;++r) {
		rows_defined_heights[r] = get_row_defined_height(r);
		rows_text_heights[r] = get_row_text_height(r);
	}

	return get_optimal_cell_sizes(rows_defined_heights, rows_text_heights, max_height);
}

template<typename CC>
typename termiq::GridBuilder<CC>::GridCellState& termiq::GridBuilder<CC>::get_current_cell() {
	return _grid[_current_cell_r][_current_cell_c];
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_column_defined_width(size_t index) {
	unsigned int width = 0;
	iterate_column(index, [&width](auto &cell){
		width = std::max(width, cell.width);
	});
	return width;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_row_defined_height(size_t index) {
	unsigned int height = 0;
	iterate_row(index, [&height](auto &cell){
		height = std::max(height, cell.height);
	});
	return height;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_column_text_width(size_t index) {
	unsigned int width = 0;
	iterate_column(index, [&width](auto &cell){
		if (!cell.has_text) return;
		width = std::max(width, cell.text.text_width());
	});
	return width;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_row_text_height(size_t index) {
	unsigned int height = 0;
	iterate_row(index, [&height](auto &cell){
		if (!cell.has_text) return;
		height = std::max(height, cell.text.text_height());
	});
	return height;
}

template<typename CC>
template<typename IT>
void termiq::GridBuilder<CC>::distribute(std::vector<IT*> &values, IT size) {
	std::vector<IT> rates;
	for (size_t i=0;i<values.size();++i) {
		rates.push_back(1);
	}
	fir_proportionally(values, rates, size);
}

template<typename CC>
template<typename IT>
void termiq::GridBuilder<CC>::distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size) {
	IT values_size = 0;
	IT rates_size = 0;
	for (size_t i=0;i<values.size();++i) {
		values_size += *(values[i]);
		rates_size += rates[i];
	}

	// Add values proportionally to rates.
	IT add_size = size;
	for (size_t i=0;i<values.size();++i) {
		IT plus = add_size * rates[i] / rates_size;
		*(values[i]) += plus;
		values_size += plus;
	}

	// Fixes error from integer division.
	add_size = size - values_size;
	for (auto *it : values) {
		if (add_size-- > 0) {
			(*it)++;
		}
	}
}

template<typename CC>
template<typename IT>
IT termiq::GridBuilder<CC>::summary(std::vector<IT> &values) {
	IT res = 0;
	for (size_t i=0;i<values.size();i++) {
		res += values[i];
	}
	return res;
}

template<typename CC>
void termiq::GridBuilder<CC>::iterate_column(size_t index, std::function<void(GridCellState&)> callback) {
	for (size_t r=0;r<_rows;++r) {
		callback(_grid[r][index]);
	}
}

template<typename CC>
void termiq::GridBuilder<CC>::iterate_row(size_t index, std::function<void(GridCellState&)> callback) {
	for (size_t c=0;c<_cols;c++) {
		callback(_grid[index][c]);
	}
}

template<typename CC>
typename termiq::GridBuilder<CC>::GridBorders termiq::GridBuilder<CC>::get_border(BorderType type) {
	if (type == BorderType::SINGLE_ASCII) {
		using namespace alt_chars;
		return GridBorders{C_H, C_V, C_X, C_HT, C_HB, C_VL, C_VR, C_TL, C_TR, C_BL, C_BR};
	} else {
		using namespace draw_chars;
		switch(type) {
			case BorderType::SINGLE:
				return GridBorders{C_H, C_V, C_X, C_HT, C_HB, C_VL, C_VR, C_TL, C_TR, C_BL, C_BR};
			case BorderType::DOUBLE:
				return GridBorders{C_H_DOUBLE, C_V_DOUBLE, C_X_DOUBLE, C_HT_DOUBLE, C_HB_DOUBLE, C_VL_DOUBLE, C_VR_DOUBLE, C_TL_DOUBLE, C_TR_DOUBLE, C_BL_DOUBLE, C_BR_DOUBLE};
			case BorderType::BOLD:
				return GridBorders{C_H_BOLD, C_V_BOLD, C_X_BOLD, C_HT_BOLD, C_HB_BOLD, C_VL_BOLD, C_VR_BOLD, C_TL_BOLD, C_TR_BOLD, C_BL_BOLD, C_BR_BOLD};
			case BorderType::ROUND:
				return GridBorders{C_H, C_V, C_X, C_HT, C_HB, C_VL, C_VR, C_TL_ROUND, C_TR_ROUND, C_BL_ROUND, C_BR_ROUND};
			case BorderType::INVISIBLE:
			{
				char_type invis_char = ' ';
				return GridBorders{invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char, invis_char};
			}
			default:
				return GridBorders{C_H, C_V, C_X, C_HT, C_HB, C_VL, C_VR, C_TL, C_TR, C_BL, C_BR};
		}
	}
}
