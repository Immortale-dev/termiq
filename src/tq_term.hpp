template<typename... P>
int termiq::run_str(std::string name, int padding, P... args) {
	char* str = tigetstr(name.data());
	if (!str) {
		return -1;
	}
	tputs(tiparm(str, args...), padding, putchar);
	return 0;
}
