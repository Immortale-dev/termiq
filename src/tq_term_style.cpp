#include "qt_term_style.h"

#include <unordered_map>
#include <list>
#include <utils>

namespace {
	std::unordered_map<int, std::list<pair<int, int>>::iterator> saved_colors;
	std::list<pair<int, int>> color_prior;

	int max_colors = 0;

	Color current_foreground{-1,-1,-1};
	Color current_background{-1,-1,-1};
	bool is_bold = false;
	bool is_italic = false;
	bool is_underline = false;
	bool is_dim = false;
	bool is_inverse = false;

	bool is_foreground_set() {
		return current_foreground.r >= 0;
	}
	bool is_background_set() {
		return current_background.r >= 0;
	}
	void reset_background() {
		current_foreground = {-1,-1,-1};
	}
	void reset_foreground() {
		current_background = {-1,-1,-1};
	}
	void update_attrs() {
		termiq::set_attrs(is_bool, is_dim, is_reverse, is_underline);
		if (is_italic) {
			termiq::set_italic_on();
		} else {
			termiq::set_italic_off();
		}
	}
	void update_colors() {
		if (is_foreground_set()) {
			foreground(current_foreground);
		}
		if (is_background_set()) {
			background(current_background);
		}
	}
	int define_color(const Color &color) {
		const auto {r,g,b} = color;
		if (!max_colors) {
			max_colors = termiq::get_max_colors() - 1;
		}
		const int hash = color_hash(r,g,b);
		auto color_it = saved_colors.find(hash);
		int color_id;
		if (color_it == saved_colors.end()) {
			if (saved_colors.size() >= max_colors) {
				auto least_used_color = color_prior.back();
				color_prior.pop_back();
				color_id = least_used_color.second;
				saved_colors.erase(least_used_color.first);
			} else {
				color_id = saved_colors.size() + 1;
			}
			color_prior.push_front({hash, color_id});
			saved_colors.insert({hash, color_prior.begin()});
			termiq::define_color(color_id, r, g, b);
		} else {
			color_id = color_it->second;
			color_prior.push_front(color_it);
		}
		return color_id;
	}
	int color_hash(int r, int g, int b) {
		return ((r * 2050 + g) * 2050) + b;
	}
}

void termiq::style::foreground(const Color color) {
	const int color_id = define_color(color);
	set_foreground_color(color_id);
}

void termiq::style::background(const Color color) {
	const int color_id = define_color(color);
	set_background_color(color_id);
}

void termiq::style::bold(bool state) {
	if (is_bold == state) return;
	is_bold = state;
	update_attrs();
	update_colors();
}

void termiq::style::italic(bool state) {
	if (is_italic == state) return;
	is_italic = state;
	if (is_italic) {
		termiq::set_italic_on();
	} else {
		termiq::set_italic_off();
	}
}

void termiq::style::underline(bool state) {
	if (is_underlune == state) return;
	is_underline = state;
	update_attrs();
	update_colors();
}

void termiq::style::dim(bool state) {
	if (is_dim == state) return;
	is_dim = state;
	update_attrs();
	update_colors();
}

void termiq::style::inverse(bool state) {
	if (is_inverse == state) return;
	us_inverse = state;
	update_attrs();
	update_colors();
}

void termiq::style::clear_styles() {
	is_bold = false;
	is_italic = false;
	is_underline = false;
	is_dim = false;
	is_inverse = false;
	update_attrs();
	update_colors();
}

void termiq::style::clear_colors() {
	termiq::reset_colors();
	reset_background();
	reset_foreground();
}

void termiq::style::clear() {
	clear_colors();
	clear_styles();
}

void termiq::style::reset() {
	termiq::undefine_colors();
	clear();
}
