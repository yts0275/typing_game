#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define ROWS 30
#define COLS 50
#define MAX_HP 31
#define BUF_SIZE 1024
#define MAX_CLNT 256

typedef struct node{
	char *word;
	int col;
}node;

// 클라이언트쪽
char map[ROWS][COLS];
char **words;
node nodes[ROWS]; // 원형 큐로 사용
int node_index;
int level;
int hp;
int score;
int file_line;                  // 파일의 라인 수
int condition;
int wnum, windex;
int sock;

int main_loop();
int menu_loop();
void* main_thread();    // 단어 생성 + 맵에 삽입
void* plus_one_word_thread();   // 하나 단어 맵에 추
void* one_line_down_thread();   // 한줄씩 떨어짐
void* input_thread();   // 입력받을거임
int init(int number);           // 맵, words 초기화
int getFileLine(char *name);
void display_map();     // 맵 출력
int display_menu();     // 메뉴 출력 + 입력
int get_front_line();   // 가장 밑에있는 단어의 줄 반환
void set_condition(int num);
void one_line_delete(int col);

// 서버쪽
int clnt_cnt;
int clnt_socks[MAX_CLNT];
int start_count;
int korean;
int english;
pthread_mutex_t mutex;
pthread_t server_t_id[3];


void *handle_clnt(void *arg);
void *signal_plus_one_word_thread(void *arg);
void *signal_one_line_down_thread(void *arg);
void server_error_handling(char *msg);
