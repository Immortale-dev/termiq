#ifndef _TQ_TERM_CANVAS_GRID
#define _TQ_TERM_CANVAS_GRID

#include <functional>
#include <vector>

#include "tq_term_adv.h"
#include "tq_term_unicode_draws.h"
#include "tq_term_style.h"
#include "tq_term_canvas_utils.h"

namespace termiq {
	namespace canvas {
		enum class BorderType {
			NONE = 0,
			SINGLE = 1,
			DOUBLE = 2,
			BOLD = 3,
			ROUND = 4,
			EMPTY = 5
		};

		template <typename CC>
		class Grid : public Content<CC> {
			using char_type = typename CC::char_type;
			struct GridCellState {
				termiq::color_t background = ::termiq::Color::NONE;
				Content<CC>* content = nullptr;
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
				Grid(unsigned int rows=1, unsigned int cols=1);
				void select_cell(size_t c);
				void select_cell(size_t r, size_t c);
				void set_background_color(termiq::color_t color);
				void set_border_foreground_color(termiq::color_t color);
				void set_border_background_color(termiq::color_t color);
				void set_cell_background_color(termiq::color_t color);
				void set_cell_content(Content<CC>* content);
				void set_width(unsigned int width);
				void set_height(unsigned int height);
				void set_cell_width(unsigned int width);
				void set_cell_height(unsigned int height);
				void set_border_type(BorderType type);
				CanvasPieces<CC> build() override;

				unsigned int get_width() override;
				unsigned int get_height() override;
				unsigned int min_width() override;
				unsigned int min_height() override;

			protected:
				virtual std::vector<unsigned int> calculate_column_sizes();
				virtual std::vector<unsigned int> calculate_row_sizes();
				virtual void calc();
				void lazy_calc();
				void invalidate_calc();
				bool is_valid_calc();
				void iterate_column(size_t index, std::function<void(GridCellState&)> callback);
				void iterate_row(size_t index, std::function<void(GridCellState&)> callback);
				unsigned int get_column_defined_width(size_t index);
				unsigned int get_row_defined_height(size_t index);
				GridCellState& get_current_cell();

				std::vector<unsigned int> _cols_widths;
				std::vector<unsigned int> _rows_heights;
				unsigned int _rows;
				unsigned int _cols;
				unsigned int _width = 0;
				unsigned int _height = 0;
				std::vector<std::vector<GridCellState>> _grid;
				size_t _current_cell_r = 0;
				size_t _current_cell_c = 0;
				termiq::color_t _border_foreground_color = termiq::Color::NONE;
				termiq::color_t _border_background_color = termiq::Color::NONE;
				BorderType _border_type = BorderType::SINGLE;

			private:
				GridBorders get_border(BorderType type);
				template<typename IT>
				void distribute(std::vector<IT*> &values, IT size);
				template<typename IT>
				void distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size);

				bool _valid_calc = false;

				static inline char_type EMPTY_SPACE = {' '};
		};
	}
}

#include "tq_term_canvas_grid.hpp"

#endif // _TQ_TERM_CANVAS_GRID
