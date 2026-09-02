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
#include <functional>

#include <stdlib.h>
#include <termios.h>

const termiq::Color termiq::Color::NONE = termiq::Color();

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

void termiq::disable_raw_mode(bool force)
{
	if (!force && --detail::raw_mode_enabled_count) return;

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

size_t termiq::detail::read_until_true(Reader* r, char* c, std::function<bool(char)> fn, size_t limit)
{
	size_t s=0;
	while(s < limit) {
		while(!r->read(c + s, 1));
		if (fn(c[s++])) return s;
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

std::string termiq::detail::base64_encode(std::string&& src)
{
	return base64_encode(src);
}

std::string termiq::detail::base64_encode(std::string_view src)
{
	unsigned char *out, *pos;
	const unsigned char *end, *in;

	// 3 bytes blocks to 4 bytes
	size_t len = 4 * ((src.size() + 2) / 3);

	// int overflow
	if (len < src.size()) return std::string{};

	std::string result;
	result.resize(len);
	out = reinterpret_cast<unsigned char*>(result.data());

	in = reinterpret_cast<const unsigned char*>(src.data());
	end = in + src.size();
	pos = out;
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0b00000011) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0b00001111) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0b00111111];
		in += 3;
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0b00000011) << 4];
			*pos++ = '=';
		}
		else {
			*pos++ = base64_table[((in[0] & 0b00000011) << 4) | (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0b00001111) << 2];
		}
		*pos++ = '=';
	}

	return result;
}

std::string termiq::detail::base64_decode(std::string&& src)
{
	return base64_decode(src);
}

std::string termiq::detail::base64_decode(std::string_view src)
{
	const unsigned char* p = reinterpret_cast<const unsigned char*>(src.data());
	size_t len = src.size();
	int32_t pad = len > 0 && (len % 4 || p[len - 1] == '=');
	const size_t temp_len = ((len + 3) / 4 - pad) * 4;
	std::string str(temp_len / 4 * 3 + pad, '\0');

	for (size_t i = 0, j = 0; i < temp_len; i += 4) {
		int n = base64_index[p[i]] << 18 | base64_index[p[i + 1]] << 12 | base64_index[p[i + 2]] << 6 | base64_index[p[i + 3]];
		str[j++] = n >> 16;
		str[j++] = n >> 8 & 0xFF;
		str[j++] = n & 0xFF;
	}
	if (pad) {
		int n = base64_index[p[temp_len]] << 18 | base64_index[p[temp_len + 1]] << 12;
		str[str.size() - 1] = n >> 16;

		if (len > temp_len + 2 && p[temp_len + 2] != '=') {
			n |= base64_index[p[temp_len + 2]] << 6;
			str.push_back(n >> 8 & 0xFF);
		}
	}
	return str;
}

termiq::OSC5522Status termiq::detail::osc5522_get_metadata_status(OSC5522Metadata& metadata)
{
	if (!metadata.valid) return OSC5522Status::UNKNOWN;

	auto status_it = std::find_if(metadata.headers.begin(), metadata.headers.end(), [](std::pair<std::string, std::string>& header){
		return header.first == "status";
	});

	if (status_it == metadata.headers.end()) return OSC5522Status::UNKNOWN;

	std::string code = status_it->second;
	if (code == "OK") return OSC5522Status::OK;
	if (code == "DONE") return OSC5522Status::DONE;
	if (code == "DATA") return OSC5522Status::DATA;
	if (code == "EIO") return OSC5522Status::E_IO;
	if (code == "EINVAL") return OSC5522Status::E_INVAL;
	if (code == "ENOSYS") return OSC5522Status::E_NOSYS;
	if (code == "EPERM") return OSC5522Status::E_PERM;
	if (code == "EBUSY") return OSC5522Status::E_BUSY;
	return OSC5522Status::UNKNOWN;
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

std::string termiq::set_foreground_color_str(Color color)
{
	// TODO: finish
	if (color == Color::NONE) return std::format("{}{}39m", ::termiq::code::ST, ::termiq::code::CSI);
	if (color.is_true()) {
		return std::format("{}{}38;2;{};{};{}m", ::termiq::code::ST, ::termiq::code::CSI, color.r(), color.g(), color.b());
	}
	// PlaceColor
	return std::format("{}{}38;5;{}m", ::termiq::code::ST, ::termiq::code::CSI, color.place());
}

std::string termiq::set_background_color_str(Color color)
{
	if (color == Color::NONE) return std::format("{}{}49m", ::termiq::code::ST, ::termiq::code::CSI);
	return std::format("{}{}48;2;{};{};{}m", ::termiq::code::ST, ::termiq::code::CSI, color.r(), color.g(), color.b());
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
	return std::format("{}{}?7{}", ::termiq::code::ST, ::termiq::code::CSI, ch);
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

std::string termiq::set_underline_color_str(Color color)
{
	if (color == Color::NONE) return std::format("{}{}59m", ::termiq::code::ST, ::termiq::code::CSI);
	return std::format("{}{}58;2;{};{};{}m", ::termiq::code::ST, ::termiq::code::CSI, color.r(), color.g(), color.b());
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

std::string termiq::enable_osc5522_str()
{
	return std::format("{}{}?5522h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_osc5522_str()
{
	return std::format("{}{}?5522l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::check_osc5522_str()
{
	return std::format("{}{}?5522$p", ::termiq::code::ST, ::termiq::code::CSI);
}

termiq::ProtocolSupport termiq::check_osc5522_parser(Reader* reader)
{
	std::string c;
	c.resize(7);
	while(true) {
		// read ST
		detail::read_exactly(reader, c.data(), 1);
		if (c[0] != ::termiq::code::ST) continue;
		detail::read_exactly(reader, c.data(), 1);
		// read CSI
		if (c[0] != ::termiq::code::CSI) continue;
		// read '?'
		detail::read_exactly(reader, c.data(), 1);
		if (c[0] != '?') continue;
		// read 5522
		detail::read_exactly(reader, c.data(), 4);
		if (std::string_view(c.data(), 4) != "5522") continue;
		// skip ';'
		detail::read_exactly(reader, c.data(), 1);
		std::optional<size_t> res = detail::read_unsigned_until_ch(reader, '$', 7);
		// skip 'y;
		detail::read_exactly(reader, c.data(), 1);
		if (!res || *res == 0) return ProtocolSupport::UNSUPPORTED;
		if (*res == 1) return ProtocolSupport::ENABLED;
		if (*res == 2) return ProtocolSupport::DISABLED;
	}
}

std::string termiq::osc5522_write_begin_str(std::vector<std::pair<std::string, std::string>> headers)
{
	std::string hstr;
	for (auto& [key, val] : headers) {
		hstr.push_back(';');
		hstr.insert(hstr.end(), key.begin(), key.end());
		if (key == "name" || key == "pw") {
			hstr += detail::base64_encode(val);
		} else {
			hstr += val;
		}
	}
	return std::format(
		"{}{}5522;type=write{}{}{}",
		::termiq::code::ST, ::termiq::code::OSC,
		hstr,
		::termiq::code::ST, ::termiq::code::BSL
	);
}

std::string termiq::osc5522_write_end_str()
{
	return std::format("{}{}5522;type=wdata{}{}", ::termiq::code::ST, ::termiq::code::OSC, ::termiq::code::ST, ::termiq::code::BSL);
}

std::string termiq::osc5522_write_chunk_str(std::pair<std::string, std::string_view> data)
{
	std::string mime_encoded = detail::base64_encode(data.first);
	std::string data_encoded = detail::base64_encode(data.second);
	return std::format(
		"{}{}5522;type=wdata:mime={};{}{}{}",
		::termiq::code::ST, ::termiq::code::OSC,
		mime_encoded, data_encoded,
		::termiq::code::ST, ::termiq::code::BSL
	);
}

std::string termiq::osc5522_read_str(std::vector<std::string> mtypes, std::vector<std::pair<std::string, std::string>> headers)
{
	std::string hstr;
	for (auto& [key, val] : headers) {
		hstr.push_back(':');
		hstr.insert(hstr.end(), key.begin(), key.end());
		hstr.push_back('=');
		if (key == "name" || key == "pw") {
			hstr += detail::base64_encode(val);
		} else {
			hstr += val;
		}
	}
	std::string data;
	for (auto& m : mtypes) {
		data.insert(data.end(), m.begin(), m.end());
		data.push_back(' ');
	}
	if (!data.empty()) data.pop_back();
	std::string data_encoded = detail::base64_encode(data);
	return std::format(
		"{}{}5522;type=read{};{}{}{}",
		::termiq::code::ST, ::termiq::code::OSC,
		hstr, data_encoded,
		::termiq::code::ST, ::termiq::code::BSL);
}

termiq::OSC5522Metadata termiq::osc5522_metadata_parser(Reader* reader)
{
	const size_t BUFFER_SIZE = 1024;
	std::vector<char> buffer(BUFFER_SIZE);

	while(true) {
		// read ST
		detail::read_exactly(reader, buffer.data(), 1);
		if (buffer[0] != ::termiq::code::ST) continue;
		// read OSC
		detail::read_exactly(reader, buffer.data(), 1);
		if (buffer[0] != ::termiq::code::OSC) continue;
		// read 5522
		detail::read_exactly(reader, buffer.data(), 4);
		if (std::string_view(buffer.data(), 4) != "5522") continue;
		// skip ';'
		detail::read_exactly(reader, buffer.data(), 1);

		OSC5522Metadata metadata;
		std::vector<std::pair<std::string, std::string>>& headers = metadata.headers;
		while(true) {
			size_t sz = detail::read_until_true(reader, buffer.data(), [](char c){
				return c == ::termiq::code::ST || c == ::termiq::code::BEL || c == ':' || c == ';';
			}, BUFFER_SIZE);

			std::string_view header(buffer.data(), sz-1);
			size_t eq_pos = header.find('=');
			if (eq_pos == std::string::npos) {
				// empty header?
				headers.push_back({std::string(header.begin(), header.end()), ""});
				continue;
			}
			headers.push_back({
				std::string(header.begin(), header.begin() + eq_pos),
				std::string(header.begin() + eq_pos + 1, header.end()),
			});

			char last = buffer[sz-1];
			if (last == ::termiq::code::BEL) break;
			if (last == ::termiq::code::ST) {
				// skip BSL
				detail::read_exactly(reader, buffer.data(), 1);
				break;
			}
			if (last == ';') {
				metadata.has_payload = true;
				break;
			}
			if (last == ':') {
				// more headers to come
				continue;
			}

			// buffer overflow or invalid data
			metadata.valid = false;
			break;
		}

		return metadata;
	}
}

termiq::OSC5522Status termiq::osc5522_status_parser(Reader* reader)
{
	OSC5522Metadata metadata = osc5522_metadata_parser(reader);
	return detail::osc5522_get_metadata_status(metadata);
}

termiq::OSC5522Result termiq::osc5522_read_all_parser(Reader* reader)
{
	const size_t BUFFER_SIZE = 1024;
	OSC5522Metadata metadata = osc5522_metadata_parser(reader);
	OSC5522Status status = detail::osc5522_get_metadata_status(metadata);

	// TODO: validate password (pw header)

	std::vector<MimeData> data_vec;
	if (status != OSC5522Status::OK) return {status, std::move(data_vec)};

	std::vector<char> buffer(BUFFER_SIZE);
	while(true) {
		OSC5522Metadata md = osc5522_metadata_parser(reader);
		OSC5522Status s = detail::osc5522_get_metadata_status(md);
		if (s == OSC5522Status::DONE) return {s, std::move(data_vec)};
		if (s != OSC5522Status::DATA) return {s, std::move(data_vec)};

		if (!md.has_payload) {
			// DATA event cannot be w/o payload
			return {OSC5522Status::E_INVAL, std::move(data_vec)};
		}
		// find mime header
		auto mime_it = std::find_if(md.headers.begin(), md.headers.end(), [](std::pair<std::string, std::string>& header){
			return header.first == "mime";
		});
		if (mime_it == md.headers.end()) {
			return {OSC5522Status::E_INVAL, std::move(data_vec)};
		}

		std::string mime = detail::base64_decode(mime_it->second);
		std::string data;
		while(true) {
			size_t sz = detail::read_until_true(reader, buffer.data(), [](char c){
				return c == ::termiq::code::ST || c == ::termiq::code::BEL;
			}, BUFFER_SIZE);
			if (buffer[sz-1] == ::termiq::code::ST || buffer[sz-1] == ::termiq::code::BEL) {
				data.insert(data.end(), buffer.begin(), buffer.begin() + sz - 1);
				data_vec.push_back({std::move(mime), std::move(detail::base64_decode(data))});
				if (buffer[sz-1] == ::termiq::code::ST) {
					// skip BSL
					detail::read_exactly(reader, buffer.data(), 1);
				}
				break;
			}
			data.insert(data.end(), buffer.begin(), buffer.end());
		}
	}
}

std::string termiq::osc52_write_str(std::string_view data, char loc)
{
	return std::format(
		"{}{}52;{};{}{}{}",
		::termiq::code::ST,
		::termiq::code::OSC,
		loc, detail::base64_encode(data),
		::termiq::code::ST, ::termiq::code::BSL
	);
}

std::string termiq::osc52_read_str(char loc)
{
	return std::format(
		"{}{}52;{};?{}{}",
		::termiq::code::ST,
		::termiq::code::OSC,
		loc,
		::termiq::code::ST, ::termiq::code::BSL
	);
}

std::string termiq::osc52_read_parser(Reader* reader)
{
	const size_t BUFFER_SIZE = 1024;
	std::vector<char> buffer(BUFFER_SIZE);

	while(true) {
		// read ST
		detail::read_exactly(reader, buffer.data(), 1);
		if (buffer[0] != ::termiq::code::ST) continue;
		// read OSC
		detail::read_exactly(reader, buffer.data(), 1);
		if (buffer[0] != ::termiq::code::OSC) continue;
		// read 52
		detail::read_exactly(reader, buffer.data(), 2);
		if (std::string_view(buffer.data(), 2) != "52") continue;
		// skip ';'
		detail::read_exactly(reader, buffer.data(), 1);
		// read location
		char loc;
		detail::read_exactly(reader, &loc, 1);
		// skip ';'
		detail::read_exactly(reader, buffer.data(), 1);
		// read data
		std::string data;
		while(true) {
			size_t sz = detail::read_until_true(reader, buffer.data(), [](char c){
				return c == ::termiq::code::ST || c == ::termiq::code::BEL;
			}, BUFFER_SIZE);
			char last = buffer[sz-1];
			if (last != ::termiq::code::ST && last != ::termiq::code::BEL) {
				data.insert(data.end(), buffer.begin(), buffer.end());
				continue;
			}
			data.insert(data.end(), buffer.begin(), buffer.begin() + sz - 1);
			if (last == ::termiq::code::ST) {
				// skip BSL
				detail::read_exactly(reader, buffer.data(), 1);
			}
			break;
		}
		return detail::base64_decode(data);
	}
}

std::string termiq::enable_mouse_buttons_str()
{
	return std::format("{}{}?1000h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_mouse_buttons_str()
{
	return std::format("{}{}?1000l", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::enable_resize_report_str()
{
	return std::format("{}{}?2048h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_resize_report_str()
{
	return std::format("{}{}?2048l", ::termiq::code::ST, ::termiq::code::CSI);
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

std::string termiq::enable_unicode_graphemes_str()
{
	return std::format("{}{}?2027h", ::termiq::code::ST, ::termiq::code::CSI);
}

std::string termiq::disable_unicode_graphemes_str()
{
	return std::format("{}{}?2027l", ::termiq::code::ST, ::termiq::code::CSI);
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

std::string termiq::query_color_str(std::vector<ColorVariant> colors)
{
	std::string query;
	for (ColorVariant& color : colors) {
		if (std::holds_alternative<ColorType>(color)) {
			std::string_view q = detail::query_color_types[(size_t)std::get<ColorType>(color)];
			query.insert(query.end(), q.begin(), q.end());
		} else {
			query.append(std::to_string(std::get<ColorIndex>(color)));
		}
		query.append("=?;");
	}
	query.pop_back(); // throw last ';'
	return std::format("{}{}21;{}{}", ::termiq::code::ST, ::termiq::code::OSC, query, ::termiq::code::BEL);
}

std::string termiq::set_color_str(std::vector<std::pair<ColorVariant, Color>> colors)
{
	std::string s;
	for (auto& [type, color] : colors) {
		std::string set_color;
		if (std::holds_alternative<ColorType>(type)) {
			set_color = detail::query_color_types[(size_t)std::get<ColorType>(type)];
		} else {
			set_color = std::to_string(std::get<ColorIndex>(type));
		}
		s.append(set_color);
		if (!color) {
			s.push_back(';');
			continue;
		}
		if (!color.is_true()) {
			throw std::invalid_argument("place color is not supported");
		}
		s.append(std::format("=#{:x}{:x}{:x};", color.r(), color.g(), color.b()));
	}
	s.pop_back(); // throw last ';'
	return std::format("{}{}21;{}{}", ::termiq::code::ST, ::termiq::code::OSC, s, ::termiq::code::BEL);
}

termiq::Color termiq::query_color_parser(Reader* reader)
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
