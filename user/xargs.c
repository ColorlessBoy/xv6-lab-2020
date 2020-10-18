#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[MAXARG+1], tmp;
    int index = 0, i;
    
    if(argc < 2) {
        fprintf(2, "Usage: xargs COMMAND [arg1 arg2 ...]\n");
        exit(1);
    }

    for(i = 0; i < argc-1; i++) {
        argv[i] = argv[i+1];
    }
    argv[argc-1] = buf;

    while(read(0, &tmp, 1)) {
        if(tmp == '\n') {
            buf[index] = 0;
            if(fork() == 0) {
                exec(argv[0], argv);
                exit(0);
            } else {
                wait(0);
            }
            index = 0;
        } else {
            buf[index++] = tmp;
        }
    }
    if(index > 0) {
        buf[index++] = 0;
        exec(argv[0], argv);
    }
    exit(0);
}

