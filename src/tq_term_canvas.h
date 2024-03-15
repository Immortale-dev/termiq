#ifndef _TQ_TERM_CANVAS
#define _TQ_TERM_CANVAS

#include <vector>
#include <memory>

namespace termiq {
	struct CharState {
		termiq::style::Color foreground;
		termiq::style::Color background;
		bool bold;
		bool italic;
		bool dim;
		bool inverse;
		bool underline;
		bool special;

		bool operator==(const CharState &other) const {
			return foreground == other.foreground && background == other.background && bold == other.bold && italic == other.italic
				&& dim == other.dim && inverse == other.inverse && underline == other.underline && special == other.special;
		}
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
		template<typename T>
		using container = std::vector<T>;
		struct CanvasPiece {
			const unsigned int width;
			const unsigned int height;
			const container<container<CC>> canvas;
		};
		public:
			class TextBuilder {
				public:
					TextBuilder(const typename CC::char_type* txt) { for(size_t i=0;txt[i]!='\0';++i) _txt.push_back(txt[i]); }
					~TextBuilder() = default;

					TextBuilder& set_foreground_color(termiq::style::Color &&color) { _foreground = color;  return *this; }
					TextBuilder& set_background_color(termiq::style::Color &&color) { _background = color;  return *this; }
					TextBuilder& set_bold() { _bold = true; return *this; }
					TextBuilder& set_italic() { _italic = true; return *this; }
					TextBuilder& set_dim() { _dim = true; return *this; }
					TextBuilder& set_underline() { _underline = true; return *this; }
					TextBuilder& set_inverse() { _inverse = true; return *this; }
					TextBuilder& set_width(unsigned int w) { _width = w; }
					CanvasPiece build();

				private:
					std::vector<typename CC::char_type> _txt;
					termiq::style::Color _foreground{-1,-1,-1};
					termiq::style::Color _background{-1,-1,-1};
					bool _bold = false;
					bool _italic = false;
					bool _dim = false;
					bool _underline = false;
					bool _inverse = false;
					unsigned int _width = 0;
			};
			class GridBuilder {
				struct GridCellState {
					termiq::style::Color _background;
					std::vector<unsigned int> _padding;
					TextBuilder _text;
				};
				public:
					GridBuilder(unsigned int rows, unsigned int cols): _rows(rows), _cols(cols), _grid(_rows, std::vector<GridCellState>(_cols)) {}
					GridBuilder& select_cell(size_t c) { _current_cell_c = c; };
					GridBuilder& select_cell(size_t r, size_t c) { _current_cell_r = r; _current_cell_c = c; }
					GridBuilder& set_background_color(Color color);
					GridBuilder& set_border_foreground_color(Color color);
					GridBuilder& set_border_background_color(Color color);
					GridBuilder& set_cell_background_color(Color color);
					GridBuilder& set_cell_padding(int top, int right, int bottom, int left);
					GridBuilder& set_padding(int top, int right, int bottom, int left);
					GridBuilder& set_cell_text(TextBuilder text);
					CanvasPiece build();

				private:
					unsigned int _rows;
					unsigned int _cols;
					std::vector<std::vector<GridCellState>> _grid;
					size_t _current_cell_r=0;
					size_t _current_cell_c=0;
			};

		public:
			Canvas() = default;
			Canvas(unsigned int h, unsigned int w, unsigned int r=0, unsigned int c=0);
			virtual ~Canvas() = default;

			void resize(unsigned int height, unsigned int width);
			void move(unsigned int row, unsigned int col);
			void draw(unsigned int row, unsigned int col, CanvasPiece&& piece);
			void paint();

			TextBuilder text(const typename CC::char_type *txt);
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
			virtual void move_cursor(unsigned int row, unsigned int col);
			virtual void set_paint_state(CharState* state);
			virtual void paint_row(size_t index);

		private:
			void paint_cell(CC &cell);

			unsigned int _height;
			unsigned int _width;
			unsigned int _row;
			unsigned int _col;
			container<container<CC>> _canvas;
	};
}

#include "tq_term_canvas.hpp"

#endif // _TQ_TERM_CANVAS
