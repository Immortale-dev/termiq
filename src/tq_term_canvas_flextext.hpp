template<typename CC>
void termiq::canvas::FlexText<CC>::suggest_width(uint32_t w) {
	_suggested_width = w;
	this->invalidate_lines();
}

template<typename CC>
void termiq::canvas::FlexText<CC>::suggest_height(uint32_t h) {
	_suggested_height = h;
	this->invalidate_lines();
}

template<typename CC>
uint32_t termiq::canvas::FlexText<CC>::get_calc_width() const {
	if (_suggested_width) return _suggested_width;
	return Text<CC>::get_calc_width();
}

template<typename CC>
uint32_t termiq::canvas::FlexText<CC>::get_calc_height() const {
	if (_suggested_height) return _suggested_height;
	return Text<CC>::get_calc_height();
}
