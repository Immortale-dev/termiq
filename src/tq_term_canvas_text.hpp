template<typename CC>
termiq::canvas::Text<CC>::Text() : Content<CC>() {}

template<typename CC>
termiq::canvas::Text<CC>::Text(std::span<Char> line) : Text({line}) {}

template<typename CC>
termiq::canvas::Text<CC>::Text(std::span<std::vector<Char>> multiline) : Content<CC>(), _txt(multiline) {}

template<typename CC>
termiq::canvas::Text<CC>::Text(std::vector<Char>&& line) : Text({line}) {}

template<typename CC>
termiq::canvas::Text<CC>::Text(std::vector<std::vector<Char>>&& multiline) : _txt(std::move(multiline)) {}

template<typename CC>
void termiq::canvas::Text<CC>::set_foreground_color(termiq::Color color) {
	_foreground = color;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_background_color(termiq::Color color) {
	_background = color;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_bold() {
	_bold = true;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_italic() {
	_italic = true;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_dim() {
	_dim = true;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_underline() {
	_underline = true;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_inverse() {
	_inverse = true;
}

template<typename CC>
void termiq::canvas::Text<CC>::set_width(uint32_t w) {
	_width = w;
	invalidate_lines();
}

template<typename CC>
void termiq::canvas::Text<CC>::set_height(uint32_t h) {
	_height = h;
	invalidate_lines();
}

template<typename CC>
typename termiq::canvas::CanvasPiece<CC> termiq::canvas::Text<CC>::build() const {
	Style cs;
	cs.foreground(_foreground);
	cs.background(_background);
	cs.bold(_bold);
	cs.italic(_italic);
	cs.dim(_dim);
	cs.underline(_underline);
	cs.inverse(_inverse);
	uint32_t lines_count = get_height();
	const auto& built_lines = get_lines();

	std::vector<CanvasLayer<CC>> layers;
	for (uint32_t l=0;l<lines_count;l++) {
		std::vector<std::vector<CC>> line(1);
		uint32_t line_width = built_lines[l].size();
		for (size_t i=0;i<line_width;++i) {
			line[0].push_back(CC{built_lines[l][i], cs});
		}
		layers.push_back(CanvasLayer<CC>(std::move(line), 1, line_width, static_cast<int32_t>(l), 0));
	}

	CanvasPiece<CC> result(std::move(layers));
	this->built(result);

	return result;
}

template<typename CC>
uint32_t termiq::canvas::Text<CC>::get_width() const {
	lazy_calculate_lines();
	return _text_width;
}

template<typename CC>
uint32_t termiq::canvas::Text<CC>::get_height() const {
	lazy_calculate_lines();
	return _text_height;
}

template<typename CC>
uint32_t termiq::canvas::Text<CC>::min_width() const {
	if (_width) {
		return _width;
	}
	return 1;
}

template<typename CC>
uint32_t termiq::canvas::Text<CC>::min_height() const {
	if (_height) {
		const uint32_t calc_height = get_height();
		if (_height < calc_height) {
			return _height;
		}
		return calc_height;
	}
	return 1;
}

template<typename CC>
const typename termiq::canvas::Text<CC>::CanvasMatrix& termiq::canvas::Text<CC>::get_lines() const {
	lazy_calculate_lines();
	return _lines;
}

template<typename CC>
void termiq::canvas::Text<CC>::calculate_lines() const {
	_lines.resize(0);
	size_t max_w = 0;
	size_t max_h = 0;
	std::vector<Char> line;
	size_t calc_w = get_calc_width();
	size_t calc_h = get_calc_height();
	for (auto& l : _txt) {
		for (size_t i=0;;++i) {
			if ( i == l.size() || (calc_w && calc_w <= line.size())) {
				max_w = std::max(max_w, line.size());
				++max_h;
				_lines.push_back(line);
				line.resize(0);
			}
			if (calc_h && _lines.size() >= calc_h) break;
			if (i >= l.size()) break;
			line.push_back(l[i]);
		}
	}
	_text_width = max_w;
	_text_height = max_h;
}

template<typename CC>
void termiq::canvas::Text<CC>::invalidate_lines() {
	_valid_lines = false;
}

template<typename CC>
void termiq::canvas::Text<CC>::lazy_calculate_lines() const {
	if (_valid_lines) return;
	calculate_lines();
	_valid_lines = true;
}

template<typename CC>
bool termiq::canvas::Text<CC>::is_valid_lines() {
	return _valid_lines;
}

template<typename CC>
uint32_t termiq::canvas::Text<CC>::get_calc_width() const {
	return _width;
}

template<typename CC>
uint32_t termiq::canvas::Text<CC>::get_calc_height() const {
	return _height;
}
