#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "game.h"

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
   char message[BUF_SIZE];
   char file_name[BUF_SIZE];
   int str_len;
   int end = 1;
   struct sockaddr_in serv_adr;
   int start_count = 0;
   char cnum[3], cindex[3];
   int slash_index = 0;

   node_index = 0;
   score = 0;

   if (argc != 3)
   {
      printf("Usage : %s <IP> <port>\n", argv[0]);
      exit(1);
   }

   sock = socket(PF_INET, SOCK_STREAM, 0);
   if (sock == -1)
      error_handling("socket() error");

   memset(&serv_adr, 0, sizeof(serv_adr));
   serv_adr.sin_family = AF_INET;
   serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
   serv_adr.sin_port = htons(atoi(argv[2]));

   if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
      error_handling("connect() error!");
   else
      puts("Connected...........");

   while (1)
   {


/*
      fputs("Input message(Q to quit): ", stdout);
      fgets(message, BUF_SIZE, stdin);
      message[strlen(message)-1] = 0;
      write(sock, message, strlen(message));
*/
      str_len = read(sock, message, BUF_SIZE - 1);
      message[str_len] = 0;
      //printf("message from server: %s\n", message);
//	fprintf(stdout, "message from server : %s\n", message);

	if(strcmp(message, "start") == 0){
//		fprintf(stdout, "check1\n");
		end = menu_loop();

		if(end == 0){
		   break;
		}else if(end == 1){
			write(sock, "korean", 7);
		}else if(end == 2){
			write(sock, "english", 8);
		}
	}else if(!strcmp(message, "korean start")){
//		fprintf(stdout, "check2\n");

		fprintf(stdout, "korean start !!\n");
		main_loop();
	}
	else if(!strcmp(message, "english start")){
//		fprintf(stdout, "check3\n");

		fprintf(stdout, "english start !!\n");
		main_loop();
	}
	else if(strcmp(message, "setcondition1") == 0){
//		fprintf(stdout, "check4\n");

		set_condition(1);
	}
	else if(strcmp(message, "setcondition2") == 0){
//		fprintf(stdout, "check5\n");

		set_condition(2);
	}
	else if(strcmp(message, "End of Message") == 0){
//		fprintf(stdout, "check6\n");
		break;
	}
	else if(message[0] == '#'){
//		fprintf(stdout, "# is first!\n");
		cnum[0] = message[1];
		if(message[2] == '/'){
			cnum[1] = '\0';
			slash_index = 2;
		}else{
			cnum[1] = message[2];
			cnum[2] = '\0';
			slash_index = 3;
		}
		cindex[0] = message[slash_index+1];
		if(message[slash_index+2] != '\0'){
			cindex[1] = message[slash_index+2];
			cindex[2] = '\0';
		}else
			cindex[1] = '\0';

		wnum = atoi(cnum);
		windex = atoi(cindex);

                nodes[node_index].word = malloc(sizeof(words[wnum] + 1));
                strcpy(nodes[node_index].word, words[wnum]);
                nodes[node_index].col = 0;

//                fprintf(stdout, "client / wnum = %d / index=%d / word=%s\n", 
//                       wnum, windex, nodes[node_index].word);


//		fprintf(stdout, "read ok\n");
		set_condition(1);
	}
	else if(strcmp(message, "10") == 0){
		score += 10;
	}

//		fprintf(stdout, "check7\n");

	}
   fprintf(stdout, "bye bye!\n");
   close(sock);
   return 0;
}

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}

