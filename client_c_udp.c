/* UDP client in the internet domain */

// Used sample code from October 12th discussion section
// Lawrence Lim
// Perm: 4560892

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
int main(int argc, char *argv[])
{
    int sock, n;
    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[129];
    
    if (argc != 3) { printf("Usage: server port\n");
        exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");
    
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");
    
    bcopy((char *)hp->h_addr,
          (char *)&server.sin_addr,
          hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);
    printf("Enter string: ");
    bzero(buffer,129);
    fgets(buffer,129,stdin);
    n=sendto(sock,buffer,
             strlen(buffer),0,(const struct sockaddr *)&server,length);
    if (n < 0) error("Sendto");

    n = recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length);
    if (n < 0) error("recvfrom");

    write(1,"From server: ",13);
    write(1,buffer,n);
    write(1, "\n", 1);

    while (n > 1) {
        n = recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length);
        if (n < 0) error("recvfrom");


        write(1,"From server: ",13);
        write(1,buffer,n);
	write(1, "\n", 1);
    }
    close(sock);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
