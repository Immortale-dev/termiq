#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <chrono>

#include "tq_term.h"
#include "tq_term_adv.h"
#include "tq_term_style.h"
#include "tq_term_canvas.h"

int main() {
	termiq::init_term();
	setlocale(LC_ALL, "C.UTF-8");

	const int SZ = 1000000;
	char* buf = new char[SZ];

	std::setvbuf(stdout, buf, _IOFBF, SZ);

	const int rows = termiq::get_rows();
	const int cols = termiq::get_cols();

	std::cout << "Lines: " << rows << std::endl;
	std::cout << "Columns: " << cols << std::endl;
	std::cout << "Colors: " << termiq::get_max_colors() << std::endl;
	std::cout << "Pairs: " << termiq::get_max_pairs() << std::endl;

	termiq::set_italic_on();
	termiq::style::foreground({500, 100, 100});
	std::cout << "ITALIC_ON" << std::endl;

	termiq::set_attrs(true, false, false, false);
	std::cout << "TURN_ON_BOLD_ATTR" << std::endl;

	termiq::reset_attrs();
	std::cout << "RESET_ATTRS" << std::endl;

	#ifdef STUBTERM
	std::cout << "TEST STUB ACTIVATED!" << std::endl;
	#endif

	getchar();

//	termiq::style::background({0, 500, 500});
//	termiq::style::bold(true);
//	termiq::style::underline(true);
////	termiq::style::italic(true);
////	termiq::style::inverse(true);
//
//	std::cout << "SOme text";
//	termiq::style::clear_colors();
//	std::cout << std::endl;
//
//	termiq::style::foreground({0, 500, 100});
//	termiq::style::underline(false);
//
//	std::cout << "Other text" << std::endl;
//	for(int i=1;i<=10;i++) {
//		termiq::style::foreground({((i%2) == 0) ? 1000 : 500, 0, 0});
//		std::cout << "This is red color" << std::endl;
//	}
//	getchar();
//
//	termiq::style::clear_colors();
//
////	termiq::alternate_chars_on();
////	termiq::style::inverse(true);
//
////	std::cout << "qwertyuiop[]" << std::endl;
//	std::cout << "asdfghjkl;'" << std::endl;
////	std::cout << "zxcvbnm,./" << std::endl;
//
////	std::cout << "jklmnq╰┓┇┊🬀" << std::endl;
////	std::cout << "opqrs" << std::endl;
//
//	std::wcout << termiq::alt_chars::C_BR << termiq::alt_chars::C_H << termiq::alt_chars::C_HB << termiq::alt_chars::C_H << termiq::alt_chars::C_BL << std::endl;
//	std::wcout << termiq::alt_chars::C_VR << termiq::alt_chars::C_H << termiq::alt_chars::C_X << termiq::alt_chars::C_H << termiq::alt_chars::C_VL << std::endl;
//	std::wcout << termiq::alt_chars::C_TR << termiq::alt_chars::C_H << termiq::alt_chars::C_HT << termiq::alt_chars::C_H << termiq::alt_chars::C_TL << std::endl;
//
//	getchar();
//
//	termiq::alternate_chars_off();
//
////	std::wcout << "HI:" << "; Hello, \u2501!" << std::endl;
//	std::wstring charw = L"NICE! \u2501 ═══ HELLO?";
//	wchar_t cc = L'\u2500';
////	std::wcout << charw.size() << "STR: " << charw << '\n';
////	std::wcout << termiq::draw_chars::C_H;
//
//	if (printf("%ls\n%ls\n", charw.data(), &cc) < 0) {
//		perror("printf");
//	}

	termiq::Canvas<termiq::CharCell<wchar_t>> canvas(rows-1,cols,0,0);

//  TEST FULL SCREEN DRAW
//	for(int r=0;r<rows-1;r++) {
//		for(int c=0;c<cols;c++) {
//			auto builder = canvas.text(L"A").set_foreground_color({(c%10 <= 5) ? 1000 : 500, 100, 100}); //.set_bold().set_inverse().build()
//			if (c%10 <= 5) {
//				builder.set_bold();
//			} else {
//				builder.set_italic();
//			}
//			canvas.draw(r,c,builder.build());
//		}
//	}

//  TEST MULTILINE TEXT
	auto t1 = std::chrono::system_clock::now();

	auto textBuilder1 = canvas.text(L"woohoo").set_background_color({200, 200, 900});
	auto textBuilder2 = canvas.text(L"hello\nworld");

	canvas.draw(3, 3, canvas.text(L"hello world").set_bold().set_background_color({300,700,300}).set_width(6).build());
	canvas.draw(10, 10, canvas.text(L"kavabanga халоу ворлд how are you?").set_foreground_color({1000, 300, 0}).set_width(4).set_height(5).build());
	canvas.draw(20, 20,
		canvas.grid(2,3)
			.set_width(20)
			.set_border_type(termiq::BorderType::SINGLE)
			.set_border_foreground_color({400,800,100})
//			.set_border_background_color{100,1000,100})
			.select_cell(0,0)
				.set_cell_background_color({500, 500, 100})
				.set_cell_content(&textBuilder1)
			.select_cell(1,1)
				.set_cell_content(&textBuilder2)
		.build()
	);

	termiq::enter_alternate_buffer();
	termiq::exit_automatic_margins();
	termiq::cursor_default();

	auto t2 = std::chrono::system_clock::now();
	termiq::cursor_hidden();
	canvas.paint();
	termiq::cursor_default();
	fflush(stdout);
	auto t3 = std::chrono::system_clock::now();
	getchar();

	termiq::enter_automatic_margins();
	termiq::exit_alternate_buffer();

	termiq::style::reset();

	std::cout << "Time to draw: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << std::endl;
	std::cout << "Time to paint: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count() << std::endl;
}
