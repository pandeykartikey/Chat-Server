#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <string.h>

#define MAX_CLIENTS 10
#define PORT 5001

struct clients {
   int sock;
   char user[8]; 
};

int main( int argc, char *argv[] ) {
   printf("Socket Opened\n");
   int sockfd, newsockfd,clilen;
   char buffer[1024];
   struct sockaddr_in serv_addr, cli_addr;
   int n , numfds = 0;
   struct pollfd poll_set[MAX_CLIENTS];
   struct clients cli[MAX_CLIENTS];
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   printf("Socket Opened\n");
   

   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(PORT);
   

   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   else{
      printf("binded\n");
   }
   poll_set[0].fd = sockfd;
   poll_set[0].events = POLLIN;
   numfds++;

   listen(sockfd,5);
   printf("listening\n");
   clilen = sizeof(cli_addr);
   
   while (1) {
      int fd_index;
      poll(poll_set, numfds, 1000000000);
      for(fd_index = 0; fd_index < numfds; fd_index++)
      {
         if( poll_set[fd_index].revents & POLLIN ){
            if(fd_index == 0){
               newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
               poll_set[numfds].fd = newsockfd;
               poll_set[numfds].events = POLLIN;
               printf("Connecting\n");
               send(poll_set[numfds].fd , "Username :" , 10 , 0 );
               bzero(cli[numfds].user,8);
               read( poll_set[numfds].fd , cli[numfds].user, 8);
               cli[numfds].sock = newsockfd; 
               printf("%s\n",cli[numfds].user);
               numfds++;
            }
            else{
               bzero(buffer,1024);
               n = read( poll_set[fd_index].fd , buffer, 1024);
               if ( n == 0)  
               {  
                 close( poll_set[fd_index].fd );  
                 poll_set[fd_index].fd = 0;  
               }       
               else
               {  
                  printf("%s\n",buffer);
                  char *result = malloc(strlen(cli[fd_index].user)+strlen(buffer)+1);
                  strcpy(result, cli[fd_index].user);
                  strcat(result, " : ");
                  strcat(result, buffer);
                  for (int j= 1 ; j<numfds;j++){

                     send(poll_set[j].fd ,result , strlen(result) , 0 );  
                  
                  }
               }  
            }      
         }
      }		
   }
   return 0;
}
