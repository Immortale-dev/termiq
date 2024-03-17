#ifndef _TQ_TERM_CANVAS_GRIDBUILDER
#define _TQ_TERM_CANVAS_GRIDBUILDER

#include "tq_term_adv.h"
#include "tq_term_unicode_draws.h"
#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_textbuilder.h"

namespace termiq {
	enum class BorderType {
		SINGLE_ASCII = 0,
		SINGLE = 1,
		DOUBLE = 2,
		BOLD = 3,
		DASHED = 4,
		DOTTED = 5,
		ROUNDED = 6,
		INVISIBLE = 7
	};

	template <typename CC>
	class GridBuilder {
		using char_type = typename CC::char_type;
		struct GridCellState {
			termiq::style::Color background{-1,-1,-1};
			TextBuilder text;
			bool has_text = false;
			unsigned int width = 0;
			unsigned int height = 0;
		};
		struct GridBorders {
			char_type C_H;  // Horizontal line.
			char_type C_V;  // Vertical line.
			char_type C_X;  // Vertical/Horiaonal cross.
			char_type C_HT; // Horizontal line with connection on the top.
			char_type C_HB; // Horizontal line with connection on the bottom.
			char_type C_VL; // Vertical line with connection on the left.
			char_type C_VR; // Vertical line with connection on the right.
			char_type C_TL; // Top left corner.
			char_type C_TR; // Top right corner.
			char_type C_BL; // Bottom left corner.
			char_type C_BR; // Bottom right corner.
		};

		public:
			GridBuilder(unsigned int rows=1, unsigned int cols=1);
			GridBuilder& select_cell(size_t c);
			GridBuilder& select_cell(size_t r, size_t c);
			GridBuilder& set_background_color(termiq::style::Color color);
			GridBuilder& set_border_foreground_color(termiq::style::Color color);
			GridBuilder& set_border_background_color(termiq::style::Color color);
			GridBuilder& set_cell_background_color(termiq::style::Color color);
			GridBuilder& set_cell_text(TextBuilder text);
			GridBuilder& set_width(unsigned int width);
			GridBuilder& set_height(unsigned int height);
			GridBuilder& set_cell_width(unsigned int width);
			GridBuilder& set_cell_height(unsigned int height);
			GridBuilder& set_border_type(BorderType type);
			CanvasPiece<CC> build();

		private:
			GridCellState& get_current_cell();
			unsigned int get_column_defined_width(size_t index);
			unsigned int get_row_defined_height(size_t index);
			unsigned int get_column_text_width(size_t index);
			unsigned int get_row_text_height(size_t index);
			void iterate_column(size_t index, std::function<void(GridCellState&)> callback);
			void iterate_row(size_t index, std::function<void(GridCellState&)> callback);
			std::vector<unsigned int> get_optimal_cell_sizes(std::vector<unsigned int>& defined_sizes, std::vector<unsigned int>& text_sizes, unsigned int size);
			std::vector<unsigned int> calculate_column_sizes();
			std::vector<unsigned int> calculate_row_sizes();
			GridBorders get_border(BorderType type);

			template<typename IT>
			void distribute(std::vector<IT*> &values, IT size);
			template<typename IT>
			void distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size);
			template<typename IT>
			IT summary(std::vector<IT> &values);

			unsigned int _rows;
			unsigned int _cols;
			unsigned int _width = 0;
			unsigned int _height = 0;
			std::vector<std::vector<GridCellState>> _grid;
			size_t _current_cell_r = 0;
			size_t _current_cell_c = 0;
			termiq::style::Color _border_foreground_color;
			termiq::style::Color _border_background_color;
			BorderType _border_type = BorderType::SINGLE_ASCII;
	};
}

#include "tq_term_canvas_gridbuilder.hpp"

#endif // _TQ_TERM_CANVAS_GRIDBUILDER
