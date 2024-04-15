#ifndef _TQ_TERM_CANVAS_FLEXIBLE
#define _TQ_TERM_CANVAS_FLEXIBLE

namespace termiq {
	namespace canvas {
		class Flexible {
			protected:
				virtual void suggest_width(unsigned int width) = 0;
				virtual void suggest_height(unsigned int height) = 0;
				virtual void apply_suggest_width(Flexible& c, unsigned int width) { c.suggest_width(width); }
				virtual void apply_suggest_height(Flexible& c, unsigned int height) { c.suggest_height(height); }
		};
	}
}

#endif // _TQ_TERM_CANVAS_FLEXIBLE
