template<typename CC>
void termiq::canvas::FlexText<CC>::suggest_width(unsigned int w) {
	_suggested_width = w;
	this->invalidate_lines();
}

template<typename CC>
void termiq::canvas::FlexText<CC>::suggest_height(unsigned int h) {
	_suggested_height = h;
	this->invalidate_lines();
}

template<typename CC>
unsigned int termiq::canvas::FlexText<CC>::get_calc_width() {
	if (_suggested_width) return _suggested_width;
	return Text<CC>::get_calc_width();
}

template<typename CC>
unsigned int termiq::canvas::FlexText<CC>::get_calc_height() {
	if (_suggested_height) return _suggested_height;
	return Text<CC>::get_calc_height();
}
