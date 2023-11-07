#include <iostream>
#include <unistd.h>
#include <cstring>
#include <ctime>
#include <sys/time.h>

using namespace std;

int main (int argc, chat *argv[]){
    //if number of arguments is less than 2, throw an error
    if (argc < 2 || argc > 2) {
        cout << "Incorrect number of arguments";
        return 1;
    }

    int pipe_cmd[2]; //pipe that will send the command from the parent to the child process
    int pipe_time[2]; //pipe that will send the starting timestamp from the child to the parent process
    struct timeval start, end;

    if(pipe(pipe_cmd) < 0 || pipe(pipe_time) < 0){
        cout << "Pipes failed";
        return 2;
    }

    pid_t pid = fork();

    if(pid < 0) {
        cout << "Can't fork the process";
        return 3;
    }

    else if (pid == 0){ //child process
        close(pipe_cmd[1]);

        char command[256]; //buffer to store the command string
        read(pipe_cmd[0], command, sizeof(command));
        close(pipe_cmd[0]); //close the read end, done with pipe

        //get the start timestamp
        gettimeofday(&start, NULL);

        //send start time to parent
        write(pipe_time[1], &start, sizeof(start));
        close(pipe_time[1]); //close write end, done with pipe

        //execute the command
        execlp(command, command, (char *)NULL);
        cout << "Error executing command";
        return 4;
    } else { //parent process
        close(pipe_cmd[0]);
        close(pipe_time[1]);

        //write command to pipe
        write(pipe_cmd[1], argv[1], strlen(argv[1] + 1));
        close(pipe_cmd[1]); //close write end of pipe

        waitpid(pid, NULL, 0); //wait for child to finish

        gettimeofday(&end, NULL); //get the end timestamp

        read(pipe_time[0], &start, sizeof(start));
        close(pipe_time[0]); //close read end of pipe

        //calculate and print the elapsed time
        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;

        cout << "Elapsed time: " << seconds << "." << microseconds << "seconds" << endl;
    }

    return 0;
}
