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

		struct FontStyle {
			std::variant<Color, bool> foreground = false;
			std::variant<Color, bool> background = false;
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
			std::variant<Color, bool> foreground = false;
			std::variant<Color, bool> background = false;
			std::variant<Color, bool> selection_foreground = false;
			std::variant<Color, bool> selection_background = false;
			std::variant<Color, bool> cursor_foreground = false;
			std::variant<Color, bool> cursor_background = false;
		};

		inline void init(SequenceExecutor* executor) { detail::executor = executor; }
		inline bool inited() { return detail::executor != nullptr; }

		void style(FontStyle style);
		void foreground(Color color);
		void background(Color color);
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
		void terminal_foreground(termiq::Color color);
		void terminal_background(termiq::Color color);
		void selection_foreground(termiq::Color color);
		void selection_background(termiq::Color color);
		void cursor_foreground(termiq::Color color);
		void cursor_background(termiq::Color color);
		void terminal_style_reset();

		inline bool is_color_defined(termiq::Color color) { return color != termiq::Color::NONE; }
	}
}

#endif // _TQ_TERM_STYLE
