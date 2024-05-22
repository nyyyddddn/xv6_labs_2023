#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define MAXARGS 0x20
#define BUFSIZE 0x500

int main(int argc, char *argv[]) {

    char buf[BUFSIZE];
    int input_data_len = 0;
    char *args[MAXARGS];
    int i;

    if (argc == 1) {
        fprintf(2, "e.g. find . | xargs file\n");
        exit(1);
    }

    if (argc > MAXARGS - 5) {
        fprintf(2, "Error: too many arguments\n");
        exit(1);
    }

    for (i = 0; i < argc; i++) {
        args[i] = argv[i];
    }

    while (read(0, buf + input_data_len, 1) == 1) {

        if (buf[input_data_len] == '\n' || buf[input_data_len] == ' ') {
            buf[input_data_len] = 0;
            args[argc] = buf;
            args[argc + 1] = 0;

            if (fork() == 0) {
                exec(argv[1], args + 1);
                fprintf(2, "exec failed\n");
                exit(1);
            } else {
                wait(0);
            }

            input_data_len = 0;
        } else {
            input_data_len++;
            if (input_data_len >= BUFSIZE - 1) {
                fprintf(2, "Error: input too long\n");
                exit(1);
            }
        }
    }

    if (input_data_len > 0) {
        buf[input_data_len] = 0;
        args[argc] = buf;
        args[argc + 1] = 0;

        if (fork() == 0) {
            exec(argv[1], args);
            fprintf(2, "exec failed\n");
            exit(1);
        } else {
            wait(0);
        }
    }

    exit(0);
}
