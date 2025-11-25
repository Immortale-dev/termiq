#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include <sstream>

#include "tq_term.h"
#include "tq_term_adv.h"
#include "tq_term_style.h"

SCENARIO_START

DESCRIBE("Term Style", {
	DESCRIBE("Streams are assigned", {
		std::istringstream input_sstream;
		std::ostringstream output_sstream;
		std::unique_ptr<std::istream> input_stream;
		std::unique_ptr<std::ostream> output_stream;
		std::unique_ptr<termiq::Reader> reader;
		std::unique_ptr<termiq::Writer> writer;
		std::unique_ptr<termiq::SequenceExecutor> se;
		BEFORE_EACH({
			input_sstream = std::istringstream();
			output_sstream = std::ostringstream();
			input_stream = std::make_unique<std::istream>(input_sstream.rdbuf());
			output_stream = std::make_unique<std::ostream>(output_sstream.rdbuf());
			reader = std::make_unique<termiq::StreamReader>(input_stream.get());
			writer = std::make_unique<termiq::StreamWriter>(output_stream.get());
			se = std::make_unique<termiq::SequenceExecutor>(reader.get(), writer.get());
			termiq::style::init(se.get());
		});
		AFTER_EACH({
			termiq::style::style_reset();
		});

		DESCRIBE("Colors", {
			IT("should set foreground color", {
				termiq::style::foreground(termiq::Color{10, 20, 30});

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[38;2;10;20;30m");
			});

			IT("should set background color", {
				termiq::style::background(termiq::Color{50, 20, 30});

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[48;2;50;20;30m");
			});

			IT("should not reset colors", {
				termiq::style::background(termiq::Color::NONE);

				std::string str = output_sstream.str();
				EXPECT(str).toBe("");
			});

			IT("should reset background color", {
				termiq::style::background(termiq::Color{10, 20, 30});
				size_t cur_str = output_sstream.str().size();
				termiq::style::background(termiq::Color::NONE);

				std::string str = output_sstream.str();
				EXPECT(std::string(str.begin() + cur_str, str.end())).toBe("\033[49m");
			});

			IT("should reset foreground color", {
				termiq::style::foreground(termiq::Color{10, 20, 30});
				size_t cur_str = output_sstream.str().size();
				termiq::style::colors_reset();

				std::string str = output_sstream.str();
				EXPECT(std::string(str.begin() + cur_str, str.end())).toBe("\033[39m");
			});

			IT("should reset colors", {
				termiq::style::foreground(termiq::Color{10, 20, 30});
				termiq::style::background(termiq::Color{100, 20, 30});
				size_t cur_str = output_sstream.str().size();
				termiq::style::colors_reset();

				std::string str = output_sstream.str();
				EXPECT(std::string(str.begin() + cur_str, str.end())).toBe("\033[39m\033[49m");
			});
		});

		DESCRIBE("Props", {
			IT("should turn on props one by one", {
				termiq::style::bold(true);
				termiq::style::italic(true);
				termiq::style::underline(true);
				termiq::style::inverse(true);
				termiq::style::strike(true);

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[1m\033[3m\033[4m\033[7m\033[9m");
			});

			IT("should not turn off inactive props", {
				termiq::style::bold(true);
				termiq::style::italic(true);
				termiq::style::underline(true);
				termiq::style::strike(false);
				termiq::style::italic(false);

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[1m\033[3m\033[4m\033[23m");
			});

			IT("should correctly turn off bold but not dim", {
				termiq::style::bold(true);
				termiq::style::dim(true);
				termiq::style::bold(false);

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[1m\033[2m\033[22;2m");
			});

			IT("should correctly turn off dim but not bold", {
				termiq::style::bold(true);
				termiq::style::dim(true);
				termiq::style::dim(false);

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[1m\033[2m\033[22;1m");
			});

			IT("should turn off bold", {
				termiq::style::bold(true);
				termiq::style::bold(false);

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[1m\033[22m");
			});

			IT("should turn on advanced underline", {
				termiq::style::underline(termiq::style::Underline{
					.color=termiq::Color{10,20,30},
					.style=termiq::UnderlineStyle::CURLY,
				});

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[58;2;10;20;30m\033[4:3m");
			});

			IT("should turn off only underline color", {
				termiq::style::underline(termiq::style::Underline{
					.color=termiq::Color{10,20,30},
					.style=termiq::UnderlineStyle::CURLY,
				});
				size_t cur_str = output_sstream.str().size();
				termiq::style::underline(termiq::style::Underline{
					.color=termiq::Color::NONE,
					.style=termiq::UnderlineStyle::CURLY,
				});

				std::string str = output_sstream.str();
				EXPECT(std::string(str.begin() + cur_str, str.end())).toBe("\033[59m\033[4:3m");
			});

			IT("should turn off fancy underline", {
				termiq::style::underline(termiq::style::Underline{
					.color=termiq::Color{10,20,30},
					.style=termiq::UnderlineStyle::CURLY,
				});
				size_t cur_str = output_sstream.str().size();
				termiq::style::underline(false);

				std::string str = output_sstream.str();
				EXPECT(std::string(str.begin() + cur_str, str.end())).toBe("\033[24m");
			});
		});

		DESCRIBE("Style", {
			IT("should set all styles correctly", {
				termiq::style::style({
					.foreground=termiq::Color{100, 0, 0},
					.background=termiq::Color{200, 0, 0},
					.bold=true,
					.dim=true,
					.italic=true,
					.underline=true,
					.inverse=true,
				});

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[38;2;100;0;0m\033[48;2;200;0;0m\033[1;2;3;4;7m");
			});

			IT("should only set bold", {
				termiq::style::style({termiq::Color::NONE, termiq::Color::NONE, true, false, false, false, false});

				std::string str = output_sstream.str();
				EXPECT(str).toBe("\033[1m");
			});

			DESCRIBE("Style is set with colors and attributes", {
				size_t pos;
				BEFORE_EACH({
					termiq::style::style({
						.foreground=termiq::Color{100, 0, 0},
						.background=termiq::Color{200, 0, 0},
						.bold=true,
						.dim=true,
						.italic=true,
						.underline=true,
						.inverse=true,
					});
					pos = output_sstream.str().size();
				});

				IT("should change colors and disable bold", {
					termiq::style::style({
						.foreground=termiq::Color{200, 0, 0},
						.background=termiq::Color{100, 0, 0},
						.bold=true,
						.dim=false,
						.italic=true,
						.underline=true,
						.inverse=true,
					});

					std::string str = output_sstream.str();
					EXPECT(std::string(str.begin()+pos, str.end())).toBe("\033[38;2;200;0;0m\033[48;2;100;0;0m\033[22;1m");
				});

				IT("should reset colors and attributes", {
					termiq::style::style({
						.foreground=termiq::Color::NONE,
						.background=termiq::Color::NONE,
						.bold=false,
						.dim=false,
						.italic=true,
						.underline=false,
						.inverse=false,
					});

					std::string str = output_sstream.str();
					EXPECT(std::string(str.begin()+pos, str.end())).toBe("\033[39m\033[49m\033[22;24;27m");
				});

				IT("should reset attributes", {
					termiq::style::props_reset();

					std::string str = output_sstream.str();
					EXPECT(std::string(str.begin()+pos, str.end())).toBe("\033[22;23;24;27m");
				});

				IT("should reset all", {
					termiq::style::style_reset();

					std::string str = output_sstream.str();
					EXPECT(std::string(str.begin()+pos, str.end())).toBe("\033[0m");
				});
			});
		});
	});
});

SCENARIO_END
