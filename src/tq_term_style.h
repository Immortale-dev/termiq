#ifndef _TQ_TERM_STYLE
#define _TQ_TERM_STYLE

#include <variant>
#include <optional>

#include "tq_term.h"
#include "tq_term_adv.h"

namespace termiq {
	namespace style {
		namespace detail {
			inline ::termiq::SequenceExecutor* executor = nullptr;
		}

		struct Underline {
			color_t color = Color::NONE;
			UnderlineStyle style = UnderlineStyle::NONE;

			bool operator==(const Underline &other) const {
				return color == other.color && style == other.style;
			}
			bool operator!=(const Underline &other) const {
				return !(*this == other);
			}
		};

		struct FontStyle {
			std::variant<color_t, bool> foreground = false;
			std::variant<color_t, bool> background = false;
			std::optional<bool> bold = NONE;
			std::optional<bool> dim = NONE;
			std::optional<bool> italic = NONE;
			std::optional<std::variant<bool, Underline>> underline = NONE;
			std::optional<bool> blinking = NONE;
			std::optional<bool> inverse = NONE;
			std::optional<bool> hidden = NONE;
			std::optional<bool> strike = NONE;

			inline static const decltype(std::nullopt) NONE = std::nullopt;
		};

		struct TerminalStyle {
			std::variant<color_t, bool> foreground = false;
			std::variant<color_t, bool> background = false;
			std::variant<color_t, bool> selection_foreground = false;
			std::variant<color_t, bool> selection_background = false;
			std::variant<color_t, bool> cursor_foreground = false;
			std::variant<color_t, bool> cursor_background = false;
		};

		inline void init(SequenceExecutor* executor) { detail::executor = executor; }
		inline bool inited() { return detail::executor != nullptr; }

		void style(FontStyle style);
		void foreground(color_t color);
		void background(color_t color);
		void bold(bool state);
		void dim(bool state);
		void italic(bool state);
		void underline(std::variant<bool, Underline>);
		void blinking(bool state);
		void inverse(bool state);
		void hidden(bool state);
		void strike(bool state);
		void style_reset();
		void colors_reset();
		void props_reset();
		void update();

		void terminal_style(TerminalStyle style);
		void terminal_foreground(termiq::color_t color);
		void terminal_background(termiq::color_t color);
		void selection_foreground(termiq::color_t color);
		void selection_background(termiq::color_t color);
		void cursor_foreground(termiq::color_t color);
		void cursor_background(termiq::color_t color);
		void terminal_style_reset();

		inline bool is_color_defined(termiq::color_t color) { return color != termiq::Color::NONE; }
	}
}

#endif // _TQ_TERM_STYLE
