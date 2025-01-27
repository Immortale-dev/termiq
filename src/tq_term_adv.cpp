#include "tq_term_adv.h"

#ifdef TERMIQ_THROW_ON_FAILED_COMMAND
#define VALIDATE_EXEC(code) if (code < 0) throw termiq::exception("Command executon error.")
#else
#define VALIDATE_EXEC(code) (void)code
#endif

// For testing.
#ifdef STUBTERM
#define CONCAT(a, b) a ## b
#define _STUB_TQ_ADV(name) termiq::term_stub_calls_.push_back(#name)
#define MAYBE_STUB(name) _STUB_TQ_ADV(name); return
#define MAYBE_STUB_ARGS(name, ...) _STUB_TQ_ADV(name); CONCAT(name, _stub_).push_back({__VA_ARGS__}); return
#define MAYBE_STUB_RET(name) _STUB_TQ_ADV(name); return CONCAT(name, _stub_ret_)
#include <vector>
#include <string>
#include <tuple>
namespace termiq {
	template<class... T>
	using stub_term_t = std::vector<std::tuple<T...>>;
	std::vector<std::string> term_stub_calls_;

	int get_cols_stub_ret_;
	int get_rows_stub_ret_;
	int get_max_colors_stub_ret_;
	int get_max_pairs_stub_ret_;

	stub_term_t<int, int> move_stub_;
	stub_term_t<int> move_left_stub_;
	stub_term_t<int> move_right_stub_;
	stub_term_t<int> move_up_stub_;
	stub_term_t<int> move_down_stub_;
	stub_term_t<int, int, int ,int> define_color_stub_;
	stub_term_t<int, int, int, int, int, int, int> define_pair_stub_;
	stub_term_t<int> set_foreground_color_stub_;
	stub_term_t<int> set_background_color_stub_;
	stub_term_t<int> set_pair_color_stub_;
	stub_term_t<bool, bool, bool, bool> set_attrs_stub_;
}
#else
#define MAYBE_STUB(name)
#define MAYBE_STUB_ARGS(name, ...)
#define MAYBE_STUB_RET(name)
#endif // STUBTERM

int termiq::get_cols() {
	MAYBE_STUB_RET(get_cols);
	const int res = get_num("cols");
	VALIDATE_EXEC(res);
	return res;
}

int termiq::get_rows() {
	MAYBE_STUB_RET(get_rows);
	const int res = get_num("lines");
	VALIDATE_EXEC(res);
	return res;
}

void termiq::move(int r, int c) {
	MAYBE_STUB_ARGS(move, r, c);
	const int res = termiq::run_str("cup", 1, r, c);
	VALIDATE_EXEC(res);
}

void termiq::move_left(int steps) {
	MAYBE_STUB_ARGS(move_left, steps);
	const int res = termiq::run_str("cub", 1, steps);
	VALIDATE_EXEC(res);
}

void termiq::move_right(int steps) {
	MAYBE_STUB_ARGS(move_right, steps);
	const int res = termiq::run_str("cuf", 1, steps);
	VALIDATE_EXEC(res);
}

void termiq::move_up(int steps) {
	MAYBE_STUB_ARGS(move_up, steps);
	const int res = termiq::run_str("cuu", 1, steps);
	VALIDATE_EXEC(res);
}

void termiq::move_down(int steps) {
	MAYBE_STUB_ARGS(move_down, steps);
	const int res = termiq::run_str("cud", 1, steps);
	VALIDATE_EXEC(res);
}

void termiq::save_cursor_position() {
	MAYBE_STUB(save_cursor_position);
	const int res = termiq::run_str("sc", 1);
	VALIDATE_EXEC(res);
}

void termiq::restore_cursor_position() {
	MAYBE_STUB(restore_cursor_position);
	const int res = termiq::run_str("rc", 1);
	VALIDATE_EXEC(res);
}

void termiq::cursor_default() {
	MAYBE_STUB(cursor_default);
	const int res = termiq::run_str("cnorm", 1);
	VALIDATE_EXEC(res);
}

void termiq::cursor_hidden() {
	MAYBE_STUB(cursor_hidden);
	const int res = termiq::run_str("civis", 1);
	VALIDATE_EXEC(res);
}

int termiq::get_max_colors() {
	MAYBE_STUB_RET(get_max_colors);
	const int res = get_num("colors");
	VALIDATE_EXEC(res);
	return res;
}

int termiq::get_max_pairs() {
	MAYBE_STUB_RET(get_max_pairs);
	const int res = get_num("pairs");
	VALIDATE_EXEC(res);
	return res;
}

void termiq::define_color(int id, int r, int g, int b) {
	MAYBE_STUB_ARGS(define_color, id, r, g, b);
	const int res = run_str("initc", 1, id, r, g, b);
	VALIDATE_EXEC(res);
}

void termiq::define_pair(int id, int r, int g, int b, int br, int bg, int bb) {
	MAYBE_STUB_ARGS(define_pair, id, r, g, b, br, bg, bb);
	const int res = run_str("initp", 1, id, r, g, b, br, bg, bb);
	VALIDATE_EXEC(res);
}

void termiq::undefine_colors() {
	MAYBE_STUB(undefine_colors);
	const int res = run_str("oc", 1);
	VALIDATE_EXEC(res);
}

void termiq::set_foreground_color(int id) {
	MAYBE_STUB_ARGS(set_foreground_color, id);
	const int res = run_str("setaf", 1, id);
	VALIDATE_EXEC(res);
}

void termiq::set_background_color(int id) {
	MAYBE_STUB_ARGS(set_background_color, id);
	const int res = run_str("setab", 1, id);
	VALIDATE_EXEC(res);
}

void termiq::set_pair_color(int id) {
	MAYBE_STUB_ARGS(set_pair_color, id);
	const int res = run_str("scp", 1, id);
	VALIDATE_EXEC(res);
}

void termiq::set_attrs(bool bold, bool dim, bool reverse, bool underline) {
	MAYBE_STUB_ARGS(set_attrs, bold, dim, reverse, underline);
	const int res = run_str("sgr", 1, 0, underline, reverse, 0, dim, bold, 0, 0, 0);
	VALIDATE_EXEC(res);
}

void termiq::reset_attrs() {
	MAYBE_STUB(reset_attrs);
	const int res = run_str("sgr0", 1);
	VALIDATE_EXEC(res);
}

void termiq::set_italic_on() {
	MAYBE_STUB(set_italic_on);
	const int res = run_str("sitm", 1);
	VALIDATE_EXEC(res);
}

void termiq::set_italic_off() {
	MAYBE_STUB(set_italic_off);
	const int res = run_str("ritm", 1);
	VALIDATE_EXEC(res);
}

void termiq::reset_colors() {
	MAYBE_STUB(reset_colors);
	const int res = run_str("op", 1);
	VALIDATE_EXEC(res);
}

void termiq::alternate_chars_on() {
	MAYBE_STUB(alternate_chars_on);
	const int res = run_str("smacs", 1);
	VALIDATE_EXEC(res);
}

void termiq::alternate_chars_off() {
	MAYBE_STUB(alternate_chars_off);
	const int res = run_str("rmacs", 1);
	VALIDATE_EXEC(res);
}

void termiq::enter_alternate_buffer() {
	MAYBE_STUB(enter_alternate_buffer);
	const int res = run_str("smcup", 1);
	VALIDATE_EXEC(res);
}

void termiq::exit_alternate_buffer() {
	MAYBE_STUB(exit_alternate_buffer);
	const int res = run_str("rmcup", 1);
	VALIDATE_EXEC(res);
}

void termiq::enter_automatic_margins() {
	MAYBE_STUB(enter_automatic_margins);
	const int res = run_str("smam", 1);
	VALIDATE_EXEC(res);
}

void termiq::exit_automatic_margins() {
	MAYBE_STUB(exit_automatic_margins);
	const int res = run_str("rmam", 1);
	VALIDATE_EXEC(res);
}

void termiq::clear() {
	MAYBE_STUB(clear);
	const int res = run_str("clear", 10);
	VALIDATE_EXEC(res);
}

void termiq::clear_before() {
	MAYBE_STUB(clear_before);
	const int res = run_str("el1", 1);
	VALIDATE_EXEC(res);
}

void termiq::clear_after() {
	MAYBE_STUB(clear_after);
	const int res = run_str("el", 1);
	VALIDATE_EXEC(res);
}

void termiq::clear_rest() {
	MAYBE_STUB(clear_rest);
	const int res = run_str("ed", 10);
	VALIDATE_EXEC(res);
}
