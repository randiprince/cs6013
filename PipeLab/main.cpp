#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {
    int fd[2]; // file descriptors read and write
    int status;
    if (pipe(fd) < 0) {
        perror("error! pipe failed!");
        exit(1);
    }
    pid_t forkVal = fork();
    if (forkVal < 0) {
        perror("error! forK failed!");
        exit(1);
    } else if (forkVal == 0) { // child process
        close(fd[1]); // close write end

        //read in message length first sent by parent to create buffer
        int receiveLength = 0;
        read(fd[0], &receiveLength, sizeof(receiveLength));

        char buffer[receiveLength]; // buffer based on received message length

        read(fd[0], buffer, sizeof(buffer)); // now read in 2nd write from parent that is the message
        std::cout << buffer;
        close(fd[0]);
    } else { //parent process
        if (argc <= 1) {
            perror("nothing to print! please pass an argument to the parent.");
            exit(1);
        }
        close(fd[0]); // close read end of parent
        // send length of message first then read in from child
        int receiveLength = strlen(argv[1]); // get length of argument to send as buffer
        char receiveBuff[receiveLength];
        write(fd[1], &receiveBuff, sizeof(receiveLength)); // send buffer with length of msg

        write(fd[1], argv[1], strlen(argv[1]) + 1); // write to child message and length
        waitpid(0, &status, 0); // wait for child process to exit
        close(fd[1]);
    }
    return 0;
}
