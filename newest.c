/*
* Name: Jenny Wright, Tia Ransom, and Ashton Harrell
* Date: 2/13/25
* Description: basic shell implementation with I/O redirection 
*/
// headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

struct ShellCommand{
    char* args[10];
    int argcount;
};

// Functions to implement
// WORKS (for now )
char* CommandPrompt(){ // display working directory and take user input
    char* input = malloc(100); // allocate memory to prevent segmentation fault 
    char* cwd = getcwd(NULL, 0);
    printf("%s$ ", cwd); // print pwd
    fgets(input, 100, stdin); // take user input
    return input;
}

struct ShellCommand ParseCommandLine(char* input) {
    // while reading user input, add individual args to args array 
    // use counter to navigate args array
    // use strtok() to split the input string by whitespace
    struct ShellCommand command;
    char *args[10];
    char* token = strtok(input, " ");
    int length = 0;
    // append tokens to *args until NULL pointer
    while (token != NULL && length < 10){
        args[length] = token;
        token = strtok(NULL, " ");
        length++;
    }
    // DEBUG
    // for (int i = 0; i < length; i++) {
    //     printf("arg %d is %s, ", i, args[i]);
    // }
    // printf("\n");
    for (int k = 0; k < 10; k++){
        command.args[k] = args[k];
    }
    command.argcount = length;
    return command;
}

void ExecuteCommand(struct ShellCommand command) {
    // if first arg of input is not a real function or not one of our defined functions, 
    // print "function : command not found"
    
    //if nothing was typed in
    if (command.argcount == 0) {
        return;
    }
    
    // if exit was the first thing typed in the terminal
    if (strcmp(command.args[0], "exit") == 0) {
       exit(0);
    }


    pid_t PID = fork();
    // child process
    if (PID == 0) {
        execvp(command.args[0], command.args);
        perror("command not found\n");
        exit(1);
    } else { // parent process
        wait(NULL); //wait for the child process to complete
    }


}

// MAIN
int main(){
    while (1) {
        char* input = CommandPrompt();
        struct ShellCommand command = ParseCommandLine(input);
        ExecuteCommand(command);
        free(input);
    }
    return 0;
} 
