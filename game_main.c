#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define ROWS 5
#define COLS 80
#define MAX_HP 30

typedef struct node{
	char *word;
	int col;
}node;

char map[ROWS][COLS];
char **words;
node nodes[ROWS]; // 원형 큐로 사용
int node_index = 0;
int level = 4;
int hp = MAX_HP;
int score;
int file_line;			// 파일의 라인 수

void* main_thread();	// 단어 생성 + 맵에 삽입
void* input_thread();	// 입력받을거임
int init(int number);		// 맵, words 초기화
int getFileLine(char *name);
void display_map();	// 맵 출력
int display_menu();	// 메뉴 출력 + 입력
void plus_one_word();	// 하나 단어 맵에 추
void one_line_down();	// 한줄씩 떨어짐
int get_front_line();	// 가장 밑에있는 단어의 줄 반환


void main(){
	pthread_t threads[2];
	int menu;
	int check_error = 1;

	srand((unsigned int)time(NULL));
	system("clear");
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
				return;
			default:
				break;
		}
		if(check_error != 0){
			pthread_create(&threads[0], NULL, main_thread, NULL);
			pthread_join(threads[0], NULL);
		}

	}
}
void* main_thread(){
	while(1){
		plus_one_word();
		display_map();
		sleep(5-level);
		one_line_down();
		system("clear");
		display_map();

		if(hp <= 0){
			fprintf(stdout, "you die\n");
			pthread_exit(0);
		}
	}
}

void* input_thread(){
	while(1){
	}
}

int  init(int number){
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
		words[i] = (char *)malloc(sizeof(char *) * strlen(buffer));
		strcpy(words[i], buffer);
	}

	for(int i=0;i<ROWS;i++){
		memset(map[i], 32, sizeof(map[i]));
		nodes[i].col = -10;
	}

	hp = MAX_HP;
	score = 0;
}

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

void display_map(){
	sprintf(map[0], "  hp = %d / level = %d", hp, 5-level);
	//sprintf(map[31], "%s"," input = ");
	sprintf(map[31], "get line = %d / %d",
		get_front_line(), nodes[31].col);

	for(int i=0;i<ROWS;i++){
		for(int j=0;j<COLS;j++){
			fprintf(stdout, "%c", map[i][j]);
		}
		fprintf(stdout, "\n");
	}
}

int display_menu(){
	int number;
	fprintf(stdout, "Rain Typing Game ! select the number !\n");
	fprintf(stdout, "1. korean words  2. english words 3. end : ");
	scanf("%d", &number);
	return number;
}

void plus_one_word(){
	int num = rand() % file_line;
	int index = rand() % 65;

	nodes[node_index].word = malloc(sizeof(words[num] + 1));
	strcpy(nodes[node_index].word, words[num]);
	nodes[node_index].col = 0;

	for(int i=0; i<strlen(nodes[node_index].word); i++){
		map[1][index++] = nodes[node_index].word[i];
	}
	node_index++;
	if(node_index == ROWS) node_index = 0;

}

void one_line_down(){
	for(int i=ROWS-2;i>0;i--){
		for(int j=0;j<COLS;j++){
			map[i][j] = map[i-1][j];
		}
	}

	for(int i=0;i<ROWS;i++)
		nodes[i].col += 1;

	if(get_front_line() >= ROWS-2)
		hp-=10;

	memset(map[1], 32, COLS);
}

int get_front_line(){
	int max = 0;
	for(int i=0;i<ROWS;i++){
		if(nodes[i].col > max)
			max = nodes[i].col;
	}

	return max;
}
