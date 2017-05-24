#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <string.h>

#define MAX_CLIENTS 10
#define PORT 5001

int main( int argc, char *argv[] ) {
   printf("Socket Opened");
   int sockfd, newsockfd,clilen;
   char buffer[1024];
   struct sockaddr_in serv_addr, cli_addr;
   int numfds = 0;
   struct pollfd poll_set[MAX_CLIENTS];
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   printf("Socket Opened");
   

   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(PORT);
   

   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   else{
      printf("binded");
   }
   poll_set[0].fd = sockfd;
   poll_set[0].events = POLLIN;
   numfds++;

   listen(sockfd,5);
   printf("listening");
   clilen = sizeof(cli_addr);
   
   while (true) {
      int fd_index;
      poll(poll_set, numfds, 10);
      for(fd_index = 0; fd_index < numfds; fd_index++)
      {
         if( poll_set[fd_index].revents & POLLIN ){
            if(fd_index == 0){
               printf("Connecting");
               newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
               poll_set[numfds].fd = newsockfd;
               poll_set[numfds].events = POLLIN;
               numfds++;
            }
            else{
               if (read( poll_set[fd_index].fd , buffer, 1024) == 0)  
               {  
                 close( poll_set[fd_index].fd );  
                 poll_set[fd_index].fd = 0;  
               }  
                    
               else
               {  
                  printf("Sending\n"); 
                  send(poll_set[fd_index].fd , buffer , strlen(buffer) , 0 );  
               }  
            }      
         }
      }
      
		
   }
   return 0;
}
