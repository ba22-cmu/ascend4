#if 0
// Define a macro to concatenate tokens
#define CONCAT(a, b) a ## b

// Define a macro to convert a string literal to a token
#define UNSTRINGIZE(s) CONCAT(s, _token)

#define p "x"

char * y = UNSTRINGIZE(p);
#endif
#include <cstdlib>
#include <cstdio>

int main()
{
	printf("%zu\n",sizeof(int));
	printf("%zu\n",sizeof(long));
	printf("%zu\n",sizeof(long long));
}
