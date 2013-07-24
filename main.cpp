#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "graph.h"
#include "orders.h"
#include <ctime>
#include "data.h"
#include <windows.h>

using namespace std;

struct Game game;

int main() {
    system("color 61");

    game.n = 8;
    game.rocket_nums = 8;

    while(true) {
        int ord;
        ord = game.welcome(); //��ʾ��ӭ����

        if(ord == 1) {
            printf("�������û���: ");
            scanf("%s", game.user_name);
            game.start_game(); //����1 �Ϳ�ʼ��Ϸ
        }
        else if(ord == 2){
            //show set menu
            char sz[] = "shezhi.txt";
            game.show_status(sz);
            int n;
            printf("\n������ѡ��: ");
            while(scanf("%d", &n), !(n == 1 || n == 2 || n == 3)) ;

            if(n == 1) { game.n = 8; game.rocket_nums = 8; }
            else if(n == 2) { game.n = 9; game.rocket_nums = 15; }
            else if(n == 3) continue;
            printf("���óɹ�, ������������ϼ��˵�\n");
            game.getchc();
        }
        else if(ord == 3) { //
            game.show_rank();
        }
        else if(ord == 4) { //�˳�
            exit(0);
        }
        //else if

    //default:
    }

    return 0;
}
