#ifndef _TQ_TERM_CANVAS_FLEXTEXT
#define _TQ_TERM_CANVAS_FLEXTEXT

#include "tq_term_canvas_text.h"
#include "tq_term_canvas_flexible.h"

namespace termiq {
	namespace canvas {
		template<typename CC>
		class FlexText : public Text<CC>, public Flexible {
			public:
				using Text<CC>::Text;

			protected:
				unsigned int get_calc_width() override;
				unsigned int get_calc_height() override;
				void suggest_width(unsigned int w) override;
				void suggest_height(unsigned int h) override;

			private:
				unsigned int _suggested_width = 0;
				unsigned int _suggested_height = 0;
		};
	}
}

#include "tq_term_canvas_flextext.hpp"

#endif // _TQ_TERM_CANVAS_FLEXTEXT
