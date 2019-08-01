 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>

 void err(char * msg)
 {
     perror(msg);
     exit(0);
 }

 int main(int argc, char *argv[]) 
 {
     int sockfd;
     int port;
     int n;
     struct sockaddr_in srvr_addr;
     struct hostent *srvr;

     char buffer[256];
    // Make sure enough arguments was passed
     if (argc < 3) 
     {
         fprintf(stderr, "usage %s hostname port\n", argv[0]);
         exit(0);
     }

     port = atoi(argv[2]);

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0)
     {
          err("Couldn't open socket");
     }
     //Get the server name
     srvr = gethostbyname(argv[1]);
     if (srvr == NULL)
     {
         fprintf(stderr, "Couldn't find host\n");
         exit(0);
     }
    
    bzero((char *) &srvr_addr, sizeof(srvr_addr));
    srvr_addr.sin_family = AF_INET;

    bcopy((char *)srvr->h_addr, (char *)&srvr_addr.sin_addr.s_addr,srvr->h_length);
    
    srvr_addr.sin_port = htons(port);

    if (connect(sockfd, &srvr_addr, sizeof(srvr_addr)) < 0)
    {
        err("Couldn't connect");
    }

    printf("Say something I'm giving up on you...");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        err("Couldn't write to socket");
    }
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        err("Coudn't read from socket");
    }

    printf("Recived message is: %s\n", buffer);

    return 0;

 }