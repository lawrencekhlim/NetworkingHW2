/* Creates a datagram server.  The port
 number is passed as an argument.  This
 server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void recurse_addition (int sock, struct sockaddr_in from, socklen_t fromlen, char buffer [], int buffer_length) {
    if (buffer_length == 1) {
        int n = sendto(sock,buffer, 1, 0,(struct sockaddr *)&from,fromlen);
        if (n < 0) error ("sendto");
    } 
    else {


	int total = 0;
        for (int i = 0; i < buffer_length; i++) {
            total+= buffer[i] -48;
	}
        char new_buffer [128];

	sprintf (new_buffer, "%d", total);
	int count = 0;
	while (new_buffer[count] != '\0' && count < 128) {
            count ++;
	}
        
	//write (1, new_buffer, count);
	int n = sendto (sock, new_buffer, count, 0,  (struct sockaddr *)&from,fromlen);
        recurse_addition (sock, from, fromlen, new_buffer, count);

	if (n < 0) error ("sendto");
    }
}	

int main(int argc, char *argv[])
{
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];
    
    
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }
    
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("Opening socket");
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));
    if (bind(sock,(struct sockaddr *)&server,length)<0)
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
    while (1) {
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
        if (n < 0) error("recvfrom");
        //write(1,"Received a datagram: ",21);
        //write(1,buf,n);
        int count = n;
        int non_num = 0;
	int ignore_last = 0;
        for (int i = 0; i < count; i++) {
            if (i != count-1 && (buf [i] < '0' || buf [i] > '9')) 
         	non_num = 1;
	    else if (i == count-1) {
		if (buf [i] < '0' || buf [i] > '9')
		    ignore_last = 1;

	    } 
		     
	}
        
	if (non_num) {
             n = sendto(sock,"Can not compute",17,
                   0,(struct sockaddr *)&from,fromlen);
	}
	else {

            char newstr [1024];
            strcpy(newstr, buf);            
	    if (ignore_last) {
	        newstr [count-1] = '\0';
	    }


          //   n = sendto(sock,"Got your message\n",17,
          //         0,(struct sockaddr *)&from,fromlen);

	     recurse_addition (sock, from, fromlen, newstr, count-1);
	}
        if (n  < 0) error("sendto");
    }
    return 0;
}

