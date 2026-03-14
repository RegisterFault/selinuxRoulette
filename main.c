#define _GNU_SOURCE
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/xattr.h>
#include <selinux/selinux.h>

#define BUF_SIZE 1024
#define RANDOMFILE "/dev/random"
#define RANDOM_MODULO 10000

// Keeping the random file open in global scope
FILE * Rand;

int bang(const char *name){
	//fully unconfined selinux label, guaranteed to make selinux wag its finger
	const char *unconfined = "unconfined_u:object_r:default_t:s0";

	if (setfilecon(name, unconfined) < 0){
		perror("setfilecon");
		return 1;
	}
	return 0;
}

void print_label(const char *name){
	size_t ret_size;
	char buf[BUF_SIZE] = {0};
	printf("%s:\t",name);
	ret_size = lgetxattr(name, "security.selinux", buf, BUF_SIZE);
	if ( ret_size  == -1 ) {
		perror("failed attr"); 
	}
	buf[ret_size] = '\0';
	printf("%s\n", buf);
}

int pull_trigger(const char *name, const struct stat *file_stats, int file_type, struct FTW * info){
	//randomly decide whether to carry out with the rest of the function
	unsigned int randval;
	fread( &randval, sizeof(randval), 1, Rand);
	if (randval % RANDOM_MODULO != 0){
		return 0;
	}

	// Print the file and its security label
	if (file_type == FTW_F) { // if the result is a file
		print_label(name);
		//bang(name);
	}
	return 0;
}


void scan( const char *root){
	//recursively iterate through each directory
	if( nftw(root, pull_trigger, 32, 0) ){
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
