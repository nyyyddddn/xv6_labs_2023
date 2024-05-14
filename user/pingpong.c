#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){

    // pN[0] read pN[1] write
    int p1[2]; // pipe from parent to child 
    int p2[2]; // pipe from child to parent
    
    pipe(p1);
    pipe(p2);

    
    int pid = fork();
    if (pid > 0){
        char byte = 'A';
        write(p1[1],&byte,1);

        wait(0); // wait child process exit
        read(p2[0],&byte,1);

        if(byte == 'B'){
            printf("%d: received pong\n", getpid());
        }

        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
    else if (pid == 0){
        char buf;
        read(p1[0],&buf,1);

        if(buf == 'A'){
            printf("%d: received ping\n", getpid());
            buf = 'B';
            write(p2[1],&buf,1);
        }
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else{
        printf("Error: fork error!\n");
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        exit(0);
    }
}