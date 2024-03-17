template<typename CC>
termiq::TextBuilder<CC>::TextBuilder(const char_type* txt) {
	for(size_t i=0;txt[i]!=STRING_TERMINATOR;++i) {
		_txt.push_back(txt[i]);
	}
	invalidate_lines();
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_foreground_color(termiq::style::Color &&color) {
	_foreground = color;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_background_color(termiq::style::Color &&color) {
	_background = color;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_bold() {
	_bold = true;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_italic() {
	_italic = true;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_dim() {
	_dim = true;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_underline() {
	_underline = true;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_inverse() {
	_inverse = true;
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_width(unsigned int w) {
	_width = w;
	invalidate_lines();
	return *this;
}

template<typename CC>
typename termiq::TextBuilder<CC>& termiq::TextBuilder<CC>::set_height(unsigned int h) {
	_height = h;
	invalidate_lines();
	return *this;
}

template<typename CC>
typename termiq::CanvasPiece<CC> termiq::TextBuilder<CC>::build() {
	std::vector<std::vector<CC>> all_lines;
	auto cs = std::make_shared<CharState>();
	cs->foreground = _foreground;
	cs->background = _background;
	cs->bold = _bold;
	cs->italic = _italic;
	cs->dim = _dim;
	cs->underline = _underline;
	cs->inverse = _inverse;
	unsigned int width = text_width();
	unsigned int lines_count = text_height();
	auto& built_lines = get_lines();

	for (size_t l=0;l<lines_count;l++) {
		std::vector<CC> line;
		for (size_t i=0;i<built_lines[l].size();++i) {
			line.push_back({built_lines[l][i], cs});
		}
		all_lines.push_back(line);
	}
	return {lines_count, width, all_lines};
}

template<typename CC>
unsigned int termiq::TextBuilder<CC>::text_width() {
	lazy_calculate_lines();
	return _text_width;
}

template<typename CC>
unsigned int termiq::TextBuilder<CC>::text_height() {
	lazy_calculate_lines();
	return _text_height;
}

template<typename CC>
const typename termiq::TextBuilder<CC>::CanvasGrid& termiq::TextBuilder<CC>::get_lines() {
	lazy_calculate_lines();
	return _lines;
}

template<typename CC>
void termiq::TextBuilder<CC>::calculate_lines() {
	_lines.resize(0);
	size_t max_w = 0;
	size_t max_h = 0;
	std::vector<char_type> line;
	for (size_t i=0;;++i) {
		if ( i == _txt.size() || (_width && _width <= line.size()) || _txt[i] == LINE_TERMINATOR) {
			max_w = std::max(max_w, line.size());
			++max_h;
			_lines.push_back(line);
			line.resize(0);
		}
		if (_height && _lines.size() >= _height) break;
		if (i >= _txt.size()) break;
		if (_txt[i] == LINE_TERMINATOR) continue;
		line.push_back(_txt[i]);
	}
	_text_width = max_w;
	_text_height = max_h;
}

template<typename CC>
void termiq::TextBuilder<CC>::invalidate_lines() {
	_valid_lines = false;
}

template<typename CC>
void termiq::TextBuilder<CC>::lazy_calculate_lines() {
	if (_valid_lines) return;
	calculate_lines();
	_valid_lines = true;
}

template<typename CC>
bool termiq::TextBuilder<CC>::is_valid_lines() {
	return _valid_lines;
}
