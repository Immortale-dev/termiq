#ifndef _TQ_TERM_CANVAS_UTILS
#define _TQ_TERM_CANVAS_UTILS

#include <memory>
#include <vector>
#include <span>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <cuchar>
#include <climits>

#include "tq_term_style.h"

namespace termiq {
	namespace canvas {
		using symbol_char_type = char;

		template<typename T>
		concept CharConcept = requires(T t) {
			std::is_default_constructible_v<T>;
			std::is_constructible_v<T, symbol_char_type*, symbol_char_type*>;
			std::is_constructible_v<T, std::vector<symbol_char_type>::iterator, std::vector<symbol_char_type>::iterator>;

			{ t.begin() } -> std::same_as<const symbol_char_type*>;
			{ t.end() } -> std::same_as<const symbol_char_type*>;
			t.size();
		};

		template<typename T>
		concept StyleConcept = requires(T t, T g) {
			std::is_default_constructible_v<T>;

			{ t.foreground() } -> std::same_as<::termiq::Color>;
			{ t.background() } -> std::same_as<::termiq::Color>;
			{ t.bold() } -> std::same_as<bool>;
			{ t.italic() } -> std::same_as<bool>;
			{ t.dim() } -> std::same_as<bool>;
			{ t.blinking() } -> std::same_as<bool>;
			{ t.inverse() } -> std::same_as<bool>;
			{ t.hidden() } -> std::same_as<bool>;
			{ t.strike() } -> std::same_as<bool>;
			{ t.underline() } -> std::same_as<Underline>;

			{ t.foreground(termiq::Color::NONE) };
			{ t.background(termiq::Color::NONE) };
			{ t.bold(true) };
			{ t.italic(true) };
			{ t.dim(true) };
			{ t.blinking(true) };
			{ t.inverse(true) };
			{ t.hidden(true) };
			{ t.strike(true) };
			{ t.underline(Underline()) };

			{ t == g } -> std::same_as<bool>;
			{ t != g } -> std::same_as<bool>;
		};

		class NarrowChar {
			public:
				NarrowChar() : symbol(' ') {}
				NarrowChar(symbol_char_type s) : symbol(s) {}

				NarrowChar(symbol_char_type* b, symbol_char_type* e) { std::memcpy(const_cast<symbol_char_type*>(begin()), b, std::min(size_t(e-b), sizeof(symbol_char_type))); }
				template<typename IT>
				NarrowChar(IT b, IT e) { std::memcpy(const_cast<symbol_char_type*>(begin()), &*b, std::min(size_t(std::distance(b,e)), sizeof(symbol_char_type))); }

				const symbol_char_type* begin() const { return reinterpret_cast<const symbol_char_type*>(&symbol); }
				const symbol_char_type* end() const { return begin() + sizeof(symbol_char_type); }
				size_t size() const { return 1; }

			private:
				symbol_char_type symbol;
		};

		class WideChar {
			public:
				WideChar() : symbol({' '}) {}
				WideChar(symbol_char_type s) : symbol({s}) {}

				WideChar(const symbol_char_type* b, const symbol_char_type* e) {
					symbol.resize(e-b);
					std::memcpy(const_cast<symbol_char_type*>(begin()), b, e-b);
				}
				template<typename IT> requires (std::contiguous_iterator<IT>)
				WideChar(IT b, IT e) {
					symbol.resize(std::distance(b,e));
					std::memcpy(const_cast<symbol_char_type*>(begin()), &*b, symbol.size());
				}

				const symbol_char_type* begin() const { return symbol.data(); }
				const symbol_char_type* end() const { return symbol.data() + symbol.size(); }
				size_t size() const { return symbol.size(); }

			private:
				std::basic_string<symbol_char_type> symbol;
		};

		template<typename T=char>
		inline std::vector<std::vector<NarrowChar>> build_char_lines(std::basic_string_view<T> str) {
			using char_type = NarrowChar;
			std::vector<std::vector<char_type>> res;
			std::vector<char_type> line;
			for (auto c : str) {
				if (c == '\n') {
					res.push_back(line);
					line.resize(0);
					continue;
				}
				if (c == '\0') break;
				line.push_back({(unsigned char)(c)});
			}
			if (line.size()) {
				res.push_back(line);
			}
			return res;
		}

		template<typename T=char16_t>
		inline std::vector<std::vector<WideChar>> build_wchar_lines(std::basic_string_view<T> str) {
			using char_type = WideChar;
			std::vector<std::vector<char_type>> res;
			std::vector<char_type> line;
			std::mbstate_t state{};
			char out[MB_LEN_MAX]{};
			for (auto c : str) {
				if (c == '\n') {
					res.push_back(line);
					line.resize(0);
					continue;
				}
				if (c == '\0') break;
				size_t rc = std::c16rtomb(out, c, &state);
				if (rc != (size_t)-1) {
					std::string_view utf8(out, rc);
					line.push_back(char_type(utf8.begin(), utf8.end()));
				}
			}
			if (line.size()) {
				res.push_back(line);
			}
			return res;
		}

		class CharStyle {
			public:
				struct Builder {
					termiq::Color foreground = termiq::Color::NONE;
					termiq::Color background = termiq::Color::NONE;
					bool bold = false;
					bool italic = false;
					bool dim = false;
					bool blinking = false;
					bool inverse = false;
					bool hidden = false;
					bool strike = false;
					Underline underline = false;
				};

			public:
				explicit CharStyle() = default;
				CharStyle(Builder b) : foreground_(b.foreground), background_(b.background), bold_(b.bold), italic_(b.italic), dim_(b.dim), inverse_(b.inverse), underline_(b.underline) {}

				termiq::Color foreground() const { return foreground_; }
				termiq::Color background() const { return background_; }
				bool bold() const { return bold_; }
				bool italic() const { return italic_; }
				bool dim() const { return dim_; }
				bool blinking() const { return blinking_; }
				bool inverse() const { return inverse_; }
				bool hidden() const { return hidden_; }
				bool strike() const { return strike_; }
				Underline underline() const { return underline_; }

				void foreground(termiq::Color color) { foreground_ = color; }
				void background(termiq::Color color) { background_ = color; }
				void bold(bool state) { bold_ = state; }
				void italic(bool state) { italic_ = state; }
				void dim(bool state) { dim_ = state; }
				void blinking(bool state) { blinking_ = state; }
				void hidden(bool state) { hidden_ = state; }
				void strike(bool state) { strike_ = state; }
				void inverse(bool state) { inverse_ = state; }
				void underline(Underline state) { underline_ = state; }

				bool operator==(const CharStyle &other) const {
					return foreground() == other.foreground() && background() == other.background()
						&& bold() == other.bold() && italic() == other.italic() && dim() == other.dim() && inverse() == other.inverse()
						&& underline() == other.underline();
				}

			private:
				termiq::Color foreground_ = termiq::Color::NONE;
				termiq::Color background_ = termiq::Color::NONE;
				bool bold_ = false;
				bool italic_ = false;
				bool dim_ = false;
				bool blinking_ = false;
				bool inverse_ = false;
				bool hidden_ = false;
				bool strike_ = false;
				Underline underline_ = false;
		};

		// CT - char type, e.g. WideChar
		// CS - char styles, e.g. CharStyle
		template<CharConcept CT, StyleConcept CS>
		class CharCell {
			public:
				using Char = CT;
				using Style = CS;

			public:
				CharCell() {}
				CharCell(Char symb, Style st, bool transparent = false, bool continuation = false)
					: symbol_(symb), state_(st), transparent_(transparent), continuation_(continuation) {}

				bool operator==(const CharCell &other) const {
					if (continuation() && other.continuation()) return true;
					if (transparent() && other.transparent()) return true;
					return symbol() == other.symbol() && style() == other.style();
				}
				bool transparent() const { return transparent_; }
				bool continuation() const { return continuation_; }

				const Char& symbol() const { return symbol_; }
				const Style& style() const { return state_; }
				Char& symbol() { return symbol_; }
				Style& style() { return state_; }

				static const CharCell<Char,Style> TRANSPARENT;
				static const CharCell<Char,Style> CONTINUATION;

			private:
				Char symbol_;
				Style state_;
				bool transparent_ = true;
				bool continuation_ = false;
		};

		template<CharConcept CT, StyleConcept CS>
		const CharCell<CT,CS> CharCell<CT,CS>::TRANSPARENT = CharCell<CT,CS>({},{},1);
		template<CharConcept CT, StyleConcept CS>
		const CharCell<CT,CS> CharCell<CT,CS>::CONTINUATION = CharCell<CT,CS>({},{},0,1);

		template<typename CC>
		class CanvasLayer {
			public:
				CanvasLayer(std::vector<std::vector<CC>>&& canvas, uint32_t h, uint32_t w, int32_t r=0, int32_t c=0)
					: rows_(h), cols_(w), offset_rows_(r), offset_cols_(c), canvas_(std::move(canvas)) {}

				uint32_t rows() const { return rows_; }
				uint32_t cols() const { return cols_; }
				int32_t row() const { return offset_rows_; }
				int32_t col() const { return offset_cols_; }
				const CC& cell(uint32_t r, uint32_t c) const { return canvas_[r][c]; }

			private:
				const unsigned int rows_;
				const unsigned int cols_;
				const int offset_rows_ = 0;
				const int offset_cols_ = 0;
				std::vector<std::vector<CC>> canvas_;
		};

		template<typename CC>
		class CanvasPiece {
			public:
				CanvasPiece() = default;
				CanvasPiece(std::vector<CanvasLayer<CC>>&& layers) : layers_(std::move(layers)) {}

				std::span<const CanvasLayer<CC>> layers() const { return std::span(layers_.begin(), layers_.end()); }

			private:
				std::vector<CanvasLayer<CC>> layers_;
		};

		template<typename CC>
		class CanvasContainer {
			public:
				CanvasContainer() = default;
				CanvasContainer(uint32_t height, uint32_t width)
					: height_(height), width_(width), grid_(height_, std::vector<CC>(width_)) {}

				void resize(uint32_t height, uint32_t width) {
					height_ = height;
					width_ = width;
					grid_.resize(height);
					for (auto& l : grid_) l.resize(width_);
				}

				uint32_t rows() const { return height_; }
				uint32_t cols() const { return width_; }

				void move([[maybe_unused]] int32_t r, [[maybe_unused]] int32_t c) {} // Nothing to move

				void set(uint32_t r, uint32_t c, CC cell) {
					grid_[r][c] = cell;
				}

				CC get(uint32_t r, uint32_t c) const {
					return grid_[r][c];
				}

			private:
				uint32_t height_ = 0;
				uint32_t width_ = 0;
				std::vector<std::vector<CC>> grid_;
		};

		// Buildable content interface
		template<typename CC>
		class Content {
			public:
				Content() = default;
				virtual ~Content() = default;

				virtual uint32_t get_width() const = 0;
				virtual uint32_t get_height() const = 0;
				virtual uint32_t min_width() const = 0;
				virtual uint32_t min_height() const = 0;
				virtual CanvasPiece<CC> build() const = 0;

				virtual void built(CanvasPiece<CC>&) const {}
		};

		template<typename CC>
		class Nothing : public Content<CC> {
			public:
				CanvasPiece<CC> build() { return {}; }
				uint32_t get_width() override { return 0; }
				unsigned int get_height() override { return 0; }
				unsigned int min_width() override { return 0; }
				unsigned int min_height() override { return 0; }
		};
	}
}

#endif // _TQ_TERM_CANVAS_UTILS
