#include <stdexcept>
#include <string>
#include <term.h>

#include "tq_term.h"

void termiq::init_term() {
	int err;
	if (setupterm(0, 1, &err)) {
		if (err == 1) {
			throw std::logic_error("The terminal is hardcopy.");
		} else if (err == 0) {
			throw std::logic_error("Could not find terminal.");
		} else if (err == -1){
			throw std::logic_error("Could not find terminfo database.");
		} else {
			throw std::logic_error("Unknown error.");
		}
	}
}

bool termiq::get_flag(std::string name) {
	return tigetflag(name.data());
}

int termiq::get_num(std::string name) {
	return tigetnum(name.data());
}

int termiq::run_str(std::string name, int padding) {
	char* str = tigetstr(name.data());
	if (!str) {
		return -1;
	}
	tputs(str, padding, putchar);
	return 0;
}
