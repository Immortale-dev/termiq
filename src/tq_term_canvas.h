#ifndef _TQ_TERM_CANVAS
#define _TQ_TERM_CANVAS

#include <vector>
#include <memory>

namespace termiq {
	// CT - Character type, e.g. char, wchar, etc.
	template<typename CT>
	class Canvas {
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
		struct CharCell {
			CT symbol = ' ';
			std::shared_ptr<CharState> state = nullptr;
			bool dirty = false;
			bool operator==(const CharCell& other) {
				if (state == nullptr && other.state != nullptr || state != nullptr && other.state == nullptr) return false;
				return symbol == other.symbol && (state == other.state || *state == *(other.state));
			}
		};
		public:
			struct TextBuilder {

			};
			struct GridBuilder {

			}
			struct CanvasPiece {
				const unsigned int width;
				const unsigned int height;
				const std::vector<const std::vector<const CharCell>> piece;
			};

		public:
			Canvas() = default;
			Canvas(unsigned int w, unsigned int h, unsigned int r=0, unsigned int c=0);
			virtual ~Canvas() = default;

			void resize(unsigned int width, unsigned int height);
			void move(unsigned int row, unsigned int col);
			void draw(unsigned int row, unsigned int col, CanvasPiece&& piece);

			TextBuilder text(CT* txt);
			GridBuilder grid(unsigned int rows, unsigned int cols);

		private:
			unsigned int _width;
			unsigned int _height;
			unsigned int _row;
			unsigned int _col;
			std::vector<std::vector<CharCell>> _canvas;
			std::vector<std::vector<CharCell>> _current_canvas;
	};
}

#endif // _TQ_TERM_CANVAS
