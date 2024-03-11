#include "tq_term_adv.h"
#include "tq_term_style.h"

#include <iostream>
#include <unordered_map>
#include <list>
#include <utility>

namespace {
	std::unordered_map<int, std::list<std::pair<int, int>>::iterator> saved_colors;
	std::list<std::pair<int, int>> color_prior;

	int max_colors_count = 0;
	int start_colors_offset = 16;

	termiq::style::Color current_foreground{-1,-1,-1};
	termiq::style::Color current_background{-1,-1,-1};
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
		if (is_bold || is_dim || is_inverse || is_underline) {
			termiq::set_attrs(is_bold, is_dim, is_inverse, is_underline);
		} else {
			termiq::reset_attrs();
		}
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
	int color_hash(int r, int g, int b) {
		return ((r * 2050 + g) * 2050) + b;
	}
	int define_color(const termiq::style::Color &color) {
		const auto [r,g,b] = color;
		if (!max_colors_count) {
			max_colors_count = termiq::get_max_colors() - start_colors_offset;
		}
		const int hash = color_hash(r,g,b);
		auto color_it = saved_colors.find(hash);
		int color_id;
		if (color_it == saved_colors.end()) {
			if ((int)saved_colors.size() >= max_colors_count) {
				auto least_used_color = color_prior.back();
				color_prior.pop_back();
				color_id = least_used_color.second;
				saved_colors.erase(least_used_color.first);
			} else {
				color_id = saved_colors.size() + start_colors_offset;
			}
			color_prior.push_front({hash, color_id});
			saved_colors.insert({hash, color_prior.begin()});
			termiq::define_color(color_id, r, g, b);
		} else {
			color_id = color_it->second->second;
			color_prior.splice(color_prior.begin(), color_prior, color_it->second);
		}
		return color_id;
	}
}

void termiq::style::foreground(const Color color) {
	const int color_id = ::define_color(color);
	set_foreground_color(color_id);
	current_foreground = color;
}

void termiq::style::background(const Color color) {
	const int color_id = ::define_color(color);
	set_background_color(color_id);
	current_background = color;
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
	if (is_underline == state) return;
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
	is_inverse = state;
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
