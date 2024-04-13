#include "tq_term_unicode_draws.h"

template<typename CC>
termiq::GridBuilder<CC>::GridBuilder(unsigned int rows, unsigned int cols):
	FlexibleContentBuilder<CC>(), _rows(rows), _cols(cols), _grid(_rows, std::vector<GridCellState>(_cols)) {}

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
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_content(FlexibleContentBuilder<CC>* content) {
	auto &cell = get_current_cell();
	cell.content = content;
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_width(unsigned int width) {
	_width = width;
	invalidate_calc();
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_height(unsigned int height) {
	_height = height;
	invalidate_calc();
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_width(unsigned int width) {
	auto &cell = get_current_cell();
	cell.width = width;
	invalidate_calc();
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_cell_height(unsigned int height) {
	auto &cell = get_current_cell();
	cell.height = height;
	invalidate_calc();
	return *this;
}

template<typename CC>
typename termiq::GridBuilder<CC>& termiq::GridBuilder<CC>::set_border_type(BorderType type) {
	_border_type = type;
	invalidate_calc();
	return *this;
}

template<typename CC>
typename termiq::CanvasPieces<CC> termiq::GridBuilder<CC>::build() {
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
	}

	// Fill cell backgrounds and text.
	size_t border_size = _border_type == BorderType::NONE ? 0 : 1;
	for (size_t i=0,r=border_size;r<height;r+=rows_heights[i++]+border_size) {
		for (size_t j=0,c=border_size;c<width;c+=cols_widths[j++]+border_size) {
			GridCellState& cell = _grid[i][j];
			auto cs = std::make_shared<CharState>();
			cs->background = cell.background;
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
						if (!termiq::style::is_color_defined(cell.state->background)) {
							cell.state->background = cs->background;
						}
					}
				}
			}
		}
	}

	return {{{canvas, height, width}}};
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_width() {
	lazy_calc();
	auto width = summary(_cols_widths);
	if (_border_type == BorderType::NONE) {
		return width;
	}
	return width + _cols + 1;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_height() {
	lazy_calc();
	auto height = summary(_rows_heights);
	if (_border_type == BorderType::NONE) {
		return height;
	}
	return height + _rows + 1;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::min_width() {
	unsigned int width = 0;
	for (size_t c=0;c<_cols;++c) {
		width += get_column_content_min_width(c);
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
unsigned int termiq::GridBuilder<CC>::min_height() {
	unsigned int height = 0;
	for (size_t r=0;r<_rows;r++) {
		height += get_row_content_min_height(r);
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
void termiq::GridBuilder<CC>::suggest_width(unsigned int w) {
	_suggested_width = w;
	invalidate_calc();
}

template<typename CC>
void termiq::GridBuilder<CC>::suggest_height(unsigned int h) {
	_suggested_height = h;
	invalidate_calc();
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
	unsigned int max_width = _suggested_width;
	if (!max_width || (_width && _width < max_width)) {
		max_width = _width;
	}
	if (max_width > 0 && _border_type != BorderType::NONE) {
		max_width -= _cols + 1;
	}
	std::vector<unsigned int> cols_defined_widths(_cols, 0);
	std::vector<unsigned int> cols_text_widths(_cols, 0);

	for (size_t c=0;c<_cols;++c) {
		cols_defined_widths[c] = get_column_defined_width(c);
		cols_text_widths[c] = get_column_content_width(c);
	}

	return get_optimal_cell_sizes(cols_defined_widths, cols_text_widths, max_width);
}

template<typename CC>
std::vector<unsigned int> termiq::GridBuilder<CC>::calculate_row_sizes() {
	unsigned int max_height = _suggested_height;
	if (!max_height) {
		max_height = _height;
	}
	if (max_height > 0 && _border_type != BorderType::NONE) {
		max_height -= _rows + 1;
	}
	std::vector<unsigned int> rows_defined_heights(_rows, 0);
	std::vector<unsigned int> rows_text_heights(_rows, 0);

	for (size_t r=0;r<_rows;++r) {
		rows_defined_heights[r] = get_row_defined_height(r);
		rows_text_heights[r] = get_row_content_height(r);
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
unsigned int termiq::GridBuilder<CC>::get_column_content_min_width(size_t index) {
	unsigned int width = 0;
	iterate_column(index, [&width](auto &cell){
		if (!cell.content) return;
		width = std::max(width, cell.content->min_width());
	});
	return width;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_row_content_min_height(size_t index) {
	unsigned int height = 0;
	iterate_row(index, [&height](auto &cell){
		if (!cell.content) return;
		height = std::max(height, cell.content->min_height());
	});
	return height;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_column_content_width(size_t index) {
	unsigned int width = 0;
	iterate_column(index, [&width](auto &cell){
		if (!cell.content) return;
		width = std::max(width, cell.content->get_width());
	});
	return width;
}

template<typename CC>
unsigned int termiq::GridBuilder<CC>::get_row_content_height(size_t index) {
	unsigned int height = 0;
	iterate_row(index, [&height](auto &cell){
		if (!cell.content) return;
		height = std::max(height, cell.content->get_height());
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
		if (add_size-- <= 0) {
			break;
		}
		(*it)++;
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
void termiq::GridBuilder<CC>::calc() {
	// Reset cells text sizes.
	for (size_t r=0;r<_rows;++r) {
		for (size_t c=0;c<_cols;++c) {
			if (_grid[r][c].content) {
				this->apply_suggest_width(*_grid[r][c].content, 0);
				this->apply_suggest_height(*_grid[r][c].content, 0);
			}
		}
	}

	std::vector<unsigned int> cols_widths = calculate_column_sizes();
	// Define cells text widths.
	for (size_t r=0;r<_rows;++r) {
		for (size_t c=0;c<_cols;++c) {
			if (_grid[r][c].content) {
				this->apply_suggest_width(*_grid[r][c].content, cols_widths[c]);
			}
		}
	}

	std::vector<unsigned int> rows_heights = calculate_row_sizes();
	// Define cells text heights.
	for (size_t r=0;r<_rows;++r) {
		for (size_t c=0;c<_cols;++c) {
			if (_grid[r][c].content) {
				this->apply_suggest_height(*_grid[r][c].content, rows_heights[r]);
			}
		}
	}

	_cols_widths = cols_widths;
	_rows_heights = rows_heights;
}

template<typename CC>
void termiq::GridBuilder<CC>::lazy_calc() {
	if (_valid_calc) return;
	calc();
	_valid_calc = true;
}

template<typename CC>
void termiq::GridBuilder<CC>::invalidate_calc() {
	_valid_calc = false;
}

template<typename CC>
bool termiq::GridBuilder<CC>::is_valid_calc() {
	return _valid_calc;
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
