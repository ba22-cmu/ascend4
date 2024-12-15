#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

char get_type( unsigned char t)
{
	switch (t) {
	case DT_BLK:
		return 'b';
	case DT_CHR:
		return 'c';
	case DT_DIR:
		return 'd';
	case DT_FIFO:
		return 'p';
	case DT_LNK:
		return 'l';
	case DT_REG:
		return 'f';
	case DT_SOCK:
		return 's';
	case DT_UNKNOWN:
	default:
		return 'U';
	}
}

void traverse_directory( char *dir_path) {
	if (!dir_path) {
		return;
	}
	size_t n = strlen(dir_path);
	while (n > 1 && dir_path[n-1] == '/') {
		dir_path[n-1] = '\0';
		n--;
	}
	printf("Listing %s\n", dir_path);
	DIR *dir = opendir(dir_path);
	if (dir == NULL) {
		perror("opendir");
		return;
	}

	struct dirent *entry;
	char path[1024];

	while ((entry = readdir(dir)) != NULL) {
		// Skip the current directory and parent directory entries
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		// Construct the full path
		snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

		// Print the file path
		printf("%c: %s\n", get_type(entry->d_type ), path);

		// If the entry is a directory, recursively traverse it
		if (entry->d_type == DT_DIR) {
			traverse_directory(path);
		}
	}

	closedir(dir);
}

EMSCRIPTEN_KEEPALIVE
int main(int argc, char *argv[]) {
	traverse_directory("/");

	return EXIT_SUCCESS;
}

