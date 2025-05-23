![logo](https://github.com/Immortale-dev/termiq/blob/master/res/imgs/logo.svg?raw=true#gh-light-mode-only)
![logo](https://github.com/Immortale-dev/termiq/blob/master/res/imgs/logo_dark.svg?raw=true#gh-dark-mode-only)

C++ library that provides a **high** *(or higher)* level of abstraction on standard **terminal** API, that allows you to stylize terminal output or manipulate terminal behavior entirely.

[![CodeFactor](https://www.codefactor.io/repository/github/immortale-dev/termiq/badge)](https://www.codefactor.io/repository/github/immortale-dev/termiq)

## Table of Contest

* [Table of Contest](#table-of-contest)
* [Dependencies](#dependencies)
* [Build](#build)
* [Documentation](#documentation)
	* [qt_term.h](#qt_term.h)
		* [void termiq::init_term([std::string terminal term])](#void-termiqinit_termstdstring-terminal-term)
		* [bool termiq::get_flag(std::string name)](#bool-termiqget_flagstdstring-name)
		* [int termiq::get_num(std::string name)](#int-termiqget_numstdstring-name)
		* [int termiq::run_str(std::string name[, int padding])](#int-termiqrun_strstdstring-name-int-padding)
		* [int termiq::run_str<P…>(std::string name, int padding, P… args)](#int-termiqrun_strp...stdstring-name-int-padding-p...-args)
	* [qt_term_adv.h](#qt_term_adv.h)
		* [class termiq::exception](#class-termiqexception)
		* [int termiq::get_cols()](#int-termiqget_cols)
		* [int termiq::get_rows()](#int-termiqget_rows)
		* [void termiq::move(int r, int c)](#void-termiqmoveint-r-int-c)
		* [void termiq::move_left([int steps])](#void-termiqmove_leftint-steps)
		* [void termiq::move_right([int steps])](#void-termiqmove_rightint-steps)
		* [void termiq::move_up([int steps])](#void-termiqmove_upint-steps)
		* [void termiq::move_down([int steps])](#void-termiqmove_downint-steps)
		* [void termiq::save_cursor_position()](#void-termiqsave_cursor_position)
		* [void termiq::restore_cursor_position()](#void-termiqrestore_cursor_position)
		* [void termiq::cursor_hidden()](#void-termiqcursor_hidden)
		* [void termiq::cursor_default()](#void-termiqcursor_default)
		* [int termiq::get_max_colors()](#int-termiqget_max_colors)
		* [int termiq::get_max_pairs()](#int-termiqget_max_pairs)
		* [void termiq::define_color(int id, int r, int g, int b)](#void-termiqdefine_colorint-id-int-r-int-g-int-b)
		* [void termiq::define_pair(int id, int r, int g, int b, int br, int bg, int bb)](#void-termiqdefine_pairint-id-int-r-int-g-int-b-int-br-int-bg-int-bb)
		* [void termiq::undefine_colors()](#void-termiqundefine_colors)
		* [void termiq::set_foreground_color(int id)](#void-termiqset_foreground_colorint-id)
		* [void termiq::set_background_color(int id)](#void-termiqset_background_colorint-id)
		* [void termiq::set_pair_color(int id)](#void-termiqset_pair_colorint-id)
		* [void termiq::set_attrs(bool bold, bool dim, bool reverse, bool underline)](#void-termiqset_attrsbool-bold-bool-dim-bool-reverse-bool-underline)
		* [void termiq::reset_attrs()](#void-termiqreset_attrs)
		* [void termiq::set_italic_on()](#void-termiqset_italic_on)
		* [void termiq::set_italic_off()](#void-termiqset_italic_off)
		* [void termiq::reset_colors()](#void-termiqreset_colors)
		* [void termiq::alternate_chars_on()](#void-termiqalternate_chars_on)
		* [void termiq::alternate_chars_off()](#void-termiqalternate_chars_off)
		* [void termiq::enter_automatic_margins()](#void-termiqenter_automatic_margins)
		* [void termiq::exit_automatic_margins()](#void-termiqexit_automatic_margins)
		* [void termiq::enter_alternate_buffer()](#void-termiqenter_alternate_buffer)
		* [void termiq::exit_alternate_buffer()](#void-termiqexit_alternate_buffer)
		* [void termiq::clear()](#void-termiqclear)
		* [void termiq::clear_before()](#void-termiqclear_before)
		* [void termiq::clear_after()](#void-termiqclear_after)
		* [void termiq::clear_rest()](#void-termiqclear_rest)
		* [namespace termiq::alt_chars](#namespace-termiqalt_chars)
	* [tq_term_style.h](#tq_term_style.h)
		* [struct termiq::style::Color](#struct-termiqstylecolor)
		* [void termiq::style::foreground(Color color)](#void-termiqstyleforegroundcolor-color)
		* [void termiq::style::background(Color color)](#void-termiqstylebackgroundcolor-color)
		* [void termiq::style::bold(bool state)](#void-termiqstyleboldbool-state)
		* [void termiq::style::italic(bool state)](#void-termiqstyleitalicbool-state)
		* [void termiq::style::underline(bool state)](#void-termiqstyleunderlinebool-state)
		* [void termiq::style::dim(bool state)](#void-termiqstyledimbool-state)
		* [void termiq::style::inverse(bool state)](#void-termiqstyleinversebool-state)
		* [void termiq::style::special_chars(bool state)](#void-termiqstylespecial_charsbool-state)
		* [void termiq::style::clear_colors()](#void-termiqstyleclear_colors)
		* [void termiq::style::clear_styles([bool also_clear_colors])](#void-termiqstyleclear_stylesbool-also_clear_colors)
		* [void termiq::style::clear()](#void-termiqstyleclear)
		* [void termiq::style::reset()](#void-termiqstylereset)
		* [bool termiq::style::is_color_defined(const Color &color)](#bool-termiqstyleis_color_definedconst-color-color)
		* [void termiq::style::style(Color fg, Color bg, bool bold, bool italic, bool dim, bool underline, bool inverse)](#void-termiqstylestylecolor-fg-color-bg-bool-bold-bool-italic-bool-dim-bool-underline-bool-inverse)
	* [termiq::canvas::Canvas<CC>](#termiqcanvascanvascc)
		* [termiq::canvas::Canvas<CC>::Canvas(unsigned int h, unsigned int w[, unsigned int r, unsigned int])](#termiqcanvascanvascccanvasunsigned-int-h-unsigned-int-w-unsigned-int-r-unsigned-int)
		* [unsigned int termiq::canvas::Canvas<CC>::get_width()](#unsigned-int-termiqcanvascanvasccget_width)
		* [unsigned int termiq::canvas::Canvas<CC>::get_height()](#unsigned-int-termiqcanvascanvasccget_height)
		* [unsigned int termiq::canvas::Canvas<CC>::get_row()](#unsigned-int-termiqcanvascanvasccget_row)
		* [unsigned int termiq::canvas::Canvas<CC>::get_col()](#unsigned-int-termiqcanvascanvasccget_col)
		* [const std::vector<std::vector<CC>>& termiq::canvas::Canvas<CC>::get_canvas()](#const-stdvectorstdvectorcc-termiqcanvascanvasccget_canvas)
		* [void termiq::canvas::Canvas<CC>::resize(unsigned int height, unsigned int width)](#void-termiqcanvascanvasccresizeunsigned-int-height-unsigned-int-width)
		* [void termiq::canvas::Canvas<CC>::move(unsigned int row, unsigned int col)](#void-termiqcanvascanvasccmoveunsigned-int-row-unsigned-int-col)
		* [void termiq::canvas::Canvas<CC>::draw(unsigned int row, unsigned int col, const CanvasPieces<CC>&& pieces)](#void-termiqcanvascanvasccdrawunsigned-int-row-unsigned-int-col-const-canvaspiecescc-pieces)
		* [void termiq::canvas::Canvas<CC>::draw(unsigned int row, unsigned int col, const CanvasPieces<CC>& pieces)](#void-termiqcanvascanvasccdrawunsigned-int-row-unsigned-int-col-const-canvaspiecescc-pieces-1)
		* [void termiq::canvas::Canvas<CC>::paint()](#void-termiqcanvascanvasccpaint)
		* [virtual protected void termiq::canvas::Canvas<CC>::moved()](#virtual-protected-void-termiqcanvascanvasccmoved)
		* [virtual protected void termiq::canvas::Canvas<CC>::resized()](#virtual-protected-void-termiqcanvascanvasccresized)
		* [virtual protected void termiq::canvas::Canvas<CC>::drawn()](#virtual-protected-void-termiqcanvascanvasccdrawn)
		* [virtual protected void termiq::canvas::Canvas<CC>::paint_row(size_t index)](#virtual-protected-void-termiqcanvascanvasccpaint_rowsize_t-index)
		* [virtual protected void termiq::canvas::Canvas<CC>::move_cursor(unsigned int row, unsigned int col)](#virtual-protected-void-termiqcanvascanvasccmove_cursorunsigned-int-row-unsigned-int-col)
		* [virtual protected void termiq::canvas::Canvas<CC>::set_paint_state(CharState* state)](#virtual-protected-void-termiqcanvascanvasccset_paint_statecharstate-state)
	* [concept termiq::canvas::Type](#concept-termiqcanvastype)
	* [concept termiq::canvas::State](#concept-termiqcanvasstate)
	* [struct termiq::canvas::CharState](#struct-termiqcanvascharstate)
	* [struct termiq::canvas::CharCell<CT, CS>](#struct-termiqcanvascharcellct-cs)
		* [bool termiq::canvas::CharCell<CT>::is_transparent()](#bool-termiqcanvascharcellctis_transparent)
	* [struct termiq::canvas::CanvasPiece<CC>](#struct-termiqcanvascanvaspiececc)
	* [struct termiq::canvas::CanvasPieces<CC>](#struct-termiqcanvascanvaspiecescc)
	* [class termiq::canvas::Content<CC>](#class-termiqcanvascontentcc)
		* [virtual CanvasPieces<CC> termiq::canvas::Content<CC>::build() = 0](#virtual-canvaspiecescc-termiqcanvascontentccbuild--0)
		* [virtual unsigned int termiq::canvas::Content<CC>::get_width() = 0](#virtual-unsigned-int-termiqcanvascontentccget_width--0)
		* [virtual unsigned int termiq::canvas::Content<CC>::get_height() = 0](#virtual-unsigned-int-termiqcanvascontentccget_height--0)
		* [virtual unsigned int termiq::canvas::Content<CC>::min_width() = 0](#virtual-unsigned-int-termiqcanvascontentccmin_width--0)
		* [virtual unsigned int termiq::canvas::Content<CC>::min_height() = 0](#virtual-unsigned-int-termiqcanvascontentccmin_height--0)
		* [virtual void termiq::canvas::Content<CC>::built(CanvasPieces<CC> &pieces)](#virtual-void-termiqcanvascontentccbuiltcanvaspiecescc-pieces)
	* [class Nothing : public Content<CC>](#class-nothing--public-contentcc)
	* [class termiq::canvas::Text : public Content<CC>](#class-termiqcanvastext--public-contentcc)
		* [termiq::canvas::Text<CC>::Text()](#termiqcanvastextcctext)
		* [termiq::canvas::Text<CC>::Text(std::span<char_type> line)](#termiqcanvastextcctextstdspanchar_type-line)
		* [termiq::canvas::Text<CC>::Text(std::span<std::vector<char_type>> multiline)](#termiqcanvastextcctextstdspanstdvectorchar_type-multiline)
		* [termiq::canvas::Text<CC>::Text(std::vector<char_type>&& line)](#termiqcanvastextcctextstdvectorchar_type-line)
		* [termiq::canvas::Text<CC>::Text(std::vector<std::vector<char_type>>&& multiline)](#termiqcanvastextcctextstdvectorstdvectorchar_type-multiline)
		* [void termiq::canvas::Text<CC>::set_foreground_color(termiq::style::Color &&color)](#void-termiqcanvastextccset_foreground_colortermiqstylecolor-color)
		* [void termiq::canvas::Text<CC>::set_background_color(termiq::style::Color &&color);](#void-termiqcanvastextccset_background_colortermiqstylecolor-color)
		* [void termiq::canvas::Text<CC>::set_bold()](#void-termiqcanvastextccset_bold)
		* [void termiq::canvas::Text<CC>::set_italic()](#void-termiqcanvastextccset_italic)
		* [void termiq::canvas::Text<CC>::set_dim()](#void-termiqcanvastextccset_dim)
		* [void termiq::canvas::Text<CC>::set_underline()](#void-termiqcanvastextccset_underline)
		* [void termiq::canvas::Text<CC>::set_inverse()](#void-termiqcanvastextccset_inverse)
		* [void termiq::canvas::Text<CC>::set_width()](#void-termiqcanvastextccset_width)
		* [void termiq::canvas::Text<CC>::set_height()](#void-termiqcanvastextccset_height)
		* [const std::vector<std::vector<char_type>>& termiq::canvas::Text<CC>::get_lines()](#const-stdvectorstdvectorchar_type-termiqcanvastextccget_lines)
		* [virtual void termiq::canvas::Text<CC>::calculate_lines()](#virtual-void-termiqcanvastextcccalculate_lines)
		* [virtual unsigned int termiq::canvas::Text<CC>::get_calc_width()](#virtual-unsigned-int-termiqcanvastextccget_calc_width)
		* [virtual unsigned int termiq::canvas::Text<CC>::get_calc_height()](#virtual-unsigned-int-termiqcanvastextccget_calc_height)
		* [void termiq::canvas::Text<CC>::invalidate_lines()](#void-termiqcanvastextccinvalidate_lines)
		* [void termiq::canvas::Text<CC>::lazy_calculate_lines()](#void-termiqcanvastextcclazy_calculate_lines)
		* [bool termiq::canvas::Text<CC>::is_valid_lines()](#bool-termiqcanvastextccis_valid_lines)
	* [class termiq::canvas::Grid<CC> : public Content<CC>](#class-termiqcanvasgridcc--public-contentcc)
		* [termiq::canvas::Grid<CC>::Grid([unsigned int rows, unsigned int cols])](#termiqcanvasgridccgridunsigned-int-rows-unsigned-int-cols)
		* [void termiq::canvas::Grid<CC>::set_background_color(termiq::style::Color color)](#void-termiqcanvasgridccset_background_colortermiqstylecolor-color)
		* [void termiq::canvas::Grid<CC>::set_border_foreground_color(termiq::style::Color color)](#void-termiqcanvasgridccset_border_foreground_colortermiqstylecolor-color)
		* [void termiq::canvas::Grid<CC>::set_border_background_color(termiq::style::Color color)](#void-termiqcanvasgridccset_border_background_colortermiqstylecolor-color)
		* [void termiq::canvas::Grid<CC>::set_width(unsigned int width)](#void-termiqcanvasgridccset_widthunsigned-int-width)
		* [void termiq::canvas::Grid<CC>::set_height(unsigned int height)](#void-termiqcanvasgridccset_heightunsigned-int-height)
		* [void termiq::canvas::Grid<CC>::set_border_type(termiq::canvas::BorderType type)](#void-termiqcanvasgridccset_border_typetermiqcanvasbordertype-type)
		* [void termiq::canvas::Grid<CC>::select_cell(size_t r, size_t c)](#void-termiqcanvasgridccselect_cellsize_t-r-size_t-c)
		* [void termiq::canvas::Grid<CC>::select_cell(size_t c)](#void-termiqcanvasgridccselect_cellsize_t-c)
		* [void termiq::canvas::Grid<CC>::set_cell_background_color(termiq::style::Color color)](#void-termiqcanvasgridccset_cell_background_colortermiqstylecolor-color)
		* [void termiq::canvas::Grid<CC>::set_cell_width(unsigned int width)](#void-termiqcanvasgridccset_cell_widthunsigned-int-width)
		* [void termiq::canvas::Grid<CC>::set_cell_height(unsigned int height)](#void-termiqcanvasgridccset_cell_heightunsigned-int-height)
		* [void termiq::canvas::Grid<CC>::set_cell_content(Content<CC>* content)](#void-termiqcanvasgridccset_cell_contentcontentcc-content)
		* [virtual protected std::vector<unsigned int> termiq::canvas::Grid<CC>::calculate_column_sizes()](#virtual-protected-stdvectorunsigned-int-termiqcanvasgridcccalculate_column_sizes)
		* [virtual protected std::vector<unsigned int> termiq::canvas::Grid<CC>::calculate_row_sizes()](#virtual-protected-stdvectorunsigned-int-termiqcanvasgridcccalculate_row_sizes)
		* [virtual protected void termiq::canvas::Grid<CC>::calc()](#virtual-protected-void-termiqcanvasgridcccalc)
		* [void termiq::canvas::Grid<CC>::invalidate_calc()](#void-termiqcanvasgridccinvalidate_calc)
		* [void termiq::canvas::Text<CC>::lazy_calc()](#void-termiqcanvastextcclazy_calc)
		* [bool termiq::canvas::Text<CC>::is_valid_calc()](#bool-termiqcanvastextccis_valid_calc)
	* [class termiq::canvas::Flexible](#class-termiqcanvasflexible)
		* [virtual protected void termiq::canvas::Flexible::suggest_width(unsigned int width) = 0](#virtual-protected-void-termiqcanvasflexiblesuggest_widthunsigned-int-width--0)
		* [virtual void termiq::canvas::Flexible::suggest_height(unsigned int height) = 0](#virtual-void-termiqcanvasflexiblesuggest_heightunsigned-int-height--0)
	* [class class termiq::canvas::FlexText<CC> : public Text<CC>, public Flexible](#class-class-termiqcanvasflextextcc--public-textcc-public-flexible)
	* [class termiq::canvas::FlexGrid<CC> : public Grid<CC>, public Flexible](#class-termiqcanvasflexgridcc--public-gridcc-public-flexible)
* [Usage Examples](#usage-examples)
* [Tests](#tests)
* [Additional Information](#additional-information)
	* [Commands](#commands)
		* [all (default)](#all-default)
		* [custom](#custom)
		* [lib](#lib)
		* [clear](#clear)
* [License](#license)

## Dependencies
* [TermInfo](https://en.wikipedia.org/wiki/Terminfo) - **GNU** library and database that provides a low level abstraction over terminal escape string based API.

## Build

Library was tested using GNU G++ compiler with flag -std=c++17. So it is recommended to use C++ 17 or higher version of compiler. Compiling with another compilers might need code corrections.

`make` default command builds the tests and creates **test.exe** file in the root directory.

`make custom` builds the playground file location `test/mtest.cpp`, and creates **mtest.exe** file in the root directory of the library.

***Note:*** *default make command builds mocked object files, so if you want to build a real program, or an playground file, run `make clear` before compiling, so mocked object files are deleted.*

## Documentation

The library provides a couple of levels of abstraction depending on users needs. All of them are described in this section of the file.

### qt_term.h

This header provides a `termiq` namespace with some low (well, slightly higher) level API over **GNU terminfo** library to interact with the terminal.

>[!NOTE]
>By default if the command fails, the error is just silently ignored, if you want the error to be thrown, the `TERMIQ_THROW_ON_FAILED_COMMAND` macro must be defined.

#### void termiq::init_term([std::string terminal term])

Initially **init_term** should be called. This defines the set of terminal-dependent variables. It reads in the *terminfo* database, initializing the *terminfo* structures.

It accepts one *optional* parameter that corresponds to the terminal type that needs to be initialized. If the value is not provided, the environmental variable **TERM** is used.

This function throws a `std::logic_error` instance if the initialization did not work.

>[!IMPORTANT]
>Before using any other function from the library, **init_term** must be called.

#### bool termiq::get_flag(std::string name)

It requires one *string* parameter - the name of the capability you want to check, and returns the state of the *boolean* terminal capability, e.g.: if the automatic margins are on.

In case the capability wasn't found - returns `false`, otherwise returns `true` if the capability is on and `false` if it's off.

***Example:***

```c++
bool margins_on = termiq::get_flag("auto_right_margin");
std::count << margins_on << std::endl; // 1
```

>[!NOTE]
>*All **standard** available boolean capabilities can be found [here](https://man7.org/linux/man-pages/man5/terminfo.5.html) under the **"boolean capabilities"** section.*

#### int termiq::get_num(std::string name)

It requires one *string* parameter - the name of the capability, and returns the value of the *numeric* terminal capability e.g.: the number of rows or columns of the current terminal window.

In case the capability wasn't found return `-1`, otherwise returns the value of the capability.

***Example:***

```c++
int num_rows = termiq::get_num("lines");
std::cout << num_rows << std::endl; // 42
```

>[!NOTE]
>*All **standard** available numeric capabilities can be found [here](https://man7.org/linux/man-pages/man5/terminfo.5.html) under the **"numeric capabilities"** section.*

#### int termiq::run_str(std::string name[, int padding])

It accepts one required *string* parameter - the name of the capability that needs to be run, and one optional *int* parameter - padding, and runs a string terminal capability, e.g.: to clear the terminal screen.

Padding is needed mostly for older terminals to make sure the terminal receives enough time to make the required changes. Usually this parameter **should** be equal to the number of the lines that are being changed after the capability is run. The default value is `1`.

In case the capability doesn't exist in the terminal returns `-1`, otherwise returns `0`.

***Example:***

```c++
termiq::run_str("clear"); // Clears the terminal screen.
```

>[!NOTE]
>*All **standard** available string capabilities can be found [here](https://man7.org/linux/man-pages/man5/terminfo.5.html) under the **"string capabilities"** section.*

#### int termiq::run_str\<P...\>(std::string name, int padding, P... args)

Accepts the same parameters as above one but *padding* is required here, plus receives additional parameters that are required to be passed with a **parametric** string capability. It runs the chosen parametric string capability, e.g.: erase some amount of characters from the current cursor position.

In case the capability doesn't exist in the terminal returns `-1`, otherwise returns `0`.

***Example:***

```c++
termiq::run_str("ech", 1 /* padding */, 10); // erases 10 characters.
```

>[!NOTE]
>*All **standard** available string capabilities can be found [here](https://man7.org/linux/man-pages/man5/terminfo.5.html) under the **"string capabilities"** section.*

### qt_term_adv.h

This header provides a `termiq` namespace with some more advanced functions that can be used in combination with the functions from the previous section.

#### class termiq::exception

The header defines the *exception* class that is being thrown in case of any function from this header has failed.

termiq::exception extends **std::logic_error**.

#### int termiq::get_cols()

Doesn't accept any parameters, and returns the number of the **columns** in the current terminal window.

#### int termiq::get_rows()

Doesn't accept any parameters, and returns the number of the **rows** in the current terminal window.

#### void termiq::move(int r, int c)

Requires 2 *int* parameters to be passed, and moves terminal cursor to the **r** row and **c** column.

#### void termiq::move_left([int steps])

Accepts one optional *int* parameter **steps**, and moves the cursor **steps** characters left.

By default it moves the cursor `1` character left.

#### void termiq::move_right([int steps])

Accepts one optional *int* parameter **steps**, and moves the cursor **steps** characters right.

By default it moves the cursor `1` character right.

#### void termiq::move_up([int steps])

Accepts one optional *int* parameter **steps**, and moves the cursor **steps** characters up.

By default it moves the cursor `1` character up.

#### void termiq::move_down([int steps])

Accepts one optional *int* parameter **steps**, and moves the cursor **steps** characters down.

By default it moves the cursor `1` character down.

#### void termiq::save_cursor_position()

Save the current position of the cursor. The position can be restored using `void termiq::restore_cursor_position()`.

>[!NOTE]
>There is no stack of saved cursor positions you can restore using `restore_cursor_position` function call. Every next `save_cursor_position` call overrides the previous saved position.

#### void termiq::restore_cursor_position()

Restores the saved cursor position by calling `void termiq::save_cursor_position()` function.

#### void termiq::cursor_hidden()

Makes the cursor to be invisible on the terminal screen.

#### void termiq::cursor_default()

Returns the cursor back to it's default visibility state.

#### int termiq::get_max_colors()

Returns the number of colors available in the terminal.

#### int termiq::get_max_pairs()

Returns the number of color pairs available in the terminal.

#### void termiq::define_color(int id, int r, int g, int b)

Requires **4** *int* parameters to be passed. The first parameter **id** is the color *identifier* that needs to be initialized, and 3 next parameters represent the intensity of the 3 color channels (red, green, blue).

The **id** parameters *must* be in range of `0` to `max_colors-1`, where `max_colors` can be retrieved using `termiq::get_max_colors` function call.

The **r**, **g** and **b** parameters should be sent in the range of `0` to `1000` where `0` means zero color channel intense, and `1000` means maximal color channels intense.

#### void termiq::define_pair(int id, int r, int g, int b, int br, int bg, int bb)

Same as above with the difference that **define_pair** accepts additional 3 *int* parameters representing background color.

#### void termiq::undefine_colors()

Uninitializes all initialized color or color pairs identifiers, returning them back to default colors.

#### void termiq::set_foreground_color(int id)

Requires one *int* parameter - **id** and sets the chosen color **ID** as the foreground color for the next text that is going to be written into the terminal.

#### void termiq::set_background_color(int id)

Requires one *int* parameter - **id** and sets the chosen color **ID** as the background color for the next text that is going to be written into the terminal.

#### void termiq::set_pair_color(int id)

Requires one *int* parameter - **id** and sets the chosen color pairs **ID** as the foreground and background color for the next text that is going to be written into the terminal.

#### void termiq::set_attrs(bool bold, bool dim, bool reverse, bool underline)
Requires **4** *bool* parameters to be passed, representing the attribute states of the text to be printed:

* **bold** - makes text to be bold
* **dim** - makes text color to be dimmed.
* **reverse** - reverses text foreground and background colors.
* **underline** - Adds a straight line under the text.

#### void termiq::reset_attrs()

Resets text attribute.

>[!NOTE]
>This function resets not only attributes that can be set with `termiq::set_attrs` function, but also **foreground** and **background** colors, **italic** and **alternate characters** modes.

#### void termiq::set_italic_on()

Turns on **italic** text mode.

#### void termiq::set_italic_off()

Turns off **italic** text mode.

#### void termiq::reset_colors()

Resets **background** and **foreground** colors to default.

>[!NOTE]
>There are no distinguished **standard** capabilities to reset just background or just foreground colors.

#### void termiq::alternate_chars_on()

Sets terminal into alternate characters mode, which allows to print some of the graphic characters like border lines using regular ASCII characters from the letters range.

More info at the [Alternate characters](#) section.

#### void termiq::alternate_chars_off()

Returns terminal into the normal characters mode.

#### void termiq::enter_automatic_margins()

Sets terminal into the mode, where after cursor reaches the edge of the screen, the cursor automatically jumps into the beginning of the next line.

>[!NOTE]
>It is usually the default terminal behavior.

#### void termiq::exit_automatic_margins()

Sets terminal into the mode, where after cursor reaches the edge of the screen, the cursor is being stopped at the last column of the current row, and the next character that is printed into the screen will overwrite the current one.

#### void termiq::enter_alternate_buffer()

Makes terminals use an alternate buffer which allows to modify the terminal content in any way, without losing the original content of the terminal.

This alternate buffer usually doesn't have an ability to scroll, so if this functionality is needed, it has to be custom implemented.

Every time the alternate buffer is used, its content is cleared.

#### void termiq::exit_alternate_buffer()

Exits the alternate buffer, returning to the original one.

#### void termiq::clear()

Clears the whole content of the terminal.

#### void termiq::clear_before()

Clears the current row of the terminal *before* the cursor position.

#### void termiq::clear_after()

Clears the current row of the terminal *after* the cursor position.

#### void termiq::clear_rest()

Clears the terminal screen *after* the current position of the cursor.

#### namespace termiq::alt_chars

This namespace contains graphic characters that can be used to draw lines and table borders in a [alternate character](#) mode.

It contains next **constexpr** *char* properties:

* **termiq::alt_chars::C_H** - represents a horizontal line, e.g.: `─`.
* **termiq::alt_chars::C_V** - represents a vertical line, e.g.: `│`.
* **termiq::alt_chars::C_X** - represents a horizontal-vertical lines cross, e.g.: `┼`.
* **termiq::alt_chars::C_HT** - represents a horizontal line with an additional perpendicular line touching horizontal line from top, e.g.: `┴`.
* **termiq::alt_chars::C_HB** - represents a horizontal line with an additional perpendicular line touching horizontal line from bottom, e.g.: `┬`.
* **termiq::alt_chars::C_VL** - represents a vertical line with an additional perpendicular line touching vertical line from right, e.g.: `├`.
* **termiq::alt_chars::C_VR** - represents a vertical line with an additional perpendicular line touching vertical line from left, e.g.: `┤`.
* **termiq::alt_chars::C_TL** - represents the 2 perpendicular lines coming from top and left and touching each other in the middle, e.g.: `┘`.
* **termiq::alt_chars::C_TR** - represents the 2 perpendicular lines coming from top and right and touching each other in the middle, e.g.: `└`.
* **termiq::alt_chars::C_BL** - represents the 2 perpendicular lines coming from bottom and left and touching each other in the middle, e.g.: `┐`.
* **termiq::alt_chars::C_BR** - represents the 2 perpendicular lines coming from bottom and right and touching each other in the middle, e.g.: `┌`.

Using these special characters, you can draw rectangles or grids.

***Example:***

```c++
termiq::alternate_chars_on();
std::cout
        // First line
        << termiq::alt_chars::C_BR
        << termiq::alt_chars::C_H
        << termiq::alt_chars::C_HB
        << termiq::alt_chars::C_H
        << termiq::alt_chars::C_BL
        << '\n';
        // Second line
        << termiq::alt_chars::C_V
        << ' '
        << termiq::alt_chars::C_V
        << ' '
        << termiq::alt_chars::C_V
        << '\n'
        // Third line
        << termiq::alt_chars::C_HR
        << termiq::alt_chars::C_H
        << termiq::alt_chars::C_X
        << termiq::alt_chars::C_H
        << termiq::alt_chars::C_HL
        << '\n'
        // Fourth line
        << termiq::alt_chars::C_V
        << ' '
        << termiq::alt_chars::C_V
        << ' '
        << termiq::alt_chars::C_V
        << '\n'
        // Fifths line
        << termiq::alt_chars::C_TR
        << termiq::alt_chars::C_H
        << termiq::alt_chars::C_HT
        << termiq::alt_chars::C_H
        << termiq::alt_chars::C_TL
        << std::endl;
termiq::alternate_chars_off();

// Result:
/*
┌─┬─┐
│ │ │
├─┼─┤
│ │ │
└─┴─┘
*/
```

### tq_term_style.h

Represents the **high level** API to manipulate text *colors* and *attributes*.

>[!IMPORTANT]
>If **termiq::style** API is used, the other (e.g. from tq_term_adv.h, or low level terminfo) API must not be used, all of these APIs share the same low level color API, and them might overwrite each other.

#### struct termiq::style::Color

Defines color structure that is used for defining *background* or *foreground* color using **termiq::style::\*** functions.

It override both `==` and `!=` operators so the colors can be compared with each other.

It also defines a `static const` **UNDEFINED** member that corresponds to the undefined or "transparent" *(default)* color.

The structure declares `r`, `g` and `b` *int* members. for *red*, *green*, and *blue* color channels.

***Example:***

```c++
termiq::style::Color red{1000,0,0};
termiq::style::Color white{1000,1000,1000};
termiq::style::Color black{0,0,0};
auto transparent = termiq::style::Color::UNDEFINED;
```

#### void termiq::style::foreground(Color color)

Accepts one *termiq::style::Color* **color** parameter. Sets terminal text foreground color to be the **color**.

If `color == termiq::style::Color::UNDEFINED` is `true`, terminal foreground color is set to the default color.

#### void termiq::style::background(Color color)

Accepts one *termiq::style::Color* **color** parameter. Sets terminal text background color to be the **color**.

If `color == termiq::style::Color::UNDEFINED` is `true`, terminal background color is set to the default color.

#### void termiq::style::bold(bool state)

Accepts one *bool* parameter - **state**.

Makes terminal text to be **bold** in case of **state** is `true`, and normal otherwise.

#### void termiq::style::italic(bool state)

Accepts one *bool* parameters - **state**.

Makes terminal text to be *italic* in case of **state** is `true`, and normal otherwise.

#### void termiq::style::underline(bool state)

Accepts one *bool* parameters - **state**.

Makes terminal text to be <ins>underline</ins> in case of **state** is `true`

#### void termiq::style::dim(bool state)

Accepts one *bool* parameters - **state**.

Makes terminal text dim *(text with less intense color)* in case of **state** is `true` and normal otherwise.

#### void termiq::style::inverse(bool state)

Accepts one *bool* parameters - **state**.

Makes terminal text to inverse its *foreground* and *background* colors in case of **state** is `true`.

#### void termiq::style::special_chars(bool state)

Accepts one *bool* parameters - **state**.

Turns on *alternate characters* mode in case of **state** is `true`, and turns it off otherwise.

#### void termiq::style::clear_colors()

Returns both *foreground* and *background* colors to their default values.

#### void termiq::style::clear_styles([bool also_clear_colors])

Returns all text attributes to their default values *(basically makes text normal)*, but doesn't return colors to their default values by default.

Accepts one *bool* parameters **also_clear_color** which, when `true` also returns *both* colors to their default values.

#### void termiq::style::clear()

Basically equivalent to `termiq::style::clear_styles(true)`.

#### void termiq::style::reset()

Returns all define color IDs to their default values.

#### bool termiq::style::is_color_defined(const Color &color)

Accepts the *reference* to a *const Color*, and returns true if the passed **color** parameter equals to `termiq::style::Color::UNDEFINED`.

#### void termiq::style::style(Color fg, Color bg, bool bold, bool italic, bool dim, bool underline, bool inverse)

Sets both *foreground* and *background* together with **5** text attributes: *bold*, *italic*, *dim*, *underline* and *inverse*.

>[!NOTE]
>This function is the most **performant** of all the functions in `termiq::style` namespace, meaning it takes the less escape string characters to convert you from one *style* to another.

### termiq::canvas::Canvas\<CC\>

*(declared in tq_term_canvas.h)*

Where **CC** stands for *char cell* type, and declares the `Type` and `State` of the internal canvas matrix ***"pixel"*** .

Another high level abstraction for **text style** and **text position** manipulation.

It defines the **termiq::canvas** namespace with a bunch of helper classes and function.

It is *DefaultConstructable* and *virtually destructible*.

It is meant for users to extend the class providing additional functionality, or changing the current canvas behavior, as the class provides a bunch of *virtual protected* methods for this purpose.

#### termiq::canvas::Canvas\<CC\>::Canvas(unsigned int h, unsigned int w[, unsigned int r, unsigned int])

Accepts:
* *unsigned int* **h** - height of the canvas
* *unsigned int* **w** - width of the canvas.
* *(Optional)* *unsigned int* **r** - relative offset of the top side of the screen.
* *(Optional)* *unsigned int* **c** - relative offset of the left side of the screen.

Constructs an instance of *Canvas* with provided parameters.

#### unsigned int termiq::canvas::Canvas\<CC\>::get_width()

Returns the current width of the canvas.

#### unsigned int termiq::canvas::Canvas\<CC\>::get_height()

Returns the current height of the canvas.

#### unsigned int termiq::canvas::Canvas\<CC\>::get_row()

Returns the current row offset of the canvas.

#### unsigned int termiq::canvas::Canvas\<CC\>::get_col()

Returns the current column offset of the canvas.

#### const std::vector\<std::vector\<CC\>\>& termiq::canvas::Canvas\<CC\>::get_canvas()

Returns internal matrix of cells *(pixels)*.

#### void termiq::canvas::Canvas\<CC\>::resize(unsigned int height, unsigned int width)

Resizes the canvas to new **height** and **width**, after internal properties are set, it also calls *virtual protected* `resized()` method, which by default resizes the internal canvas.

#### void termiq::canvas::Canvas\<CC\>::move(unsigned int row, unsigned int col)

Overrides canvas' offset row and offset column properties, and calls *virtual protected* `moved()` method.

#### void termiq::canvas::Canvas\<CC\>::draw(unsigned int row, unsigned int col, const CanvasPieces\<CC\>&& pieces)

Draws canvas **pieces** into internal canvas matrix, and calls *virtual protected* `drawn()` afterwards.

#### void termiq::canvas::Canvas\<CC\>::draw(unsigned int row, unsigned int col, const CanvasPieces\<CC\>& pieces)

The same as above.

#### void termiq::canvas::Canvas\<CC\>::paint()

Paints internal canvas into the terminal taking into consideration *offset row* and *offset column* properties.

#### virtual protected void termiq::canvas::Canvas\<CC\>::moved()

This is a *virtual protected* method that is called after `move` function is called, It can be *overridden* and it allows users to provide their own additional checks, structure changes, or optimization work.

#### virtual protected void termiq::canvas::Canvas\<CC\>::resized()

This is a *virtual protected* method that is called after `resize` function is called, It can be *overridden* and it allows users to provide their own additional checks, structure changes, or optimization work.

#### virtual protected void termiq::canvas::Canvas\<CC\>::drawn()

This is a *virtual protected* method that is called after either of `draw` functions is called, It can be *overridden* and it allows users to provide their own additional checks, structure changes, or optimization work.

#### virtual protected void termiq::canvas::Canvas\<CC\>::paint_row(size_t index)

This function is responsible for printing the internal matrix object into the terminal.

It accepts one *size_t* **ind** parameters which indicates the index of the row that needs to be printed.

This function can be overridden to provide better than default printing behavior, or provide some optimization.

#### virtual protected void termiq::canvas::Canvas\<CC\>::move_cursor(unsigned int row, unsigned int col)

This method is used in the default implementation of the `paint_row` function, and is called whenever terminal cursor has to be moved.

#### virtual protected void termiq::canvas::Canvas\<CC\>::set_paint_state(CharState* state)

This method is used in the default implementation of the `paint_row` function, and is called whenever the text style needs to be changed.

### concept termiq::canvas::Type

Declares restrictions for the type that can be passed to the **CharCell**'s `CT` template argument.

It requires the type to be:
* **default constructible**
* have **constructor(termiq::canvas::symbol_char_type\* begin, termiq::canvas::symbol_char_type\* end)**
* have **constructor(std::contagious_iterator begin, std::contagious_iterator end)**
* have **termiq::canvas::symbol_char_type\* begin()** method that returns the pointer to the first byte of the stored symbol.
* have **termiq::canvas::symbol_char_type\* end()** method that returns the end pointer *(the pointer that points directly after the next to the last symbol's byte)* of the stored symbol.

### concept termiq::canvas::State

Declares restrictions for the type that can be passed to the **CharCell**'s `CS` template argument.

It requires the type to be:
* **default constructible**
* have **termiq::style::Color foreground()** method that returns symbol's foreground color.
* have **termiq::style::Color boreground()** method that returns symbol's background color.
* have **bool bold()** method that return `true` if the symbol is suppose to be bold.
* have **bool italic()** method that return `true` if the symbol is suppose to be italic.
* have **bool dim()** method that return `true` if the symbol is suppose to be dim.
* have **bool inverse()** method that return `true` if the symbol is suppose to be inversed.
* have **bool underline()** method that return `true` if the symbol is suppose to be underlined.
* have **bool special()** method that return `true` if the symbol is suppose to be displayed as a special character (default line drawing character).
* have **void foreground(termiq::style::Color)** method that sets new foreground color.
* have **void background(termiq::style::Color)** method that sets new background color.
* have **void bold(bool)** method that sets the bold state of the character.
* have **void italic(bool)** method that sets the italic state of the character.
* have **void dim(bool)** method that sets the dim state of the character.
* have **void inverse(bool)** method that sets the inverse state of the character.
* have **void underline(bool)** method that sets the underline state of the character.
* have **void special(bool)** method that sets the special character state of the character.
* comparable with `==` and `!=` operators.

### struct termiq::canvas::CharState

*(declared in tq_term_canvas_utils.h)*

Defines the state or the *style* of the canvas cell *(pixel)*.
It defines the next public properties:

* *termiq::style::Color* **foreground** - foreground color of the *"pixel"*. By default it equals to `termiq::style::Color::UNDEFINED`.
* *termiq::style::Color* **background** - background color of the *"pixel"*. By default it equals to `termiq::style::Color::UNDEFINED`
* *bool* **bold** - indicates if the *pixel* text character has to be in bold. By default is `false`.
* *bool* **italic** - indicates if the *pixel* text character has to be in italic. By default is `false`.
* *bool* **dim** - indicates if the *pixel* color has to be dimmed. By default is `false`.
* *bool* **inverse** - indicates whether the background and foreground colors of the *pixel* have to be swapped. By default is `fasle`.
* *bool* **undeline** - indicates whether the *pixel* text has to be underlined. By default is `false`.
* *bool* **special** - indicates whether the *pixel* text has to be displayed in *alternate characters* range. By default is `false`.

This struct also overrides `==` and `!=` operators so two **CharState** objects can be compared.

### struct termiq::canvas::CharCell\<CT, CS\>

*(declared in tq_term_canvas_utils.h)*

**CT** stands for **char type** and represents the type that is used to store the character information. `CT` must obey `termiq::canvas::Type` concept. 

There are 2 already implemented types that can be passed as **CT** template argument:
* **termiq::canvas::CharType** - store a single **char** symbol.
* **termiq::canvas::WCharType** - store a sequence of **char** symbols encoded in *UTF-8* encoding.

**CS** stands for **char state** and represents the type that is used to store the character's style information.

There is 1 already implemented type that can be passed as **CS** template argument: **termiq::canvas::CharState**.

The structure declares next attributes:

* *CT* **symbol** - character contained in the CharCell instance. By default is represents `' '` (empty space).
* *std::shared_ptr\<CharState\>* **state** - a style of the CharCell instance. By default is `nullptr`.

It also overrides `==` and `!=` operators, so 2 *CharCell* instances can be compared.

The struct also defines a *static const CharCell\<CT\>* **TRANSPARENT** property that can be used as a placeholder for cells that need to be transparent.

#### bool termiq::canvas::CharCell\<CT\>::is_transparent()

Returns true if the cell is transparent.

### struct termiq::canvas::CanvasPiece\<CC\>

*(declared in tq_term_canvas_utils.h)*

**CC** stands to **char cell** and represents the type that is used to store the char cell information in the internal canvas matrix.

The struct declares next properties:

* *const std::vector\<std::vector\<CC\>\>* **canvas** - the piece of canvas that needs to be drawn.
* *const unsigned int* **rows** - the number of rows in the canvas.
* *const unsigned int* **cols** - the number of columns in the canvas.
* *const int* **offset_rows** - the number of rows that need to be skipped form the original drawing point. Default is `0`.
* *const int* **offset_cols** - the number of columns that need to be skipped from the original drawing point. Default is `0`.

### struct termiq::canvas::CanvasPieces\<CC\>

*(declared in tq_term_canvas_utils.h)*

**CC** stands to **char cell** and represents the type that is used to store the char cell information in the internal canvas matrix.

Basically a collection of *CanvasPiece* instances. Declares just one property:

* *const std::vector\<CanvasPiece\<CC\>\>* **pieces** - representing the collection of drawable canvases.

>[!NOTE]
>Usually you should avoid creating *CanvasPiece* or *CanvasPieces* instances manually, they are produced at part of the build process of canvas related instance that are described at the bottom.

### class termiq::canvas::Content\<CC\>

*(declared in tq_term_canvas_utils.h)*

**CC** represents a canvas cell type *(more details above)*.

The class is *pure virtual*, *DefaultConstructable* and *virtually dewstructable* and is meant to be used as a **interface**, to be extended by other specific canvas content classes.

#### virtual CanvasPieces\<CC\> termiq::canvas::Content\<CC\>::build() = 0

*Pure virtual* method is supposed to return *termiq::canvas::CanvasPieces\<CC\>* instnace - the canvas representation of the **content** when called.

#### virtual unsigned int termiq::canvas::Content\<CC\>::get_width() = 0

*Pure virtual* method that is supposed to return the **width** of the content.

#### virtual unsigned int termiq::canvas::Content\<CC\>::get_height() = 0

*Pure virtual* method that is supposed to return the **height** of the content.

#### virtual unsigned int termiq::canvas::Content\<CC\>::min_width() = 0

*Pure virtual* method that is supposed to return the **minimal content width** of the content.

#### virtual unsigned int termiq::canvas::Content\<CC\>::min_height() = 0

*Pure virtual* method that is supposed to return the **minimal content height** of the content.

#### virtual void termiq::canvas::Content\<CC\>::built(CanvasPieces\<CC\> &pieces)

The function that is called after the **pieces** instance was built, but before returning it from the `build()` method call.

### class Nothing : public Content\<CC\>

*(declared in tq_term_canvas_utils.h)*

**CC** represents a canvas cell type *(more details above)*.

Represents the content that is not displayed and does not occupy any space.

All **\*_width** and **\*_height** methods return `0`, and **build()** function call returns an empty *CanvasPieces* instance.

### class termiq::canvas::Text : public Content\<CC\>

*(declared in tq_term_canvas_text.h)*

**CC** represents a canvas cell type *(more details above)*.

Canvas content class that allows to build and draw text content on a canvas.

The class declares **using char_type = typename CC::char_type** that is used for character related types.

#### termiq::canvas::Text\<CC\>::Text()

Default constructor represents empty text.

#### termiq::canvas::Text\<CC\>::Text(std::span\<char_type\> line)

Accepts **std::span** of char type as one line of characters.

>[!IMPORTANT]
>The characters are going to be printed as is, meaning it's on your responsibility to not send line breaks, line ending characters or any other characters that modify cursor behavior.

#### termiq::canvas::Text\<CC\>::Text(std::span\<std::vector\<char_type\>\> multiline)

Accepts multiple lines of characters, and builds a multi line content.

>[!IMPORTANT]
>The characters are going to be printed as is, meaning it's on your responsibility to not send line breaks, line ending characters or any other characters that modify cursor behavior.

>[!NOTE]
>There are 2 helper functions to help building the multiline content: `std::vector<std::vector<termiq::canvas::CharType>>` and `std::vector<std::vector<termiq::canvas::WCharType>>`:
>* **std::vector\<std::vector\<termiq::canvas::CharType\>\> termiq::canvas::build_char_lines\<T\>(std::basic_string_view\<T\> str)**
>* **std::vector\<std::vector\<termiq::canvas::CharType\>\> termiq::canvas::build_wchar_lines\<T\>(std::basic_string_view\<T\> str)**
>Where `T` is `char` for **build_char_lines** by default, and `char16_t` for **build_wchar_lines** by default.

#### termiq::canvas::Text\<CC\>::Text(std::vector\<char_type\>&& line)

Same as above, but moves the line instead of copying.

#### termiq::canvas::Text\<CC\>::Text(std::vector\<std::vector\<char_type\>\>&& multiline)

Same as above, but moves the content instead of copying.

####  void termiq::canvas::Text\<CC\>::set_foreground_color(termiq::style::Color &&color)

Defines foreground color of the text.

#### void termiq::canvas::Text\<CC\>::set_background_color(termiq::style::Color &&color);

Defines background color of the cell content.

#### void termiq::canvas::Text\<CC\>::set_bold()

Makes the text style bold.

#### void termiq::canvas::Text\<CC\>::set_italic()

Makes the text style italic.

#### void termiq::canvas::Text\<CC\>::set_dim()

Makes the text color dimmed.

#### void termiq::canvas::Text\<CC\>::set_underline()

Makes the text underline.

#### void termiq::canvas::Text\<CC\>::set_inverse()

Inverses the foreground and background colors of the text.

#### void termiq::canvas::Text\<CC\>::set_width()

Defines the content instance width.

#### void termiq::canvas::Text\<CC\>::set_height()

Defines the content instance height.

#### const std::vector\<std::vector\<char_type\>\>& termiq::canvas::Text\<CC\>::get_lines()

Returns the *calculated* text content in the way that is going to be drawn to the canvas.

___

The class also provides a bunch of **protected** and **virtual protected** methods that can be used or overridden in order to provide additional functionality to the classes extending the **termiq::canvas::Text** class.

#### virtual void termiq::canvas::Text\<CC\>::calculate_lines()

Used to calculate the text lines and assign the result to **\_lines** class **protected** property.

#### virtual unsigned int termiq::canvas::Text\<CC\>::get_calc_width()

Used to calculate the text contet width.

#### virtual unsigned int termiq::canvas::Text\<CC\>::get_calc_height()

Used to calculated the text content height.

#### void termiq::canvas::Text\<CC\>::invalidate_lines()

Should be called when the internal properties that define text position of the class instance were changed, and the new text position needs to be calculated, e.g. when the **width** or **height** properties were changed.

####  void termiq::canvas::Text\<CC\>::lazy_calculate_lines()

Should be called when the text lines need to be calculated.

>[!NOTE]
>It is preferred to use this method instead of `calculate_lines()`, as it provides the caching mechanism that avoids recalculating lines if the **text position dependent** properties *(the ones that call **invalidate_lines** after being executed)* were not changed.

#### bool termiq::canvas::Text\<CC\>::is_valid_lines()

Returns `true` if the text content lines are valid and `false` otherwise.

___

*More protected properties can be checked directly from the code declaration.*

***Example:***

```c++
termiq::canvas::Text<termiq::canvas::CharCell<
	termiq::canvas::CharType,
	termiq::canvas::CharState
>> tb(termiq::canvas::build_char_lines("Hello Wordl!"));

tb.set_bold();
tb.set_width(6);

auto pieces = tb.build()

// It is going to be drawn in two lines as the width is limited to 6 characters.
/*
Hello
World!
*/
```

### class termiq::canvas::Grid\<CC\> : public Content\<CC\>

*(declared in tq_term_canvas_grid.h)*

**CC** represents a canvas cell type *(more details above)*.

Canvas content class that allows to build and drawn grids on a canvas.

The class declares **using char_type = typename CC::char_type** that is used for character related types.

#### termiq::canvas::Grid\<CC\>::Grid([unsigned int rows, unsigned int cols])

Constructor that accepts the number of **rows** and **columns** in a grid. Both parameters are optional, and equals to `1` by default.

#### void termiq::canvas::Grid\<CC\>::set_background_color(termiq::style::Color color)

Defines grid background color.

#### void termiq::canvas::Grid\<CC\>::set_border_foreground_color(termiq::style::Color color)

Defines grid border foreground color.

#### void termiq::canvas::Grid\<CC\>::set_border_background_color(termiq::style::Color color)

Defines grid border background color.

#### void termiq::canvas::Grid\<CC\>::set_width(unsigned int width)

Defines grid width.

#### void termiq::canvas::Grid\<CC\>::set_height(unsigned int height)

Defines grid height.

#### void termiq::canvas::Grid\<CC\>::set_border_type(termiq::canvas::BorderType type)

Sets the border of the grid. There are multiple options available for border type.

*enum class* **BorderType** is declared in the same header file *(tq_term_canvas_grid.h)*, and contains next options:

* **NONE** - displays *no* border, meaning the border doesn't even take any space *(see examples below)*.
* **SINGLE_ASCII** - default option. Regular single lined border. The border is built from the *ASCII* characters, so use of this type is compatible with the *char* type.
* **SINGLE** - Regular single lined border, but built from *wchar_t* wide characters. Use of this border type is not compatible with regular *char* type.
* **DOUBLE** - Double lined grid, built from *wchar_t* characters.
* **BOLD** - Single lined bold border type. Built from *wchar_t* characters.
* **ROUND** - Single lined border with rounded corners. Built from *wchar_t* characters.
* **INVISIBLE** - the border is not visible, but still occupies the space.

>[!NOTE]
>Use of this border types built from **wchar_t** characters are not compatible with regular *char* type.

#### void termiq::canvas::Grid\<CC\>::select_cell(size_t r, size_t c)

Selects the cell for further modifications described below. Numeration of the **rows** and **columns** start from `0`.

#### void termiq::canvas::Grid\<CC\>::select_cell(size_t c)

Same as above, but **row** is always `0`.

#### void termiq::canvas::Grid\<CC\>::set_cell_background_color(termiq::style::Color color)

Defines background color for the **selected cell**. Color provided for a distinguished cell override the color provided for the whole grid.

#### void termiq::canvas::Grid\<CC\>::set_cell_width(unsigned int width)

Defines width property for the **selected cell**.

#### void termiq::canvas::Grid\<CC\>::set_cell_height(unsigned int height)

Defines height property for the **selected cell**.

>[!NOTE]
>This is a primitive grid class where **width** and **height** properties have to be provided for each cell separately, otherwise the grid **width/height** will be distributed equally between the cells with not defined **width/height** properties.
>If none individual **width/height** nor global grid **width/height** properties were provided, the grid cells sizes will be equal to `0`.

#### void termiq::canvas::Grid\<CC\>::set_cell_content(Content\<CC\>* content)

Provides **selected cell** with the content that will be displayed inside the cell.

***Example:***

```c++
using CC = termiq::canvas::CharCell<termiq::canvas::WCharType,termiq::canvas::CharState>;
termiq::canvas::Grid<CC> grid(1,2);
termiq::canvas::Text<CC> txt1(termiq::canvas::build_wchar_lines(u"Hello"));
termiq::canvas::Text<CC> txt2(termiq::canvas::build_wchar_lines(u"World!"));

grid.set_cell_type(termiq::canvas::GridType::DOUBLE);

grid.select_cell(0,0);
grid.set_cell_width(5);
grid.set_cell_height(1);
grid.set_cell_content(&txt1);

grid.select_cell(1); // [0;1]
grid.set_cell_width(10);
grid.set_cell_height(2);
grid.set_cell_content(&txt2);

auto pieces = grid.build();

// This code is going to build canvas pieces that will draw into something similar to:
/*
╔═════╦══════════╗
║Hello║World!    ║
║     ║          ║
╚═════╩══════════╝
*/
```

___

Besides public methods, the class also provide some *protected* and *virtual protected* methods that can be overwritten and additional functionality provided to the grid.

#### virtual protected std::vector\<unsigned int\> termiq::canvas::Grid\<CC\>::calculate_column_sizes()

Calculates and returns column sizes.

#### virtual protected std::vector\<unsigned int\> termiq::canvas::Grid\<CC\>::calculate_row_sizes()

Calculates and returns row sizes.

#### virtual protected void termiq::canvas::Grid\<CC\>::calc()

Calculates **columns** and **rows** sizes and assigns them to *protected* **\_col\_widths** and **\_row\_heights** properties respectfully.

#### void termiq::canvas::Grid\<CC\>::invalidate_calc()

Should be called when the internal properties that define grid column or row sizes of the class instance were changed, and the new column or row sizes need to be calculated, e.g. when the **width** or **height** properties were changed.

####  void termiq::canvas::Text\<CC\>::lazy_calc()

Should be called when the grid column or row sizes need to be calculated.

>[!NOTE]
>It is preferred to use this method instead of `calc()`, as it provides the caching mechanism that avoids recalculating sizes if the **sizes dependent** properties *(the ones that call **invalidate_calc** after being executed)* were not changed.

#### bool termiq::canvas::Text\<CC\>::is_valid_calc()

Returns `true` if the grid sizes are valid and `false` otherwise.

___

More protected properties you can find in the class declaration fine *(tq_term_canvas_grid.h)*.

### class termiq::canvas::Flexible

*(declared in tq_term_canvas_flexible.h)*

An abstract *(pure virtual)* interface that declares protected functions essential for making canvas content to be *flexible*.

See useful examples in **FlexGrid** section of the documentation.

#### virtual protected void termiq::canvas::Flexible::suggest_width(unsigned int width) = 0

*(pure virtual)* protected function that is called from other canvas content instances, and *suggests* the width that needs to be applied to the content instance before the calculation process.

It is for users to decide if they want to respect the suggested width, in order to make the content **flexible**.

#### virtual void termiq::canvas::Flexible::suggest_height(unsigned int height) = 0

*(pure virtual)* protected function that is called from other canvas content instances, and *suggests* the height that needs to be applied to the content instance before the calculation process.

It is for users to decide if they want to respect the suggested height, in order to make the content **flexible**.

### class class termiq::canvas::FlexText\<CC\> : public Text\<CC\>, public Flexible

Text content class that respects **Flexible** interface.

### class termiq::canvas::FlexGrid\<CC\> : public Grid\<CC\>, public Flexible

Grid content class that respects **Flexible** interface and influences its content with **Flexible** API.

***Example:***

```c++
using CC = termiq::canvas::CharCell<termiq::canvas::WCharType, termiq::canvas::CharState>;
termiq::canvas::FlexGrid<CC> grid(1,2);
termiq::canvas::FlexText<CC> txt1(termiq::canvas::build_wchar_lines(L"Hello"));
termiq::canvas::FlexText<CC> txt2(termiq::canvas::build_wchar_lines(L"Hello world!"));

grid.set_cell_type(termiq::canvas::GridType::SINGLE);
grid.set_width(12); // The width is too small to fit everything in one line.

grid.select_cell(0,0);
grid.set_cell_content(&txt1);

grid.select_cell(1); // [0;1]
grid.set_cell_content(&txt2);

auto pieces = grid.build();

// The result is going to be similar to:
/*
┌───┬──────┐
│Hel│Hello │
│lo │World!│
└───┴──────┘
*/
// Notice how the cell sizes are choosen automatically with the second cell bigger as it has more content.
```

## Usage Examples

```c++
#include "tq_term_adv.h"
#include "tq_term_canvas.h"
#include "tq_term_canvas_flextext.h"
#include "tq_term_canvas_flexgrid.h"

// Initializes structures to work with terminal.
termiq::init_term();

// Get current terminal window row and column sizes.
const int rows = termiq::get_rows();
const int cols = termiq::get_cols();

using CC = termiq::canvas::CharCell<termiq::canvas::WCharType, termiq::canvas::CharState>;
using BText = termiq::canvas::FlexText<CC>;
using BGrid = termiq::canvas::FlexGrid<CC>;

termiq::canvas::Canvas<CC> canvas(rows-1,cols,0,0);

// Defines text instances.
auto tb1 = BText(termiq::canvas::build_wchar_lines(L"woohoo"));
tb1.set_background_color({200, 200, 900});
auto tb2 = BText(termiq::canvas::build_wchar_lines(L"hello\nworld"));
auto tb3 = BText(termiq::canvas::build_wchar_lines(L"Awesome"));
auto tb4 = BText(termiq::canvas::build_wchar_lines(L"Nice"));

// Defines and configures grid instance.
auto gb1 = BGrid(1,2);
gb1.set_width(10);
gb1.set_border_type(termiq::canvas::BorderType::SINGLE);
gb1.select_cell(0,1);
gb1.set_cell_content(&tb3);
gb1.select_cell(0,0);
gb1.set_cell_content(&tb4);

// Defines another grid instance.
BGrid gb3 = BGrid(2,3);
gb3.set_width(30);
gb3.set_border_type(termiq::canvas::BorderType::SINGLE);
gb3.set_border_foreground_color({400,800,100});

// Assigsn big grid cells content.
gb3.select_cell(0,0);
gb3.set_cell_background_color({500, 500, 100});
gb3.set_cell_content(&tb1);

gb3.select_cell(1,1);
gb3.set_cell_content(&tb2);

gb3.select_cell(0,2);
gb3.set_cell_content(&gb1);

// Draw the grid.
canvas.draw(3, 5, gb3.build());

// Enter alternate buffer screen and
termiq::enter_alternate_buffer();

// Paint the canvas to the terminal.
canvas.paint();

// Wait for the input
getchar();

// Cleanup in the end.
termiq::exit_alternate_buffer();
termiq::style::reset();
```

Possible output:

![usage_example_output](https://github.com/Immortale-dev/termiq/blob/master/res/imgs/usage_example_1.png?raw=true)

## Tests

Tests are defined under the **test** folder that leaves in the root folder of the library.

By running `make all` *(default command)*, the **test/test.cpp** file is being compiled and the **test.exe** file is being generated in the root directory of the library. Run the file, to run library tests.

## Additional Information

**makefile** contains `custom` command, that compiles **test/mtest.cpp** file. This file can be used as a quick start sandbox file, to try things out. There are some neat example already provided in the file.

>[!IMPORTANT]
>when switching between `make` and `make custom`, don't forget to clean up generated object files, as regular unit tests that are generated by using default `make` command - generate *mocked* objects in order to test the library.
>The cleaning process can be done using the `make clear` command.

### Commands

#### all (default)

Builds unit test and creates a **test.exe** executable file in a library root diretory.

#### custom

Builds a sandbox *test/mtest.cpp* file, and creates a **mtest.exe** executable file in a library root directory.

#### lib

Builds library file and generate an archive **liboutput.a** file that can be linked to the final executable file.

#### clear

Cleanup objects, executable and archive files.

## License

MIT

Have fun :)
