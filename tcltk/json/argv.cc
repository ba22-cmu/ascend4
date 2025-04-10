#include "argv.h"
#include <cstdlib>
#include <cstring>
#include <cerrno>

void freeArgv(char **argv)
{
	if (!argv)
		return;
	argv--;
	free(argv[0]);
	free(argv);
}

int toArgv(const char *vs, const char *sep, int *argc, char ***argv) 
{
	if (!argc || !argv || !vs || !sep)
		return EINVAL;
	*argc = 0;
	size_t i = 0;
	int c = 0;

	// (over)estimate the count
	while (vs[i] != '\0') {
		int k = 0;
		while (sep[k] != '\0') {
			if (vs[i] == sep[k]) {
				c++;
				break;
			}
			k++;
		}
		i++;
	}
	// space for possible last element, null list terminator, and free-ptr
	c += 3 ;

	char *root = strdup(vs);
	if (!root)
		return ENOMEM;

	*argv = (char **)calloc(sizeof(char *) , c);
	if (!*argv) {
		return ENOMEM;
	}
	(*argv)[0] = root;
	(*argv)++;

	i = 0;
	char *token; char *next = NULL;
	for (token = strtok_r(root, sep, &next);
		token != NULL;
		token = strtok_r(NULL, sep, &next)) {
		(*argv)[*argc] = token;
		(*argc)++;
	}
	(*argv)[*argc] = NULL;
	return 0;
}

#ifdef ARGV_MAIN
#include <iostream>
int testToArgv() {
	const char *vs = "\v\va\vb\vc\v\v1\v2\v";
	int argc;
	char **argv;
	int err = toArgv(vs, "\v", &argc, &argv);
	if (err) {
		std::cout << "err:" <<  err << std::endl;
		return err;
	}
	for (int i = 0; i < argc; i++) {
		std::cout << argv[i] << std::endl;
	}
	freeArgv(argv);
	return err;
}

int main() {
	return testToArgv();
}
#endif
