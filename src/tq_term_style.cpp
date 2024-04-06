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

	termiq::style::Color current_foreground = termiq::style::Color::UNDEFINED;
	termiq::style::Color current_background = termiq::style::Color::UNDEFINED;
	bool is_bold = false;
	bool is_italic = false;
	bool is_underline = false;
	bool is_dim = false;
	bool is_inverse = false;
	bool is_special = false;

	bool is_foreground_set() {
		return termiq::style::is_color_defined(current_foreground);
	}
	bool is_background_set() {
		return termiq::style::is_color_defined(current_background);
	}
	void reset_background() {
		current_foreground = termiq::style::Color::UNDEFINED;
	}
	void reset_foreground() {
		current_background = termiq::style::Color::UNDEFINED;
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
			termiq::style::foreground(current_foreground);
		}
		if (is_background_set()) {
			termiq::style::background(current_background);
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

// For testing.
#ifdef STUBTERM
namespace termiq {
	void term_stub_reset_styles_() {
		::max_colors_count = 0;
		::saved_colors.clear();
		::color_prior.clear();
	}
}
#endif // STUBTERM

const termiq::style::Color termiq::style::Color::UNDEFINED = {-1, -1, -1};

void termiq::style::style(Color fg, Color bg, bool bold, bool italic, bool dim, bool underline, bool inverse) {
	bool attr_updated = false;
	if (is_bold != bold) {
		is_bold = bold;
		attr_updated = true;
	}
	if (is_italic != italic) {
		is_italic = italic;
		attr_updated = true;
	}
	if (is_dim != dim) {
		is_dim = dim;
		attr_updated = true;
	}
	if (is_underline != underline) {
		is_underline = underline;
		attr_updated = true;
	}
	if (is_inverse != inverse) {
		is_inverse = inverse;
		attr_updated = true;
	}
	if (attr_updated) {
		update_attrs();
	}
	bool clrs_updated = false;
	if (!attr_updated && ( (is_foreground_set() && !termiq::style::is_color_defined(fg)) || (is_background_set() && !termiq::style::is_color_defined(bg)) )) {
		reset_colors();
		clrs_updated = true;
	}
	if (current_foreground != fg) {
		current_foreground = fg;
		clrs_updated = true;
	}
	if (current_background != bg) {
		current_background = bg;
		clrs_updated = true;
	}
	if (clrs_updated) {
		update_colors();
	}
}

void termiq::style::foreground(const Color color) {
	if (!is_color_defined(color)) {
		if (!is_foreground_set()) return;
		reset_colors();
		reset_foreground();
		update_colors();
		return;
	}
	const int color_id = ::define_color(color);
	set_foreground_color(color_id);
	current_foreground = color;
}

void termiq::style::background(const Color color) {
	if (!is_color_defined(color)) {
		if (!is_background_set()) return;
		reset_colors();
		reset_background();
		update_colors();
		return;
	}
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

void termiq::style::special_chars(bool state) {
	if (state == is_special) return;
	is_special = state;
	if (is_special) {
		termiq::alternate_chars_on();
	} else {
		termiq::alternate_chars_off();
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

void termiq::style::clear_styles(bool also_clear_colors) {
	if (!is_bold && !is_italic && !is_underline && !is_dim && !is_inverse) {
		clear_colors();
		return;
	}
	is_bold = false;
	is_italic = false;
	is_underline = false;
	is_dim = false;
	is_inverse = false;
	update_attrs();
	if(also_clear_colors) {
		reset_background();
		reset_foreground();
	}
	update_colors();
}

void termiq::style::clear_colors() {
	if (!is_foreground_set() && !is_background_set()) {
		return;
	}
	termiq::reset_colors();
	reset_background();
	reset_foreground();
}

void termiq::style::clear() {
	clear_styles(true);
}

void termiq::style::reset() {
	termiq::undefine_colors();
	clear();
}

bool termiq::style::is_color_defined(const termiq::style::Color &color) {
	return color.r >= 0;
}
