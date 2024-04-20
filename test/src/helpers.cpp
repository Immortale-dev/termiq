#include <string>
#include <algorithm>

#include "tq_term_canvas_utils.h"

template<typename CC>
std::vector<std::vector<CC>> pieces_to_grid(termiq::canvas::CanvasPieces<CC> p) {
	unsigned int max_w = 0;
	unsigned int max_h = 0;
	for (auto &piece : p.pieces) {
		max_w = std::max(max_w, piece.offset_cols + piece.cols);
		max_h = std::max(max_h, piece.offset_rows + piece.rows);
	}

	std::vector<std::vector<CC>> result(max_h, std::vector<CC>(max_w));

	for(auto &piece : p.pieces) {
		int r = piece.offset_rows;
		int c = piece.offset_cols;
		for(auto &row : piece.canvas) {
			for (auto &cell : row) {
				result[r][c] = cell;
				++c;
			}
			++r;
		}
	}

	return result;
}

template<typename CC>
std::vector<std::basic_string<typename CC::char_type>> pieces_to_text(termiq::canvas::CanvasPieces<CC> p) {
	using CT = typename CC::char_type;
	auto grid = pieces_to_grid(p);
	int max_h = grid.size();
	int max_w = grid.size()>0 ? grid[0].size() : 0;
	std::vector<std::basic_string<CT>> result(max_h, std::basic_string<CT>(max_w, ' '));

	for(int r=0;r<max_h;r++) {
		for(int c=0;c<max_w;c++) {
			result[r][c] = grid[r][c].symbol;
		}
	}

	return result;
}
