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

void print_directory( const char *dir_path) {
	if (!dir_path) {
		return;
	}
	char *p = strdup(dir_path);
	size_t n = strlen(p);
	while (n > 1 && p[n-1] == '/') {
		p[n-1] = '\0';
		n--;
	}
	printf("Listing %s\n", p);
	DIR *dir = opendir(p);
	if (dir == NULL) {
		perror("opendir");
		goto out;
	}

	struct dirent *entry;
	char path[1024];

	while ((entry = readdir(dir)) != NULL) {
		// Skip the current directory and parent directory entries
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		// Construct the full path
		if (strlen(p) > 1)
			snprintf(path, sizeof(path), "%s/%s", p, entry->d_name);
		else
			snprintf(path, sizeof(path), "/%s", entry->d_name);

		// Print the file path
		printf("%c: %s\n", get_type(entry->d_type ), path);

		// If the entry is a directory, recursively traverse it
		if (entry->d_type == DT_DIR) {
			print_directory(path);
		}
	}

out:
	free(p);
	closedir(dir);
}

