#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define N 100

int isRegularFile(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0; // failed to get file status
    }
    return S_ISREG(path_stat.st_mode);
}

int isSymbolicLink(const char *path)
{
    struct stat path_stat;
    if (lstat(path, &path_stat) != 0) {
        return 0; // failed to get file status
    }
    return S_ISLNK(path_stat.st_mode);
}

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

    // j - the index of '-'
    int j;
    // check if the input was valid
    for(int i = 0; i < strlen(options); i++) {
        if(options[i] == '-') {
            j = i;
            break;
        }
    }
    if(j != -1) {
        // check if there are any non-whitespace characters before the dash
        for(int i = 0; i < j; i++) {
            if(!isspace(options[i])) {
                printf("Error: Invalid input\n");
                return 0;
            }
        }

        // if we get here it means that the input is valid
        // we compute the operations received from the input
        for (int i = 0; i < strlen(options); i++)
        {
            switch (options[i])
            {
            case 'n':
                
                break;
            case 'd':

                break;
            case 'h':

                break;
            case 'm':

                break;
            case 'a':

                break;
            case 'l':

                break;
            default:
                break;
            }
        }
        
        
    } else {
        printf("Error: Invalid input\n");
    }
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
    
    // j - the index of '-'
    int j;
    // check if the input was valid
    for(int i = 0; i < strlen(options); i++) {
        if(options[i] == '-') {
            j = i;
            break;
        }
    }
    if(j != -1) {
        // check if there are any non-whitespace characters before the dash
        for(int i = 0; i < j; i++) {
            if(!isspace(options[i])) {
                printf("Error: Invalid input\n");
                return 0;
            }
        }

        // if we get here it means that the input is valid
        // we compute the operations received from the input
        for (int i = 0; i < strlen(options); i++)
        {
            switch (options[i])
            {
            case 'n':
                
                break;
            case 'l':

                break;
            case 'd':

                break;
            case 't':

                break;
            case 'a':
            
                break;
            default:
                break;
            }
        }
        
        
    } else {
        printf("Error: Invalid input\n");
    }
    
}

int main(int argc, char** argv) {
    // check if there were arguments passed
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            // check if it's a regular file / symbolic link
            if (isRegularFile(argv[i]))
            {
                regularFileMenu(argv[i]);
            } else if (isSymbolicLink(argv[i]))
            {
                symbolicLinkMenu(argv[i]);
            }
            
        }
        
    } else {
        printf("Pass as arguments paths to regular files / directories / symbolic links\n");
    }
    

    return 0;
}