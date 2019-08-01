#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void err(char *msg) 
{
    perror(msg);
    exit(1);
}
void reverse(char *s)
{
    int length, c;
    char *begin, *end, temp;

    length = strlength(s);
    begin = s;
    end = s;

    for (c = 0; c < length-1; c++)
    {
        end++;
    }
    for (c = 0; c < length/2; c++)
    {
        temp = *end;
        *end = *begin;
        *begin = temp;

        begin++;
        end--;
    }
}

int strlength(char* pt)
{
    int c = 0;
    while(*(pt + c) != '\0')
    c++;

    return c;
}

int main(int argc, char *argv[])
{
    int sockfd;
    int newsockfd;
    int port;
    int client;
    char buffer[256];
    char rvrsbuffer[256];
    struct sockaddr_in srvr_addr, clnt_addr;
    int n;
    // Make sure enough arguments are passed
    if (argc < 2)
    {
        fprintf(stderr, "Port number, you dumbass\n");
        exit(1);
    } 
    //Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //Make sure socket was created
    if (sockfd < 0) 
    {
        err("Can't open socket");
    }

    bzero((char *) &srvr_addr, sizeof(srvr_addr));
    // Prepare the server address
    port = atoi(argv[1]);

    srvr_addr.sin_family = AF_INET;
    srvr_addr.sin_addr.s_addr  = INADDR_ANY;
    srvr_addr.sin_port = htons(port);
    //Make sure the socket address was binded
    if(bind(sockfd, (struct sockaddr *) &srvr_addr, sizeof(srvr_addr)) < 0)
    {
        err("couldn't bind");
    }
    //Start listening 
    listen(sockfd, 1); //defines how many clients server can listen to
    client = sizeof(clnt_addr);
    //Accept client address
    newsockfd = accept(sockfd, (struct sockaddr *) &clnt_addr, &client);
    //Make sure you accepted client addr
    if (newsockfd < 0)
    {
        err("Couldn't accept");
    }
    //Clear the buffer
    bzero(buffer, 256);
    //Read the message from the client
    n = read(newsockfd, buffer, 255);
    //Makes sure you could read from the client
    if (n < 0)
    {
        err("Couldn't read from socket");
    }


    //Reverse the client message
    bzero(rvrsbuffer, 256);
  //  for (int i = 0; i < 256; i++ )
//    {
 //       rvrsbuffer[i] = buffer[4 - i];
 
   // }
   
    printf("Reversed message is: %s\n", rvrsbuffer);
       //Print the received message
    printf("The message is: %s\n", buffer);
    reverse(buffer);
    //Send back reversed message
    n = write(newsockfd, buffer, 255);
    //Make sure reversed message was send
    if (n < 0) 
    {
        err("Couldn't write to socket");
    }
    
    return 0;
}