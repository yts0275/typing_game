#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

int hp = 100;
int a = 0, i = 0;
int length[3] = {0};
node *tpr = 0;
char enterText[20] = {0};
int enterHere = 0;
int sleep_time = 1;

void function(int signum){
	reset();
	curs_set(1);
	endwin();
	exit(1);
}

// 큐를 비워주고 메모리 할당을 해제해주는 함수
void reset(){
	node *temp = NULL;
	node *temp2 = NULL;

	temp = ptr;

	if(temp !=NULL){
		while(length > 0){
			temp = ptr;

			while(temp->link){
				temp2 = temp;
				temp = temp->link;
			}

			free(temp);
			if(temp2 != NULL)
				temp2->link = NULL;

			length--;
			temp2 = NULL;
		}

		ptr = NULL;
	}
}

// 화면에 글자를 보이게만 하는 함수
void thread_1(void *none){
	int t = sleep_time;
	while(hp > 0){
		node *temp = 0;

		addQueue(returnWord(), (rand() % 40) + 8);
		temp = ptr;

		while(temp){
			draw(temp->row, temp->col, temp->str);
			temp = temp->link;
		}

		move(17, 12);
		sleep(t);

	}
}

// 자연수를 문자열로
int itoa(int n, char *str){
	int temp;

	if(n <= 0){
		strcpy(str, "0");
		return 0;
	}

	temp = itoa(n / 10, str);
	*(str + temp) = 48 + (n % 10);
	return temp + 1;
}

// 입력한 문자열을 큐에서 찾아서 삭제
void findWord(char *str){
	node *temp = 0;
	temp = ptr;

	while(temp){
		if(!strcmp(temp->str, str)){
			strcpy(temp->str, "");
			return;
		}
		else
			temp = temp->link;
	}
}

// 노드 생성
node *makeNode(){
	node *temp=0;
	temp = (node *)malloc(sizeof(*temp));
	temp->link = 0;
}

// 한줄 씩 밑으로 내려오게 해주는 함수
