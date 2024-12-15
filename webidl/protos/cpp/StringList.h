#ifndef StringList_h_seen
#define StringList_h_seen
#include <string>
#include <vector>
class StringList {
	private:
		std::vector<std::string> sv;
	public:
	StringList(std::vector<std::string> v) : sv(v) {}
	int add(char **a);
	// caller will need to free result.
	char * get(size_t i);
	size_t size();
};
#endif
