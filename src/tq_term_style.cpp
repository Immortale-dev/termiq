#include "tq_term_style.h"

#include <variant>

#include "tq_term.h"
#include "tq_term_adv.h"

namespace termiq::style {
	namespace {
		termiq::color_t current_foreground = ::termiq::Color::NONE;
		termiq::color_t current_background = ::termiq::Color::NONE;
		bool is_bold = false;
		bool is_dim = false;
		bool is_italic = false;
		std::variant<bool, Underline> is_underline = false;
		bool is_blinking = false;
		bool is_inverse = false;
		bool is_hidden = false;
		bool is_strike = false;

		termiq::color_t current_terminal_foreground = ::termiq::Color::NONE;
		termiq::color_t current_terminal_background = ::termiq::Color::NONE;
		termiq::color_t current_terminal_selection_foreground = ::termiq::Color::NONE;
		termiq::color_t current_terminal_selection_background = ::termiq::Color::NONE;
		termiq::color_t current_terminal_cursor_foreground = ::termiq::Color::NONE;
		termiq::color_t current_terminal_cursor_background = ::termiq::Color::NONE;

		void reset_color_values() {
			current_foreground = Color::NONE;
			current_background = Color::NONE;
		}

		void reset_attr_values() {
			is_bold = false;
			is_dim = false;
			is_italic = false;
			is_underline = false;
			is_blinking = false;
			is_inverse = false;
			is_hidden = false;
			is_strike = false;
		}
	}
}

void termiq::style::style(style::FontStyle s) {
	std::vector<StyleProp> list{};
	if (s.foreground.index() == 0) {
		::termiq::style::foreground(std::get<0>(s.foreground));
	}
	if (s.background.index() == 0) {
		::termiq::style::background(std::get<0>(s.background));
	}

	bool bold_changed = false;
	bool dim_changed = false;
	if (s.bold && *s.bold != is_bold) {
		is_bold = *s.bold;
		bold_changed = true;
	}
	if (s.dim && *s.dim != is_dim) {
		is_dim = *s.dim;
		dim_changed = true;
	}
	if ((bold_changed && !is_bold) || (dim_changed && !is_dim)) list.push_back(StyleProp::NORMAL);
	if (is_bold) list.push_back(StyleProp::BOLD);
	if (is_dim) list.push_back(StyleProp::DIM);

	if (s.italic && is_italic != *s.italic) {
		is_italic = *s.italic;
		list.push_back(is_italic ? StyleProp::ITALIC : StyleProp::STRAIGHT);
	}

	if (s.underline && *s.underline != is_underline) {
		is_underline = *s.underline;
		if (is_underline.index() == 0) {
			bool st = std::get<0>(is_underline);
			list.push_back(st ? StyleProp::UNDERLINE : StyleProp::NONDERLINE);
		} else {
			Underline &u = std::get<1>(is_underline);
			detail::executor->execute<se::set_underline_color>(u.color);
			detail::executor->execute<se::set_underline_style>(u.style);
		}
	}

	if (s.blinking && *s.blinking != is_blinking) {
		is_blinking = *s.blinking;
		list.push_back(is_blinking ? StyleProp::BLINKING : StyleProp::STARING);
	}

	if (s.inverse && *s.inverse != is_inverse) {
		is_inverse = *s.inverse;
		list.push_back(is_inverse ? StyleProp::INVERSE : StyleProp::OUTVERSE);
	}

	if (s.hidden && *s.hidden != is_hidden) {
		is_hidden = *s.hidden;
		list.push_back(is_hidden ? StyleProp::HIDDEN : StyleProp::VISIBLE);
	}

	if (s.strike && *s.strike != is_strike) {
		is_strike = *s.strike;
		list.push_back(is_strike ? StyleProp::STRIKE : StyleProp::UNSTRIKE);
	}

	if (!list.size()) return;

	detail::executor->execute<se::set_styles>(std::move(list));
}

void termiq::style::foreground(const color_t color) {
	if (color == current_foreground) return;
	current_foreground = color;
	detail::executor->execute<se::set_foreground_color>(color);
}

void termiq::style::background(const color_t color) {
	if (color == current_background) return;
	current_background = color;
	detail::executor->execute<se::set_background_color>(color);
}

void termiq::style::bold(bool state) {
	if (is_bold == state) return;
	is_bold = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::BOLD});
	} else if (is_dim) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::NORMAL, StyleProp::DIM});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::NORMAL});
	}
}

void termiq::style::dim(bool state) {
	if (is_dim == state) return;
	is_dim = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::DIM});
	} else if (is_bold) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::NORMAL, StyleProp::BOLD});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::NORMAL});
	}
}

void termiq::style::italic(bool state) {
	if (is_italic == state) return;
	is_italic = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::ITALIC});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::STRAIGHT});
	}
}

void termiq::style::underline(std::variant<bool, Underline> state) {
	if (is_underline == state) return;
	is_underline = state;
	if (state.index() == 0) {
		bool s = std::get<0>(state);
		if (s) {
			detail::executor->execute<se::set_styles>(std::vector{StyleProp::UNDERLINE});
		} else {
			detail::executor->execute<se::set_styles>(std::vector{StyleProp::NONDERLINE});
		}
	} else {
		Underline &u = std::get<1>(state);
		detail::executor->execute<se::set_underline_color>(u.color);
		detail::executor->execute<se::set_underline_style>(u.style);
	}
}

void termiq::style::blinking(bool state) {
	if (is_blinking == state) return;
	is_blinking = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::BLINKING});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::STARING});
	}
}

void termiq::style::inverse(bool state) {
	if (is_inverse == state) return;
	is_inverse = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::INVERSE});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::OUTVERSE});
	}
}

void termiq::style::hidden(bool state) {
	if (is_hidden == state) return;
	is_hidden = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::HIDDEN});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::VISIBLE});
	}
}

void termiq::style::strike(bool state) {
	if (is_strike == state) return;
	is_strike = state;
	if (state) {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::STRIKE});
	} else {
		detail::executor->execute<se::set_styles>(std::vector{StyleProp::UNSTRIKE});
	}
}

void termiq::style::style_reset() {
	reset_attr_values();
	reset_color_values();
	detail::executor->execute<se::reset_styles>();
}

void termiq::style::props_reset()
{
	std::vector<StyleProp> list;
	if (is_bold || is_dim) list.push_back(StyleProp::NORMAL);
	if (is_italic) list.push_back(StyleProp::STRAIGHT);
	if (is_underline.index() == 1 || (is_underline.index() == 0 && std::get<0>(is_underline))) list.push_back(StyleProp::NONDERLINE);
	if (is_blinking) list.push_back(StyleProp::STARING);
	if (is_inverse) list.push_back(StyleProp::OUTVERSE);
	if (is_hidden) list.push_back(StyleProp::VISIBLE);
	if (is_strike) list.push_back(StyleProp::UNSTRIKE);
	if (!list.size()) return;
	detail::executor->execute<se::set_styles>(std::move(list));
	reset_attr_values();
}

void termiq::style::colors_reset() {
	foreground(Color::NONE);
	background(Color::NONE);
}

void termiq::style::update() {
	style(FontStyle{
		.foreground = current_foreground,
		.background = current_background,
		.bold = is_bold,
		.dim = is_dim,
		.italic = is_italic,
		.underline = is_underline,
		.blinking = is_blinking,
		.inverse = is_inverse,
		.hidden = is_hidden,
		.strike = is_strike,
	});
}

void termiq::style::terminal_style(TerminalStyle style)
{
	if (style.foreground.index() == 0) terminal_foreground(std::get<0>(style.foreground));
	if (style.background.index() == 0) terminal_background(std::get<0>(style.background));
	if (style.selection_foreground.index() == 0) selection_foreground(std::get<0>(style.selection_foreground));
	if (style.selection_background.index() == 0) selection_background(std::get<0>(style.selection_background));
	if (style.cursor_foreground.index() == 0) cursor_foreground(std::get<0>(style.cursor_foreground));
	if (style.cursor_background.index() == 0) cursor_background(std::get<0>(style.cursor_background));
}

void termiq::style::terminal_foreground(termiq::color_t color)
{
	if (color == current_terminal_foreground) return;
	current_terminal_foreground = color;
	detail::executor->execute<se::set_color>(ColorType::FOREGROUND, color);
}

void termiq::style::terminal_background(termiq::color_t color)
{
	if (color == current_terminal_background) return;
	current_terminal_background = color;
	detail::executor->execute<se::set_color>(ColorType::BACKGROUND, color);
}

void termiq::style::selection_foreground(termiq::color_t color)
{
	if (color == current_terminal_selection_foreground) return;
	current_terminal_selection_foreground = color;
	detail::executor->execute<se::set_color>(ColorType::SELECTION_FOREGROUND, color);
}

void termiq::style::selection_background(termiq::color_t color)
{
	if (color == current_terminal_selection_background) return;
	current_terminal_selection_background = color;
	detail::executor->execute<se::set_color>(ColorType::SELECTION_BACKGROUND, color);
}

void termiq::style::cursor_foreground(termiq::color_t color)
{
	if (color == current_terminal_cursor_foreground) return;
	current_terminal_cursor_foreground = color;
	detail::executor->execute<se::set_color>(ColorType::CURSOR_FOREGROUND, color);
}

void termiq::style::cursor_background(termiq::color_t color)
{
	if (color == current_terminal_cursor_background) return;
	current_terminal_cursor_background = color;
	detail::executor->execute<se::set_color>(ColorType::CURSOR_BACKGROUND, color);
}

void termiq::style::terminal_style_reset()
{
	terminal_style(TerminalStyle{
		.foreground = Color::NONE,
		.background = Color::NONE,
		.selection_foreground = Color::NONE,
		.selection_background = Color::NONE,
		.cursor_foreground = Color::NONE,
		.cursor_background = Color::NONE,
	});
}
