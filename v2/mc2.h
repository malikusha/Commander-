//Malika Nurbekova, Remy Kaldawy

#ifndef HELPER_H
#define HELPER_H

//define a command struct
typedef struct {
  char* name;
  char* desc;
  //used for preset commands
  char* path;
  //the info field is used when user inputs info
  char** info;
  int flag;    // 0 if its the foreground process, 1 if its the background process
} Command;

//the command list and the pointer to the end of the array is initialized
Command* COMMAND_LIST;
Command* ARRAY_END;

//same for the character commands
Command* CHAR_COMMAND_LIST;
Command* CHAR_ARRAY_END;

//the max commands allowed is 20
const int MAX_COMMANDS = 20;

//we actively count the number of commands in each list of commands
int COMMAND_COUNT = 3;
const int CHAR_COMMAND_COUNT = 4;

//we typedef the process struct beforehand because it self references in its fields
typedef struct Process Process;

//the process struct has a PID, a struct time that tells when it started, and pointers
//to previous and next values
struct Process{
  int PID;
  struct timeval start;
  Process* prev;
  Process* next;
};

//we create the linked list variable and a pointer to its stub
Process* PROCESS_LIST;
Process* LAST_PROCESS;
int PROCESS_COUNT = 0;

#endif
