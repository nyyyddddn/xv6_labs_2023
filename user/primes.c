#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int read_fd) {
    int p;
    
    if (read(read_fd, &p, sizeof(int)) == 0) {
        close(read_fd);
        exit(0);
    }
    printf("prime %d\n", p);

    int next_read_fd[2];
    pipe(next_read_fd);

    if (fork() == 0) {
        close(next_read_fd[1]);
        sieve(next_read_fd[0]);
    } else {
        close(next_read_fd[0]);
        int num;
        while (read(read_fd, &num, sizeof(int)) != 0) {
            if (num % p != 0) {
                write(next_read_fd[1], &num, sizeof(int));
            }
        }
        close(next_read_fd[1]);
        close(read_fd);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]){

    int nums[34];
    for (int i = 0; i < 34; i++) {
        nums[i] = i + 2;
    }

    int initial_fd[2];
    pipe(initial_fd);

    if (fork() == 0) {
        close(initial_fd[1]);
        sieve(initial_fd[0]);
    } else {
        close(initial_fd[0]);
        for (int i = 0; i < 34; i++) {
            write(initial_fd[1], &nums[i], sizeof(int));
        }
        close(initial_fd[1]);
        wait(0);
        exit(0);
    }

    return 0;
}
