#include <vector>
#include <string>
#include <utility>

#include "tq_term_adv.h"
#include "tq_term_style.h"

namespace termiq {
	extern int get_max_colors_stub_ret_;
	extern void term_stub_reset_styles_();
	extern std::vector<std::string> term_stub_calls_;
	extern std::vector<std::tuple<int, int, int ,int>> define_color_stub_;
	extern std::vector<std::tuple<int>> set_foreground_color_stub_;
	extern std::vector<std::tuple<int>> set_background_color_stub_;
	extern std::vector<std::tuple<bool, bool, bool, bool>> set_attrs_stub_;
}

SCENARIO_START

DESCRIBE("Term Style", {
	DESCRIBE("terminal has max 21 colors", {
		BEFORE_EACH({
			termiq::get_max_colors_stub_ret_ = 21;
		});
		AFTER_EACH({
			termiq::term_stub_reset_styles_();
			termiq::term_stub_calls_.clear();
		});
		DESCRIBE("Foreground color is set", {
			BEFORE_EACH({
				termiq::style::foreground({100, 120, 130});
			});
			AFTER_EACH({
				termiq::define_color_stub_.clear();
				termiq::set_foreground_color_stub_.clear();
				termiq::set_background_color_stub_.clear();
			});

			IT("should call max colors check", {
				EXPECT(termiq::term_stub_calls_[0]).toBe("get_max_colors");
			});

			IT("should define color with id 16 and color {100, 120, 130}", {
				auto tpl = termiq::define_color_stub_[0];

				EXPECT(termiq::term_stub_calls_[1]).toBe("define_color");
				EXPECT(std::get<0>(tpl)).toBe(16);
				EXPECT(std::get<1>(tpl)).toBe(100);
				EXPECT(std::get<2>(tpl)).toBe(120);
				EXPECT(std::get<3>(tpl)).toBe(130);
			});

			IT("should call set foreground with id 16", {
				auto tpl = termiq::set_foreground_color_stub_[0];

				EXPECT(termiq::term_stub_calls_[2]).toBe("set_foreground_color");
				EXPECT(std::get<0>(tpl)).toBe(16);
			});

			IT("should define and use additional color with id 17", {
				termiq::style::background({200, 120, 130});

				auto tpl = termiq::define_color_stub_[1];
				auto tpl_bg = termiq::set_background_color_stub_[0];

				EXPECT(std::get<0>(tpl)).toBe(17);
				EXPECT(std::get<1>(tpl)).toBe(200);
				EXPECT(std::get<2>(tpl)).toBe(120);
				EXPECT(std::get<3>(tpl)).toBe(130);
				EXPECT(std::get<0>(tpl_bg)).toBe(17);
			});

			IT("should not reset colors", {
				termiq::style::background(termiq::style::Color::UNDEFINED);
				EXPECT(termiq::term_stub_calls_.size()).toBe(3);
			});

			IT("should reset background color", {
				termiq::style::background({100, 120, 130});
				termiq::style::background(termiq::style::Color::UNDEFINED);

				// Reset and foreground reassign.
				EXPECT(termiq::term_stub_calls_.size()).toBe(6);
				EXPECT(termiq::term_stub_calls_[4]).toBe("reset_colors");
				EXPECT(termiq::term_stub_calls_[5]).toBe("set_foreground_color");
			});

			IT("should reset foreground color", {
				termiq::style::foreground(termiq::style::Color::UNDEFINED);

				// No background reassignment as it is undefined.
				EXPECT(termiq::term_stub_calls_.size()).toBe(4);
				EXPECT(termiq::term_stub_calls_[3]).toBe("reset_colors");
			});

			IT("should reset colors", {
				termiq::style::clear_colors();

				EXPECT(termiq::term_stub_calls_.size()).toBe(4);
				EXPECT(termiq::term_stub_calls_[3]).toBe("reset_colors");
			});

			DESCRIBE("clear colors is called", {
				BEFORE_EACH({
					termiq::style::clear_colors();
				});

				IT("should not clear colors", {
					termiq::style::clear_colors();

					// Extra call is skipped.
					EXPECT(termiq::term_stub_calls_.size()).toBe(4);
					EXPECT(termiq::term_stub_calls_[3]).toBe("reset_colors");
				});
			});

			DESCRIBE("2 more colors are defined", {
				BEFORE_EACH({
					termiq::style::foreground({500, 0, 0});
					termiq::style::foreground({0, 500, 0});
				});

				IT("should reuse the color id 17", {
					termiq::style::background({500, 0, 0});

					auto tpl = termiq::set_background_color_stub_[0];

					EXPECT(std::get<0>(tpl)).toBe(17);
				});
			});

			DESCRIBE("4 more colors are defined", {
				BEFORE_EACH({
					termiq::style::background({100, 100, 0});
					termiq::style::background({0, 100, 100});
					termiq::style::background({100, 0, 100});
					termiq::style::background({100, 100, 100});
				});

				IT("should define all colors with unique ids", {
					EXPECT(std::get<0>(termiq::define_color_stub_[1])).toBe(17);
					EXPECT(std::get<0>(termiq::define_color_stub_[2])).toBe(18);
					EXPECT(std::get<0>(termiq::define_color_stub_[3])).toBe(19);
					EXPECT(std::get<0>(termiq::define_color_stub_[4])).toBe(20);
				});

				IT("should redefine id 17 with new color", {
					// Use the first color one more time
					termiq::style::background({100, 120, 130});
					// Use new color
					termiq::style::foreground({200, 200, 200});

					EXPECT(std::get<0>(termiq::define_color_stub_[5])).toBe(17);
				});
			});
		});
		DESCRIBE("attributes", {
			AFTER_EACH({
				termiq::set_attrs_stub_.clear();
			});
			IT("should turn on bold", {
				termiq::style::bold(true);

				auto tpl = termiq::set_attrs_stub_[0];

				EXPECT(termiq::term_stub_calls_[0]).toBe("set_attrs");
				EXPECT(std::get<0>(tpl)).toBe(true);
				EXPECT(std::get<1>(tpl)).toBe(false);
				EXPECT(std::get<2>(tpl)).toBe(false);
				EXPECT(std::get<3>(tpl)).toBe(false);
			});
			IT("should turn on bold and bold, dim, underline and inverse", {
				termiq::style::bold(true);
				termiq::style::dim(true);
				termiq::style::underline(true);
				termiq::style::inverse(true);

				auto tpl2 = termiq::set_attrs_stub_[1];
				auto tpl3 = termiq::set_attrs_stub_[2];
				auto tpl4 = termiq::set_attrs_stub_[3];

				EXPECT(termiq::term_stub_calls_.size()).toBe(4);
				EXPECT(std::get<0>(tpl2)).toBe(true);
				EXPECT(std::get<1>(tpl2)).toBe(true);
				EXPECT(std::get<2>(tpl2)).toBe(false);
				EXPECT(std::get<3>(tpl2)).toBe(false);

				EXPECT(std::get<0>(tpl3)).toBe(true);
				EXPECT(std::get<1>(tpl3)).toBe(true);
				EXPECT(std::get<2>(tpl3)).toBe(false);
				EXPECT(std::get<3>(tpl3)).toBe(true);

				EXPECT(std::get<0>(tpl4)).toBe(true);
				EXPECT(std::get<1>(tpl4)).toBe(true);
				EXPECT(std::get<2>(tpl4)).toBe(true);
				EXPECT(std::get<3>(tpl4)).toBe(true);
			});

			IT("should left just inverse turned on", {
				termiq::style::bold(true);
				termiq::style::inverse(true);
				termiq::style::bold(false);

				auto tpl3 = termiq::set_attrs_stub_[2];

				EXPECT(termiq::term_stub_calls_.size()).toBe(3);
				EXPECT(std::get<0>(tpl3)).toBe(false);
				EXPECT(std::get<1>(tpl3)).toBe(false);
				EXPECT(std::get<2>(tpl3)).toBe(true);
				EXPECT(std::get<3>(tpl3)).toBe(false);
			});

			IT("should turn in italic", {
				termiq::style::italic(true);

				EXPECT(termiq::term_stub_calls_.size()).toBe(1);
				EXPECT(termiq::term_stub_calls_[0]).toBe("set_italic_on");
			});

			IT("should turn on italic and dim", {
				termiq::style::italic(true);
				termiq::style::dim(true);

				EXPECT(termiq::term_stub_calls_.size()).toBe(3);
				EXPECT(termiq::term_stub_calls_[1]).toBe("set_attrs");
				EXPECT(termiq::term_stub_calls_[2]).toBe("set_italic_on");
			});

			IT("should reenable colors if attribute changed", {
				termiq::style::foreground({100, 0, 0});
				termiq::style::background({100, 0, 0});
				termiq::style::underline(true);

				EXPECT(termiq::term_stub_calls_.size()).toBe(7);
				EXPECT(termiq::term_stub_calls_[4]).toBe("set_attrs");
				EXPECT(termiq::term_stub_calls_[5]).toBe("set_foreground_color");
				EXPECT(termiq::term_stub_calls_[6]).toBe("set_background_color");
			});

			IT("should call reset_attrs", {
				termiq::style::bold(true);
				termiq::style::bold(false);

				EXPECT(termiq::term_stub_calls_.size()).toBe(2);
				EXPECT(termiq::term_stub_calls_[1]).toBe("reset_attrs");
			});
		});
		DESCRIBE("Style", {
			AFTER_EACH({
				termiq::set_attrs_stub_.clear();
				termiq::define_color_stub_.clear();
				termiq::set_foreground_color_stub_.clear();
				termiq::set_background_color_stub_.clear();
			});

			IT("should set all styles correctly", {
				termiq::style::style({100, 0, 0}, {200, 0, 0}, true, true, true, true, true);

				auto tpl = termiq::set_attrs_stub_[0];

				EXPECT(termiq::term_stub_calls_).toBe({"set_attrs", "set_italic_on", "get_max_colors", "define_color", "set_foreground_color", "define_color", "set_background_color"});
				EXPECT(std::get<0>(tpl)).toBe(true);
				EXPECT(std::get<1>(tpl)).toBe(true);
				EXPECT(std::get<2>(tpl)).toBe(true);
				EXPECT(std::get<3>(tpl)).toBe(true);
			});

			IT("should only call set_attrs", {
				termiq::style::style(termiq::style::Color::UNDEFINED, termiq::style::Color::UNDEFINED, true, false, false, false, false);

				EXPECT(termiq::term_stub_calls_).toBe({"set_attrs"});
			});

			IT("should not call set_attrs", {
				termiq::style::style({200, 0, 0}, {200, 0, 0}, false, false, false, false, false);

				EXPECT(termiq::term_stub_calls_).toBe({"get_max_colors", "define_color", "set_foreground_color", "set_background_color"});
			});

			DESCRIBE("Style is set with colors and attributes", {
				BEFORE_EACH({
					termiq::style::style({100, 0, 0}, {200, 0, 0}, true, true, true, true, true);
				});

				IT("should call both set_attrs and set backgrounds", {
					termiq::style::style({200, 0, 0}, {100, 0, 0}, true, false, true, true, true);

					EXPECT(termiq::term_stub_calls_.size()).toBe(10);
					EXPECT(termiq::term_stub_calls_[7]).toBe("set_attrs");
					EXPECT(termiq::term_stub_calls_[8]).toBe("set_foreground_color");
					EXPECT(termiq::term_stub_calls_[9]).toBe("set_background_color");
				});

				IT("should call only set_attrs and set_italic_on", {
					termiq::style::style(termiq::style::Color::UNDEFINED, termiq::style::Color::UNDEFINED, false, true, false, false, false);

					EXPECT(termiq::term_stub_calls_.size()).toBe(9);
					EXPECT(termiq::term_stub_calls_[7]).toBe("reset_attrs");
					EXPECT(termiq::term_stub_calls_[8]).toBe("set_italic_on");
				});

				IT("should call reset_attrs and set_background_color", {
					termiq::style::style({100, 0, 0}, termiq::style::Color::UNDEFINED, false, false, false, false, false);

					EXPECT(termiq::term_stub_calls_.size()).toBe(9);
					EXPECT(termiq::term_stub_calls_[7]).toBe("reset_attrs");
					EXPECT(termiq::term_stub_calls_[8]).toBe("set_foreground_color");
				});

				IT("should call reset_attrs and set_background_color", {
					termiq::style::style(termiq::style::Color::UNDEFINED, {200, 0, 0}, false, false, false, false, false);

					EXPECT(termiq::term_stub_calls_.size()).toBe(9);
					EXPECT(termiq::term_stub_calls_[7]).toBe("reset_attrs");
					EXPECT(termiq::term_stub_calls_[8]).toBe("set_background_color");
				});
			});
		});
	});
});

SCENARIO_END
