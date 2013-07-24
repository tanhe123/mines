#pragma once

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "data.h"

using namespace std;

#define BLANK ('-') //��ʼ����
#define BLOCK ('*') //���
#define NOROT ('#') //0��ש��
#define MARKFLAG ('^')  //���

#define HELP ("\t\t˵��:\n\t\t  WSAD�ֱ��Ӧ��������\n\t\t  �س�����\n\t\t  �ո���\n")
#define NOTE ("\t\t*Ϊ���λ��, #Ϊ�հף�~Ϊδ��ȡ�ķ���, ^Ϊ�������\n\n")

int dx[] = {-1, 1, 0, 0}; //��������
int dy[] = {0, 0, -1, 1};
int ddx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int ddy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

const int maxn = 100;

struct User{
  char name[100];
  int time;
}uesrs[maxn];

struct Game {
    int cur_x, cur_y;
    int n, rocket_nums, has_blanks, flag_cnt;   //n*n, ը��������ʣ����������������
    time_t begin_time; int game_time;   //��Ϸʱ�����
    bool game_start;
    char user_name[1000];

    char G[maxn][maxn], old_char;     //��ʾ��ͼ
    int r_G[maxn][maxn];    //��ʵ��ͼ
    bool vis[maxn][maxn];

    void init();
    void show();
    void show_status(char *s);
    int welcome();

    void start_game();
    void end_game();

    char getchc();    // ���ذ���ASCII
    int get_dir(char ch);   //���ַ�ת���ɷ���

    void move(int d);   //���ݷ����ƶ�
    void dfs(int x, int y); ////�ڵĶ���������dfs������ɵ�
    bool check_win();   //�Ƿ�ʤ��
    bool interact(char ch); //����,��Ӧ����
    void mark();    //���
    int get_game_time();   //��ȡ��Ϸʱ��
    void set_n(int n);

    //�ļ�
    int get_user_cnt();

    void read_users(User rank[],int count);   //��ȡ����
    void save_users(User rank[], int t); //д������
    void save_new_user();
    void show_rank();
};

void Game::show() { //�޸�����
    system("cls");
    puts(HELP);
    puts(NOTE);

    //ͼ�����
    for(int i=0; i<n; i++) {
        printf("\t\t\t\t");
        for(int j=0;j<n; j++) {
            putchar(G[i][j]);
            putchar(' ');  putchar(' ');
        }
        putchar('\n'); putchar('\n');
    }
}

int Game::welcome() {
    char str[] = "welcome.txt";
    show_status(str);

    int ord;
    printf("��ѡ��:\t");
    scanf("%d", &ord);

    return ord;
}

void Game::show_status(char *str) {
    system("cls");
    if(strcmp(str, "win.txt") == 0 || strcmp(str, "lose.txt") == 0) printf("your gametime: %ds\n", get_game_time());
	FILE *fp;
	if((fp=fopen(str,"r")) == NULL) {
        printf("open file fails");
        return;
    }

    char ch[1000];
    while(!feof(fp)) {
        fgets(ch,1000,fp);
        printf("%s\n", ch);
    }

    fclose(fp);
}

void Game::init() {
    has_blanks = n*n-rocket_nums;
    cur_x = cur_y = 0;
    flag_cnt = 0;

    game_start = false; //��Ϸδ��ʼ

    memset(vis, 0, sizeof(vis));    //�Ƿ��Ѿ��ڿ�

    //��ʼ����ͼ
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            G[i][j] = BLANK;
        }
    }

    G[0][0] = BLOCK;    //����ʼ�� ��(0,0)

    //��ʼ��ը��
    memset(r_G, 0, sizeof(r_G));
    srand(time(NULL));
    int cnt = 0;
    while(cnt < rocket_nums) {
        int x = rand()%n, y = rand()%n;
        if(r_G[x][y] != -1) {
            r_G[x][y] = -1;
            cnt++;
        }
    }

    //��ʼ��ʵ�ʵ�ͼ
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(r_G[i][j] == -1) continue;
            int cnt = 0, nx, ny;
            for(int d=0; d<8; d++) {
                nx = i+ddx[d], ny = j+ddy[d];
                if(nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
                if(r_G[nx][ny] == -1) cnt++;
            }
            r_G[i][j] = cnt;
        }
    }

    old_char = BLANK;
}

#endif // GRAPH_H_INCLUDED
