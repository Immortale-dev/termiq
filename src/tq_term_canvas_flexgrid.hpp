template<typename CC>
void termiq::canvas::FlexGrid<CC>::suggest_width(uint32_t w) {
	_suggested_width = w;
	this->invalidate_calc();
}

template<typename CC>
void termiq::canvas::FlexGrid<CC>::suggest_height(uint32_t h) {
	_suggested_height = h;
	this->invalidate_calc();
}


template<typename CC>
std::vector<uint32_t> termiq::canvas::FlexGrid<CC>::get_optimal_cell_sizes(
	std::vector<uint32_t> &defined_sizes,
	std::vector<uint32_t> &text_sizes,
	std::vector<uint32_t> &min_content_sizes,
	uint32_t size
) const {
	size_t vals_count = defined_sizes.size();
	std::vector<uint32_t> cols_widths(vals_count);
	for (size_t c=0;c<vals_count;++c) {
		cols_widths[c] = defined_sizes[c] ? defined_sizes[c] : text_sizes[c];
	}
	if (!size) return cols_widths;

	std::vector<uint32_t*> fit_values;
	std::vector<uint32_t> fit_rates;

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
		distribute_rated<uint32_t>(fit_values, fit_rates, rest);
		return cols_widths;
	}

	for (size_t c=0;c<vals_count;c++) {
		if (!defined_sizes[c]) {
			cols_widths[c] = min_content_sizes[c];
		}
	}

	rest = size - summary(cols_widths);
	if (rest >= 0) {
		// Distribute proportionally between columns which has content, and which width was not set.
		for (size_t c=0;c<vals_count;++c) {
			if (!defined_sizes[c] && text_sizes[c]) {
				fit_values.push_back(&cols_widths[c]);
				fit_rates.push_back(text_sizes[c]+1);
			}
		}
		distribute_rated<uint32_t>(fit_values, fit_rates, rest);
		return cols_widths;
	}

	for (size_t c=0;c<vals_count;c++) {
		cols_widths[c] = min_content_sizes[c];
	}
	rest = size - summary(cols_widths);
	if (rest > 0) {
		// Distribute between columns with defined widths
		for (size_t c=0;c<vals_count;++c) {
			if (defined_sizes[c]) {
				fit_values.push_back(&cols_widths[c]);
				fit_rates.push_back(defined_sizes[c]);
			}
		}
		if (!fit_values.size()) {
			// If not columns with defined sizes found, distribute between all columns proportionally
			// to defined width + text width.
			for (size_t c=0;c<vals_count;++c) {
				fit_values.push_back(&cols_widths[c]);
				fit_rates.push_back(defined_sizes[c] + text_sizes[c] + 1);
			}
		}
		distribute_rated<uint32_t>(fit_values, fit_rates, rest);
	}
	return cols_widths;
}

template<typename CC>
std::vector<uint32_t> termiq::canvas::FlexGrid<CC>::calculate_column_sizes() const {
	uint32_t max_width = _suggested_width;
	if (!max_width || (this->_width && this->_width < max_width)) {
		max_width = this->_width;
	}
	if (max_width > 0 && this->_border_type != BorderType::NONE) {
		max_width -= this->_cols + 1;
	}
	std::vector<uint32_t> cols_defined_widths(this->_cols, 0);
	std::vector<uint32_t> cols_text_widths(this->_cols, 0);
	std::vector<uint32_t> cols_content_min_widths(this->_cols, 0);

	for (size_t c=0;c<this->_cols;++c) {
		cols_defined_widths[c] = this->get_column_defined_width(c);
		cols_text_widths[c] = get_column_content_width(c);
		cols_content_min_widths[c] = get_column_content_min_width(c);
	}

	return get_optimal_cell_sizes(cols_defined_widths, cols_text_widths, cols_content_min_widths, max_width);
}

template<typename CC>
std::vector<uint32_t> termiq::canvas::FlexGrid<CC>::calculate_row_sizes() const {
	uint32_t max_height = _suggested_height;
	if (!max_height || (this->_height && this->_height < max_height)) {
		max_height = this->_height;
	}
	if (max_height > 0 && this->_border_type != BorderType::NONE) {
		max_height -= this->_rows + 1;
	}
	std::vector<uint32_t> rows_defined_heights(this->_rows, 0);
	std::vector<uint32_t> rows_text_heights(this->_rows, 0);
	std::vector<uint32_t> rows_content_min_heights(this->_rows, 0);

	for (size_t r=0;r<this->_rows;++r) {
		rows_defined_heights[r] = this->get_row_defined_height(r);
		rows_text_heights[r] = get_row_content_height(r);
		rows_content_min_heights[r] = get_row_content_min_height(r);
	}

	return get_optimal_cell_sizes(rows_defined_heights, rows_text_heights, rows_content_min_heights, max_height);
}

template<typename CC>
uint32_t termiq::canvas::FlexGrid<CC>::get_column_content_min_width(size_t index) const {
	uint32_t width = 0;
	this->iterate_column(index, [&width](auto &cell){
		if (!cell.content) return;
		width = std::max(width, cell.content->min_width());
	});
	return width;
}

template<typename CC>
uint32_t termiq::canvas::FlexGrid<CC>::get_row_content_min_height(size_t index) const {
	uint32_t height = 0;
	this->iterate_row(index, [&height](auto &cell){
		if (!cell.content) return;
		height = std::max(height, cell.content->min_height());
	});
	return height;
}

template<typename CC>
uint32_t termiq::canvas::FlexGrid<CC>::get_column_content_width(size_t index) const {
	uint32_t width = 0;
	this->iterate_column(index, [&width](auto &cell){
		if (!cell.content) return;
		width = std::max(width, cell.content->get_width());
	});
	return width;
}

template<typename CC>
uint32_t termiq::canvas::FlexGrid<CC>::get_row_content_height(size_t index) const {
	uint32_t height = 0;
	this->iterate_row(index, [&height](auto &cell){
		if (!cell.content) return;
		height = std::max(height, cell.content->get_height());
	});
	return height;
}

template<typename CC>
template<typename IT>
void termiq::canvas::FlexGrid<CC>::distribute(std::vector<IT*> &values, IT size) const {
	std::vector<IT> rates;
	for (size_t i=0;i<values.size();++i) {
		rates.push_back(1);
	}
	fir_proportionally(values, rates, size);
}

template<typename CC>
template<typename IT>
void termiq::canvas::FlexGrid<CC>::distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size) const {
	if (!size) return;

	IT rates_size = 0;
	for (size_t i=0;i<values.size();++i) {
		rates_size += rates[i];
	}

	// Add values proportionally to rates.
	IT add_size = size;
	IT plus_values_size = 0;
	for (size_t i=0;i<values.size();++i) {
		IT plus = add_size * rates[i] / rates_size;
		*(values[i]) += plus;
		plus_values_size += plus;
	}

	// Fixes error from integer division.
	add_size = size - plus_values_size;
	for (auto *it : values) {
		if (add_size-- <= 0) {
			break;
		}
		(*it)++;
	}
}

template<typename CC>
template<typename IT>
IT termiq::canvas::FlexGrid<CC>::summary(std::vector<IT> &values) const {
	IT res = 0;
	for (size_t i=0;i<values.size();i++) {
		res += values[i];
	}
	return res;
}

template<typename CC>
void termiq::canvas::FlexGrid<CC>::calc() const {
	// Reset cells text sizes.
	for (size_t r=0;r<this->_rows;++r) {
		for (size_t c=0;c<this->_cols;++c) {
			if (this->_grid[r][c].content) {
				Flexible* f = dynamic_cast<Flexible*>(this->_grid[r][c].content);
				if (!f) continue;
				this->apply_suggest_width(*f, 0);
				this->apply_suggest_height(*f, 0);
			}
		}
	}

	std::vector<uint32_t> cols_widths = calculate_column_sizes();
	// Define cells text widths.
	for (size_t r=0;r<this->_rows;++r) {
		for (size_t c=0;c<this->_cols;++c) {
			if (this->_grid[r][c].content) {
				Flexible* f = dynamic_cast<Flexible*>(this->_grid[r][c].content);
				if (!f) continue;
				this->apply_suggest_width(*f, cols_widths[c]);
			}
		}
	}

	std::vector<uint32_t> rows_heights = calculate_row_sizes();
	// Define cells text heights.
	for (size_t r=0;r<this->_rows;++r) {
		for (size_t c=0;c<this->_cols;++c) {
			if (this->_grid[r][c].content) {
				Flexible* f = dynamic_cast<Flexible*>(this->_grid[r][c].content);
				if (!f) continue;
				this->apply_suggest_height(*f, rows_heights[r]);
			}
		}
	}

	this->_cols_widths = cols_widths;
	this->_rows_heights = rows_heights;
}
