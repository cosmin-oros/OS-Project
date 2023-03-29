#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define N 100

// function that computes the operations on a regular file
void regularFileMenu(char* file) {
    char options[N];
    printf("---- Menu ----\n");
    printf("  * n: display file name\n");
    printf("  * d: display file size\n");
    printf("  * h: display hard link count\n");
    printf("  * m: display time of last modification\n");
    printf("  * a: display access rights\n");
    printf("  * l: create symbolic link\n");
    printf("Please enter your options\n\n");
    printf("STANDARD INPUT:");
    // read the string from stdin
    fgets(options, N, stdin);

}

// function that computes the operations on a symbolic link
void symbolicLinkMenu(char* file) {
    char options[N];
    printf("---- Menu ----\n");
    printf("  * n: display file name\n");
    printf("  * l: delete symbolic link\n");
    printf("  * d: size of symbolic link\n");
    printf("  * t: size of target file\n");
    printf("  * a: access rights\n");
    printf("Please enter your options\n\n");
    printf("STANDARD INPUT:");
    // read the string from stdin
    fgets(options, N, stdin);
    
}

int main(int argc, char** argv) {
    

    return 0;
}