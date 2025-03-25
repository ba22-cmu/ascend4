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

	printf("names count: ");
	auto sv = d.getNames();
	std::cout << sv->size() << std::endl;

	printf("lies count: ");
	auto sv2 = d.getLies();
	std::cout << sv2->size() << std::endl;

	d.addNames(sv2);
	printf("sv info 1: \n");
	sv = d.getNames();
	std::cout << sv->size() << std::endl;
	std::cout << sv->get(1) << std::endl;
	std::cout << sv->get(sv->size()-1) << std::endl;

	const char *json = "[\"j1\", \"j2\", \"j3\", 4]";
	sv->add_json(json);
	printf("sv info 2: \n");
	std::cout << sv->size() << std::endl;
	std::cout << sv->get(sv->size()-1) << std::endl;
	std::cout << sv->get(sv->size()-2) << std::endl;

	char *js = sv->json();
	printf("===\n%s\n", js);
	free(js);
	return 0;
}
