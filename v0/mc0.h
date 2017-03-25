
#ifndef HELPER_H
#define HELPER_H

//define a command struct
typedef struct {
  char* name;
  char* desc;
  char* path;
} Command;

//create a global command array
Command* COMMAND_LIST;
//the total number of command options (just the preset)
const int COMMAND_COUNT = 3;

#endif
