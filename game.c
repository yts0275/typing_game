

#include "game.h"
pthread_mutex_t cons_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cons_cond = PTHREAD_COND_INITIALIZER;

// 컨디션 number
// 0 = main_thread
// 1 = plus_one_word / 2 = one_line_down
// 3개의 쓰레드를 조종해줘야함
// condition variable 은 서버에서 받음


void* main_thread(){
        while(1){
		//fprintf(stdout, "condition in main_thread = %d\n", condition);
		pthread_mutex_lock(&cons_lock);
		while(condition !=0)
			pthread_cond_wait(&cons_cond, &cons_lock);
//		condition = 2;
		//plus_one_word();

                if(hp <= 0){
                        fprintf(stdout, "main : you die\n");
			condition = 5;
                        pthread_exit(0);
                }

		system("clear");
//		clear();
                display_map();
		usleep(500000);

		pthread_cond_broadcast(&cons_cond);
		pthread_mutex_unlock(&cons_lock);
        }
}

// 단어 하나 맵에 추가
void* plus_one_word_thread(){
	while(1){
		pthread_mutex_lock(&cons_lock);
		while(condition !=1)
			pthread_cond_wait(&cons_cond, &cons_lock);
		condition = 2;

//		fprintf(stdout, "game.c /index=%d / word=%s\n", node_index,
//			nodes[node_index].word);
/*
	        nodes[node_index].word = malloc(sizeof(words[wnum] + 1));
	        strcpy(nodes[node_index].word, words[wnum]);
	        nodes[node_index].col = 0;
*/


	        for(int i=0; i<strlen(nodes[node_index].word); i++){
	                map[1][windex] = nodes[node_index].word[i];
			windex += 1;
	        }
	        node_index++;
	        if(node_index == ROWS) node_index = 0;

		pthread_cond_broadcast(&cons_cond);
		pthread_mutex_unlock(&cons_lock);

                if(hp <= 0){
                        fprintf(stdout, "plus one : you die\n");
			condition=0;
                        pthread_exit(0);
                }

	}
}

// 한줄씩 떨어짐
void* one_line_down_thread(){
	while(1){
//		fprintf(stdout, "----------------------------%d\n", ROWS);
		pthread_mutex_lock(&cons_lock);
		//fprintf(stdout, "condition in one_line_down = %d\n", condition);
		while(condition != 2){
			//fprintf(stdout, "condtion in while in one_line = %d\n", condition);
			pthread_cond_wait(&cons_cond, &cons_lock);
		}
		condition = 0;

	        for(int i=ROWS-3;i>1;i--){
	                for(int j=0;j<COLS;j++){
	                        map[i][j] = map[i-1][j];
	                }
	        }

	        for(int i=1;i<ROWS-3;i++){
			if(strcmp(nodes[i].word, "gogotest") != 0)
		                nodes[i].col += 1;
			fprintf(stdout, "#%d / %d\n", i, nodes[i].col);
		}

	        if(get_front_line() >= ROWS-3)
	                hp-=10;


		memset(map[1], 32, COLS-1);

//		display_map();

		pthread_cond_broadcast(&cons_cond);
		pthread_mutex_unlock(&cons_lock);

                if(hp <= 0){
                        fprintf(stdout, "down line : you die\n");
			condition = 1;
                        pthread_exit(0);
                }

	}
}

void* input_thread(){
	char input[100];
	int one;
	int index = 0;
	while(1){
		while(1){
			one = getchar();
			input[index++] = one;
			if(one == '\n')
				break;
		}
		input[index-1] = '\0';
//		fprintf(stdout, "send text : %s\n", input);
		if(index > 0){
//			fprintf(stdout, "%d / text = %s\n", index, input);
			for(int i=0;i<ROWS;i++){
//				fprintf(stdout, "node = %s\n", nodes[i].word);
				if(strcmp(nodes[i].word, input) == 0){
					fprintf(stdout, "score up!!\n");
					score+=10;
					one_line_delete(nodes[i].col+1);
					nodes[i].word[0] = '\0';
					nodes[i].col = 0;
					break;
				}
			}
					index = 0;
					memset(input, '\0', 100);
		}

	}
}

void one_line_delete(int col){
	memset(map[col], ' ', COLS-1);
}


void *signal_plus_one_word_thread(void *arg){
        int sn = *((int *)arg);
        while(1){
                //fprintf(stdout, "send message to %d\n", sn);
		wnum = rand() % file_line;
		windex = rand() % (COLS - 14) + 1;
		char buf[20];
		sprintf(buf, "#%d/%d",wnum,windex);
//		fprintf(stdout, "%d/%d\n", wnum, windex);
                write(sn, buf, strlen(buf));
                write(sn-1, buf, strlen(buf));
                sleep(1);
        }

}
void *signal_one_line_down_thread(void *arg){
//	fprintf(stdout, "check3\n");
        int sn = *((int *)arg);
        while(1){
//                fprintf(stdout, "send message to %d\n", sn);
                write(sn, "setcondition2", 2);
//                write(sn-1, "setcondition2", 2);
                sleep(1);
        }


}
void * handle_clnt(void * arg)
{
        int clnt_sock=*((int*)arg);
        int str_len, i;
//      char file_name[BUF_SIZE];
//      FILE *fp;
        char buf[BUF_SIZE];
//      char message[BUF_SIZE];
        char endmsg[]="End of Message";


        while(1)
        {
                while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
                {
                        fprintf(stdout, "clnt_sock in handle_clnt = %d\n", clnt_sock);

                        buf[str_len]=0;
                        fprintf(stdout, "receive from client = %s\n", buf);
                        //q, Q�� �� ä���� �����ϰ� ����
                        if(strcmp(buf, "end") == 0)
                        {
//				fprintf(stdout, "check7\n");
                                write(clnt_sock, endmsg, strlen(endmsg));
                                break;
                        }
                        else if(strcmp(buf, "korean") == 0)
                        {
                                korean += 1;

                        }else if(strcmp(buf, "english") == 0){
                                english += 1;
                                fprintf(stdout, "english = %d\n", english);
                        }

                        if(korean == 2){
                                for(int i=4;i<10;i++){
                                        write(i, "korean start", 13);
                                }
                                korean = 0;
                                english = 0;
                        }else if(english == 2){
                                for(int i=4;i<10;i++){
                                        write(i, "english start", 14);
                                }
                                korean = 0;
                                english = 0;
                        }

                }

                close(clnt_sock);
                puts("client disconnected...");
                clnt_cnt--;
                fprintf(stdout, "user number = %d\n", clnt_cnt);

                return 0;

        }

        pthread_mutex_lock(&mutex);
        for(i=0; i<clnt_cnt; i++)   // remove disconnected client
        {
                if(clnt_sock==clnt_socks[i])
                {
                        while(i++<clnt_cnt-1)
                                clnt_socks[i]=clnt_socks[i+1];
                        break;
                }
        }
        pthread_mutex_unlock(&mutex);

        close(clnt_sock);
        return NULL;
}

void server_error_handling(char * msg)
{
        fputs(msg, stderr);
        fputc('\n', stderr);
        exit(1);
}



void set_condition(int number){
	condition = number;
	pthread_cond_broadcast(&cons_cond);
	//fprintf(stdout, "condition in set_condition = %d\n", condition);
}

int menu_loop(){
	int menu, end = 1, check_error = 1;
        while(1){
                menu = display_menu();
                switch(menu){
                        case 1:
                                check_error = init(1);
                                break;
                        case 2:
                                check_error = init(2);
                                break;
                        case 3:
				end = 0;
				break;
                        default:
                                break;
                }
                if(check_error != 0 && end != 0){
			//fprintf(stdout, "end = %d / return menu = %d\n", end, menu);
			return menu;
                }else
			return 0;
        }
}

int main_loop(){
        pthread_t threads[4];
	pthread_attr_t attr;
	condition = 0;

	pthread_mutex_init(&cons_lock, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

        srand((unsigned int)time(NULL));
        system("clear");

//	fprintf(stdout, "thread on!\n");

        pthread_create(&threads[0], NULL, main_thread, NULL);
	pthread_create(&threads[1], NULL, plus_one_word_thread, NULL);
	pthread_create(&threads[2], NULL, one_line_down_thread, NULL);
	pthread_create(&threads[3], NULL, input_thread, NULL);

/*
	pthread_join(&threads[0], NULL);
	pthread_join(&threads[1], NULL);
	pthread_join(&threads[2], NULL);
	pthread_join(&threads[3], NULL);
*/

	fprintf(stdout, "you lose!\n");
}

// 맵 초기화 + 번호에 따라 다른 words 생성
int  init(int number){
	score = 0;
	level = 4;
	hp = MAX_HP;
        FILE *fp = NULL;
        char file_name[20];
        char buffer[20];

        switch(number){
                // 한글단어들
                case 1:
                        strcpy(file_name, "word_korean.txt");
                        fp = fopen(file_name, "r");
                        break;
                // 영어단어들
                case 2:
                        strcpy(file_name, "word_english.txt");
                        fp = fopen(file_name, "r");
                        break;
        }





        if(fp == NULL){
                fprintf(stdout, "file open error!!\n");
                return 0;
        }





        file_line = getFileLine(file_name);


        fprintf(stdout, "file_size = %d\n", file_line);

        words = (char **)malloc(sizeof(char *) * file_line);
        for(int i=0;i<file_line;i++){
                fgets(buffer, sizeof(buffer), fp);
                words[i] = (char *)malloc(sizeof(char *) * strlen(buffer)+1);
                strcpy(words[i], buffer);
		words[i][strlen(buffer)-1] = '\0';
		fprintf(stdout, "%s", words[i]);
        }

        for(int i=0;i<ROWS;i++){
                memset(map[i], ' ', COLS-1);
                nodes[i].col = 0;
		nodes[i].word = malloc(10);
		strcpy(nodes[i].word, "gogotest");
        }

//	memset(map[5], '?', COLS-1);

        hp = MAX_HP;
        score = 0;

	return 1;
}

// 전체 파일 라인 수 반환
int getFileLine(char *name){
        FILE *fp = NULL;
        int line = 0;
        char c;
        fp = fopen(name, "r");
        while((c=fgetc(fp))!=EOF)
                if(c=='\n') line++;

        fclose(fp);
        return(line);
}

// 맵 출력
void display_map(){
	        sprintf(map[0], "  hp = %d / score = %d", hp, score);
	        //sprintf(map[31], "%s"," input = ");
//		sprintf(map[ROWS-2], " #%d / input = ", ROWS-2);
		memset(map[ROWS-2], '-', COLS);
	        sprintf(map[ROWS-1], " #front line = %d\n",
	                get_front_line());


	        for(int i=0;i<ROWS;i++){
	                for(int j=0;j<COLS;j++){
				move(i, j);
	                        fprintf(stdout, "%c", map[i][j]);
	                }
	                fprintf(stdout, "\n");
	        }
		move(0,0);
		refresh();
}




// 메뉴 출력 + 입력
int display_menu(){
        int number;
	char buffer[10];
        fprintf(stdout, "Rain Typing Game ! select the number !\n");
        fprintf(stdout, "1.korean words  2.english words  3.end : ");
        scanf("%s", buffer);
	number = atoi(buffer);
        return number;
}


// 가장 밑에 있는 단어의 줄 반환
int get_front_line(){
        int max = 0;
        for(int i=0;i<ROWS-1;i++){
                if(nodes[i].col > max)
                        max = nodes[i].col;
        }

        return max;
}

