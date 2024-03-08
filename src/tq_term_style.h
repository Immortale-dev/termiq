#ifndef _TQ_TERM_STYLE
#define _TQ_TERM_STYLE

namespace termiq {
	namespace style {
		struct Color {
			int r,g,b;
		};

		void foreground(int r, int g, int b);
		void background(int r, int g, int b);
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
