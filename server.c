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
};

struct client* head;

void addClient(int sock){
   printf("%s",head);
   struct client* temp = head;
   if(temp == NULL){
      printf("Adding HEAD\n");
      head = (struct client*) malloc(sizeof(struct client));
      head->sock = sock;
      head->next = NULL;
      printf("%s\n",head );
   }
   else{
      while(temp!=NULL)
         temp = temp->next;
      temp->next = (struct client*) malloc(sizeof(struct client));
      temp->next->sock = sock;
      printf("Adding node\n");
      temp->next->next = NULL;
   }
}
void showAll(){
struct client* temp = head;
   while(temp!=NULL){
      int n = write(temp->sock,"I got your message",18);
      printf("Showing message to client\n");
      temp=temp->next;  
   }
}
int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   head = NULL;
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
   		addClient(newsockfd);
      printf("%i\n",newsockfd);
      /* Create child process */
      pid = fork();
      if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the client process */
         // close(sockfd);
         doprocessing(newsockfd);
         exit(0);
      }
      else {
         close(newsockfd);
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
   showAll();
   // n = write(sock,"I got your message",18);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   }
}