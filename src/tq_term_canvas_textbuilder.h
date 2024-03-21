#ifndef _TQ_TERM_CANVAS_TEXTBUILDER
#define _TQ_TERM_CANVAS_TEXTBUILDER

#include "tq_term_canvas_utils.h"

namespace termiq {
	template<typename CC>
	class TextBuilder {
		public:
			using char_type = typename CC::char_type;
			using CanvasGrid = std::vector<std::vector<char_type>>;

			TextBuilder(const char_type* txt = nullptr);
			virtual ~TextBuilder() = default;

			TextBuilder& set_foreground_color(termiq::style::Color &&color);
			TextBuilder& set_background_color(termiq::style::Color &&color);
			TextBuilder& set_bold();
			TextBuilder& set_italic();
			TextBuilder& set_dim();
			TextBuilder& set_underline();
			TextBuilder& set_inverse();
			TextBuilder& set_width(unsigned int w);
			TextBuilder& set_height(unsigned int h);
			CanvasPieces<CC> build();

			unsigned int text_width();
			unsigned int text_height();
			const CanvasGrid& get_lines();

		private:
			void calculate_lines();
			void invalidate_lines();
			void lazy_calculate_lines();
			bool is_valid_lines();

			std::vector<char_type> _txt;
			termiq::style::Color _foreground = style::Color::UNDEFINED;
			termiq::style::Color _background = style::Color::UNDEFINED;
			bool _bold = false;
			bool _italic = false;
			bool _dim = false;
			bool _underline = false;
			bool _inverse = false;
			unsigned int _width = 0;
			unsigned int _height = 0;
			unsigned int _text_width = 0;
			unsigned int _text_height = 0;
			std::vector<std::vector<char_type>> _lines;
			bool _valid_lines = true;

			static inline char_type STRING_TERMINATOR = '\0';
			static inline char_type LINE_TERMINATOR = '\n';
	};
}

#include "tq_term_canvas_textbuilder.hpp"

#endif // _TQ_TERM_CANVAS_TEXTBUILDER
