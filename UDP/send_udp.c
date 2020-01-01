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

int main(int argc, char *argv[])
{
    int socket_fd;
    struct sockaddr_in  dest;
    struct hostent *hostptr;
    char msgbuf[32] = {0};

    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);         //Create a new socket
    bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
    hostptr = gethostbyname(argv[1]);      // Return entry from host data base for host with NAME.
    dest.sin_family = (short) AF_INET;      //this  assign the socket address and port

    /* Copy N bytes of SRC to DEST (like memmove, but args reversed).  */
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
    dest.sin_port = htons((u_short)0x3030);
    strcpy(msgbuf,"Raphael-client");/* IMPORTANT! */
    //Send socket use system call
    sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,sizeof(dest));

    printf("Got data: ");
    int i =0;
    while (msgbuf[i]){
        printf("%c",msgbuf[i]);
        i++;
    }
    printf("\n");

    return 0;
}

