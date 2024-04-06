#ifndef _TQ_TERM_STYLE
#define _TQ_TERM_STYLE

namespace termiq {
	namespace style {
		struct Color {
			int r,g,b;
			bool operator==(const Color &other) const { return r == other.r && g == other.g && b == other.b; }
			bool operator!=(const Color &other) const { return !(*this == other); }

			static const Color UNDEFINED;// = {-1, -1, -1};
		};

		void style(Color fg, Color bg, bool bold, bool italic, bool dim, bool underline, bool inverse);
		void foreground(Color color);
		void background(Color color);
		void bold(bool state);
		void italic(bool state);
		void special_chars(bool state);
		void underline(bool state);
		void dim(bool state);
		void inverse(bool state);
		void clear_styles(bool also_clear_colors=false);
		void clear_colors();
		void clear();
		void reset();

		bool is_color_defined(const Color &color);
	}
}

#endif // _TQ_TERM_STYLE
