#ifndef _TQ_TERM_STYLE
#define _TQ_TERM_STYLE

namespace termiq {
	namespace style {
		struct Color {
			int r,g,b;
		};

		void foreground(Color color);
		void background(Color color);
		void bold(bool state);
		void italic(bool state);
		void underline(bool state);
		void dim(bool state);
		void inverse(bool state);
		void clear_styles();
		void clear_colors();
		void clear();
		void reset();
	}
}

#endif // _TQ_TERM_STYLE
