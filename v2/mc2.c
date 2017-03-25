//Malika Nurbekova, Remy Kaldawy

//holds our main file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include "mc2.h"

/**
  * add_command: Adds a new command to the global array of available commands
  * @param array_end: pointer to the last element of the command array
  * @param title: The title of the new command.
  * @param desc: The description of the new command.
  * @param path: The file path of the new command.
  * @param info: Any preset arguments of the command.
  * @param flag: Indicates whether the command should be ran in background
**/
void add_command(Command* array_end, char* title, char* desc, char* path, char** info, int flag){
  //if we are using the int option array
  if (array_end == ARRAY_END){
    //set the name of the new command
    ARRAY_END->name = strndup(title, 100);
    //set the description of the new command
    ARRAY_END->desc = strndup(desc, 500);
    //set the path of the new command
    ARRAY_END->path = strndup(path, 500);
    //set the information of the new command
    ARRAY_END->info = info;
    //set the flag of the new command
    ARRAY_END->flag = flag;
  }
  //if we are adding to the char option array
  else {
    //see above
    CHAR_ARRAY_END->name = strndup(title, 100);
    CHAR_ARRAY_END->desc = strndup(desc, 500);
    CHAR_ARRAY_END->path = strndup(path, 500);
    CHAR_ARRAY_END->flag = flag;
    CHAR_ARRAY_END->info = NULL;
  }
}

/**
  * add_process: adds a background process to the process linked list
  * @param PID the integer PID identifying the process in the list
**/
void add_process(int PID){
  //reserve the space of the new list spot
  Process* new_process = (Process*) malloc(sizeof(Process));
  //start timing when the process is added to the array
  gettimeofday(&(new_process->start), NULL);
  //assign process values and attach the process to the linked list
  new_process->PID = PID;
  new_process->prev = LAST_PROCESS;
  new_process->next = NULL;
  LAST_PROCESS->next = new_process;
  //this node is now the last in the list
  LAST_PROCESS = new_process;
  PROCESS_COUNT++;
}

/**
  * remove_process: removes a background process to the process linked list
  * @param PID the integer PID identifying the process in the list
**/
void remove_process(int PID){
  //immediately stop if 0 makes its way in, as to not delete the stub
  if (PID == 0){ return; }
  //start at the stub which begins the linked list
  Process* check = PROCESS_LIST;
  //loop through until we find the right PID
  while(check->PID != PID){
    //if we couldnt find the PID, just return
    if (check == NULL){
      return;
    }
    check = check->next;
  }
  //if we are the last element, remove it from just the previous element
  if(check->next == NULL){
    LAST_PROCESS = check->prev;
    (check->prev)->next = check->next;
  }
  //otherwise we link between the previous and next elements
  else {
    (check->prev)->next = check->next;
    (check->next)->prev = check->prev;
  }
  //we no longer need it malloc'd
  free(check);
  //we are down one process
  PROCESS_COUNT--;
}

/**
  * find_process: finds a background process from a process linked list
  * @param PID the integer PID identifying the process in the list
**/
Process* find_process(int PID){
  //once again, abort if we are looking for the stub
  if (PID == 0){ return NULL; }
  Process* check = PROCESS_LIST;
  //loop through the list until we find the desired PID
  while(check->PID != PID){
    check = check->next;
  }
  //return the process associated to that PID
  return check;
}

/**
  * initialize_commands: adds in the three pre-set commands
**/
void initialize_commands(){
  //initialize the process list
  PROCESS_LIST = (Process*) malloc(sizeof(Process));
  LAST_PROCESS = PROCESS_LIST;
  //build the stub from which the linked list is built on
  PROCESS_LIST->PID = 0;
  //PROCESS_LIST->start = NULL;
  PROCESS_LIST->prev = NULL;
  PROCESS_LIST->next = NULL;

  //initialize the command array
  COMMAND_LIST = (Command*) calloc(MAX_COMMANDS, sizeof(Command));
  ARRAY_END = COMMAND_LIST;
  //add the cpre-set int commands
  add_command(ARRAY_END, "whoami   :", "Prints out the result of the whoamicommand", "/usr/bin/whoami", NULL, 0);
  //manually increment the array pointer
  ARRAY_END++;
  add_command(ARRAY_END, "last     :", "Prints out the result of the last command", "/usr/bin/last", NULL, 0);
  ARRAY_END++;
  add_command(ARRAY_END, "ls       :", "Prints out the result of a listing on a user-specified path", "/bin/ls", NULL, 0);
  ARRAY_END++;

  //initialize the character command array
  CHAR_COMMAND_LIST = (Command*) calloc(CHAR_COMMAND_COUNT, sizeof(Command));
  CHAR_ARRAY_END = CHAR_COMMAND_LIST;
  //add all four char commands
  add_command(CHAR_ARRAY_END, "add command :", "Adds a new command to the menu", "/", NULL, 0);
  //we manually incrememnt the pointer to the end of the array
  CHAR_ARRAY_END++;
  add_command(CHAR_ARRAY_END, "change directory :", "Changes process working directory", "/", NULL, 0);
  CHAR_ARRAY_END++;
  add_command(CHAR_ARRAY_END, "exit :", "Leave Mid-Day Commander", "/", NULL, 0);
  CHAR_ARRAY_END++;
  add_command(CHAR_ARRAY_END, "pwd :", "Prints working directory", "/bin/pwd", NULL, 0);
  CHAR_ARRAY_END++;
  add_command(CHAR_ARRAY_END, "running processes :", "Print list of running processes", "/", NULL, 0);
  CHAR_ARRAY_END++;
}

/**
  * print_menu: prints the current Mid Day Commander menu
**/
void print_menu(){
  printf("==== Mid-Day Commander, v1 ====\n");
  printf("G'Day Commander!, What command would you like to run?\n");
  for (int i = 0; i < COMMAND_COUNT; i++){
    //print each command from an array
    printf("%d. %s %s\n", i, COMMAND_LIST[i].name, COMMAND_LIST[i].desc);
  }
  //the char options are static
  printf("a. add command : Adds a new command to the menu.\n");
  printf("c. change directory : Changes process working directory.\n");
  printf("e. exit : Leave Mid-Day Commander\n");
  printf("p. pwd : Prints working directory\n");
  printf("r. running processes : Print list of running processes\n");
}

/**
  * read_input: reads a line of input from the current input source; to be the option
  * @param int_option The value that will be updated if an int was found
  * @param char_option The valued that will be updated if a char was found
**/
void read_input(int* int_option, char** char_option){
  int option;
  //initialize pointer to be used to read the string as an int
  char** endptr = (char**) malloc(sizeof(char*));
  char* input_line = NULL;
  //the array where the input will be entered
  char* input = (char*)calloc(10, sizeof(char));
  int buff_size = 0;

  printf("Option?: ");
  //read the entire line and take the firt input from i
  getline(&input_line, &buff_size, stdin);
  //eof check; used for file input
  if (feof(stdin) != 0){
    printf("\n");
    exit(0);
  }
  sscanf(input_line, "%s", input);
  printf("\n");
  //try to convert the string to an int
  option = (int)strtol(input, endptr, 10);
  //if we fail, update the character option
  if (*endptr == input){
    *char_option = input;
  }
  //if we succeed, update the int option
  else {
    *int_option = option;
  }
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
  char* input = (char*)calloc(10, sizeof(char));
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
  * perform_char_instruction: Runs a parent-based command based on the
  * character inputted.
  * @param char_option Pointer to the string holding the character inputted
  * @param prev_hard = Pointer to the int storing previous cumulative hard faults
  * @param prev_soft = Pointer to the int storing previous cumulative soft faults
**/
void perform_char_instruction(char** char_option, int* prev_hard, int* prev_soft){
  //declare some structs to store usage and time values
  struct timeval start, end;
  struct rusage usage;

  //before initiating the command, check for previous background processes which
  //have finished
  while(1){
    //run a nohang to catch any remaining background checks
    int num_check = wait3(NULL, WNOHANG, &usage);
    //if nohang returns 0 or an error, we move on and run the command
    if (num_check == 0 || num_check == -1){
      break;
    }
    //otherwise calculate and report statistics
    else {
      gettimeofday(&end, NULL);
      //we use the time struct stored when we added the process from the linked list
      start = find_process(num_check)->start;
      //calculate the total time waited from the structs
      int elapsed_time = ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000));
      //pull the ummulative child usage
      getrusage(RUSAGE_CHILDREN, &usage);
      //print statistics
      //update the cummulative check
      print_stats(elapsed_time, (usage.ru_majflt - *prev_hard), (usage.ru_minflt - *prev_soft));
      *prev_hard = usage.ru_majflt;
      *prev_soft = usage.ru_minflt;
      printf("\n");
      //we remove that process once we're done
      remove_process(num_check);
    }
  }
  //adds a new command to the list
  if (strcmp(*char_option, "a") == 0){
    printf("-- Add a command --\n");
    printf("Command to add?: ");
    //set up getline parameters
    char* input_line = NULL;
    int buff_size = 0;
    int chars_read = getline(&input_line, &buff_size, stdin);
    //if nothing was inputted, just return
    if(chars_read == 1){
      return;
    }

    //converts the newline char into a null terminator at the end of the input
    int len = strlen(input_line);
    input_line[len-1] = '\0';

    //the input becomes the command name
    char* name = strdup(input_line);

    //the array of arguments strtok will write into
    char** args = (char**)calloc(32, sizeof(char*));
    char** arg_end = args;

    //the command name is the first element of the array
    char* new_command = strndup(strtok(input_line, " "), 20);
    *arg_end = new_command;
    //the rest of our array will store arguments
    arg_end++;

    char* token;
    while(1){
      //read each token
      token = strtok(NULL, " ");
      //stop reading once we read the null character
      if(token == NULL){
        break;
      }
      //copy the token onto the argument array
      *arg_end = strndup(token, 20);
      arg_end++;
    }
    arg_end--;

    //check for the ampersand symbol to see if the command will be background
    if (**arg_end == '&'){
      //replace it with null
      *arg_end = (char*)NULL;
      //tell the command array that this program is background
      add_command(ARRAY_END, strcat(name, "   :"), "A user-added command.", "", args, 1);
    }
    else {
      //otherwise add it as a foeground command
      add_command(ARRAY_END, strcat(name, "   :"), "A user-added command.", "", args, 0);
    }

    //increment the counter of commands
    COMMAND_COUNT++;
    //Print that the new command has been added
    printf("Okay, added with ID %d\n", COMMAND_COUNT - 1);
    //remember, we manually adjust the ARRAY_END field
    ARRAY_END++;

  }
  //command to change directory
  else if (strcmp(*char_option, "c") == 0){
    printf("-- Change Directory --\n");
    printf("New Directory?: \n");
    //read in the new directory, and run the command
    char* new_directory = read_line();
    chdir(new_directory);
  }
  //command to print the current directory
  else if (strcmp(*char_option, "p") == 0){
    printf("-- Current Directory --\n");
    printf("Directory: ");
    char buf[500];
    //load the current directory onto the buffer, and print
    *getcwd(buf, sizeof(buf));
    printf("%s", buf);
  }
  //command to print out all current processes
  else if (strcmp(*char_option, "r") == 0) {
    printf("-- Background Processes --\n");
    //create a pointer to the first non-stub element
    Process* start = PROCESS_LIST->next;

    for (int i = 0; i < PROCESS_COUNT; i++){
      //print each pid in the process linked list
      printf("[%d]: PID: %d\n", i+1, start->PID);
      start++;
    }
  }
  //comand to log out
  else if (strcmp(*char_option, "e") == 0){
    //if there are no other background processes, simply log out
    if(PROCESS_COUNT == 0){
      printf("Logging you out, Commander.\n");
      exit(0);
    }
    //otherwise wait for all background processes to finish, then return
    else {
      printf("Please wait for all background processes to complete...\n");
      while(PROCESS_COUNT != 0){
        //wait for a background process
        int num_check = wait3(NULL, 0, &usage);
        //report stats as indicated above
        gettimeofday(&end, NULL);
        start = find_process(num_check)->start;
        int elapsed_time = ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000));
        getrusage(RUSAGE_CHILDREN, &usage);
        print_stats(elapsed_time, (usage.ru_majflt - *prev_hard), (usage.ru_minflt - *prev_soft));
        *prev_hard = usage.ru_majflt;
        *prev_soft = usage.ru_minflt;
        printf("\n");
        //remove process with that PID
        remove_process(num_check);
      }
      exit(0);
    }
  }
  else if (strcmp(*char_option, "") == 0){
    exit(0);
  }
}

/**
  * perform_instruction: performs the process of the selected instruction, and
  * reports its statistics
  * @param int_option Point to the integer option selected, or -1
  * @param prev_hard Pointer to the previous number of child hard faults.
  * @param prev_soft Pointer to the cumulative number of child soft faultss
**/
void perform_instruction(int* int_option, int* prev_hard, int* prev_soft){

  //declare structs for tracking time and usage
  struct timeval start, start_bkg, end;
  struct timezone tzp;
  struct rusage usage;

  //declare strings for reading ls arguments
  char* arguments;
  char* path;

  //checks if some other number was inputted
  if (*int_option < 0 || *int_option > COMMAND_COUNT - 1 || *int_option > MAX_COMMANDS - 1){
    printf("There is no such option. Please try again. Wumpus grumpus.\n");
  }

  //we read the ls inputs before forking so that we dont time it
  if (*int_option == 2) {
    printf("-- Directory Listing --\n");
    printf("Arguments?: ");
    //uses the read_line helper
    arguments = read_line();
    printf("Path?: ");
    path = read_line();
    printf("\n");
  }

  //we start timing
  gettimeofday(&start, NULL);
  //we then fork
  int rc = fork();

  //check if the fork failed
  if (rc < 0) {         // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  //if we are the child, we exec the entered command
  else if (rc == 0) { // child (new process)
    //whoami
    if (*int_option == 0){
      printf("-- Who Am I? --\n");
      //we run it using the data stored in the command array
      execl(COMMAND_LIST[0].path, "placeholder", (char*) NULL);
    }
    //lastused
    else if (*int_option == 1){
      printf("-- Last Logins --\n");
      execl(COMMAND_LIST[1].path, "placeholder", (char*) NULL);
    }
    //ls uses the arguments collected before the fork to avoid timing error
    else if (*int_option == 2) {
      execl(COMMAND_LIST[2].path, "placeholder", arguments, path, (char*) NULL);
    }
    //run a custom command
    else {
      printf("\n");
      printf("-- Command: %s --\n", COMMAND_LIST[*int_option].name);
      //the function is the first element
      char* function =  COMMAND_LIST[*int_option].info[0];
      //we need to feed the function name with the arguments. who knows why
      char** arguments = COMMAND_LIST[*int_option].info;
      //execvp will find the desired path
      execvp(function, arguments);
    }
  }
  else {
    //if it is a foreground process
    if (COMMAND_LIST[*int_option].flag == 0){
      //initialize our wait3 check
      int num_check = 0;
      //we keep looping hard wait3's until the foreground process returns
      //this colects background processes that end before the foreground process
      while(num_check != rc){
        num_check = wait3(NULL, 0, &usage);
        //if we collect a backround process, calculate and report statistics
        if (num_check != rc && num_check != 0 && num_check != 1){
          //see above
          gettimeofday(&end, NULL);
          start_bkg = find_process(num_check)->start;
          int elapsed_time = ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start_bkg.tv_sec * 1000 + start_bkg.tv_usec / 1000));
          //getrusage(RUSAGE_CHILDREN, &usage);
          print_stats(elapsed_time, (usage.ru_majflt - *prev_hard), (usage.ru_minflt - *prev_soft));
          *prev_hard = usage.ru_majflt;
          *prev_soft = usage.ru_minflt;
          printf("\n");
          remove_process(num_check);
        }
        //if we collect the foreground process, do the same, but try to remove it
        //from the background process linked list
        else {
          gettimeofday(&end, NULL);
          int elapsed_time = ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000));
          getrusage(RUSAGE_CHILDREN, &usage);
          printf("\n");
          print_stats(elapsed_time, (usage.ru_majflt - *prev_hard), (usage.ru_minflt - *prev_soft));
          *prev_hard = usage.ru_majflt;
          *prev_soft = usage.ru_minflt;
          printf("\n");
        }
      }
    }
    //if a background process was run, add it to the process linked list
    else {
      add_process(rc);
    }
    //after the foreground process, or if a background process was run, check to
    //see if any other background processes are still running
    while(1){
      //once the wait3 returns 0 or error, we go back to printing the console
      int num_check = wait3(NULL, WNOHANG, &usage);
      if (num_check == -1 || num_check == 0){
        break;
      }
      else {
        //if we have caught a background process, report its statistics
        gettimeofday(&end, NULL);
        start_bkg = find_process(num_check)->start;
        printf("\n");
        int elapsed_time = ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start_bkg.tv_sec * 1000 + start_bkg.tv_usec / 1000));

        //print out information about the completed background process
        printf(" Job Complete[%d] --\n", *int_option); //incorrect
        printf("Process ID: %d", num_check);
        //getrusage(RUSAGE_CHILDREN, &usage);

        // Print statistics
        print_stats(elapsed_time, (usage.ru_majflt - *prev_hard), (usage.ru_minflt - *prev_soft));
        *prev_hard = usage.ru_majflt;
        *prev_soft = usage.ru_minflt;
        remove_process(num_check);
        printf("\n");
        //num_check is a pid and we use usage to get some info
      }
    }
  }
}
int main(){
  //initialize the arrays and preset commands
  initialize_commands();
  //initialize the usage tracker for cumulative child page faults
  int prev_hard_val = 0;
  int prev_soft_val = 0;
  int* prev_hard = &prev_hard_val;
  int* prev_soft = &prev_soft_val;

  //initialize the pointers to the integer and char option
  int int_option_val;
  int* int_option = &int_option_val;
  char* char_option_val;
  char** char_option = &char_option_val;

  //keep looping until something exits
  while(1){
    //reset our option values
    *int_option = 0;
    *char_option = NULL;
    //print the console
    print_menu();
    //read the input, which will differentiate between an int or char command
    read_input(int_option, char_option);

    //if it is a char command, perform one of those options
    if (*char_option != NULL){
      perform_char_instruction(char_option, prev_hard, prev_soft);
    }
    //if it is an int command, perform one of those options
    else {
      perform_instruction(int_option, prev_hard, prev_soft);
    }
    printf("\n");
  }
  return 0;
}
