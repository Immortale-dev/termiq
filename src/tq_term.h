#ifndef _TQ_TERM_INIT
#define _TQ_TERM_INIT

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string_view>
#include <initializer_list>
#include <optional>
#include <iostream>
#include <vector>
#include <span>

#include <termios.h>
#include <unistd.h>

namespace termiq {
	struct Color;
	using color_t = std::optional<Color>;
	struct Color {
		uint8_t r,g,b;

		bool operator==(const Color &other) const { return r==other.r && g==other.g && b==other.b; }
		bool operator!=(const Color &other) const { return !(*this == other); }

		static const color_t NONE;
	};

	struct Pos {
		int32_t r,c;
	};

	enum class UnderlineStyle {
		NONE      = 0,
		STRAIGHT  = 1,
		DOUBLE    = 2,
		CURLY     = 3,
		DOTTED    = 4,
		DASHED    = 5,
	};

	enum class StyleProp {
		RESET       = 0,
		BOLD        = 1,
		DIM         = 2,
		ITALIC      = 3,
		UNDERLINE   = 4,
		BLINKING    = 5,
		INVERSE     = 7,
		HIDDEN      = 8,
		STRIKE      = 9,
		NORMAL      = 22,
		STRAIGHT    = 23,
		NONDERLINE  = 24,
		STARING     = 25,
		OUTVERSE    = 27,
		VISIBLE     = 28,
		UNSTRIKE    = 29,
	};

	enum class CursorStyle {
		DEFAULT             = 0,
		BLINKING_BLOCK      = 1,
		STEADY_BLOCK        = 2,
		BLINKING_UNDERLINE  = 3,
		STEADY_UNDERLINE    = 4,
		BLINKING_BAR        = 5,
		STEADY_BAR          = 6,
	};

	enum class CursorShape {
		NONE           = 0,
		ALIAS          = 1,
		CELL           = 2,
		COPY           = 3,
		CROSSHAIR      = 4,
		DEFAULT        = 5,
		E_RESIZE       = 6,
		EW_RESIZE      = 7,
		GRAP           = 8,
		GRABBING       = 9,
		HELP           = 10,
		MOVE           = 11,
		N_RESIZE       = 12,
		NE_RESIZE      = 13,
		NESW_RESIZE    = 14,
		NO_DROP        = 15,
		NOT_ALLOWED    = 16,
		NS_RESIZE      = 17,
		NW_RESIZE      = 18,
		NWSE_RESIZE    = 19,
		POINTER        = 20,
		PROGRESS       = 21,
		S_RESIZE       = 22,
		SE_RESIZE      = 23,
		SW_RESIZE      = 24,
		TEST           = 25,
		VERTICAL_TEXT  = 26,
		W_RESIZE       = 27,
		WAIT           = 28,
		ZOOM_IN        = 29,
		ZOOM_OUT       = 30,
	};

	enum class ColorType {
		FOREGROUND            = 0,
		BACKGROUND            = 1,
		SELECTION_FOREGROUND  = 2,
		SELECTION_BACKGROUND  = 3,
		CURSOR_FOREGROUND     = 4,
		CURSOR_BACKGROUND     = 5,
	};

	enum class KittyFlags {
		DISAMBIGUATE       = 1,
		EVENT_TYPES        = 2,
		ALTERNATE_KEYS     = 4,
		ONLY_ESCAPE_CODES  = 8,
		ASSOCIATED_TEXT    = 16,
		ALL                = 31,
	};
	inline KittyFlags operator | (KittyFlags a, KittyFlags b) {
		return static_cast<KittyFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	namespace code {
		inline constexpr char BEL = 0x7;  // Bell
		inline constexpr char ST  = 0x1B; // String Terminator
		inline constexpr char IND = 0x44; // Index
		inline constexpr char NEL = 0x45; // New Line
		inline constexpr char HTC = 0x48; // Tab Set
		inline constexpr char RI  = 0x4D; // Reverse Index
		inline constexpr char SS2 = 0x4E; // Single Shift Select of G2 Character Set
		inline constexpr char SS3 = 0x4F; // Single Shift Select of G3 Character Set
		inline constexpr char DCS = 0x50; // Device Control String
		inline constexpr char SPA = 0x56; // Start of Guarded Area
		inline constexpr char EPA = 0x57; // End of Guarded Area
		inline constexpr char SOS = 0x58; // Start of String
		inline constexpr char CSI = 0x5B; // Control Sequence Introducer
		inline constexpr char BSL = 0x5C; // Back slash
		inline constexpr char OSC = 0x5D; // Operating System Command
		inline constexpr char PM  = 0x5E; // Privacy Message
		inline constexpr char APC = 0x5F; // Application Program Command
	}

	class Reader {
		public:
			virtual size_t read(char* buf, size_t sz) = 0;
	};

	class CReader : public Reader {
		public:
			CReader(int fd = STDIN_FILENO) : fd_(fd) {}
			size_t read(char* buf, size_t sz) override { return ::read(fd_, buf, sz); }

		private:
			int fd_;
	};

	class FReader : public Reader {
		public:
			FReader(std::FILE* file = stdin) : file_(file) {}
			size_t read(char* buf, size_t sz) override {
				return std::fread(buf, sizeof(char), sz, file_);
			}

		private:
			std::FILE* file_;
	};

	class StreamReader : public Reader {
		public:
			StreamReader(std::istream* stream = &std::cin) : stream_(stream) {}
			size_t read(char* buf, size_t sz) override { stream_->read(buf, sz); return stream_->gcount(); }

		private:
			std::istream* stream_;
	};

	class Writer {
		public:
			void write(std::string&& str) { write(str.data(), str.size()); }
			void write(std::string_view str) { write(str.data(), str.size()); }
			virtual void write(const char* buf, size_t sz) = 0;
			virtual void flush() = 0;
	};

	class CWriter : public Writer {
		public:
			CWriter(int fd = STDOUT_FILENO) : fd_(fd) {}
			void write(const char* buf, size_t sz) override {
				std::span<const char> sp(buf, sz);
				buffer_.insert(buffer_.end(), sp.begin(), sp.end());
			}
			void flush() override { ::write(fd_, buffer_.data(), buffer_.size()); buffer_.resize(0); }

		private:
			int fd_;
			std::vector<char> buffer_;
	};

	class FWriter : public Writer {
		public:
			FWriter(std::FILE* file = stdout) : file_(file) {}
			void write(const char* buf, size_t sz) override {
				/*std::fwrite(buf, sizeof(char), sz, file_);*/
				std::cout << std::string_view(buf, sz);
			}
			void flush() override { std::fflush(file_); }

		private:
			std::FILE* file_;
	};

	class StreamWriter : public Writer {
		public:
			StreamWriter(std::ostream* stream = &std::cout) : stream_(stream) {}
			void write(const char* buf, size_t sz) override { stream_->write(buf, sz); }
			void flush() override { stream_->flush(); }

		private:
			std::ostream* stream_;
	};

	void init_term(int input = STDIN_FILENO, int output = STDOUT_FILENO);

	void set_utf_locale();

	void enable_raw_mode(size_t limit = 0, size_t timeout = 1, bool stop_sigs = true);
	void disable_raw_mode();
	bool in_raw_mode();

	// ESC sequences

	std::string push_kitty_keys_str(KittyFlags flags);
	std::string pop_kitty_keys_str();
	std::string set_kitty_keys_str(KittyFlags flags);
	std::string add_kitty_keys_str(KittyFlags flags);
	std::string remove_kitty_keys_str(KittyFlags flags);

	std::string get_size_px_str();
	std::optional<Pos> get_size_px_parser(Reader* reader);
	std::string get_size_ch_str();
	std::optional<Pos> get_size_ch_parser(Reader* reader);
	std::string get_pos_str();
	std::optional<Pos> get_pos_parser(Reader* reader);

	std::string move_str(uint32_t r, uint32_t c);
	std::string move_backward_str(uint32_t steps=1);
	std::string move_forward_str(uint32_t steps=1);
	std::string move_up_str(uint32_t steps=1);
	std::string move_down_str(uint32_t steps=1);

	std::string save_cursor_position_str();
	std::string restore_cursor_position_str();

	std::string cursor_default_str();
	std::string cursor_hidden_str();

	std::string set_foreground_color_str(color_t color = Color::NONE);
	std::string set_background_color_str(color_t color = Color::NONE);

	std::string enter_alternate_buffer_str();
	std::string exit_alternate_buffer_str();

	std::string clear_str();
	std::string clear_before_str();
	std::string clear_after_str();

	std::string clear_line_str();
	std::string clear_line_before_str();
	std::string clear_line_after_str();

	std::string set_automatic_newline_str(bool value);
	std::string set_wraparound_str(bool value);

	std::string insert_chars_str(uint32_t cnt);
	std::string insert_lines_str(uint32_t cnt);
	std::string delete_chars_str(uint32_t cnt);
	std::string delete_lines_str(uint32_t cnt);
	std::string erase_chars_str(uint32_t cnt);
	std::string scroll_up_str(uint32_t cnt);
	std::string scroll_down_str(uint32_t cnt);

	std::string set_styles_str(std::vector<StyleProp> &&styles);
	std::string reset_styles_str();
	std::string set_underline_style_str(UnderlineStyle style=UnderlineStyle::STRAIGHT);
	std::string set_underline_color_str(color_t color = Color::NONE);

	std::string set_cursor_str(CursorStyle style);
	std::string set_cursor_shape_str(CursorShape shape);
	std::string push_cursor_shape_str(CursorShape shape);
	std::string pop_cursor_shape_str();

	std::string enable_mouse_buttons_str();
	std::string disable_mouse_buttons_str();
	std::string enable_mouse_cell_motions_str();
	std::string disable_mouse_cell_motions_str();
	std::string enable_mouse_all_motions_str();
	std::string disable_mouse_all_motions_str();

	std::string enable_paste_brackets_str();
	std::string disable_paste_brackets_str();

	std::string sync_begin_str();
	std::string sync_end_str();

	std::string query_color_str(ColorType type);
	color_t query_color_parser(Reader* reader);
	std::string set_color_str(ColorType type, color_t color);

	namespace detail {
		using namespace std::literals::string_view_literals;

		void exit_raw_mode();
		void fatal(std::string_view msg);

		int get_input_file_descriptor();
		int get_output_file_descriptor();

		size_t read_exactly(Reader* r, char* c, size_t sz);
		size_t read_until_ch(Reader* r, char* c, char ch, size_t limit);
		std::optional<size_t> read_unsigned_until_ch(Reader*r, char ch, size_t limit);

		inline int input = STDIN_FILENO;
		inline int output = STDOUT_FILENO;

		inline int32_t raw_mode_enabled_count = 0;
		inline termios orig_termios;

		inline constexpr std::array<std::string_view, 6> query_color_types{
			"foreground"sv,
			"background"sv,
			"selection_foreground"sv,
			"selection_background"sv,
			"cursor"sv,
			"cursor_text"sv,
		};

		inline constexpr std::array<std::string_view, 31> cursor_shapes{
			""sv,
			"alias"sv,
			"cell"sv,
			"copy"sv,
			"crosshair"sv,
			"default"sv,
			"e-resize"sv,
			"ew-resize"sv,
			"grab"sv,
			"grabbing"sv,
			"help"sv,
			"move"sv,
			"n-resize"sv,
			"ne-resize"sv,
			"nesw-resize"sv,
			"no-drop"sv,
			"not-allowed"sv,
			"ns-resize"sv,
			"nw-resize"sv,
			"nwse-resize"sv,
			"pointer"sv,
			"progress"sv,
			"s-resize"sv,
			"se-resize"sv,
			"sw-resize"sv,
			"text"sv,
			"vertical-text"sv,
			"w-resize"sv,
			"wait"sv,
			"zoom-in"sv,
			"zoom-out"sv,
		};
	}
}

#endif // _TQ_TERM_INIT
