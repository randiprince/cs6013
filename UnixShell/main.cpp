#include <iostream>
#include "shelpers.hpp"
#import <readline/readline.h>

int main(int argc, char *argv[]) {
    std::vector<std::string> tokens;
    std::cout << "UnixShell %: "; // cout just to provide better ux
    char *buffer; // declare buffer for table completion
    while ((buffer = readline("> ")) != nullptr) {
        std::string userInput = buffer;
        if (buffer) {
            free(buffer);
        }

        tokens = tokenize(userInput);
        std::vector<Command> commands = getCommands(tokens);

        int status;
        for (int i = 0; i < commands.size(); i++) {
            std::cout << "UnixShell %: "; // add cout again so it is seen with every command
            if (commands[i].exec == "exit") {
                printf("you have exited the unix shell...bye");
                exit(0);
            }

            if (commands[i].exec == "cd") { // handling for cd shell built in
                if (commands[i].argv.size() == 2) {
                    chdir(getenv("HOME"));
                } else {
                    chdir(commands[i].argv[1]);
                }
                continue;
            }

            pid_t forkVal = fork(); // fork new process
            if (forkVal < 0) {
                perror("error! fork failed!");
                exit(1);
            } else if (forkVal == 0) { // child process

                if (commands[i].fdStdout != 1) { // really only need to call dup if write out isn't set to 1 system call
                    if (dup2(commands[i].fdStdout, 1) == -1) { // need to check return value of dup. -1 means error and we need to handle it
                        perror("fdStdout failed");
                        exit(1);
                    }
                }

                if (commands[i].fdStdin != 0) {
                    if (dup2(commands[i].fdStdin, 0) == -1) {
                        perror("fdStdin failed");
                        exit(1);
                    }
                }
                if (execvp(commands[i].exec.c_str(), const_cast<char *const *>(commands[i].argv.data())) == -1) {
                    perror("execvp failed");
                    exit(1);
                }


            } else { //parent process
                if (waitpid(forkVal, &status, 0) == -1) {
                    perror("error in wait");
                    exit(1);
                }
                if (commands[i].fdStdin != 0) {
                    if (close(commands[i].fdStdin) == -1) {
                        perror("close failed!");
                        exit(1);
                    }
                }

                if (commands[i].fdStdout != 1) {
                    if (close(commands[i].fdStdout) == -1) {
                        perror("close failed!");
                        exit(1);
                    }
                }

            }
        }

    }
    return 0;
}
