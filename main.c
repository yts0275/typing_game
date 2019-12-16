#include "game.h"

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

