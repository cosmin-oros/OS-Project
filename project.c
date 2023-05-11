#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>

// function to extract filename from path
void get_filename(char path[1024], char filename[1024]) {
    int len = strlen(path);
    int count = 0, j = len;

    // find the last '/' in the path
    while (path[j] != '/' && j >= 0) {
        j--;
    }

    // extract the filename from the path
    while (j != len) {
        filename[count++] = path[++j];
    }
}

// function to print the type of file (regular file, directory, symbolic link)
void print_type(struct stat buf) {
    // check POSIX flags to determine the file type
    if (S_ISREG(buf.st_mode)) {
        printf("REGULAR\n");
        return;
    } else if (S_ISDIR(buf.st_mode)) {
        printf("DIRECTORY\n");
        return;
    } else if (S_ISLNK(buf.st_mode)) {
        printf("SYMBOLIC LINK\n");
        return;
    }
}

// function to print the menu for a given file type
void menu(struct stat buf) {
    printf("---- MENU ----\n");
    if (S_ISREG(buf.st_mode)) {
        // if file is a regular file
        printf("\u2022 n: name\n\u2022 d: size\n\u2022 h: hard link count\n\u2022 m: time of last modification\n\u2022 a: access rights\n\u2022 l: create symbolic link\n");
        return;
    }
    if (S_ISDIR(buf.st_mode)) {
        // if file is a directory
        printf("\u2022 n: name\n\u2022 d: size\n\u2022 a: access rights\n\u2022 c: total number of files with .c extension\n");
        return;
    }
    if (S_ISLNK(buf.st_mode)) {
        // if file is a symbolic link
        printf("\u2022 n: name\n\u2022 l: delete symbolic link\n\u2022 d: size of symbolic link\n\u2022 t: size of target file\n\u2022 a: access rights\n");
        return;
    }
}

// This function waits for child processes to terminate and prints a message indicating whether the process terminated normally or abnormally.
void wait_for_children(){
    pid_t awaited_child; // Declare variable to store the id of the awaited child process.
    int status; // Declare variable to store the status of the awaited child process.

    // Wait for a child process to terminate and store its id in the awaited_child variable, while also storing its status in the status variable.
    awaited_child = wait(&status);

    // Enter a loop that continues as long as there are still child processes to wait for.
    while(awaited_child > 0){
        // If the child process terminated normally, print a message indicating its id and exit status code.
        if(WIFEXITED(status)){ 
            printf("Child process with pid %d has terminated normally with code %d.\n\n", awaited_child, WEXITSTATUS(status));
        }
        // If the child process terminated abnormally, print a message indicating its id and exit status code.
        else{
            printf("Child process with pid %d has terminated abnormally with code %d.\n\n", awaited_child, WEXITSTATUS(status));
        }

        // Wait for the next child process to terminate and update the awaited_child and status variables.
        awaited_child = wait(&status);
    }
}

// This function prints the access rights for a file, given its stat buffer.
void print_access_rights(struct stat buf){
    // Print the access rights for the user.
    printf("User:\n");
    if(buf.st_mode & S_IRUSR){
        printf("Read: YES\n");
    }
    else{
        printf("Read: NO\n");
    }

    if(buf.st_mode & S_IWUSR){
        printf("Write: YES\n");
    }
    else{
        printf("Write: NO\n");
    }

    if(buf.st_mode & S_IXUSR){
        printf("Exec: YES\n");
    }
    else{
        printf("Exec: NO\n");
    }

    // Print the access rights for the group.
    printf("Group:\n");
    if(buf.st_mode & S_IRGRP){
        printf("Read: YES\n");
    }
    else{
        printf("Read: NO\n");
    }

    if(buf.st_mode & S_IWGRP){
        printf("Write: YES\n");
    }
    else{
        printf("Write: NO\n");
    }

    if(buf.st_mode & S_IXGRP){
        printf("Exec: YES\n");
    }
    else{
        printf("Exec: NO\n");
    }

    // Print the access rights for others.
    printf("Others:\n");
    if(buf.st_mode & S_IROTH){
        printf("Read: YES\n");
    }
    else{
        printf("Read: NO\n");
    }

    if(buf.st_mode & S_IWOTH){
        printf("Write: YES\n");
    }
    else{
        printf("Write: NO\n");
    }

    if(buf.st_mode & S_IXOTH){
        printf("Exec: YES\n");
    }
    else{
        printf("Exec: NO\n");
    }
}

int count_lines(char *path) {
    // Open the file with read-only mode
    FILE *f = fopen(path, "r");

    // Check if the file was opened successfully
    if(f == NULL) {
        perror(strerror(errno));
        exit(errno);
    }

    // Initialize variables
    char c;
    int lines = 0, is_empty = 1;

    // Loop through the file until the end
    while((c = fgetc(f)) != EOF) {
        // Check if the file is not empty
        if(c != EOF) {
            is_empty = 0;
        }

        // Count the number of lines
        if(c == '\n') {
            lines++;
        }
    }

    // If the file is empty, return 0. Otherwise, return the number of lines + 1
    if(is_empty == 1) {
        return 0;
    }
    else {
        return lines + 1;
    }

    // Close the file
    fclose(f);
    return lines;
}

void c_extension_work(char* path, struct stat buf){

    char filename[1024];
    get_filename(path, filename); // function not shown, gets the filename from the path

    pid_t pid2;

    pid2 = fork(); // create a child process
    if(pid2 < 0){ // fork failed
        perror(strerror(errno)); // print an error message
        exit(errno); // exit with an error code
    }

    else if(pid2 == 0){ // child process
        if(filename[strlen(filename)-1]=='c' && filename[strlen(filename)-2]=='.'){ // if the file has a .c extension
            int errors = 0, warnings = 0, score;

            // calculate the score based on the number of errors and warnings
            if(errors == 0 && warnings == 0){
                score = 10;
            }

            if(errors >=1){
                score = 1;
            }

            if(errors == 0 && warnings > 10){
                score = 2;
            }

            if(errors == 0 && warnings <= 10){
                score = 2 + 8 * (10 - warnings) / 10;
            }

            int fd;
            fd = open("grades.txt", O_RDWR | O_CREAT); // open a file for writing
            if(fd == -1){ // check if opening the file failed
                perror(strerror(errno)); // print an error message
                exit(errno); // exit with an error code
            }

            char score_string[3];
            score_string[0] = score / 10 + '0'; // convert the tens digit of the score to a character
            score_string[1] = score % 10 + '0'; // convert the ones digit of the score to a character
            score_string[2] = '\0'; // terminate the string

            char output[1050];
            strcpy(output, filename); // copy the filename to the output string
            strcat(output, ":"); // append a colon to the output string
            strcat(output, score_string); // append the score to the output string

            int check;
            check = write(fd, output, strlen(output)); // write the output string to the file
            if(check == -1){ // check if writing to the file failed
                perror(strerror(errno)); // print an error message
                exit(errno); // exit with an error code
            }

            close(fd); // close the file
        }

        else{ // if the file does not have a .c extension

            printf("The number of lines in this file is:\n");
            int check;
            check = execlp("wc", "wc", "-l", path, NULL); // execute the wc command to count the number of lines in the file

            if(check == -1){ // check if executing the command failed
                perror(strerror(errno)); // print an error message
                exit(errno); // exit with an error code
            }
            
        } 

        exit(EXIT_SUCCESS); // exit the child process with a success code
    }

    else if(pid2 > 0){ // parent process
        wait_for_children(); // wait for all child processes to finish
    }  
}

void create_new_file(char* path, struct stat buf){
    pid_t pid2;
    
    pid2 = fork();
    if(pid2 < 0){
        perror(strerror(errno));
        exit(errno);
    }

    if(pid2==0){
        char filename[1024], new_filename[1050], new_path[1050];

        get_filename(path, filename);
        strcpy(new_filename, filename);
        strcat(new_filename, "_file.txt");
        
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, new_filename);

        /*int check;
        check = creat(new_path, S_IRUSR);*/

        int check;
        check = execlp("touch", "touch", new_path, NULL);
        if(check == -1){
            perror(strerror(errno));
            exit(errno);
        }

        //close(check);

        //exit(EXIT_SUCCESS);
    }

    else if(pid2>0){
        wait_for_children();
    }
}

void change_link_permissions(char* path, struct stat buf) {
    pid_t pid2;

    // Create a child process
    pid2 = fork();
    if (pid2 < 0) {
        // If fork() fails, print an error message and exit
        perror(strerror(errno));
        exit(errno);
    }

    if (pid2 == 0) {
        // If the current process is the child process
        int check;

        // Use the "chmod" command to change the file permissions of the file
        check = execlp("chmod", "chmod", "u+rwx,g+rw-x,o-rwx", path, NULL);
        if (check == -1) {
            // If execlp() fails, print an error message and exit
            perror(strerror(errno));
            exit(errno);
        }

        // Exit the child process with a success status
        //exit(EXIT_SUCCESS);
    }

    else if (pid2 > 0) {
        // If the current process is the parent process
        wait_for_children();
    }
}

void input_options(char* path, struct stat buf);

void options_REG(char* path, struct stat buf, char* options){
    int l, i;
    l = strlen(options);

    char filename[1024];

    for(i=1;i<l;i++){
        if(!strchr("ndhmal", options[i])){
            printf("Wrong input. Please enter your options again:\n");
            input_options(path, buf);
            return;
        }
    }
    
    for(i=1;i<l;i++){
        if(options[i]=='n'){
            get_filename(path, filename);
            printf("The name is %s\n", filename);
        }

        if(options[i]=='d'){
            long size;
            size = buf.st_size;
            printf("The size is %ld bytes\n", size);
        }

        if(options[i]=='h'){
            int count;
            count = buf.st_nlink;
            printf("The number of hard links is %d\n", count);
        }

        if(options[i]=='m'){
            struct timespec ts;
            timespec_get(&ts, buf.st_mtime);
            printf("The last modification time is %ld.%.9ld\n", ts.tv_sec, ts.tv_nsec);
        }

        if(options[i]=='a'){
            print_access_rights(buf);
        }

        if(options[i]=='l'){
            char link_name[1024];

            printf("Input name of link: ");
            scanf("%s", link_name);

            int check;
            check = symlink(path, link_name);
            if(check == -1){
                perror(strerror(errno));
                exit(errno);
            }
            printf("Link has been created!\n");
        }
    }

    c_extension_work(path, buf);
}

void options_DIR(char* path, struct stat buf, char* options){
    int l, i;
    l = strlen(options);

    for(i=1;i<l;i++){
        if(!strchr("ndac", options[i])){
            printf("Wrong input. Please enter your options again:\n");
            input_options(path, buf);
            return;
        }
    }     

    char filename[1024];

    for(i=1;i<l;i++){
        switch(options[i]){
            case 'n':{
                get_filename(path, filename);
                printf("The name is %s\n", filename);

                break;
            }

            case 'd':{
                long size;
                size = buf.st_size;
                printf("The size is %ld bytes\n", size);
                break;
            }

            case 'a':{
                print_access_rights(buf);
                break;
            }

            case 'c':{
                DIR *dir;
                dir = opendir(path);

                if(dir == NULL){
                    perror(strerror(errno));
                    exit(errno);
                }

                struct dirent *entry;
                int counter = 0;

                entry = readdir(dir);
                while(entry!=NULL){
                    
                    //printf("%s %c %c\n", entry->d_name, entry->d_name[strlen(entry->d_name)-1], entry->d_name[strlen(entry->d_name)-1]);
                    if(entry->d_name[strlen(entry->d_name)-1]=='c' && entry->d_name[strlen(entry->d_name)-2]=='.'){
                        counter++;
                    }

                    entry = readdir(dir);
                }

                int check;
                check = closedir(dir);
                if(check == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("There are %d files with the .c extension in this directory.\n", counter);
                break;
            }
        }
    }

    create_new_file(path, buf);
}


void options_LNK(char* path, struct stat buf, char* options){
    int i, l;
    l = strlen(options);

    for(i=1;i<l;i++){
        if(!strchr("nldta", options[i])){
            printf("Wrong input. Please enter your options again:\n");
            input_options(path, buf);
            return;
        }
    }     

    char filename[1024];

    for(i=1;i<l;i++){
        if(options[i]=='n'){
            get_filename(path, filename);
            printf("The name is %s\n", filename);
        }

        if(options[i]=='l'){
            int check;
            check = unlink(path);

            if(check == -1){
                perror(strerror(errno));
                exit(errno);
            }

            printf("Symbolic link has been deleted\n");
            return;
        }

        if(options[i]=='d'){
            long size;
            size = buf.st_size;
            printf("The size is %ld bytes\n", size);
        }

        if(options[i]=='t'){
            struct stat buf2;

            int check;
            check = stat(path, &buf2);
            if(check == -1){
                perror(strerror(errno));
                exit(errno);
            }

            long size;
            size = buf2.st_size;
            printf("The size is %ld bytes\n", size);
        }

        if(options[i]=='a'){
            print_access_rights(buf);
        }
    }

    change_link_permissions(path, buf);
}

void input_options(char* path, struct stat buf){

    printf("Please enter your options!\n");

    char options[10];
    scanf("%s", options);

    if(options[0]!='-'){
        printf("Wrong input. Please enter your options again:\n");
        input_options(path, buf);
        return;
    }

    if(S_ISREG(buf.st_mode)){
        options_REG(path, buf, options);
        return;
    }
            
    else if(S_ISDIR(buf.st_mode)){
        options_DIR(path, buf, options);
        return;
    }

    else if(S_ISLNK(buf.st_mode)){
        options_LNK(path, buf, options);
        return;
    }
    
}

int main(int argc, char* argv[]){
    
    if(argc == 1){
        printf("Not enough arguments\n");
        exit(-1);
    }

    for(int i=1;i<argc;i++){
        
        //start a child process for each argument
        pid_t pid = fork();
        if(pid < 0){
            perror(strerror(errno));
            exit(errno);
        }

        //child process
        else if (pid == 0){
            //get path of arguments
            char path[1024];
            strcpy(path, argv[i]);
            

            //get filename
            char filename[1024];
            get_filename(path, filename);
            printf("%s - ", filename);
            
            //use lstat on file and print file type
            int check;
            struct stat buf;
            
            check = lstat(path, &buf);
            if(check == -1){
                perror(strerror(errno));
                exit(errno);
            }
            print_type(buf);

            //print Menu for file type
            menu(buf);

            //input of the desired options
            input_options(path, buf);

            exit(EXIT_SUCCESS);
        }

        //parent process
        else if (pid > 0){

            wait_for_children();
        }
    }

    return 0;
}