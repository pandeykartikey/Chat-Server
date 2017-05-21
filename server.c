#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

void doprocessing (int sock);

struct client
{
   int sock;
   struct client* next;
}* head =NULL;

// struct client* head;

void addClient(int sock){
   printf("here");
   printf("%s",head);
   struct client* temp = (struct client*) malloc(sizeof(struct client));
   temp->next = NULL;
   temp->sock = sock;
   struct client* temp2 = head;
   if(head == NULL){
      printf("Adding HEAD\n");
      head = temp;
      printf("%s\n",head );
   }
   else{
      while(temp2->next!=NULL)
         temp2 = temp2->next;
      printf("Adding node\n");
      temp2->next = temp;
   }
}
void showAll(char buffer[],char username[]){
struct client* temp = head;
int i = 0;
while(temp!=NULL){
   temp=temp->next;
   i++;
}
printf("%i",i);
temp = head;
   while(temp!=NULL){
      int n = write(temp->sock,buffer,18);
      if (n < 0) {
         perror("ERROR writing to socket");
         exit(1);
      }
      printf("Showing message to client %i\n",temp->sock);
      temp=temp->next;  
   }
}
int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   // head = NULL;
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   
   /* Now start listening for the clients, here
      * process will go in sleep mode and will wait
      * for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   while (1) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }
      printf("%i\n",newsockfd);
         addClient(newsockfd);
      /* Create child process */
      
      if(fork() == 0) {
         /* This is the client process */
         close(sockfd);
         doprocessing(newsockfd);
         exit(0);
      }
      
		
   } /* end of while */
}

void doprocessing (int sock) {
   int n;
   char buffer[256];
   char username[10];
   n = write(sock,"Enter a username:",17);
   bzero(buffer,256);
   n = read(sock,username,10);
   printf("%s\n",username);
   while(1){
   bzero(buffer,256);
   n = read(sock,buffer,255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   printf("%sHere is the message: %s\n",username,buffer);
   showAll(buffer,username);
   // n = write(sock,"I got your message",18);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   }
}