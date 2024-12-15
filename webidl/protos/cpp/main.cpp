#include "Dumper.h"
#include "ls.h"
#include <string>
#include <iostream>

int main(int argc, char **argv) {
	Dumper d;
#ifdef __EMSCRIPTEN__
	const char *f = "/models/foo";
#else
	const char *f = "./foo";
#endif
	std::string txt = d.getFile(f);
	std::cout << f << std::endl;
	std::cout << txt << std::endl;

	std::cout << "printfile" << std::endl;
	d.printFile(f);

	std::string txt2 = txt + txt;
	std::string f2 = std::string(f) + ".clone";
	int merr = d.makeFile(f2, txt2);
	printf("err: %d\n", merr);
#ifdef __EMSCRIPTEN__
	print_directory("/models");
#else
	print_directory(".");
#endif
	d.penv();
	d.printFile(f2);
#ifdef __EMSCRIPTEN__
	print_directory("/");
#else
	print_directory("..");
#endif

	auto sv = d.getNames();
	std::cout << sv->size() << std::endl;

	auto sv2 = d.getLies();
	std::cout << sv2->size() << std::endl;

	d.addNames(sv2);
	sv = d.getNames();
	std::cout << sv->size() << std::endl;
	std::cout << sv->get(1) << std::endl;
	std::cout << sv->get(sv->size()-1) << std::endl;

	return 0;
}
