#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#define PORT 5001

void recieve(int sock){
      char buffer[256];
      bzero(buffer,256);
      printf("Recieving  a message\n");
      int n = read(sock, buffer, 256);
      if (n < 0) {
         perror("ERROR reading from socket");
       exit(1);
      }
      printf("%s \n",buffer);
}
void send_msg(int sock){
   char buffer[256];
   bzero(buffer,256);
   printf("Send a message\n");
   fgets(buffer,255,stdin);
   int n = write(sock, buffer, strlen(buffer));
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
}
int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[256];
   int pid;
   
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   // if (server == NULL) {
   //    fprintf(stderr,"ERROR, no such host\n");
   //    exit(0);
   // }
   memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
   // bzero((char *) &serv_addr, sizeof(serv_addr));
   // serv_addr.sin_family = AF_INET;
   // bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   // serv_addr.sin_port = htons(portno);
   if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        perror("Invalid address/ Address not supported ");
        exit(1);
    }
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
   printf("here\n");
   recieve(sockfd);
   send_msg(sockfd);
   send_msg(sockfd);
   recieve(sockfd);
   
   /* Now ask for a message from the user, this message
      * will be read by server
   */
   
   /* Send message to the server */
   // pid=fork();
   // if (pid < 0) {
         // perror("ERROR on fork");
         // exit(1);
      // }
   // if(pid == 0){

//    if(fork()==0){while(1)send_msg(sockfd);}
// // }
//    // }
   
//    /* Now read server response */
//    // pid=fork();
//    // if (pid < 0) {
//          // perror("ERROR on fork");
//          // exit(1);
//       // }
//    // if(pid == 0){
//       // while(1){
//       else{while(1)recieve(sockfd);
//       }   
   // }

   return 0;
}