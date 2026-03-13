#define _GNU_SOURCE
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/xattr.h>

#define BUF_SIZE 1024
#define RANDOMFILE "/dev/random"
#define RANDOM_MODULO 100000

// Keeping the random file open in global scope
FILE * Rand;

int print_label(const char *name, const struct stat *file_stats, int file_type, struct FTW * info){
	//randomly decide whether to carry out with the rest of the function
    unsigned int randval;
	fread( &randval, sizeof(randval), 1, Rand);
	if (randval % RANDOM_MODULO != 0){
		return 0;
	}

	// Print the file and its security label
	size_t ret_size;
	char buf[BUF_SIZE] = {0};
	if (file_type == FTW_F) { // if the result is a file
		printf("%s: ",name);
		ret_size = lgetxattr(name, "security.selinux", buf, BUF_SIZE);
		if ( ret_size  == -1 ) {
			perror("failed attr");
			return 0; // continue the loop
		}
		buf[ret_size] = '\0';
		printf("%s\n", buf);
	}
	return 0;
}

void scan( const char *root){
    //recursively iterate through each directory
	if( nftw(root, print_label, 32, 0) ){
		perror("ntfw");
	}
}

int main(){
	Rand = fopen(RANDOMFILE, "r");
	scan("/usr/share");
	scan("/usr/libexec");
	scan("/usr/share");
	scan("/usr/sbin");
	scan("/usr/include");
	scan("/lib");
	scan("/lib64");
	scan("/dev");
	scan("/etc");

    fclose(Rand);
}
