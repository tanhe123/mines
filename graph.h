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

#define BLANK ('-') //初始方块
#define BLOCK ('*') //光标
#define NOROT ('#') //0号砖块
#define MARKFLAG ('^')  //标记

#define HELP ("\t\t说明:\n\t\t  WSAD分别对应上下左右\n\t\t  回车挖雷\n\t\t  空格标记\n")
#define NOTE ("\t\t*为光标位置, #为空白，~为未挖取的方格, ^为标记旗帜\n\n")

int dx[] = {-1, 1, 0, 0}; //上下左右
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
    int n, rocket_nums, has_blanks, flag_cnt;   //n*n, 炸弹数量，剩余数量，旗帜数量
    time_t begin_time; int game_time;   //游戏时间相关
    bool game_start;
    char user_name[1000];

    char G[maxn][maxn], old_char;     //显示地图
    int r_G[maxn][maxn];    //真实地图
    bool vis[maxn][maxn];

    void init();
    void show();
    void show_status(char *s);
    int welcome();

    void start_game();
    void end_game();

    char getchc();    // 返回按键ASCII
    int get_dir(char ch);   //将字符转化成方向

    void move(int d);   //根据方向移动
    void dfs(int x, int y); ////挖的动作都是在dfs里面完成的
    bool check_win();   //是否胜利
    bool interact(char ch); //交互,响应按键
    void mark();    //标记
    int get_game_time();   //获取游戏时间
    void set_n(int n);

    //文件
    int get_user_cnt();

    void read_users(User rank[],int count);   //读取排行
    void save_users(User rank[], int t); //写入排行
    void save_new_user();
    void show_rank();
};

void Game::show() { //修改这里
    system("cls");
    puts(HELP);
    puts(NOTE);

    //图形输出
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
    printf("请选择:\t");
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

    game_start = false; //游戏未开始

    memset(vis, 0, sizeof(vis));    //是否已经挖开

    //初始化地图
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            G[i][j] = BLANK;
        }
    }

    G[0][0] = BLOCK;    //光标初始化 在(0,0)

    //初始化炸弹
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

    //初始化实际地图
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
