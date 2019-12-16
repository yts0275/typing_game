#include "game.h"

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	FILE *fp = NULL;
        srand((unsigned int)time(NULL));


	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	clnt_cnt = 0;
	start_count = 0;
	korean = 0;
	english = 0;

	pthread_mutex_init(&mutex, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		server_error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		server_error_handling("listen() error");

	file_line = getFileLine("word_english.txt");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			puts("new client connected...");

		pthread_mutex_lock(&mutex);
		clnt_socks[clnt_cnt++]=clnt_sock;
		pthread_mutex_unlock(&mutex);


		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
		pthread_create(&server_t_id[0], NULL, handle_clnt, (void*)&clnt_sock);

		if(clnt_cnt == 2){
//			pthread_create(&server_t_id[1], NULL, signal_one_line_down_thread, (void*)&clnt_sock);
			pthread_create(&server_t_id[2], NULL, signal_plus_one_word_thread, (void*)&clnt_sock);
		}

		pthread_detach(server_t_id[0]);
		pthread_detach(server_t_id[1]);
		pthread_detach(server_t_id[2]);
		printf("client fd = %d\n", clnt_sock);

		if(clnt_cnt == 2){
			printf("2 client connect ! start game! \n");
			for(int i=4;i<10;i++){
				write(i, "start", 6);
			}
		}
	}
	close(serv_sock);
	return 0;
}

