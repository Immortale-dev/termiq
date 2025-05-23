#include <string>
#include <algorithm>
#include <type_traits>
#include <locale>
#include <codecvt>

#include "tq_term_canvas_utils.h"

template<typename CC, typename C>
std::vector<std::vector<typename CC::char_type>> build_text(std::basic_string<C> str){
	using char_type = typename CC::char_type;
	std::vector<std::vector<char_type>> res;
	std::vector<char_type> line;
	std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
	for (auto c : str) {
		if (c == '\n') {
			res.push_back(line);
			line.resize(0);
			continue;
		}
		if (c == '\0') break;
		if constexpr (std::is_same_v<C,char16_t>) {
			auto utf8 = cvt.to_bytes({c});
			line.push_back(char_type(utf8.begin(), utf8.end()));
		} else {
			line.push_back({c});
		}
	}
	if (line.size()) {
		res.push_back(line);
	}
	return res;
}

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
		for(auto &row : piece.canvas) {
			int c = piece.offset_cols;
			for (auto &cell : row) {
				result[r][c] = cell;
				++c;
			}
			++r;
		}
	}

	return result;
}


template<typename CC, typename C = char>
std::vector<std::basic_string<C>> grid_to_text(std::vector<std::vector<CC>> grid) {
	using CT = typename CC::char_type;
	using symbol_type = C;
	int max_h = grid.size();
	int max_w = grid.size()>0 ? grid[0].size() : 0;
	std::vector<std::basic_string<symbol_type>> result(max_h, std::basic_string<symbol_type>(max_w, ' '));
	std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;

	for(int r=0;r<max_h;r++) {
		for(int c=0;c<max_w;c++) {
			CT &symbol = grid[r][c].symbol;
			if constexpr (std::is_same_v<C,char16_t>) {
				auto str = cvt.from_bytes(std::basic_string<char>(reinterpret_cast<const char*>(symbol.begin()), size_t(symbol.end()-symbol.begin())));
				result[r][c] = str[0];
			} else {
				result[r][c] = *symbol.begin();
			}
		}
	}

	return result;
}

template<typename CC, typename C>
std::vector<std::basic_string<C>> pieces_to_text(termiq::canvas::CanvasPieces<CC> p) {
	auto grid = pieces_to_grid<CC>(p);
	return grid_to_text<CC, C>(grid);
}
