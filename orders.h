#ifndef ORDERS_INCLUDED
#define ORDERS_INCLUDED

#include <ctime>
#include <conio.h>
#include "graph.h"
#include <algorithm>

using namespace std;


char Game::getchc() { //���ն�����
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
    switch(ch) {    //�������� -1��ʾ����
        case 'w': d = 0; break;
        case 's': d = 1; break;
        case 'a': d = 2; break;
        case 'd': d = 3; break;
        default: d = -1; break;
    }
    return d;
}

void Game::move(int d) {    //�����ƶ�
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
    if(!vis[x][y] && r_G[x][y] == 0) {//��Χû��ը��
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

bool Game::interact(char ch){ //����,��Ӧ����, trueΪ���� false Ϊʧ��
    if(ch == 13) {    //����س�
        if(vis[cur_x][cur_y] == true) return true;
        else if(r_G[cur_x][cur_y] != -1) {  //�������ը��
            if(r_G[cur_x][cur_y] == 0) old_char = NOROT;
            else old_char = r_G[cur_x][cur_y]+'0';
            dfs(cur_x, cur_y);
        }
        else {  //�ڵ���ը��
            end_game(); //������Ϸ
            show();
            return false;
        }
    }
    else if(ch == ' ') {    //����ո���
        mark();
        show();
    }
    else if(get_dir(ch) != -1) {//����������������
        int d = get_dir(ch);
        move(d);
        show();
    }
    else {  //����
        //�������κβ�����
    }
    return true;
}

void Game::mark(){    //���
int x = cur_x, y = cur_y;

    if(flag_cnt > rocket_nums) return ;

    if(G[x][y] != MARKFLAG && old_char != MARKFLAG) {   //�����ǰû�����
        if(!vis[x][y]) {  //��ǰû���ڹ�
            G[x][y] = MARKFLAG;
            old_char = MARKFLAG;
            flag_cnt++;
        }
    }
    else if(old_char == MARKFLAG){  //��ǰ�Ѿ������
        flag_cnt--;
        old_char = BLANK;
        G[x][y] = BLANK;
    }
}

void Game::start_game() {  //��ʼ��Ϸ
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
            //��������
            save_new_user();
            char win[] = "win.txt";
            show_status(win); //show_win
            end_game();

            //�Ƿ�Ҫ�鿴����?
            printf("�Ƿ�Ҫ�鿴����(y/n)?\n");
            char s[5];
            scanf("%s", s);
            if(s[0] == 'y') show_rank();
            break;
        }
    }
}

void Game::end_game() { //������Ϸ
    get_game_time();
    game_start = false;
}

int Game::get_game_time() {    //������Ϸʱ��
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

//������ҵ���Ϣ����ļ�
void Game::save_new_user(){
    int cnt = get_user_cnt();
    read_users(users, cnt);

    //�µļ�¼
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
    else printf("���ļ�ʧ��\n");
}

// ���ļ�
void Game::read_users(User rank[],int count)
{
	FILE *fp;
	if((fp=fopen("score.dat","rb"))!=NULL)
	{
        fread(rank,sizeof(rank[0]),count,fp);
        fclose(fp);
	}
	else printf("���ļ�ʧ��\n");

}

void Game::show_rank(){
    system("cls");
    int cnt = get_user_cnt();

    printf("\t\t\t\t����\n");
    for(int i=0; i<70; i++) putchar('-');
    putchar('\n');

    printf("\t\t\t����\t����\t\t�ɼ�\n\n");
    read_users(users, cnt);
    for(int i=0; i<cnt; i++) {
        printf("\t\t\t%d\t%s\t\t%d\n", i+1, users[i].name, users[i].time);
    }

    putchar('\n');
    putchar('\n');
    system("pause");
}

#endif // ORDERS_INCLUDED
