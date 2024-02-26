#ifndef _TQ_TERM_ADV
#define _TQ_TERM_ADV

#include <stdexcept>

#include "tq_term.h"

namespace termiq {
	class exception : public std::logic_error {
		public:
			exception(const char* msg) : std::logic_error(msg) {};
	};

	int get_cols();
	int get_rows();

	void move(int c, int r);

	void cursor_default();
	void cursor_hidden();

	int get_max_colors();
	int get_max_pairs();

	void define_color(int id, int r, int g, int b);
	void define_pair(int id, int r, int g, int b, int br, int bg, int bb);
	void undefine_colors();

	void set_foreground_color(int id);
	void set_background_color(int id);
	void set_pair_color(int id);
	void set_attrs(bool bold, bool dim, bool reverse, bool underline);
	void reset_attrs();
	void set_italic_on();
	void set_italic_off();
	void reset_colors();

	void enter_alternate_buffer();
	void exit_alternate_buffer();

	void clear();
	void clear_before();
	void clear_after();
	void clear_rest();
}

#endif // TQ_TERM_ADV
