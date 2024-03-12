#ifndef _TQ_TERM_CANVAS
#define _TQ_TERM_CANVAS

#include <vector>
#include <memory>

namespace termiq {
	struct CharState {
		Color foreground;
		Color background;
		bool bold;
		bool italic;
		bool dim;
		bool inverse;
		bool underline;
		bool special;
	};
	// CT - char type, e.g. char, wchar_t, etc.
	template<typename CT>
	struct CharCell {
		CT symbol = ' ';
		std::shared_ptr<CharState> state = nullptr;
		using char_type = CT;
		bool operator==(const CharCell &other) const {
			if (state == nullptr && other.state != nullptr || state != nullptr && other.state == nullptr) return false;
			return symbol == other.symbol && (state == other.state || *state == *(other.state));
		}
	};
	// CC - char cell type.
	template<typename CC>
	class Canvas {
		using container = std::vector;
		public:
			class TextBuilder {
				public:
					TextBuilder(typename CC::char_type* txt);
					~TextBuilder() = default;

					TextBuilder& set_foreground(Color &&color) { _foreground = color; }
					TextBuilder& set_background(Color &&color) { _background = color; }
					TextBuilder& set_bold() { _bold = true; }
					TextBuilder& set_italic() { _italic = true; }
					TextBuilder& set_dim() { _dim = true; }
					TextBuilder& set_underline() { _underline = true; }
					TextBuilder& set_inverse() { _inverse = true; }
					CanvasPiece build();

				private:
					typename CC::char_type* _txt;
					Color _foreground;
					Color _background;
					bool _bold = false;
					bool _italic = false;
					bool _dim;
					bool _underline;
					bool _inverse;
			};
			struct GridBuilder {
				
			};
			struct CanvasPiece {
				const unsigned int width;
				const unsigned int height;
				const container<const container<const CharCell>> canvas;
			};

		public:
			Canvas() = default;
			Canvas(unsigned int w, unsigned int h, unsigned int r=0, unsigned int c=0);
			virtual ~Canvas() = default;

			void resize(unsigned int width, unsigned int height);
			void move(unsigned int row, unsigned int col);
			void draw(unsigned int row, unsigned int col, CanvasPiece&& piece);
			void paint();

			TextBuilder text(typename CC::char_type *txt);
			GridBuilder grid(unsigned int rows, unsigned int cols);

		protected:
			unsigned int get_width();
			unsigned int get_height();
			unsigned int get_col();
			unsigned int get_row();
			const container<const container<CC>>& get_canvas();

			virtual void moved();
			virtual void resized();
			virtual void drawn();
			virtual void set_paint_state(CharState* state);
			virtual void paint_row(size_t index);

		private:
			void paint_cell(CC &cell);

			unsigned int _width;
			unsigned int _height;
			unsigned int _row;
			unsigned int _col;
			container<container<CC>> _canvas;
	};
}

#include "tq_term_canvas.hpp"

#endif // _TQ_TERM_CANVAS
