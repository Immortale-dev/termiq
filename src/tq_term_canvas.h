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
					TextBuilder(const typename CC::char_type* txt): _txt(txt) {}
					~TextBuilder() = default;

					TextBuilder& set_foreground_color(termiq::style::Color &&color) { _foreground = color; _fg_set = true; return *this; }
					TextBuilder& set_background_color(termiq::style::Color &&color) { _background = color; _bg_set = true; return *this; }
					TextBuilder& set_bold() { _bold = true; return *this; }
					TextBuilder& set_italic() { _italic = true; return *this; }
					TextBuilder& set_dim() { _dim = true; return *this; }
					TextBuilder& set_underline() { _underline = true; return *this; }
					TextBuilder& set_inverse() { _inverse = true; return *this; }
					CanvasPiece build();

				private:
					const typename CC::char_type* _txt;
					termiq::style::Color _foreground{-1,-1,-1};
					termiq::style::Color _background{-1,-1,-1};
					bool _fg_set = false;
					bool _bg_set = false;
					bool _bold = false;
					bool _italic = false;
					bool _dim;
					bool _underline;
					bool _inverse;
			};
			struct GridBuilder {
				
			};

		public:
			Canvas() = default;
			Canvas(unsigned int w, unsigned int h, unsigned int r=0, unsigned int c=0);
			virtual ~Canvas() = default;

			void resize(unsigned int width, unsigned int height);
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

			unsigned int _width;
			unsigned int _height;
			unsigned int _row;
			unsigned int _col;
			container<container<CC>> _canvas;
	};
}

#include "tq_term_canvas.hpp"

#endif // _TQ_TERM_CANVAS
