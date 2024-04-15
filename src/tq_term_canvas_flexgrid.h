#ifndef _TQ_TERM_CANVAS_FLEXGRID
#define _TQ_TERM_CANVAS_FLEXGRID

#include "tq_term_canvas_grid.h"
#include "tq_term_canvas_flexible.h"

namespace termiq {
	namespace canvas {
		template<typename CC>
		class FlexGrid : public Grid<CC>, public Flexible {
			public:
				using Grid<CC>::Grid;

			protected:
				void suggest_width(unsigned int w) override;
				void suggest_height(unsigned int h) override;
				std::vector<unsigned int> calculate_column_sizes() override;
				std::vector<unsigned int> calculate_row_sizes() override;
				void calc() override;

			private:
				unsigned int get_column_content_min_width(size_t index);
				unsigned int get_row_content_min_height(size_t index);
				unsigned int get_column_content_width(size_t index);
				unsigned int get_row_content_height(size_t index);
				std::vector<unsigned int> get_optimal_cell_sizes(std::vector<unsigned int> &defined_sizes, std::vector<unsigned int> &text_sizes,
					std::vector<unsigned int> &min_content_sizes, unsigned int size);
				template<typename IT>
				void distribute(std::vector<IT*> &values, IT size);
				template<typename IT>
				void distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size);
				template<typename IT>
				IT summary(std::vector<IT> &values);

				unsigned int _suggested_width = 0;
				unsigned int _suggested_height = 0;
		};
	}
}

#include "tq_term_canvas_flexgrid.hpp"

#endif // _TQ_TERM_CANVAS_FLEXGRID
