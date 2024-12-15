#ifndef Dumper_h_seen
#define Dumper_h_seen
#include <string>
#include "StringList.h"
class Dumper {
public:
	char *getFile(std::string fname);
	void printFile(std::string fname);
	int makeFile(std::string path, std::string content);
	void penv();
	StringList* getNames();
	int addNames(std::vector<std::string> nl);
	int addNames(StringList *sl);
	int addNames(char **nl);
	StringList* getLies();
	std::vector<std::string> getLiesSTL();
private:
	std::vector<std::string> names;
	std::vector<std::string> lies;
};
#endif
