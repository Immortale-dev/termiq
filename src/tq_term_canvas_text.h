#ifndef _TQ_TERM_CANVAS_TEXT
#define _TQ_TERM_CANVAS_TEXT

#include <span>
#include <vector>

#include "tq_term_canvas_utils.h"

namespace termiq {
	namespace canvas {
		template<typename CC>
		class Text : public Content<CC> {
			public:
				using char_type = typename CC::char_type;
				using CanvasMatrix = std::vector<std::vector<char_type>>;

				Text();
				Text(std::span<char_type> line);
				Text(std::span<std::vector<char_type>> multiline);
				Text(std::vector<char_type>&& line);
				Text(std::vector<std::vector<char_type>>&& multiline);

				void set_foreground_color(termiq::color_t color);
				void set_background_color(termiq::color_t color);
				void set_bold();
				void set_italic();
				void set_dim();
				void set_underline();
				void set_inverse();
				void set_width(unsigned int w);
				void set_height(unsigned int h);
				CanvasPieces<CC> build() override;

				unsigned int get_width() override;
				unsigned int get_height() override;
				unsigned int min_width() override;
				unsigned int min_height() override;
				const CanvasMatrix& get_lines();

			protected:
				virtual void calculate_lines();
				void invalidate_lines();
				void lazy_calculate_lines();
				bool is_valid_lines();
				virtual unsigned int get_calc_width();
				virtual unsigned int get_calc_height();

				std::vector<std::vector<char_type>> _txt;
				termiq::color_t _foreground = ::termiq::Color::NONE;
				termiq::color_t _background = ::termiq::Color::NONE;
				bool _bold = false;
				bool _italic = false;
				bool _dim = false;
				bool _underline = false;
				bool _inverse = false;
				unsigned int _width = 0;
				unsigned int _height = 0;
				unsigned int _text_width = 0;
				unsigned int _text_height = 0;
				CanvasMatrix _lines;

			private:
				bool _valid_lines = false;
		};
	}
}

#include "tq_term_canvas_text.hpp"

#endif // _TQ_TERM_CANVAS_TEXT
