#include "StringList.h"
#include <cstring>

//StringList::StringList(std::vector<std::string> v){}

int StringList::add(char **a) {
	int i = 0;
	while (a && a[i] != NULL) {
		sv.push_back(a[i]);	
		i++;
	}
	return i;
}
char* StringList::get(size_t i)
{
	static std::string nulls("");
	if (i < sv.size() && i >= 0)
	       return strdup(sv[i].c_str());
       	else
	       return NULL;
}

size_t StringList::size() { return sv.size(); }
