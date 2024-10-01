#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUFLEN 63

int non_persistance_write(const char *path, const char *buf, const size_t n);
int persistance_write    (const char *path, const char *buf, const size_t n);
int append_only_logs     (const char *path, const char *buf, const size_t n);

int main(void)
{
    non_persistance_write("./data.txt", "Hello World0\n", strlen("Hello World0\n"));
    append_only_logs("./data.txt", "Hello World1\n", strlen("Hello World1\n"));
    persistance_write("./data.txt", "Hello World2\n", strlen("Hello World2\n"));
    append_only_logs("./data.txt", "Hello World1\n", strlen("Hello World1\n"));
}

int non_persistance_write(const char *path, const char *buf, const size_t n)
{
    const int fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    int bytes_written = write(fd, buf, n);
    close(fd);

    return bytes_written;
}

int persistance_write(const char *path, const char *buf, const size_t n)
{
    srandomdev();

    char tmp_path[MAX_BUFLEN];
    sprintf(tmp_path, "%s.tmp.%ld.txt", path, random());

    const int fd = open(tmp_path, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) printf("Unable to open specified file");

    int bytes_written = write(fd, buf, n);
    if (bytes_written < 0)
    {
        remove(tmp_path);
        return bytes_written;
    }

    if (fsync(fd) < 0)
    {
        remove(tmp_path);
        printf("Unable to flush the write stream to drive\n");
    }
    //OSX specifix code below to flush the write stream to drive :: read man fcntl
    //Comment this part if on other OS
    if (fcntl(fd, F_FULLFSYNC) < 0) //F_FULLFSYNC forces data to be written in the itended order on disk
    {
        remove(tmp_path);
        printf("Unable to flush the write stream to drive\n");
    }

    close(fd);

    if (rename(tmp_path, path) < 0)
    {
        printf("Error writing to file\n");
        return -1;
    }

    return bytes_written;
}

/*
 * Append only logs are used because fsync only ensures flushing data to the disk
 * metadata could still be corrupted or half-written during rename to avoid this
 * file could be made append only to keep the writing ordered and previous data intact
*/
int append_only_logs(const char *path, const char *buf, const size_t n)
{
    const int fd = open(path, O_RDWR|O_CREAT|O_APPEND, S_IRWXU);
    if (fd < 0) printf("Unable to open specified file");

    int bytes_written = write(fd, buf, n);
    if (bytes_written < 0) return bytes_written;

    if (fsync(fd) < 0) printf("Unable to flush the write stream to drive\n");
    //OSX specifix code below to flush the write stream to drive :: read man fcntl
    //Comment this part if on other OS
    if (fcntl(fd, F_FULLFSYNC) < 0) printf("Unable to flush the write stream to drive\n"); //F_FULLFSYNC forces data to be written in the itended order on disk

    close(fd);
    return bytes_written;
}
