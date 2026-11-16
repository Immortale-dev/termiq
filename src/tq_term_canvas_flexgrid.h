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
				void suggest_width(uint32_t w) override;
				void suggest_height(uint32_t h) override;
				std::vector<uint32_t> calculate_column_sizes() const override;
				std::vector<uint32_t> calculate_row_sizes() const override;
				void calc() const override;

			private:
				uint32_t get_column_content_min_width(size_t index) const;
				uint32_t get_row_content_min_height(size_t index) const;
				uint32_t get_column_content_width(size_t index) const;
				uint32_t get_row_content_height(size_t index) const;
				std::vector<uint32_t> get_optimal_cell_sizes(
					std::vector<uint32_t> &defined_sizes,
					std::vector<uint32_t> &text_sizes,
					std::vector<uint32_t> &min_content_sizes,
					uint32_t size) const;
				template<typename IT>
				void distribute(std::vector<IT*> &values, IT size) const;
				template<typename IT>
				void distribute_rated(std::vector<IT*> &values, std::vector<IT> &rates, IT size) const;
				template<typename IT>
				IT summary(std::vector<IT> &values) const;

				uint32_t _suggested_width = 0;
				uint32_t _suggested_height = 0;
		};
	}
}

#include "tq_term_canvas_flexgrid.hpp"

#endif // _TQ_TERM_CANVAS_FLEXGRID
