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
			using Char = typename CC::Char;
			using Style = typename CC::Style;
			struct GridCellState {
				termiq::Color background = ::termiq::Color::NONE;
				Content<CC>* content = nullptr;
				uint32_t width = 0;
				uint32_t height = 0;
			};
			struct GridBorders {
				Char C_H;  // Horizontal line.
				Char C_V;  // Vertical line.
				Char C_X;  // Vertical/Horiaonal cross.
				Char C_HT; // Horizontal line with connection on the top.
				Char C_HB; // Horizontal line with connection on the bottom.
				Char C_VL; // Vertical line with connection on the left.
				Char C_VR; // Vertical line with connection on the right.
				Char C_TL; // Top left corner.
				Char C_TR; // Top right corner.
				Char C_BL; // Bottom left corner.
				Char C_BR; // Bottom right corner.
			};

			public:
				Grid(uint32_t rows=1, uint32_t cols=1);
				void select_cell(size_t c);
				void select_cell(size_t r, size_t c);
				void set_background_color(termiq::Color color);
				void set_border_foreground_color(termiq::Color color);
				void set_border_background_color(termiq::Color color);
				void set_cell_background_color(termiq::Color color);
				void set_cell_content(Content<CC>* content);
				void set_width(uint32_t width);
				void set_height(uint32_t height);
				void set_cell_width(uint32_t width);
				void set_cell_height(uint32_t height);
				void set_border_type(BorderType type);

				CanvasPiece<CC> build() const override;
				uint32_t get_width() const override;
				uint32_t get_height() const override;
				uint32_t min_width() const override;
				uint32_t min_height() const override;

			protected:
				virtual std::vector<uint32_t> calculate_column_sizes() const;
				virtual std::vector<uint32_t> calculate_row_sizes() const;
				virtual void calc() const;
				void lazy_calc() const;
				void invalidate_calc();
				bool is_valid_calc();
				void iterate_column(size_t index, std::function<void(const GridCellState&)> callback) const;
				void iterate_row(size_t index, std::function<void(const GridCellState&)> callback) const;
				uint32_t get_column_defined_width(size_t index) const;
				uint32_t get_row_defined_height(size_t index) const;
				GridCellState& get_current_cell();

				mutable std::vector<uint32_t> _cols_widths;
				mutable std::vector<uint32_t> _rows_heights;
				uint32_t _rows;
				uint32_t _cols;
				uint32_t _width = 0;
				uint32_t _height = 0;
				std::vector<std::vector<GridCellState>> _grid;
				size_t _current_cell_r = 0;
				size_t _current_cell_c = 0;
				termiq::Color _border_foreground_color = termiq::Color::NONE;
				termiq::Color _border_background_color = termiq::Color::NONE;
				BorderType _border_type = BorderType::SINGLE;

			private:
				GridBorders get_border(BorderType type) const;
				template<typename IT>
				void distribute(std::vector<IT*> &values, IT size);
				template<typename IT>
				void distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size);

				mutable bool _valid_calc = false;

				static inline Char EMPTY_SPACE = {' '};
		};
	}
}

#include "tq_term_canvas_grid.hpp"

#endif // _TQ_TERM_CANVAS_GRID
