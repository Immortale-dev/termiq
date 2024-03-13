#include <iostream>
#include <cstdio>
#include <unistd.h>

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

	termiq::enter_alternate_buffer();
	termiq::exit_automatic_margins();
	termiq::cursor_default();

//	for(int i=0;i<30;i++) {
//		termiq::cursor_hidden();
//		termiq::move(0,0);
//		for(int r=0;r<rows;r++) {
//			std::string s = "";
//			termiq::move(0,r-1);
//			for(int c=0;c<cols;c++) {
////				s.push_back('A'+i);
//				putchar('A'+i);
//			}
//			//fprint();
//		}
//		termiq::cursor_default();
//		fflush(stdout);
////		sleep(1);
//		getchar();
//	}

	termiq::Canvas<termiq::CharCell<char>> canvas(10,10,10,10);
	canvas.draw(3, 3, canvas.text("hello").set_bold().build());
	canvas.draw(4, 3, canvas.text("world").set_foreground_color({1000, 300, 0}).build());
	canvas.paint();
	fflush(stdout);

	getchar();

	termiq::enter_automatic_margins();
	termiq::exit_alternate_buffer();

	termiq::style::reset();
}
