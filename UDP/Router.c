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
#include <time.h>
#include <stdlib.h>

void printsin(struct sockaddr_in *s, char *str1, char *str2) {
    printf("%s\n", str1);
    printf("%s: ", str2);
    printf("ip= %s, port %d", inet_ntoa(s->sin_addr),s->sin_port);
    printf("\n");
}

int main(int argc, char *argv[])
{
    int socket_fd, cc, fsize;
    struct sockaddr_in  s_in,client,server,temp;
    char msg[32] = {0};

    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

    bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

    //Router
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
    s_in.sin_port = htons((u_short)0x2020);

    //Server
    server.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
    server.sin_family = (short) AF_INET;
    server.sin_port = htons((u_short)0x3232);

    //Client
    client.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
    client.sin_family = (short) AF_INET;
    client.sin_port = 0;

    printsin( &s_in, "UDP-Router", "Local socket is");
    fflush(stdout);     //clear buffer

    bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));        //use system call to connect socket to address

    double x = 0,randomNum = 0;
    srand(time(NULL));   // Initialization, should only be called once.

    printf("Enter a threshold number");
    scanf("%lf",&x);


    for(;;) {
        fsize = sizeof(temp);
        recvfrom(socket_fd, msg, sizeof(msg), 0, (struct sockaddr *)&temp, &fsize);

        if (temp.sin_port == server.sin_port){
            sendto(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&client, sizeof(client));
        }
        else{
            client.sin_port=temp.sin_port;
            randomNum = (double)rand()/RAND_MAX;
            printf("%lf",randomNum);

            if (randomNum<x)
                sendto(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&server, sizeof(server));
            else
                sendto(socket_fd, "The package was dropped", sizeof(msg), 0, (struct sockaddr *)&client, sizeof(client));
        }


        fflush(stdout);
    }

    return 0;
}


