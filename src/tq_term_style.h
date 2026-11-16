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

		inline constexpr decltype(std::nullopt) UNDEFINED = std::nullopt;

		struct FontStyle {
			std::optional<Color> foreground = UNDEFINED;
			std::optional<Color> background = UNDEFINED;
			std::optional<bool> bold = UNDEFINED;
			std::optional<bool> dim = UNDEFINED;
			std::optional<bool> italic = UNDEFINED;
			std::optional<Underline> underline = UNDEFINED;
			std::optional<bool> blinking = UNDEFINED;
			std::optional<bool> inverse = UNDEFINED;
			std::optional<bool> hidden = UNDEFINED;
			std::optional<bool> strike = UNDEFINED;
		};

		struct TerminalStyle {
			std::optional<Color> foreground = UNDEFINED;
			std::optional<Color> background = UNDEFINED;
			std::optional<Color> selection_foreground = UNDEFINED;
			std::optional<Color> selection_background = UNDEFINED;
			std::optional<Color> cursor_foreground = UNDEFINED;
			std::optional<Color> cursor_background = UNDEFINED;
		};

		inline void init(SequenceExecutor* executor) { detail::executor = executor; }
		inline bool inited() { return detail::executor != nullptr; }

		void style(FontStyle style);
		void foreground(Color color);
		void background(Color color);
		void bold(bool state);
		void dim(bool state);
		void italic(bool state);
		void underline(Underline state);
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
