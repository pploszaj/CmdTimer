#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main (int argc, chat *argv[]){
    //if number of arguments is less than 2, throw an error
    if (argc < 2 || argc > 2) {
        cout << "Incorrect number of arguments";
        return 1;
    }

    int pipe_cmd[2]; //pipe that will send the command from the parent to the child process
    int pipe_time[2]; //pipe that will send the starting timestamp from the child to the parent process

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


    }
}
