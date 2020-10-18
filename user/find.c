#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
getFilename(char *path)
{
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;

    p++;
    return p;
}

void
find(char *filename, char *path)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", buf);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", buf);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        p = getFilename(path);
        if(strcmp(filename, p) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 
            || strcmp(de.name, ".") == 0
            || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, strlen(de.name));
            p[strlen(de.name)] = 0;
            find(filename, buf);
        }
        break;
    }
    close(fd);
    return;
}

int
main(int argc, char *argv[])
{
    if(argc <= 2){
        fprintf(2, "Usage: find DIRPATH FILENAME\n");
        exit(1);
    }
    find(argv[2], argv[1]);
    exit(0);
}

