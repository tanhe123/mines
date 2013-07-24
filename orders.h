#ifndef ORDERS_INCLUDED
#define ORDERS_INCLUDED

#include <ctime>
#include <conio.h>
#include "graph.h"
#include <algorithm>

using namespace std;


char Game::getchc() { //从终端输入
    /*int c;
    system("stty raw");
    c = getchar();
    system("stty cooked");

    return c;*/
    char ch = getch();
    return ch;
}

int Game::get_dir(char ch) {
    int d;
    switch(ch) {    //上下左右 -1表示出错
        case 'w': d = 0; break;
        case 's': d = 1; break;
        case 'a': d = 2; break;
        case 'd': d = 3; break;
        default: d = -1; break;
    }
    return d;
}

void Game::move(int d) {    //光标的移动
    int nx = cur_x+dx[d], ny = cur_y+dy[d];
    if(!(nx < 0 || nx >= n || ny < 0 || ny >= n)) {
        G[cur_x][cur_y] = old_char;

        old_char = G[nx][ny];
        G[nx][ny] = BLOCK;
        cur_x = nx; cur_y = ny;
    }
    show();
}

bool Game::check_win(){
    if(has_blanks == 0) return true;
    else return false;
}

void Game::dfs(int x, int y) {
    has_blanks--;
    if(!vis[x][y] && r_G[x][y] == 0) {//周围没有炸弹
        G[x][y] = NOROT;
        vis[x][y] = true;
        for(int d=0; d<8; d++) {
            int nx = x+ddx[d], ny = y+ddy[d];
            if(nx < 0 || nx >= n || ny < 0 || ny >= n) continue ;
            if(vis[nx][ny]) continue;
            dfs(nx, ny);
        }
    }
    else if(!vis[x][y] && r_G[x][y] != -1) {
        G[x][y] = r_G[x][y]+'0';
        vis[x][y] = true;
    }
    show();
}

bool Game::interact(char ch){ //交互,响应按键, true为处理， false 为失败
    if(ch == 13) {    //输入回车
        if(vis[cur_x][cur_y] == true) return true;
        else if(r_G[cur_x][cur_y] != -1) {  //如果不是炸弹
            if(r_G[cur_x][cur_y] == 0) old_char = NOROT;
            else old_char = r_G[cur_x][cur_y]+'0';
            dfs(cur_x, cur_y);
        }
        else {  //挖到了炸弹
            end_game(); //结束游戏
            show();
            return false;
        }
    }
    else if(ch == ' ') {    //输入空格标记
        mark();
        show();
    }
    else if(get_dir(ch) != -1) {//方向属于上下左右
        int d = get_dir(ch);
        move(d);
        show();
    }
    else {  //出错
        //不进行任何操作。
    }
    return true;
}

void Game::mark(){    //标记
int x = cur_x, y = cur_y;

    if(flag_cnt > rocket_nums) return ;

    if(G[x][y] != MARKFLAG && old_char != MARKFLAG) {   //如果当前没被标记
        if(!vis[x][y]) {  //当前没被挖过
            G[x][y] = MARKFLAG;
            old_char = MARKFLAG;
            flag_cnt++;
        }
    }
    else if(old_char == MARKFLAG){  //当前已经被标记
        flag_cnt--;
        old_char = BLANK;
        G[x][y] = BLANK;
    }
}

void Game::start_game() {  //开始游戏
    init();
    show();

    game_start = true;
    begin_time = time(0);

    char ch;

    while((ch = getchc())) {
        if(!interact(ch)) {    //lose ?
            char lose[] = "lose.txt";
            show_status(lose);
            //printf("you lose"); //show_lose
            system("pause");
            break;
        }

        if(check_win()) {  //win?
            //加入排名
            save_new_user();
            char win[] = "win.txt";
            show_status(win); //show_win
            end_game();

            //是否要查看排名?
            printf("是否要查看排名(y/n)?\n");
            char s[5];
            scanf("%s", s);
            if(s[0] == 'y') show_rank();
            break;
        }
    }
}

void Game::end_game() { //结束游戏
    get_game_time();
    game_start = false;
}

int Game::get_game_time() {    //返回游戏时间
    if(game_start) {
        game_time = (int)(time(0) - begin_time);
    }

    return game_time;
}

void Game::set_n(int cn) {
    n = cn;
}

int Game::get_user_cnt() {
    int cnt;
    FILE *fp = fopen("data.ini", "r");
    fscanf(fp, "%d", &cnt);
    fclose(fp);
    return cnt;
}

User users[1000];

bool cmp(const User &a, const User &b) {
    return a.time < b.time;
}

//把新玩家的信息存进文件
void Game::save_new_user(){
    int cnt = get_user_cnt();
    read_users(users, cnt);

    //新的记录
    User us;
    strcpy(us.name, user_name);
    us.time = get_game_time();

    users[cnt++] = us;

    sort(users, users+cnt, cmp);

    save_users(users, cnt);

    FILE *fp = fopen("data.ini", "w");
    fprintf(fp, "%d", cnt);
    fclose(fp);
}

void Game::save_users(User users[], int cnt) {
    FILE *fp;
    if((fp=fopen("score.dat","wb"))!=NULL) {
        fwrite(users, sizeof(users[0]), cnt, fp);
        fclose(fp);
    }
    else printf("打开文件失败\n");
}

// 读文件
void Game::read_users(User rank[],int count)
{
	FILE *fp;
	if((fp=fopen("score.dat","rb"))!=NULL)
	{
        fread(rank,sizeof(rank[0]),count,fp);
        fclose(fp);
	}
	else printf("打开文件失败\n");

}

void Game::show_rank(){
    system("cls");
    int cnt = get_user_cnt();

    printf("\t\t\t\t排名\n");
    for(int i=0; i<70; i++) putchar('-');
    putchar('\n');

    printf("\t\t\t排名\t姓名\t\t成绩\n\n");
    read_users(users, cnt);
    for(int i=0; i<cnt; i++) {
        printf("\t\t\t%d\t%s\t\t%d\n", i+1, users[i].name, users[i].time);
    }

    putchar('\n');
    putchar('\n');
    system("pause");
}

#endif // ORDERS_INCLUDED
