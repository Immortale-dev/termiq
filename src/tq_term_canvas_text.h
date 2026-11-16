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
				using Char = typename CC::Char;
				using Style = typename CC::Style;
				using CanvasMatrix = std::vector<std::vector<Char>>;

				Text();
				Text(std::span<Char> line);
				Text(std::span<std::vector<Char>> multiline);
				Text(std::vector<Char>&& line);
				Text(std::vector<std::vector<Char>>&& multiline);

				void set_foreground_color(termiq::Color color);
				void set_background_color(termiq::Color color);
				void set_bold();
				void set_italic();
				void set_dim();
				void set_underline();
				void set_inverse();
				void set_width(uint32_t w);
				void set_height(uint32_t h);

				CanvasPiece<CC> build() const override;
				uint32_t get_width() const override;
				uint32_t get_height() const override;
				uint32_t min_width() const override;
				uint32_t min_height() const override;
				const CanvasMatrix& get_lines() const;

			protected:
				virtual void calculate_lines() const;
				void invalidate_lines();
				void lazy_calculate_lines() const;
				bool is_valid_lines();

				virtual uint32_t get_calc_width() const;
				virtual uint32_t get_calc_height() const;

				std::vector<std::vector<Char>> _txt;
				termiq::Color _foreground = ::termiq::Color::NONE;
				termiq::Color _background = ::termiq::Color::NONE;
				bool _bold = false;
				bool _italic = false;
				bool _dim = false;
				bool _underline = false;
				bool _inverse = false;
				uint32_t _width = 0;
				uint32_t _height = 0;
				mutable uint32_t _text_width = 0;
				mutable uint32_t _text_height = 0;
				mutable CanvasMatrix _lines;

			private:
				mutable bool _valid_lines = false;
		};
	}
}

#include "tq_term_canvas_text.hpp"

#endif // _TQ_TERM_CANVAS_TEXT
