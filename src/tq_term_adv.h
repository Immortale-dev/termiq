#ifndef _TQ_TERM_ADV
#define _TQ_TERM_ADV

#include <stdexcept>
#include <memory>
#include <functional>

#include "tq_term.h"

#define DECLARE_SEQUENCE_FUNCTION_W_PARSER_(n, s, f) struct n{        \
	static constexpr inline auto str = &s;                            \
	static constexpr inline auto parser = &f;                         \
}
#define DECLARE_SEQUENCE_FUNCTION_WO_PARSER_(n, s) struct n{          \
	static constexpr inline auto str = &s;                            \
	static constexpr inline auto parser = nullptr;                    \
}
#define EXPAND_SEQUENCE_FUNCTION_DECLARATION_(x) x
#define GET_SEQUENCE_FUNCTION_DECLARATION_MACRO_(_1, _2, _3, name, ...) name

/**
 * Declares sequence function.
 * Accepts Name, a str function and optionally a parser function.
 */
#define DECLARE_SEQUENCE_FUNCTION(...) \
	EXPAND_SEQUENCE_FUNCTION_DECLARATION_( \
		GET_SEQUENCE_FUNCTION_DECLARATION_MACRO_( \
			__VA_ARGS__, DECLARE_SEQUENCE_FUNCTION_W_PARSER_, DECLARE_SEQUENCE_FUNCTION_WO_PARSER_)(__VA_ARGS__))

namespace termiq {
	class exception : public std::logic_error {
		public:
			exception(const char* msg) : std::logic_error(msg) {};
	};

	template<typename L, typename U> requires (std::invocable<L> && std::invocable<L>)
	class Locker {
		public:
			Locker(L lock, U unlock) : lock_(lock), unlock_(unlock) { lock_(); }
			~Locker() { unlock_(); }

		private:
			L lock_;
			U unlock_;
	};

	class SequenceExecutor {
		public:
			SequenceExecutor(Reader* reader, Writer* writer) : reader_(reader), writer_(writer) {}
			virtual ~SequenceExecutor() = default;

			template<typename F, typename... Args> requires (F::parser != nullptr)
			std::result_of_t<decltype(F::parser)(Reader*)> execute(Args&&... args) {
				Locker lock([this]{ this->enable_reader_mode(); }, [this]{ this->disable_reader_mode(); });
				writer_->write(F::str(std::forward<Args>(args)...));
				writer_->flush();
				return F::parser(reader_);
			}

			template<typename F, typename... Args> requires (F::parser == nullptr)
			void execute(Args&&... args) {
				writer_->write(F::str(std::forward<Args>(args)...));
			}

			Reader* reader() { return reader_; }
			Writer* writer() { return writer_; }

			void flush() { writer_->flush(); }

		protected:
			virtual void enable_reader_mode() {
				// BLocking interruptable read
				::termiq::enable_raw_mode(1, 0, false);
			}
			virtual void disable_reader_mode() {
				::termiq::disable_raw_mode();
			}

		private:
			Reader* reader_;
			Writer* writer_;
	};

	template<typename E = SequenceExecutor>
	class SequenceRunner {
		public:
			SequenceRunner(std::unique_ptr<Reader> reader, std::unique_ptr<Writer> writer)
				: reader_(std::move(reader)), writer_(std::move(writer))
				{ executor_ = std::make_unique<E>(reader_.get(), writer_.get()); }
			virtual ~SequenceRunner() = default;

			SequenceExecutor* get() { return executor_.get(); }

			void run(std::function<void(SequenceExecutor*)> fun) {
				fun(executor_.get());
				executor_->flush();
			}

		private:
			std::unique_ptr<Reader> reader_;
			std::unique_ptr<Writer> writer_;
			std::unique_ptr<SequenceExecutor> executor_;
	};

	template<typename E = SequenceExecutor>
	class FDSequenceRunner : public SequenceRunner<E> {
		public:
			FDSequenceRunner(int in = STDIN_FILENO, int out = STDOUT_FILENO)
				: SequenceRunner<E>(std::make_unique<CReader>(in), std::make_unique<CWriter>(out)) {}
	};

	template<typename E = SequenceExecutor>
	class FileSequenceRunner : public SequenceRunner<E> {
		public:
			FileSequenceRunner(FILE* in = stdin, FILE* out = stdout)
				: SequenceRunner<E>(std::make_unique<FReader>(in), std::make_unique<FWriter>(out)) {}
	};

	template<typename E = SequenceExecutor>
	class StreamSequenceRunner : public SequenceRunner<E> {
		public:
			StreamSequenceRunner(std::istream* in = &std::cin, std::ostream* out = &std::cout)
				: SequenceRunner<E>(std::make_unique<StreamReader>(in), std::make_unique<StreamWriter>(out)) {}
	};

	namespace se {
		DECLARE_SEQUENCE_FUNCTION(push_kitty_keys, push_kitty_keys_str);
		DECLARE_SEQUENCE_FUNCTION(pop_kitty_keys, pop_kitty_keys_str);
		DECLARE_SEQUENCE_FUNCTION(set_kitty_keys, set_kitty_keys_str);
		DECLARE_SEQUENCE_FUNCTION(add_kitty_keys, add_kitty_keys_str);
		DECLARE_SEQUENCE_FUNCTION(remove_kitty_keys, remove_kitty_keys_str);

		DECLARE_SEQUENCE_FUNCTION(get_size_px, get_size_px_str, get_size_px_parser);
		DECLARE_SEQUENCE_FUNCTION(get_size_ch, get_size_ch_str, get_size_ch_parser);
		DECLARE_SEQUENCE_FUNCTION(get_pos, get_pos_str, get_pos_parser);

		DECLARE_SEQUENCE_FUNCTION(move, move_str);
		DECLARE_SEQUENCE_FUNCTION(move_backward, move_backward_str);
		DECLARE_SEQUENCE_FUNCTION(move_forward, move_forward_str);
		DECLARE_SEQUENCE_FUNCTION(move_up, move_up_str);
		DECLARE_SEQUENCE_FUNCTION(move_down, move_down_str);

		DECLARE_SEQUENCE_FUNCTION(save_cursor_position, save_cursor_position_str);
		DECLARE_SEQUENCE_FUNCTION(restore_cursor_position, restore_cursor_position_str);

		DECLARE_SEQUENCE_FUNCTION(cursor_default, cursor_default_str);
		DECLARE_SEQUENCE_FUNCTION(cursor_hidden, cursor_hidden_str);

		DECLARE_SEQUENCE_FUNCTION(set_foreground_color, set_foreground_color_str);
		DECLARE_SEQUENCE_FUNCTION(set_background_color, set_background_color_str);

		DECLARE_SEQUENCE_FUNCTION(enter_alternate_buffer, enter_alternate_buffer_str);
		DECLARE_SEQUENCE_FUNCTION(exit_alternate_buffer, exit_alternate_buffer_str);

		DECLARE_SEQUENCE_FUNCTION(clear, clear_str);
		DECLARE_SEQUENCE_FUNCTION(clear_before, clear_before_str);
		DECLARE_SEQUENCE_FUNCTION(clear_after, clear_after_str);

		DECLARE_SEQUENCE_FUNCTION(clear_line, clear_line_str);
		DECLARE_SEQUENCE_FUNCTION(clear_line_before, clear_line_before_str);
		DECLARE_SEQUENCE_FUNCTION(clear_line_after, clear_line_after_str);

		DECLARE_SEQUENCE_FUNCTION(set_automatic_newline, set_automatic_newline_str);
		DECLARE_SEQUENCE_FUNCTION(set_wraparound, set_wraparound_str);

		DECLARE_SEQUENCE_FUNCTION(insert_chars, insert_chars_str);
		DECLARE_SEQUENCE_FUNCTION(insert_lines, insert_lines_str);
		DECLARE_SEQUENCE_FUNCTION(delete_chars, delete_chars_str);
		DECLARE_SEQUENCE_FUNCTION(delete_lines, delete_lines_str);
		DECLARE_SEQUENCE_FUNCTION(erase_chars, erase_chars_str);
		DECLARE_SEQUENCE_FUNCTION(scroll_up, scroll_up_str);
		DECLARE_SEQUENCE_FUNCTION(scroll_down, scroll_down_str);

		DECLARE_SEQUENCE_FUNCTION(set_styles, set_styles_str);
		DECLARE_SEQUENCE_FUNCTION(reset_styles, reset_styles_str);
		DECLARE_SEQUENCE_FUNCTION(set_underline_style, set_underline_style_str);
		DECLARE_SEQUENCE_FUNCTION(set_underline_color, set_underline_color_str);

		DECLARE_SEQUENCE_FUNCTION(set_cursor, set_cursor_str);
		DECLARE_SEQUENCE_FUNCTION(set_cursor_shape, set_cursor_shape_str);
		DECLARE_SEQUENCE_FUNCTION(push_cursor_shape, push_cursor_shape_str);
		DECLARE_SEQUENCE_FUNCTION(pop_cursor_shape, pop_cursor_shape_str);

		DECLARE_SEQUENCE_FUNCTION(enable_mouse_buttons, enable_mouse_buttons_str);
		DECLARE_SEQUENCE_FUNCTION(disable_mouse_buttons, disable_mouse_buttons_str);
		DECLARE_SEQUENCE_FUNCTION(enable_mouse_cell_motions, enable_mouse_cell_motions_str);
		DECLARE_SEQUENCE_FUNCTION(disable_mouse_cell_motions, disable_mouse_cell_motions_str);
		DECLARE_SEQUENCE_FUNCTION(enable_mouse_all_motions, enable_mouse_all_motions_str);
		DECLARE_SEQUENCE_FUNCTION(disable_mouse_all_motions, disable_mouse_all_motions_str);

		DECLARE_SEQUENCE_FUNCTION(enable_paste_brackets, enable_paste_brackets_str);
		DECLARE_SEQUENCE_FUNCTION(disable_paste_brackets, disable_paste_brackets_str);

		DECLARE_SEQUENCE_FUNCTION(sync_begin, sync_begin_str);
		DECLARE_SEQUENCE_FUNCTION(sync_end, sync_end_str);

		DECLARE_SEQUENCE_FUNCTION(query_color, query_color_str, query_color_parser);
		DECLARE_SEQUENCE_FUNCTION(set_color, set_color_str);
	}
}

#endif // TQ_TERM_ADV
