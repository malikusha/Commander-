
//This source file holds our main file and our helper functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include "mc0.h"

/**
  * add_command: Adds a new command to the global array of available commands
  * @param array_end: pointer to the last element of the command array
  * @param title: The title of the new command.
  * @param desc: The description of the new command.
  * @param path: The file path of the new command.
  * @return A pointer to the end of the command array
**/
Command* add_command(Command* array_end, char* title, char* desc, char* path){
  //set the name of the new command
  array_end->name = strndup(title, 100);
  //set the description of the new command
  array_end->desc = strndup(desc, 500);
  //set the path of the new command
  array_end->path = strndup(path, 500);
  //increment the command array pointer (the new pointer is added)
  array_end++;
  return array_end;
}

/**
  * initialize_commands: adds in the three pre-set commands
**/
void initialize_commands(){
  //initialize the command array
  COMMAND_LIST = (Command*) calloc(COMMAND_COUNT, sizeof(Command));
  //initialize the pointer to the last element
  Command* array_end = COMMAND_LIST;
  //add the preset commands
  array_end = add_command(array_end, "whoami   :", "Prints out the result of the whoamicommand", "/usr/bin/whoami");
  array_end = add_command(array_end, "last     :", "Prints out the result of the last command", "/usr/bin/last");
  array_end = add_command(array_end, "ls       :", "Prints out the result of a listing on a user-specified path", "/bin/ls");
}

/**
  * print_menu: prints the current Mid Day Commander menu
**/
void print_menu(){
  printf("==== Mid-Day Commander, v0 ====\n");
  printf("G'Day Commander!, What command would you like to run?\n");
  //print each command in the array
  for (int i = 0; i < COMMAND_COUNT; i++){
    //access the struct values to print
    printf("%d. %s %s\n", i, COMMAND_LIST[i].name, COMMAND_LIST[i].desc);
  }
}

/**
  * read_input: reads a line of input from the current input source; to be the option
  * @return the option number or -1 if a valid number was not passed in
**/
int read_input(){
  int option, read_ret;
  //initialize pointer to be used to read the string as an int
  char** endptr = (char**) malloc(sizeof(char*));
  char* input = (char*)calloc(100, sizeof(char));

  char* input_line = NULL;
  int buff_size = 0;
  int n = 0;
  //keep waiting until we get something to read

  printf("Option?: ");
  //read the entire line and take the firt input from it
  getline(&input_line, &buff_size, stdin);
  if (feof(stdin) != 0){
    printf("\n");
    exit(0);
  }
  sscanf(input_line, "%s", input);
  printf("\n");

  //try to parse the input as an int
  read_ret = (int)strtol(input, endptr, 10);
  //if we fail, return -1. Otherwise, return the scanned number
  if (*endptr == input){
    option = -1;
  }
  else {
    option = read_ret;
  }
  return option;
}

/**
  * read_line: reads a line of input from the current input source;
  * @return the first thing read on the input line
**/
char* read_line(){
  //set stub values for getline
  char* input_line = NULL;
  int buff_size = 0;
  //reserve space for the input
  char* input = (char*)calloc(100, sizeof(char));
  //read the input
  getline(&input_line, &buff_size, stdin);
  //take the first element from the read input
  sscanf(input_line, "%s", input);
  return input;
}

/**
  * print_stats: prints the statistics of a terminated child, including time and
  * page faults.
  * @param elapsed_time The time elapsed in milliseconds
  * @param hard_faults The number of hard faults.
  * @param soft_faults The number of soft faults.
**/
void print_stats(int elapsed_time, long hard_faults, long soft_faults){
  printf("-- Statistics --\n");
  printf("Elapsed time: %d milliseconds\n", elapsed_time);
  printf("Page Faults: %ld\n", hard_faults);
  printf("Page Faults (reclaimed): %ld\n", soft_faults);
}

/**
  * perform_instruction: performs the process of the selected instruction, and
  * reports its statistics
  * @param option The integer option selected, or -1
  * @param prev_hard Pointer to the previous number of child hard faults.
  * @param prev_soft Pointer to the cumulative number of child soft faultss
**/
void perform_instruction(int option, int* prev_hard, int* prev_soft){

  //declare structs for tracking time and usage
  struct timeval start, end;
  struct timezone tzp;
  struct rusage usage;

  //declare strings for reading ls arguments
  char* arguments;
  char* path;

  //we read the ls inputs before forking so that we dont time it
  if (option == 2) {
    printf("-- Directory Listing --\n");
    printf("Arguments?: ");
    arguments = read_line();
    printf("Path?: ");
    path = read_line();
    printf("\n");
 }

 //we fork here to allow the desired process to run
 int rc = fork();

 //start timing here
 gettimeofday(&start, &tzp);

 //if the fork failed, we exit
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    // for whoami
    if (option == 0){
      printf("-- Who Am I? --\n");
      //run based on the information stored in the command list array
      execl(COMMAND_LIST[0].path, "placeholder", (char*) NULL);
    }
    //for lastused
    else if (option == 1){
      printf("-- Last Logins --\n");
      execl(COMMAND_LIST[1].path, "placeholder", (char*) NULL);
    }
    //for ls, run with the arguments we picked up before
    else if (option == 2) {
      execl(COMMAND_LIST[2].path, "placeholder", arguments, path, (char*) NULL);
    }
    //otherwise report an invalid option was selected
    else {
      printf("There is no such command, please chose among 0, 1, 2\n");
      printf("\n");
    }
  }
  else {
    //if we are the parent, wait for the child
    waitpid(rc, NULL, 0);
    //print the end
    gettimeofday(&end, &tzp);
    printf("\n");
    //calculate the elapsed time from seconds and microseconds
    int elapsed_time = ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000));
    //read the usage from all previous children
    getrusage(RUSAGE_CHILDREN, &usage);
    //calculate the usage of this child; update the previous cumulative values
    //print statistics
    print_stats(elapsed_time, (usage.ru_majflt - *prev_hard), (usage.ru_minflt - *prev_soft));
    *prev_hard = usage.ru_majflt;
    *prev_soft = usage.ru_minflt;
    printf("\n");
  }
}

int main(){
  //initialize the arrays and preset commands
  initialize_commands();
  //initialize the usage tracker
  int* prev_hard = (int*) malloc(sizeof(int));
  int* prev_soft = (int*) malloc(sizeof(int));
  *prev_hard = 0;
  *prev_soft = 0;
  //keep looping for options ad infinitum
  while(1){
    //print the console
    print_menu();
    //read what option is selected
    int option = read_input();
    //perform the process with that option
    perform_instruction(option, prev_hard, prev_soft);
  }
  return 0;
}
