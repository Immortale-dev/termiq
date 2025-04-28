//#define TEST_ONLY_RULE

#include "qtest.h"

#include "src/helpers.cpp"

#include "src/tq_term_style.test.cpp"
#include "src/tq_term_canvas_text.test.cpp"
#include "src/tq_term_canvas_grid.test.cpp"
#include "src/tq_term_canvas_flex.test.cpp"
#include "src/tq_term_canvas.test.cpp"

int main() {
	setlocale(LC_ALL, "C.UTF-8");
	return 0;
}
