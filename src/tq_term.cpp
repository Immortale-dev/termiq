#include "tq_term.h"

#include <stdexcept>
#include <string>
#include <istream>
#include <ostream>
#include <clocale>
#include <cstdio>
#include <unistd.h>
#include <initializer_list>
#include <format>
#include <vector>
#include <csignal>

#include <stdlib.h>
#include <termios.h>

const termiq::color_t termiq::Color::NONE = std::nullopt;

void termiq::init_term(int input_file_descriptor, int output_file_descriptor)
{
	detail::input = input_file_descriptor;
	detail::output = output_file_descriptor;
	std::atexit(detail::exit_raw_mode);
}

int termiq::detail::get_input_file_descriptor()
{
	return detail::input;
}

int termiq::detail::get_output_file_descriptor()
{
	return detail::output;
}

void termiq::enable_raw_mode(size_t limit, size_t timeout, bool stop_sigs)
{
	if (detail::raw_mode_enabled_count++) return;

	int fd = detail::get_input_file_descriptor();
	if (!isatty(fd)) detail::fatal("STDIN_FILENO check failed");
	if (tcgetattr(fd, &detail::orig_termios) == -1) detail::fatal("tcgetattr STDIN failed");
	termios raw = detail::orig_termios;

	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	/* output modes - disable post processing */
	raw.c_oflag &= ~(OPOST);
	/* control modes - set 8 bit chars */
	raw.c_cflag |= (CS8);
	/* local modes - choing off, canonical off, no extended functions,
	 * no signal chars (^Z,^C) */
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | (stop_sigs ? ISIG : 0));
	/* control chars - set return condition: min number of bytes and timer. */
	raw.c_cc[VMIN] = limit; /* Return each byte, or zero for timeout. */
	raw.c_cc[VTIME] = timeout; /* 100 ms timeout (unit is tens of second). */

	if (tcsetattr(fd, TCSAFLUSH, &raw) < 0) detail::fatal("tssetattr failed");
}

void termiq::disable_raw_mode()
{
	if (--detail::raw_mode_enabled_count) return;

	tcsetattr(detail::get_input_file_descriptor(), TCSAFLUSH, &detail::orig_termios);
}

bool termiq::in_raw_mode()
{
	return detail::raw_mode_enabled_count > 0;
}

void termiq::set_utf_locale()
{
	std::setlocale(LC_ALL, "en_US.utf8");
}

void termiq::detail::fatal(std::string_view msg)
{
    std::cout << msg << std::endl;
    exit(1);
}

void termiq::detail::exit_raw_mode()
{
	if (raw_mode_enabled_count == 0) return;
	tcsetattr(detail::get_input_file_descriptor(), TCSAFLUSH, &detail::orig_termios);
}

size_t termiq::detail::read_exactly(Reader* r, char* c, size_t sz)
{
	size_t s=0;
	while(s < sz) s += r->read(c + s, sz - s);
	return sz;
}

size_t termiq::detail::read_until_ch(Reader* r, char* c, char ch, size_t limit)
{
	size_t s=0;
	while(s < limit) {
		while(!r->read(c + s, 1));
		if (c[s++] == ch) return s;
	}
	return s;
}

std::optional<size_t> termiq::detail::read_unsigned_until_ch(Reader* reader, char ch, size_t limit)
{
	char n;
	size_t h = 0;
	while(true) {
		detail::read_exactly(reader, &n, 1);
		if (n == ch) break;
		if (n < '0' || n > '9') return std::nullopt;
		h = h*10+(n-'0');
		if (h > limit) return std::nullopt;
	}
	return h;
}

// ESC sequences

std::string termiq::push_kitty_keys_str(KittyFlags flags)
{
	return std::format("{}{}>{}u", ::termiq::code::ST, ::termiq::code::CSI, (uint32_t)(flags));
}

std::string termiq::pop_kitty_keys_str()
{
	return std::format("{}{}<u", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::set_kitty_keys_str(KittyFlags flags)
{
	return std::format("{}{}={}1u", ::termiq::code::ST, ::termiq::code::CSI, (uint32_t)(flags));
}

std::string termiq::add_kitty_keys_str(KittyFlags flags)
{
	return std::format("{}{}={}2u", ::termiq::code::ST, ::termiq::code::CSI, (uint32_t)(flags));
}

std::string termiq::remove_kitty_keys_str(KittyFlags flags)
{
	return std::format("{}{}={}3u", ::termiq::code::ST, ::termiq::code::CSI, (uint32_t)(flags));
}

std::string termiq::get_size_px_str()
{
	return std::format("{}{}14t", ::termiq::code::ST, ::termiq::code::CSI);
}

std::optional<termiq::Pos> termiq::get_size_px_parser(Reader* reader)
{
	std::vector<char> c(2);
	while(true) {
		// read ST
		detail::read_exactly(reader, c.data(), 1);
		if (c[0] != ::termiq::code::ST) continue;
		// read CSI 4
		detail::read_exactly(reader, c.data(), 2);
		if (c[0] != ::termiq::code::CSI || c[1] != '4') continue;
	}
	// skip ";"
	detail::read_exactly(reader, c.data(), 1);
	std::optional<size_t> oh,ow;
	// read height until ";"
	oh = detail::read_unsigned_until_ch(reader, ';', 1e7);
	if (!oh) return std::nullopt;
	// read width until "t"
	ow = detail::read_unsigned_until_ch(reader, 't', 1e7);
	if (!ow) return std::nullopt;
	return Pos{static_cast<int32_t>(*oh),static_cast<int32_t>(*ow)};
}

std::string termiq::get_size_ch_str()
{
	return std::format("{}{}18t", ::termiq::code::ST, ::termiq::code::CSI);
}

std::optional<termiq::Pos> termiq::get_size_ch_parser(Reader* reader)
{
	std::vector<char> c(2);
	while(true) {
		// read ST
		detail::read_exactly(reader, c.data(), 1);
		if (c[0] != ::termiq::code::ST) continue;
		// read CSI 8
		detail::read_exactly(reader, c.data(), 2);
		if (c[0] == ::termiq::code::CSI && c[1] == '8') break;
	}
	// skip ";"
	detail::read_exactly(reader, c.data(), 1);
	std::optional<size_t> oh,ow;
	// read height until ";"
	oh = detail::read_unsigned_until_ch(reader, ';', 1e7);
	if (!oh) return std::nullopt;
	// read width until "t"
	ow = detail::read_unsigned_until_ch(reader, 't', 1e7);
	if (!ow) return std::nullopt;
	return Pos{static_cast<int32_t>(*oh),static_cast<int32_t>(*ow)};
}

std::string termiq::get_pos_str()
{
	return std::format("{}{}6n", ::termiq::code::ST, ::termiq::code::CSI);
}

std::optional<termiq::Pos> termiq::get_pos_parser(Reader* reader)
{
	char c;
	while(true) {
		// read ST
		detail::read_exactly(reader, &c, 1);
		if (c != ::termiq::code::ST) continue;
		// read CSI
		detail::read_exactly(reader, &c, 1);
		if (c != ::termiq::code::CSI) continue;
	}
	std::optional<size_t> oh,ow;
	// read height until ";"
	oh = detail::read_unsigned_until_ch(reader, ';', 1e7);
	if (!oh) return std::nullopt;
	// read width until "R"
	ow = detail::read_unsigned_until_ch(reader, 'R', 1e7);
	if (!ow) return std::nullopt;
	return Pos{static_cast<int32_t>(*oh),static_cast<int32_t>(*ow)};
}

std::string termiq::move_str(uint32_t r, uint32_t c)
{
	return std::format("{}{}{};{}H", ::termiq::code::ST, ::termiq::code::CSI, r+1, c+1);
}

std::string termiq::move_backward_str(uint32_t steps)
{
	return std::format("{}{}{}D", ::termiq::code::ST, ::termiq::code::CSI, steps);
}

std::string termiq::move_forward_str(uint32_t steps)
{
	return std::format("{}{}{}C", ::termiq::code::ST, ::termiq::code::CSI, steps);
}

std::string termiq::move_up_str(uint32_t steps)
{
	return std::format("{}{}{}A", ::termiq::code::ST, ::termiq::code::CSI, steps);
}

std::string termiq::move_down_str(uint32_t steps)
{
	return std::format("{}{}{}B", ::termiq::code::ST, ::termiq::code::CSI, steps);
}

std::string termiq::save_cursor_position_str()
{
	return std::format("{}7", ::termiq::code::ST);
}

std::string termiq::restore_cursor_position_str()
{
	return std::format("{}8", ::termiq::code::ST);
}

std::string termiq::cursor_default_str()
{
	return std::format("{}{}?25h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::cursor_hidden_str()
{
	return std::format("{}{}?25l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::set_foreground_color_str(color_t color)
{
	if (color == Color::NONE) return std::format("{}{}39m", ::termiq::code::ST, ::termiq::code::CSI);
	return std::format("{}{}38;2;{};{};{}m", ::termiq::code::ST, ::termiq::code::CSI, color->r, color->g, color->b);
}

std::string termiq::set_background_color_str(color_t color)
{
	if (color == Color::NONE) return std::format("{}{}49m", ::termiq::code::ST, ::termiq::code::CSI);
	return std::format("{}{}48;2;{};{};{}m", ::termiq::code::ST, ::termiq::code::CSI, color->r, color->g, color->b);
}

std::string termiq::enter_alternate_buffer_str()
{
	return std::format("{}{}?1049h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::exit_alternate_buffer_str()
{
	return std::format("{}{}?1049l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::clear_str()
{
	return std::format("{}{}2J", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::clear_before_str()
{
	return std::format("{}{}1J", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::clear_after_str()
{
	return std::format("{}{}0J", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::clear_line_str()
{
	return std::format("{}{}2K", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::clear_line_before_str()
{
	return std::format("{}{}1K", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::clear_line_after_str()
{
	return std::format("{}{}0K", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::set_automatic_newline_str(bool value)
{
	char ch = value ? 'h' : 'l';
	return std::format("{}{}20{}", ::termiq::code::ST, ::termiq::code::CSI, ch);
}

std::string termiq::set_wraparound_str(bool value)
{
	char ch = value ? 'h' : 'l';
	return std::format("{}{}7{}", ::termiq::code::ST, ::termiq::code::CSI, ch);
}

std::string termiq::insert_chars_str(uint32_t cnt)
{
	return std::format("{}{}{}@", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::insert_lines_str(uint32_t cnt)
{
	return std::format("{}{}{}L", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::delete_chars_str(uint32_t cnt)
{
	return std::format("{}{}{}P", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::delete_lines_str(uint32_t cnt)
{
	return std::format("{}{}{}M", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::erase_chars_str(uint32_t cnt)
{
	return std::format("{}{}{}X", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::scroll_up_str(uint32_t cnt)
{
	return std::format("{}{}{}S", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::scroll_down_str(uint32_t cnt)
{
	return std::format("{}{}{}T", ::termiq::code::ST, ::termiq::code::CSI, cnt);
}

std::string termiq::set_styles_str(std::vector<StyleProp> &&styles)
{
	std::string str;
	for (StyleProp s : styles) {
		str.append(std::to_string((int)s));
		str.push_back(';');
	}
	if (str.size() > 0) str.pop_back();
	return std::format("{}{}{}m", ::termiq::code::ST, ::termiq::code::CSI, str);
}

std::string termiq::reset_styles_str()
{
	return std::format("{}{}0m", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::set_underline_style_str(UnderlineStyle style)
{
	return std::format("{}{}4:{}m", ::termiq::code::ST, ::termiq::code::CSI, (int)style);
}

std::string termiq::set_underline_color_str(color_t color)
{
	if (color == Color::NONE) return std::format("{}{}59m", ::termiq::code::ST, ::termiq::code::CSI);
	return std::format("{}{}58;2;{};{};{}m", ::termiq::code::ST, ::termiq::code::CSI, color->r, color->g, color->b);
}

std::string termiq::set_cursor_str(CursorStyle style)
{
	return std::format("{}{}{} q", ::termiq::code::ST, ::termiq::code::CSI, (int)style);
}

std::string termiq::set_cursor_shape_str(CursorShape shape)
{
	std::string_view shape_str = detail::cursor_shapes[(size_t)shape];
	return std::format("{}{}22;{}{}{}", ::termiq::code::ST, ::termiq::code::OSC, shape_str, ::termiq::code::ST, ::termiq::code::BSL);
}

std::string termiq::push_cursor_shape_str(CursorShape shape)
{
	std::string_view shape_str = detail::cursor_shapes[(size_t)shape];
	return std::format("{}{}22;>{}{}{}", ::termiq::code::ST, ::termiq::code::OSC, shape_str, ::termiq::code::ST, ::termiq::code::BSL);
}

std::string termiq::pop_cursor_shape_str()
{
	return std::format("{}{}22;<{}{}", ::termiq::code::ST, ::termiq::code::OSC, ::termiq::code::ST, ::termiq::code::BSL);
}

std::string termiq::enable_mouse_buttons_str()
{
	return std::format("{}{}?1000h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_mouse_buttons_str()
{
	return std::format("{}{}?1000l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::enable_mouse_cell_motions_str()
{
	return std::format("{}{}?1002h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_mouse_cell_motions_str()
{
	return std::format("{}{}?1002l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::enable_mouse_all_motions_str()
{
	return std::format("{}{}?1003h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_mouse_all_motions_str()
{
	return std::format("{}{}?1003l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::enable_paste_brackets_str()
{
	return std::format("{}{}?2004h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_paste_brackets_str()
{
	return std::format("{}{}?2004l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::sync_begin_str()
{
	return std::format("{}{}?2026h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::sync_end_str()
{
	return std::format("{}{}?2026l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::query_color_str(ColorType type)
{
	std::string_view q = detail::query_color_types[(size_t)type];
	return std::format("{}{}21;{}=?{}", ::termiq::code::ST, ::termiq::code::OSC, q, ::termiq::code::BEL);
}

termiq::color_t termiq::query_color_parser(Reader* reader)
{
	std::vector<char> c(30);
	while(true) {
		// read ST
		detail::read_exactly(reader, c.data(), 1);
		if (c[0] != ::termiq::code::ST) continue;
		// read OSC 21
		detail::read_exactly(reader, c.data(), 3);
		if (c[0] != ::termiq::code::OSC || std::string_view(c.data()+1, 2) != "21") continue;
	}
	// skip ";"
	detail::read_exactly(reader, c.data(), 1);
	size_t r = detail::read_until_ch(reader, c.data(), '=', 30);
	if (r == 30 && c.back() != '=') return Color::NONE;
	detail::read_exactly(reader, c.data(), 4);
	if (std::string_view(c.data(), 4) != "rgb:") return Color::NONE;
	detail::read_exactly(reader, c.data(), 8);
	if (c[2] != '/' || c[5] != '/') return Color::NONE;
	std::string sr(c.begin(), c.begin() + 2);
	std::string sg(c.begin() + 3, c.begin() + 5);
	std::string sb(c.begin() + 6, c.end());
	size_t rv = std::stoul(sr, nullptr, 16);
	size_t gv = std::stoul(sg, nullptr, 16);
	size_t bv = std::stoul(sb, nullptr, 16);
	if (rv > 255 || gv > 255 || bv > 255) return Color::NONE;
	return Color{static_cast<uint8_t>(rv), static_cast<uint8_t>(gv), static_cast<uint8_t>(bv)};
}

std::string termiq::set_color_str(ColorType type, color_t color)
{
	std::string_view q = detail::query_color_types[(size_t)type];
	std::string v;
	if (color) v = std::format("=#{:x}{:x}{:x}", color->r, color->g, color->b);
	return std::format("{}{}21;{}{}{}", ::termiq::code::ST, ::termiq::code::OSC, q, v, ::termiq::code::BEL);
}
