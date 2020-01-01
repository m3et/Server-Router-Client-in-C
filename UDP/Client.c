#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

void printsin(struct sockaddr_in *s, char *str1, char *str2) {
    printf("%s\n", str1);
    printf("%s: ", str2);
    printf("ip= %s, port %d", inet_ntoa(s->sin_addr),s->sin_port);
    printf("\n");
}

int main(int argc, char *argv[])
{
    int socket_fd;
    struct sockaddr_in  dest;
    struct hostent *hostptr;
    char msgbuf[32] = {0};

    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
    bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
    hostptr = gethostbyname(argv[1]);
    dest.sin_family = (short) AF_INET;
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
    dest.sin_port = htons((u_short)0x2020);

    fflush(stdout);     //clear buffer

    printf("Send massege:");
    scanf("%s",msgbuf);
    while (strcmp(msgbuf,"exit")){

        sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,sizeof(dest));

        unsigned int fsize = sizeof(dest);
        int i =0;
        recvfrom(socket_fd,msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,&fsize);
        printf("Got massege: ");
        while (msgbuf[i]){
            printf("%c",msgbuf[i]);
            i++;
        }
        printf("\n");
        fflush(stdout);
        printf("Send massege:");
        scanf("%s", msgbuf);
    }
    return 0;
}


