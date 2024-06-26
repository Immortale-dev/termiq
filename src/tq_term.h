#ifndef _TQ_TERM_INIT
#define _TQ_TERM_INIT

#include <string>
#include <term.h>

namespace termiq {
	void init_term(std::string term="");
	bool get_flag(std::string name);
	int get_num(std::string name);
	int run_str(std::string name, int padding=1);
	template<typename... P>
	int run_str(std::string name, int padding, P... args);
}

#include "tq_term.hpp"

#endif // _TQ_TERM_INIT
