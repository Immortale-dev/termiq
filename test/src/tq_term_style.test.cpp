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
				// TODO: background is assigned with undefined color;
			});

			IT("should reset background color", {
				// TODO: background assigned a color, and then assigned undefined color
			});

			IT("should reset foreground color", {
				// TODO: foreground color assigned undefined color
			});

			IT("should reset colors", {
				// TODO: clear colors is called.
			});

			DESCRIBE("clear colors is called", {
				BEFORE_EACH({
					termiq::style::clear_colors();
				});

				IT("should not clear colors", {
					// TODO: run clear colors, check that the actual call is skipped.
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
			// TODO
		});
		DESCRIBE("reset", {
			// TODO
		});
	});
});

SCENARIO_END
