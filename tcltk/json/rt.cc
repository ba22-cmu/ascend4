#include "rt.hpp"
rc::rc()
{
	e = 3;
	t = SVcstr;
	s = "no-msg";
	v = s.c_str();
}
int main() {
	// rc rt("a",1,2);
	//printf("%d\n", rt.e);
	return 0;
}
