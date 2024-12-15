#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif
#include "Dumper.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
char * Dumper::getFile(std::string fname)
{
    std::ifstream f(fname);
    std::string s;

    if (f.is_open()) {
        std::getline(f, s, '\0');
        f.close();
	return strdup(s.c_str());
    } 
    return strdup("");
}

void Dumper::printFile(std::string fname)
{
	std::cout << fname << std::endl;
	std::string s = getFile(fname);
	std::cout << s;
}

void Dumper::penv() {
	int i = 0;
	std::ofstream f("edump");
	bool addnames = (names.size() == 0);
	while (environ[i] != NULL) {
		std::cout << environ[i] <<std::endl;
		f << environ[i] <<std::endl;
		if (addnames) {
			std::string e(environ[i]);
			std::string f = e.substr(0, e.find('='));
			names.push_back(f);
		}
		i++;
	}
	char dname[1024];
	getcwd(dname, sizeof(dname));
	std::cout << "DIR is " << dname << std::endl;
}

int Dumper::makeFile(std::string path, std::string s)
{
	std::ofstream f(path);
	f << s ;
	return 0;
}

StringList* Dumper::getNames()
{
	auto l = new StringList(names);
	return l;
}

StringList* Dumper::getLies()
{
	auto l = new StringList(getLiesSTL()); 
	return l;
}

int Dumper::addNames(std::vector<std::string> nl) {
	for (auto &i : nl)
		names.push_back(i);
	return 0;
}

int Dumper::addNames(char **nl) {
	size_t i = 0;
	while(nl && nl[i] != NULL) {
		names.push_back(nl[i]);
		i++;
	}
	return i;
}
int Dumper::addNames(StringList *sl)
{
	if (!sl)
		return 0;
	size_t i = 0, len = sl->size();
	
	while(i < len) {
		names.push_back(sl->get(i));
		i++;
	}
	return i;
}


std::vector<std::string> Dumper::getLiesSTL()
{
	std::vector<std::string> a = { "a=1", "b=2", "c=3" };
	return a;
}
