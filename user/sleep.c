#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    
    int time = 0;

    if (argc == 1 || argc > 2){
        printf("Error: Please provide the number of seconds to sleep\n e.g. sleep 5\n");
        exit(1);
    }

    time = atoi(argv[1]);
    if (time > 0 && time < 0x1000){
        printf("Zzzzzzzzz.....\n");
        sleep(time);
    }
    exit(0);
}