#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int i, p[2], p_close[2];
    pipe(p);
    pipe(p_close);

    if(fork() == 0) {
        //child process
        int p2[2], base_prime, num, flag = 0;
        close(p[1]); close(p_close[0]);
        read(p[0], &base_prime, 4);
        printf("prime %d\n", base_prime);
        while(read(p[0], &num, 4) == 4) {
            if(num % base_prime != 0) {
                if(!flag) {
                    pipe(p2);
                    if(fork() == 0) {
                        close(p[0]); close(p2[1]);
                        p[0] = p2[0];
                        read(p[0], &base_prime, 4);
                        printf("prime %d\n", base_prime);
                    } else {
                        flag = 1;
                        close(p2[0]);
                        write(p2[1], &num, 4);
                    }
                } else {
                    write(p2[1], &num, 4);
                }
            }
        }
        close(p2[1]);
        close(p[0]); close(p_close[1]);
    } else {
        //parent process
        close(p[0]);
        close(p_close[1]);
        for(i = 2; i <= 35; ++i) {
            write(p[1], &i, 4);
        }
        close(p[1]);

        // wait for all child process.
        // when p_close[1] is closed, read will get 0x00.
        read(p_close[0], &i, 1);
        close(p_close[0]);
    }
    exit(0);
}
