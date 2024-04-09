#ifndef _TQ_TERM_CANVAS_UTILS
#define _TQ_TERM_CANVAS_UTILS

#include <memory>
#include <vector>

#include "tq_term_style.h"

namespace termiq {
	// Style state of the symbol.
	struct CharState {
		termiq::style::Color foreground = style::Color::UNDEFINED;
		termiq::style::Color background = style::Color::UNDEFINED;
		bool bold = false;
		bool italic = false;
		bool dim = false;
		bool inverse = false;
		bool underline = false;
		bool special = false;

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
		bool is_transparent() { return !state; }

		static const CharCell<CT> TRANSPARENT;
	};

	template<typename CT>
	const CharCell<CT> CharCell<CT>::TRANSPARENT = CharCell<CT>();

	// Piece of the canvas.
	template<typename CC>
	struct CanvasPiece {
		const std::vector<std::vector<CC>> canvas;
		const unsigned int rows;
		const unsigned int cols;
		const unsigned int offset_rows = 0;
		const unsigned int offset_cols = 0;
	};

	// Collection of pieces.
	template<typename CC>
	struct CanvasPieces {
		const std::vector<CanvasPiece<CC>> pieces;
	};

	// Buildable content interface
	template<typename CC>
	class ContentBuilder {
		public:
			ContentBuilder() = default;
			virtual ~ContentBuilder() = default;

			virtual CanvasPieces<CC> build() = 0;
			virtual unsigned int get_width();
			virtual unsigned int get_height();
			virtual unsigned int min_width();
			virtual unsigned int min_height();
	};

	template<typename CC>
	class NothingBuilder : public ContentBuilder<CC> {
		public:
			CanvasPieces<CC> build() { return {{}}; }
			unsigned int get_width() { return 0; }
			unsigned int get_height() {  return 0; }
			unsigned int min_width() { return 0; }
			unsigned int min_height() { return 0; }
	};
}

#endif // _TQ_TERM_CANVAS_UTILS
