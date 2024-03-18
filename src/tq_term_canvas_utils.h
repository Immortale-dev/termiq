#ifndef _TQ_TERM_CANVAS_UTILS
#define _TQ_TERM_CANVAS_UTILS

#include <memory>
#include <vector>

#include "tq_term_style.h"

namespace termiq {
	// Style state of the symbol.
	struct CharState {
		termiq::style::Color foreground{-1,-1,-1};
		termiq::style::Color background{-1,-1,-1};
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
	};

	// Piece of the canvas.
	template<typename CC>
	struct CanvasPiece {
		const unsigned int rows;
		const unsigned int cols;
		const std::vector<std::vector<CC>> canvas;
	};
}

#endif // _TQ_TERM_CANVAS_UTILS
