#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MY_MAGIC 'x'
#define CMD_CLEAR _IO(MY_MAGIC, 0)
#define CMD_FILL  _IOW(MY_MAGIC, 1, char)

int main()
{
    int fd = open("/dev/ioctl_char_device", O_RDWR);
    if(fd < 0){ 
        perror("open"); 
        return 1; 
    }

    ioctl(fd, CMD_FILL, 'A');
    printf("Wrote A’s into kernel buffer\n");

    ioctl(fd, CMD_CLEAR);
    printf("Cleared kernel buffer\n");

    close(fd);
    return 0;
}
