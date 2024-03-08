#include <iostream>
#include <stdio.h>

#include "tq_term.h"
#include "tq_term_adv.h"

int main() {
	termiq::init_term();

	std::cout << "Lines: " << termiq::get_rows() << std::endl;
	std::cout << "Columns: " << termiq::get_cols() << std::endl;
	std::cout << "Colors: " << termiq::get_max_colors() << std::endl;
	std::cout << "Pairs: " << termiq::get_max_pairs() << std::endl;

	termiq::define_color(5, 1000, 10, 10);
	termiq::define_color(6, 0, 0, 0);
	termiq::set_foreground_color(5);

	printf("Color is red");



	termiq::set_attrs(1, 0, 1, 1);
	termiq::set_italic_on();
	termiq::set_foreground_color(5);
	termiq::set_background_color(6);
	printf("Color is green");

	termiq::set_italic_off();
	termiq::reset_attrs();

	termiq::reset_colors();

	printf("romal color");

	termiq::run_str("sgr0", 1);

	getchar();

	termiq::undefine_colors();
}
