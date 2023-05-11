#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size(char *fname, size_t *blocks);

/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR *dir;
	struct dirent *entry;
	char path[1024];

	if ((dir = opendir(dname)) == NULL) {
    	perror("opendir");
    	return -1;
	}

	while ((entry = readdir(dir)) != NULL) {
    	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        	continue;

    	snprintf(path, sizeof(path), "%s/%s", dname, entry->d_name);
    	if (get_size(path, blocks) == -1)
        	return -1;
	}

	closedir(dir);
	return 0;
}

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat st;

	if (lstat(fname, &st) == -1) {
    	perror("lstat");
    	return -1;
	}

	if (S_ISDIR(st.st_mode)) {
    	if (get_size_dir(fname, blocks) == -1)
        	return -1;
	} else {
    	*blocks += st.st_blocks;
	}

	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	size_t blocks;
	int i;

	if (argc < 2) {
    	fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
    	return 1;
	}

	for (i = 1; i < argc; i++) {
    	blocks = 0;
    	if (get_size(argv[i], &blocks) == -1)
        	return 1;

    	printf("%zuK %s\n", (blocks * 512 + 1023) / 1024, argv[i]);
	}

	return 0;
}
