#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <chrono>

#include "tq_term.h"
#include "tq_term_adv.h"
#include "tq_term_style.h"
#include "tq_term_canvas.h"
#include "tq_term_canvas_flextext.h"
#include "tq_term_canvas_flexgrid.h"

using CC = termiq::canvas::CharCell<termiq::canvas::WCharType, termiq::canvas::CharState>;
using BText = termiq::canvas::FlexText<CC>;
using BGrid = termiq::canvas::FlexGrid<CC>;

int rows, cols;

void paint_texts(termiq::SequenceExecutor* se) {
	termiq::style::background(termiq::Color{0, 128, 128});
	termiq::style::bold(true);
	termiq::style::underline(true);

	std::cout << "Some text";
	termiq::style::colors_reset();
	std::cout << "\n";

	termiq::style::foreground(termiq::Color{0, 128, 50});
	termiq::style::underline(false);

	std::cout << "Other text\n";
	for(int i=1;i<=10;i++) {
		termiq::style::foreground(termiq::Color{(uint8_t)((i%2) == 0 ? 255 : 128), 0, 0});
		std::cout << "This is red color\n";
	}
	termiq::style::colors_reset();

	termiq::style::bold(false);

	std::cout << "colors were reset\n";

	termiq::style::style(termiq::style::FontStyle{.dim=true, .inverse=true});
	std::cout << "dim and reverse\n";

	termiq::style::style(termiq::style::FontStyle{
		.dim=false,
		.underline=termiq::Underline(
			termiq::UnderlineStyle::CURLY,
			termiq::Color{250,10,10}
		),
		.inverse=false,
	});

	std::cout << "with curly red underline\n";

	termiq::style::style(termiq::style::FontStyle{
		.bold=true,
		.underline=false,
		.strike=true
	});

	std::cout << "bold, striked text\n";

	termiq::style::italic(true);
	termiq::style::inverse(true);
	termiq::style::strike(false);

	termiq::style::style({
		.background=termiq::Color{130,40,150},
	});

	std::cout << "bold, italic, inversed, colored\n";

	termiq::style::props_reset();

	std::cout << "with colored background\n";

	termiq::style::colors_reset();

	std::cout << "press enter...\n";
	se->flush();
	getchar();

	termiq::style::terminal_background(termiq::Color{60,70,200});
	termiq::style::terminal_style({
		.foreground=termiq::Color{100,250,250},
	});

	se->flush();

	std::cout << "press enter...\n";
	getchar();

	termiq::style::terminal_style_reset();
}

void paint_canvas(termiq::SequenceExecutor* se) {
	termiq::canvas::SECanvas<CC> canvas(se, rows,cols,0,0);

	auto t1 = std::chrono::system_clock::now();

	auto tb1 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"woohoo"));
	tb1.set_background_color(termiq::Color{100, 100, 220});
	auto tb2 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"hello\nworld"));
	auto tb3 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"Awesome")); //.set_width(4);
	auto tb4 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"Nice"));
	auto gb1 = BGrid(1,2);
	gb1.set_width(10);
	gb1.set_border_type(termiq::canvas::BorderType::SINGLE);
	gb1.select_cell(0,1);
	gb1.set_cell_content(&tb3);
	gb1.select_cell(0,0);
	gb1.set_cell_content(&tb4);

	auto gt1 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"a"));
	auto gt2 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"aa"));
	auto gt3 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"aa aa aa"));
	auto gt4 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"aaaaaaaaaaaaaaaa aaaaaaaaaaaaa"));
	BGrid gb2 = BGrid(2,4);
	gb2.set_width(40);
	gb2.set_border_type(termiq::canvas::BorderType::SINGLE);
	gb2.select_cell(0,0);
	gb2.set_cell_content(&gt1);
	gb2.set_cell_width(5);
	gb2.select_cell(0,1);
	gb2.set_cell_content(&gt2);
	gb2.set_cell_width(10);
	gb2.select_cell(0,2);
	gb2.set_cell_content(&gt3);
	gb2.select_cell(0,3);
	gb2.set_cell_content(&gt4);

	BGrid gb3 = BGrid(2,3);
	gb3.set_width(30);
	gb3.set_border_type(termiq::canvas::BorderType::SINGLE);
	gb3.set_border_foreground_color(termiq::Color{128,210,10});
	gb3.select_cell(0,0);
	gb3.set_cell_background_color(termiq::Color{128, 128, 50});
	gb3.set_cell_content(&tb1);
	gb3.select_cell(1,1);
	gb3.set_cell_content(&tb2);
	gb3.select_cell(0,2);
	gb3.set_cell_content(&gb1);

	auto tbf1 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"hello world"));
	tbf1.set_bold();
	tbf1.set_background_color(termiq::Color{100,200,100});
	tbf1.set_width(6);

	auto tbf2 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"kavabanga халоу ворлд how are you?"));
	tbf2.set_foreground_color(termiq::Color{255, 100, 0});
	tbf2.set_width(4);
	tbf2.set_height(5);

	auto tbf3 = BText(termiq::canvas::build_wchar_lines<char16_t>(u"bikini bottom"));

	canvas.draw(3, 3, tbf1.build());
	canvas.draw(10, 10, tbf2.build());
	canvas.draw(20, 20, gb3.build());
	canvas.draw(3, 15, gb2.build());
	canvas.draw(0, 3, tbf3.build());

	se->execute<termiq::se::enter_alternate_buffer>();
	se->execute<termiq::se::set_wraparound>(false);
	se->execute<termiq::se::cursor_default>();

	auto t2 = std::chrono::system_clock::now();
	se->execute<termiq::se::cursor_hidden>();
	se->execute<termiq::se::sync_begin>();
	canvas.paint();
	se->execute<termiq::se::sync_end>();
	se->execute<termiq::se::cursor_default>();
	auto t3 = std::chrono::system_clock::now();

	se->flush();

	getchar();

	se->execute<termiq::se::set_wraparound>(true);
	se->execute<termiq::se::exit_alternate_buffer>();

	termiq::style::style_reset();
	se->flush();

	std::cout << "Time to draw: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "\n";
	std::cout << "Time to paint: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count() << "\n";
}

void paint_full_screen(termiq::SequenceExecutor* se) {
	// TEST FULL SCREEN DRAW
	termiq::canvas::SECanvas<CC> canvas(se, rows, cols, 0, 0);
	auto t1 = std::chrono::system_clock::now();
	for(int r=0;r<rows;r++) {
		for(int c=0;c<cols;c++) {
			auto builder = BText(termiq::canvas::build_wchar_lines<char16_t>(u"A"));
			if (c%2) {
				builder.set_bold();
				builder.set_foreground_color(termiq::Color{255, 10, 10});
			} else {
				builder.set_foreground_color(termiq::Color{128, 10, 10});
				builder.set_italic();
			}
			canvas.draw(r,c,builder.build());
		}
	}

	se->execute<termiq::se::enter_alternate_buffer>();
	se->execute<termiq::se::set_wraparound>(false);
	se->execute<termiq::se::cursor_default>();
	se->flush();

	auto t2 = std::chrono::system_clock::now();
	se->execute<termiq::se::cursor_hidden>();
	se->execute<termiq::se::sync_begin>();
	canvas.paint();
	se->execute<termiq::se::sync_end>();
	se->execute<termiq::se::cursor_default>();
	se->flush();
	auto t3 = std::chrono::system_clock::now();
	getchar();

	se->execute<termiq::se::set_wraparound>(true);
	se->execute<termiq::se::exit_alternate_buffer>();

	termiq::style::style_reset();
	se->flush();

	std::cout << "Time to draw: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "\n";
	std::cout << "Time to paint: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count() << "\n";
}

void test_osc5522(termiq::SequenceExecutor* se) {
	std::cout << "OSC 5522 Start\n";
	se->flush();
	se->execute<termiq::se::enable_osc5522>();
	termiq::ProtocolSupport osc5522support = se->execute<termiq::se::check_osc5522>();
	if (osc5522support == termiq::ProtocolSupport::ENABLED) {
		std::cout << "OSC 5522 enabled!" << std::endl;

		se->execute<termiq::se::osc5522_write_begin>(std::vector<std::pair<std::string, std::string>>{
			{"name", "test"},
			{"pw", "some_fancy_id_123"}
		});
		se->execute<termiq::se::osc5522_write_chunk>(std::pair<std::string, std::string_view>{"text/plain", "Hello World! 5522"});
		termiq::OSC5522Status status = se->execute<termiq::se::osc5522_write_end>();
		se->flush();

		std::cout << "OSC 5522 write status: " << (int)status << "\n";
		se->flush();

		getchar();

		termiq::OSC5522Result result = se->execute<termiq::se::osc5522_read>(
			std::vector<std::string>{"text/plain"},
			std::vector<std::pair<std::string, std::string>>{
				{"name", "test"},
				{"pw", "some_fancy_id_123"}
			}
		);

		// second read with the same name/password
		se->execute<termiq::se::osc5522_read>(
			std::vector<std::string>{"text/plain"},
			std::vector<std::pair<std::string, std::string>>{
				{"name", "test"},
				{"pw", "some_fancy_id_123"}
			}
		);

		std::cout << "OSC5522 read status: " << (int)result.status << std::endl;
		for (auto& d : result.data) {
			std::cout << "DATA: " << d.mime << " - " << d.data << std::endl;
		}

		getchar();
	} else {
		std::cout << "No OSC 5522 support\n";
	}

	std::cout << "OSC52 write" << std::endl;
	se->execute<termiq::se::osc52_write>("Hello World! 52", 'c');

	getchar();

	std::string osc52read = se->execute<termiq::se::osc52_read>('c');
	std::cout << "OSC52 read: " << osc52read << std::endl;

	se->execute<termiq::se::disable_osc5522>();
	se->flush();

	getchar();
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	termiq::init_term();
	setlocale(LC_ALL, "C.UTF-8");

	auto runner = std::make_unique<termiq::StreamSequenceRunner<>>();
	termiq::style::init(runner->get());

	std::optional<termiq::Pos> term_size;
	std::optional<termiq::ProtocolSupport> osc5522support;

	runner->run([&term_size, &osc5522support](termiq::SequenceExecutor* e){
		term_size = e->execute<termiq::se::get_size_ch>();
		e->execute<termiq::se::enable_osc5522>();
		osc5522support = e->execute<termiq::se::check_osc5522>();
	});

	auto se = runner->get();

	if (!term_size) return 1;
	rows = term_size->r;
	cols = term_size->c;
	std::cout << "Lines: " << term_size->r << "\n";
	std::cout << "Columns: " << term_size->c << "\n";
	std::cout << "OSC5522: " << (int)(*osc5522support) << "\n";

	termiq::style::italic(true);
	termiq::style::foreground(termiq::Color{255, 50, 50});
	se->flush();
	std::cout << "ITALIC_ON" << "\n";

	termiq::style::style({.bold=true});
	se->flush();
	std::cout << "TURN_ON_BOLD_ATTR" << "\n";

	termiq::style::style_reset();
	se->flush();
	std::cout << "RESET_ATTRS" << "\n";

	// unicode char
	std::vector<unsigned char> em = {0xf0, 0x9f, 0xa7, 0x99};
	for (size_t i=0;i<em.size();i++) {
		std::cout << em[i];
	}
	std::cout << "\n";

	std::cout << "Press enter..." << "\n";

	std::wstring charw = L"\u4ef6\u4ef6\u4ef6\u4ef6\u4ef6\u4ef6\u4ef6\u4ef6\u4ef6\u4ef6";
	printf("%ls", charw.data());
	fflush(stdout);

	runner->run([](termiq::SequenceExecutor* e){
		e->execute<termiq::se::move_backward>(10);
	});

	getchar();

	test_osc5522(runner->get());
	paint_texts(runner->get());
	paint_full_screen(runner->get());
	paint_canvas(runner->get());
}
