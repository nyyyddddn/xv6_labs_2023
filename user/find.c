#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // open path
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "Error: find cannot open %s\n", path);
        return;
    }

    // get directory statistics
    if(fstat(fd, &st) < 0) {
        fprintf(2, "Error: find cannot stat %s\n", path);
        close(fd);
        return;
    }

    // ensure it is a directory
    if(st.type != T_DIR) {
        fprintf(2, "Error: find %s is not a directory\n", path);
        close(fd);
        return;
    }

    // read the directory entries
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    
    // get each item
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0) {
            fprintf(2, "Error: findcannot stat %s\n", buf);
            continue;
        }
        // Skip "." and ".."
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        if(st.type == T_DIR) {
            // find sub directory
            find(buf, filename);
        } else if(st.type == T_FILE) {
            // find the file
            if(strcmp(de.name, filename) == 0) {
                printf("%s\n", buf);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {

    if(argc < 3) {
        fprintf(2, "e.g. find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);

    exit(0);
}
