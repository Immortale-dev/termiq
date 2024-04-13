#ifndef _TQ_TERM_CANVAS_GRIDBUILDER
#define _TQ_TERM_CANVAS_GRIDBUILDER

#include <functional>
#include <vector>

#include "tq_term_adv.h"
#include "tq_term_unicode_draws.h"
#include "tq_term_style.h"
#include "tq_term_canvas_utils.h"
#include "tq_term_canvas_textbuilder.h"

namespace termiq {
	enum class BorderType {
		NONE = 0,
		SINGLE_ASCII = 1,
		SINGLE = 2,
		DOUBLE = 3,
		BOLD = 4,
		ROUND = 5,
		INVISIBLE = 6
	};

	template <typename CC>
	class GridBuilder : public FlexibleContentBuilder<CC> {
		using char_type = typename CC::char_type;
		struct GridCellState {
			termiq::style::Color background = style::Color::UNDEFINED;
			FlexibleContentBuilder<CC>* content = nullptr;
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
			GridBuilder& set_cell_content(FlexibleContentBuilder<CC>* content);
			GridBuilder& set_width(unsigned int width);
			GridBuilder& set_height(unsigned int height);
			GridBuilder& set_cell_width(unsigned int width);
			GridBuilder& set_cell_height(unsigned int height);
			GridBuilder& set_border_type(BorderType type);
			CanvasPieces<CC> build();

			unsigned int get_width();
			unsigned int get_height();
			unsigned int min_width();
			unsigned int min_height();

		protected:
			void suggest_width(unsigned int w) override;
			void suggest_height(unsigned int h) override;

		private:
			GridCellState& get_current_cell();
			unsigned int get_column_defined_width(size_t index);
			unsigned int get_row_defined_height(size_t index);
			unsigned int get_column_content_min_width(size_t index);
			unsigned int get_row_content_min_height(size_t index);
			unsigned int get_column_content_width(size_t index);
			unsigned int get_row_content_height(size_t index);
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

			void calc();
			void lazy_calc();
			void invalidate_calc();
			bool is_valid_calc();

			unsigned int _rows;
			unsigned int _cols;
			unsigned int _width = 0;
			unsigned int _height = 0;
			unsigned int _suggested_width = 0;
			unsigned int _suggested_height = 0;
			std::vector<std::vector<GridCellState>> _grid;
			size_t _current_cell_r = 0;
			size_t _current_cell_c = 0;
			std::vector<unsigned int> _cols_widths;
			std::vector<unsigned int> _rows_heights;
			termiq::style::Color _border_foreground_color = style::Color::UNDEFINED;
			termiq::style::Color _border_background_color = style::Color::UNDEFINED;
			BorderType _border_type = BorderType::SINGLE_ASCII;

			bool _valid_calc = false;

			static inline char_type EMPTY_SPACE = ' ';
	};
}

#include "tq_term_canvas_gridbuilder.hpp"

#endif // _TQ_TERM_CANVAS_GRIDBUILDER
