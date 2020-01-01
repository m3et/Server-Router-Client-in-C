#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80
#define MAXBUF 1024
#define LEN 50
#define HTTP "http://"

int main(int argc, char* argv[])
{
    int sock, bytsRead;
    struct sockaddr_in cli_name;
    struct addrinfo* res;
    struct sockaddr_in* socket_addr;
    char* url;
    char hostname[LEN], path[LEN];
    char data[MAXBUF];

    //check if number of argument is right
    if (argc != 2) {
        perror("Usage: hostnamelookup <hostname>\n");
        exit(1);
    }

    //set the hostname to the first pass argument
    url = argv[1];

    //check if hostname is has protocol element HTTP
    if(strncmp(url,HTTP,7)){
        printf("the URL doesnt have http:// \n");
        exit(1);
    }

    //cut the url string to an hostname and path
    sscanf(url,"http://%99[^/]/%99[^\n]",hostname,path);

    //check if hostname is valid
    if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
        fprintf(stderr, "Error in resolving hostname %s\n", hostname);
        exit(1);
    }

    printf("Client is alive and establishing socket connection.\n");

    //creat an new socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    { perror ("Error opening channel");
        close(sock);
        exit(1);
    }

    socket_addr = (struct sockaddr_in*)res->ai_addr;

    memset((char *)&cli_name,0, sizeof(cli_name));
    cli_name.sin_family = AF_INET;
    cli_name.sin_addr.s_addr = inet_addr(inet_ntoa(socket_addr->sin_addr));
    cli_name.sin_port = htons(PORT);

    //connect to server
    if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
        close(sock);
        exit(1);
    }

    sprintf(data,"GET %s HTTP/1.0\nHOST:%s\n\n",url,hostname);
    send(sock, data, strlen(data),0);

    bytsRead = recv(sock,data, sizeof(data),0);
    while (bytsRead > 0){
        printf("%s",data);
        bzero(data, sizeof(data));
        bytsRead = recv(sock,data, sizeof(data),0);
    }

    printf("Exiting now.\n");

    close(sock);
    exit(0);

}
