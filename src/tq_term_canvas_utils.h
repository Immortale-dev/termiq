#ifndef _TQ_TERM_CANVAS_UTILS
#define _TQ_TERM_CANVAS_UTILS

#include <memory>
#include <vector>
#include <span>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <locale>
#include <codecvt>

#include "tq_term_style.h"

namespace termiq {
	namespace canvas {
		using symbol_char_type = char;

		template<typename T>
		concept Char = requires(T t) {
			std::is_default_constructible_v<T>;
			std::is_constructible_v<T, symbol_char_type*, symbol_char_type*>;
			std::is_constructible_v<T, std::vector<symbol_char_type>::iterator, std::vector<symbol_char_type>::iterator>;

			{ t.begin() } -> std::same_as<symbol_char_type*>;
			{ t.end() } -> std::same_as<symbol_char_type*>;
		};

		template<typename T>
		concept State = requires(T t, T g) {
			std::is_default_constructible_v<T>;

			{ t.foreground() } -> std::same_as<style::Color>;
			{ t.background() } -> std::same_as<style::Color>;
			{ t.bold() } -> std::same_as<bool>;
			{ t.italic() } -> std::same_as<bool>;
			{ t.dim() } -> std::same_as<bool>;
			{ t.inverse() } -> std::same_as<bool>;
			{ t.underline() } -> std::same_as<bool>;
			{ t.special() } -> std::same_as<bool>;

			{ t.foreground(style::Color{}) };
			{ t.background(style::Color{}) };
			{ t.bold(true) };
			{ t.italic(true) };
			{ t.dim(true) };
			{ t.inverse(true) };
			{ t.underline(true) };
			{ t.special(true) };

			{ t == g } -> std::same_as<bool>;
			{ t != g } -> std::same_as<bool>;
		};

		class CharType {
			public:
				CharType() : symbol(' ') {}
				CharType(symbol_char_type s) : symbol(s) {}

				CharType(symbol_char_type* b, symbol_char_type* e) { std::memcpy(begin(), b, std::min(size_t(e-b), sizeof(char))); }
				template<typename IT>
				CharType(IT b, IT e) { std::memcpy(begin(), &*b, std::min(size_t(std::distance(b,e)), sizeof(char))); }

				symbol_char_type* begin() { return reinterpret_cast<symbol_char_type*>(&symbol); }
				symbol_char_type* end() { return begin() + sizeof(symbol_char_type); }

			private:
				symbol_char_type symbol;
		};

		class WCharType {
			public:
				WCharType() : symbol({' '}) {}
				WCharType(symbol_char_type s) : symbol({s}) {}

				WCharType(const symbol_char_type* b, const symbol_char_type* e) {
					symbol.resize(e-b);
					std::memcpy(begin(), b, e-b);
				}
				template<typename IT> requires (std::contiguous_iterator<IT>)
				WCharType(IT b, IT e) {
					symbol.resize(std::distance(b,e));
					std::memcpy(begin(), &*b, symbol.size());
				}

				symbol_char_type* begin() { return symbol.data(); }
				symbol_char_type* end() { return symbol.data() + symbol.size(); }

			private:
				std::basic_string<symbol_char_type> symbol;
		};

		template<typename T=char>
		inline std::vector<std::vector<CharType>> build_char_lines(std::basic_string_view<T> str) {
			using char_type = CharType;
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
		inline std::vector<std::vector<WCharType>> build_wchar_lines(std::basic_string_view<T> str) {
			using char_type = WCharType;
			std::vector<std::vector<char_type>> res;
			std::vector<char_type> line;
			std::wstring_convert<std::codecvt_utf8<T>, T> cvt;
			for (auto c : str) {
				if (c == '\n') {
					res.push_back(line);
					line.resize(0);
					continue;
				}
				if (c == '\0') break;
				auto utf8 = cvt.to_bytes({c});
				line.push_back(char_type(utf8.begin(), utf8.end()));
			}
			if (line.size()) {
				res.push_back(line);
			}
			return res;
		}

		class CharState {
			public:
				struct Builder {
					style::Color foreground = style::Color::UNDEFINED;
					style::Color background = style::Color::UNDEFINED;
					bool bold = false;
					bool italic = false;
					bool dim = false;
					bool inverse = false;
					bool underline = false;
					bool special = false;
				};

			public:
				CharState() = default;
				CharState(Builder b) : foreground_(b.foreground), background_(b.background), bold_(b.bold), italic_(b.italic), dim_(b.dim), inverse_(b.inverse), underline_(b.underline), special_(b.special) {}

				style::Color foreground() const { return foreground_; }
				style::Color background() const { return background_; }
				bool bold() const { return bold_; }
				bool italic() const { return italic_; }
				bool dim() const { return dim_; }
				bool inverse() const { return inverse_; }
				bool underline() const { return underline_; }
				bool special() const { return special_; }

				void foreground(style::Color color) { foreground_ = color; }
				void background(style::Color color) { background_ = color; }
				void bold(bool state) { bold_ = state; }
				void italic(bool state) { italic_ = state; }
				void dim(bool state) { dim_ = state; }
				void inverse(bool state) { inverse_ = state; }
				void underline(bool state) { underline_ = state; }
				void special(bool state) { special_ = state; }

				bool operator==(const CharState &other) const {
					return foreground() == other.foreground() && background() == other.background() && bold() == other.bold() && italic() == other.italic()
						&& dim() == other.dim() && inverse() == other.inverse() && underline() == other.underline() && special() == other.special();
				}
				bool operator!=(const CharState &other) const {
					return !(*this == other);
				}

			private:
				termiq::style::Color foreground_ = style::Color::UNDEFINED;
				termiq::style::Color background_ = style::Color::UNDEFINED;
				bool bold_ = false;
				bool italic_ = false;
				bool dim_ = false;
				bool inverse_ = false;
				bool underline_ = false;
				bool special_ = false;
		};

		// CT - char type, e.g. WCharType
		// CS - char state, e.g. CharState
		template<Char CT, State CS>
		struct CharCell {
			CT symbol;
			std::shared_ptr<CS> state = nullptr;
			using char_type = CT;
			using char_state_type = CS;
			bool operator==(const CharCell &other) const {
				if (state == nullptr && other.state != nullptr || state != nullptr && other.state == nullptr) return false;
				return symbol == other.symbol && (state == other.state || *state == *(other.state));
			}
			bool operator!=(const CharCell &other) const {
				return !(*this == other);
			}
			bool is_transparent() const { return !state; }

			static const CharCell<CT,CS> TRANSPARENT;
		};

		template<Char CT, State CS>
		const CharCell<CT,CS> CharCell<CT,CS>::TRANSPARENT = CharCell<CT,CS>();

		// Piece of the canvas.
		template<typename CC>
		struct CanvasPiece {
			const std::vector<std::vector<CC>> canvas;
			const unsigned int rows;
			const unsigned int cols;
			const int offset_rows = 0;
			const int offset_cols = 0;
		};

		// Collection of pieces.
		template<typename CC>
		struct CanvasPieces {
			const std::vector<CanvasPiece<CC>> pieces;
		};

		// Buildable content interface
		template<typename CC>
		class Content {
			public:
				Content() = default;
				virtual ~Content() = default;

				virtual CanvasPieces<CC> build() = 0;
				virtual unsigned int get_width() = 0;
				virtual unsigned int get_height() = 0;
				virtual unsigned int min_width() = 0;
				virtual unsigned int min_height() = 0;

				virtual void built(CanvasPieces<CC> &pieces) { (void)pieces; }
		};

		template<typename CC>
		class Nothing : public Content<CC> {
			public:
				CanvasPieces<CC> build() { return {{}}; }
				unsigned int get_width() { return 0; }
				unsigned int get_height() { return 0; }
				unsigned int min_width() { return 0; }
				unsigned int min_height() { return 0; }
		};
	}
}

#endif // _TQ_TERM_CANVAS_UTILS
