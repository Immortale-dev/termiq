#include "tq_term_adv.h"

#define VALIDATE_EXEC(code) if (code < 0) throw termiq::exception("Command executon error.");

int termiq::get_cols() {
	const int res = get_num("cols");
	VALIDATE_EXEC(res);
	return res;
}

int termiq::get_rows() {
	const int res = get_num("lines");
	VALIDATE_EXEC(res);
	return res;
}

void termiq::move(int c, int r) {
	const int res = termiq::run_str("cup", 1, r, c);
	VALIDATE_EXEC(res);
}

void save_cursor_position() {
	const int res = termiq::run_str("sc", 1);
	VALIDATE_EXEC(res);
}

void restore_cursor_position() {
	const int res = termiq::run_str("rs", 1);
	VALIDATE_EXEC(res);
}

void termiq::cursor_default() {
	const int res = termiq::run_str("cnorm", 1);
	VALIDATE_EXEC(res);
}

void termiq::cursor_hidden() {
	const int res = termiq::run_str("civis", 1);
	VALIDATE_EXEC(res);
}

int termiq::get_max_colors() {
	const int res = get_num("colors");
	VALIDATE_EXEC(res);
	return res;
}

int termiq::get_max_pairs() {
	const int res = get_num("pairs");
	VALIDATE_EXEC(res);
	return res;
}

void termiq::define_color(int id, int r, int g, int b) {
	const int res = run_str("initc", 1, id, r, g, b);
	VALIDATE_EXEC(res);
}

void termiq::define_pair(int id, int r, int g, int b, int br, int bg, int bb) {
	const int res = run_str("initp", 1, id, r, g, b, br, bg, bb);
	VALIDATE_EXEC(res);
}

void termiq::undefine_colors() {
	const int res = run_str("oc", 1);
	VALIDATE_EXEC(res);
}

void termiq::set_foreground_color(int id) {
	const int res = run_str("setaf", 1, id);
	VALIDATE_EXEC(res);
}

void termiq::set_background_color(int id) {
	const int res = run_str("setab", 1, id);
	VALIDATE_EXEC(res);
}

void termiq::set_pair_color(int id) {
	const int res = run_str("scp", 1, id);
	VALIDATE_EXEC(res);
}

void termiq::set_attrs(bool bold, bool dim, bool reverse, bool underline) {
	const int res = run_str("sgr", 1, 0, underline, reverse, 0, dim, bold, 0, 0, 0);
	VALIDATE_EXEC(res);
}

void termiq::reset_attrs() {
	const int res = run_str("sgr0", 1);
	VALIDATE_EXEC(res);
}

void termiq::set_italic_on() {
	const int res = run_str("sitm", 1);
	VALIDATE_EXEC(res);
}

void termiq::set_italic_off() {
	const int res = run_str("ritm", 1);
	VALIDATE_EXEC(res);
}

void termiq::reset_colors() {
	const int res = run_str("op", 1);
	VALIDATE_EXEC(res);
}

void termiq::enter_alternate_buffer() {
	const int res = run_str("smcup", 1);
	VALIDATE_EXEC(res);
}

void termiq::exit_alternate_buffer() {
	const int res = run_str("rmcup", 1);
	VALIDATE_EXEC(res);
}

void termiq::clear() {
	const int res = run_str("clear", 10);
	VALIDATE_EXEC(res);
}

void termiq::clear_before() {
	const int res = run_str("el1", 1);
	VALIDATE_EXEC(res);
}

void termiq::clear_after() {
	const int res = run_str("el", 1);
	VALIDATE_EXEC(res);
}

void termiq::clear_rest() {
	const int res = run_str("ed", 10);
	VALIDATE_EXEC(res);
}
