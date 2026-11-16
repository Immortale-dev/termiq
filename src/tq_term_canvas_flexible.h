#ifndef _TQ_TERM_CANVAS_FLEXIBLE
#define _TQ_TERM_CANVAS_FLEXIBLE

namespace termiq {
	namespace canvas {
		class Flexible {
			protected:
				virtual void suggest_width(uint32_t width) = 0;
				virtual void suggest_height(uint32_t height) = 0;
				virtual void apply_suggest_width(Flexible& c, uint32_t width) const { c.suggest_width(width); }
				virtual void apply_suggest_height(Flexible& c, uint32_t height) const { c.suggest_height(height); }
		};
	}
}

#endif // _TQ_TERM_CANVAS_FLEXIBLE
