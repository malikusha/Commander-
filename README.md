/** file readme.txt
*/

All files: mc0, mc1, mc2
How to compile and link: make
To clean up the source and object directories: make clean

1) mc0
Usage: ./mc0
Description: Program Mid-Day Commander for Fixed Commands. The prompt with three options is displayed, where each option has its unique number, that correspond to the specific command. The options are whoami, last and ls. When the option is chosen by the user, the corresponding process is run and the statistics, such as the elapsed time of the execution, number of hard and soft pages are printed.
Implementation specifications: The structure for the command was created that keeps track of the command's name, description of what the given command does, path and arguments for the given command. The description of the command is stored in the structure to print it later in the menu. The three commands are initialized with the help of add command(initializes the attributes). To read the input from the user's keyboard, we use the getline function and based on the option the user entered, the commands are performed as follows. We check if user entered any input, if no then we exit. If user entered -ls option, he is asked to also input the arguments and path for listing the directory. The reason why this is located before the fork() function in the code is because we want to print the elapsed time later, not including the time the user was entering the option. The timer is set right before fork(). Then the fork() is called, if fork failed, we exit. If it returns 0, then the process is the child process and we check the option the user entered and by using execl() function, we execute the command, passing the path of the command(path in OS libraries). The parent in the same time waits for either of those commands to complete and picks up the finished process. Right after the parent picked up the finished process, the timer stops. The elapsed time is calculated and the statistics is printed. The prompted is asked again. When getline is prompted to read a new line, the final character of the line input is truncated because it is the newline character. Furthermore, often sscanf is used to read the first string separated by whitespace from the input line.

2) mc1
Usage: ./mc1
Description: Program Extendable Mid-Day Commander.
Implementation specifications: Additional commands are added to the Mid-Day Commander, such as change directory, exit the Mid-Day Commander, print working directory and add a new command. Change directory, exit and print working directory are the commands that are performed as the parent processes. We also divided the input option from the user into two: integer option and character option. If it is the integert option, the processes are child processes and the character options are parent processes. We wrote two separated methods to perform instructions: one for character input and one for integer option. Also, we have two lists storing two different types of commands. CHAR_COMMAND_LIST is for the commands that have a character option and COMMAND_LIST is for the commands that have the integer option. The change directory option is implemented with chdir() function, the print current directory option is performed with *getcwd() function, the exit option just exits with code 0.
    For add command: The add command option is implemented by adding a new command with the integer option into
    the COMMAND_LIST.The input line entered by user is divided into tokens(words). The new command is
    created with the name that is the first token(word) in the input line and stored in the zeroth element of
    command->info. Everything after the name of the command is considered as the arguments of this command and
    stored in command->info attribute(starting from the first element). The command is executed with execvp.
    Also, if the input is read from the file and file reached its end, the program will see that there is no
    more input and will exit Mid-Day Commander.

3) mc2
Usage: ./mc2
Description: Commander with background execution. The tasks with '&' on the end are run in the background, while the simple tasks run on the foreground. When a task is run in background, mc2 does not block until the child process completes. Instead, it prompts the user for another command. A background task can terminate at any time. In this case, mc2 displays a message that the particular task has terminated, and it follows that message with the statistics about the command of that task. Output from background commands that is directed to the terminal window may intermingle with the output of other commands and with the output from your mc2. If the user tries to exit mc2 before all background tasks have completed, then mc2 refuses to exit, prints a message, and wait()-s for those tasks to be completed. Also, the command for displaying all the running backgroundd processes is added into the Mid-Day Commander.
Implementation specification: The new attribute was added to the command structure, which is a flag that
    indicates if the command should be run in foreground or in a background(1 if its the background process and
    0 if its the foreground process). We check the input of the add command. If it has '&' in the end, we are
    adding a new command with the flag = 1 and we are cutting the input string, so that it does not include the
    last element which is '&'(then this input string is saved in a new command's info field).
    To store the running background processes to retrieve them later when the user enters the -r option
    command(Print list of running processes), we used linked list data structure. The linked list is defined as
    a Process structure that holds the current process id, the pointer to previous and
    the pointer to the next process. The method for adding background processes into that list puts the new
    process into the end of the linked list, setting its next process to null. The method to remove the process
    from the list of running background processes works by finding the process with the given id in the list and
    removing it from the list and reoorganizing(linking) them. Also, the function for finding processes was
    created. It returns the process with the given pid from the list without removing it.
    The algorithm of Mid-day Commander: The Prompt is asked, user enters one of the integer options. The program
    checks the flag of the command. If its a foreground command, we wait for the current process to complete and
    we also pick up all of those background commands that finished with wait3 in a loop. The statistic of each
    collected child is printed. Next, wait3 is again engaged with the nohang option to check for any processes
    background processes which may have ended alongside or after the foreground process. If it is a background
    command, we use wait3 nohang to check if any other background processes have finished before the current
    background process. If not, the parent returns to the main console, waiting for the initialized background
    process to terminate. The statistics are printed every time a valid process is caught by wait.









