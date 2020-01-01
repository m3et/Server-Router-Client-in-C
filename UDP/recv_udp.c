#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  printf("ip= %s, port %d", inet_ntoa(s->sin_addr),s->sin_port);
  printf("\n");
}

int main(int argc, char *argv[])
{
    int socket_fd, fsize;
    struct sockaddr_in  s_in, from;
    char msg[32] = {0};

    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);        //Create a new socket

    bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this */

    /*
     * this  assign the socket address and port
     * I think its using system call for the port and adders
     */
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
    s_in.sin_port = htons((u_short)0x3030);

    printsin( &s_in, "UDP-SERVER", "Local socket is");
    fflush(stdout);     //Clean the buffer

    /* Give the socket FD the local address ADDR (which is LEN bytes long).  */
    bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

    for(;;) {
        fsize = sizeof(from); // return size of structure
        //Read bytes into BUF through socket FD and store the actual size of the address use system call
        recvfrom(socket_fd,msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
        /**
         * print the data received from the client
         */
        printsin( &from, "UDP-SERVER", "Packet from");
        printf("Got data: ");
        int i =0;
        while (msg[i]){
            printf("%c",msg[i]);
            i++;
        }

        //Send resound to client
        strcpy(msg,"Raphael-server");
        sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,sizeof(from));

        fflush(stdout);
    }

    return 0;
}
