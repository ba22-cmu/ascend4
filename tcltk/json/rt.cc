#include "rt.hpp"
rc::rc()
{
	e = 3;
}
int main() {
	rc rt("a",1,2);
	printf("%d\n", rt.e);
}
