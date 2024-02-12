/*(追加分)
 *
 * 敵移動(追跡型)
 * Map表示
 * セーブ機能を追加、-rオプションで続きから実行可能、オプション無しで最初から始める
 * バトルの大幅な改修(HP表示を追加、敵キャラ絵を表示、システムを「たたかう」「くすり」などにコマンド化、それに伴う「ヘルプ」機能の追加)
 * 「まほう」の追加、敵行動にも「まほう」あり、敵とHeroに魔法攻撃、魔法防御ステータスを追加
 * 魔法の種類の増加(回復魔法、攻撃魔法など)
 * Heroにレベルシステムを追加、ステータス画面に経験値のバー表示を追加、レベルアップボーナスを追加、それに伴う難易度の調整
 * 状態異常システムを追加、状態異常「毒」「腐敗」の実装
 * 新ドロップアイテム「杖」「ローブ」の追加
 * 新ステータス「MP」を追加
 * MP不足救済コマンド「めいそう」追加
 * 強化魔法「不屈」を追加、それに伴う新状態「不屈」を実装
 * 敵に個性(HPが多い、攻撃が高い等)を追加
 * ５階層ごとにBOSSモンスターが出現するよう調整(階段座標で遭遇、通常の敵より強いアイテムを確定でドロップする)
 * 新システム「固有スキル」を追加(低確率でザコが所持、BOSSは確定所持、倒した敵の持つ固有スキルを取得するか捨てるか選べる)
 * 新システム「職業」を追加(職業によってレベルアップ時のステータス上昇値が変化し、職業スキルを入手)、それに伴い「職業スキル」を追加
 * Map移動画面からも特定の魔法が使えるように調整
 * 装備を捨てたときと敵を倒したときに「お金」が入手できるように調整
 * Map画面に「商人」が出現するよう調整(商人からは装備１つと回復薬をお金を払うことで購入できる、階層が変わると販売している商品も変化)
 * 製作者用チートを追加(Map画面で「g」を入力でMP全回復、戦闘中に「g」を入力すると敵のHPを１にする)
 * 課題６ー２のフィードバックを受け構造体宣言をヘッダーファイルへ移行、それに伴う一部の修正
 * その他、表示や細かな不具合などを修正
 *
*/

#include <stdio.h>                                                                                                                                             
#include <stdlib.h>       
#include<string.h>
#include<time.h>
#include<unistd.h>
/* 関数の宣言を myrogue4.h にする */
#include "myrogue4.h" 
#define BUF 256              


/* trush */
char trush[BUF];        /* ごみ */

// option
char *options[] = {"r"};

static int boss;
static int flag;
static int d_offence;
static int d_defence;
static int e_defence;
static int ori;
static int TURN;
static int old_offence;
static int a;
static int strong;
static int drug;
static int sum;
static int money1;
static int money2;
static int fflag;

int main(int argc, char *argv[]) {

    int input;
    int dev = 0;
    extern int flag;
    flag = 0;
    extern int boss;
    boss = 0;
    char str;
    FILE *fp = fopen("Savefile", "r");
    char *p;
    int old_x;
    int old_y;
    int e_old_x;
    int e_old_y;
    extern int d_offence;
    extern int d_defence;
    extern int e_defence;
    extern int ori;
    extern int TURN;
    extern int old_offence;
    extern int a;
    extern int strong;
    extern int drug;
    extern int sum;
    extern int money1;
    extern int money2;
    extern int fflag;

    struct Enemy* enemy;
    struct Map* map;
    struct Hero* hero;
    struct Merchant* merchant;

    enemy=(struct Enemy*)malloc(sizeof(struct Enemy));
    map=(struct Map*)malloc(sizeof(struct Map));
    hero=(struct Hero*)malloc(sizeof(struct Hero));
    merchant = (struct Merchant*)malloc(sizeof(struct Merchant));

    hero_create(hero);
    map_create(map);
    current_point(hero,map);

    srandomdev();

    for(int i = 0; i < argc; i++){

  	  p = argv[i];

	  if(*p == '-'){

		  p++;

   		 switch(*p){
		
			    case 'r':
				    if((strcmp(p, options[0])) == 0){


   					 /*ファイルの内容からデータをロード*/
   					 if( fp == NULL ) {
						 printf("ファイルオープンエラー\n");
					  	 exit(0);
                		         }
    					 else{
   					  //ファイルからデータを読み込む
     					  fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &hero->point, &hero->x, &hero->y, &hero->life,
					  &hero->life_upper, &hero->offence, &hero->defence, &hero->drug, &hero->sword, &hero->buckler, &hero->armer,
					  &hero->m_offence, &hero->m_defence, &hero->level, &hero->count, &hero->staff, &hero->robe, &hero->status, &hero->mp,
					  &hero->mp_upper, &hero->status2, &hero->origin, &hero->ori_num, &hero->job, &hero->money);
			   	          fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d\n", &enemy->x, &enemy->y, &enemy->life, &enemy->offence, &enemy->defence, 
					                      &enemy->item_suit, &enemy->item_strong, &enemy->m_offence, &enemy->m_defence, &enemy->type, &enemy->status, 
							      &enemy->origin);
                 			  fscanf(fp, "%d %d %d %d %d %d %d %d %d", &map->wide_x, &map->wide_y, &map->ent_x, &map->ent_y, &map->floor,
					                &map->item_exist, &map->item_strong, &map->item_x, &map->item_y);
					  fscanf(fp, "%d %d %d %d %d %d %d %d %d %d\n", &boss, &flag, &ori, &a, &strong, &drug, &sum, &money1, &money2, &fflag);
					  fscanf(fp, "%d %d\n", &merchant->x, &merchant->y);
                		          fclose(fp);//解放

                        		  }

		   		 }
		  		  break;
	    	}
	}

    }


    for (;;) {

	//敵生成
        if (enemy->life <= 0) {
            enemy_create(enemy, map, hero);
        }

	printf("-----------------------------------------------------------------\n");

	// Map
	for (int j = map->wide_y; j >= 0; j--) {

		for (int i = 0; i <= map->wide_x; i++) {
			
			if (i == hero->x && j == hero->y) { //自機座標

				printf(" ★");

			}
			else if(i == map->ent_x && j == map->ent_y && flag > 0){ //階段座標

				printf(" ■");

			}
			else if(i == enemy->x && j == enemy->y){ //敵座標

				printf(" ●");

			}
			else if(i == merchant->x && j == merchant->y){ //商人座標

				printf(" ▲");

			}
			else { //それ以外

				printf("・");

			}

		}

		printf("\n");

	}

        printf("-----------------------------------------------------------------\n");
	printf("マップ(%d, %d)\n\n", map->wide_x+1, map->wide_y+1);
        printf("現在位置★ (%d, %d): 地下 %d 階\n\n", hero->x, hero->y, map->floor);
        printf("敵位置● (%d, %d)\n\n", enemy->x, enemy->y);
        printf("階段■ (%d, %d)\n\n", map->ent_x, map->ent_y);
	printf("商人▲ (%d, %d)\n", merchant->x, merchant->y);
        printf("-----------------------------------------------------------------\n");

	old_x = hero->x;
	old_y = hero->y;
	e_old_x = enemy->x;
	e_old_y = enemy->y;
	//d_offence = hero->offence;
	//d_defence = hero->defence;
	//printf("\n%d\n", boss);
	//printf("\n%d\n", fflag);

        command(hero, map, enemy, merchant);

        if (pos_compare(map->item_x, map->item_y, hero->x, hero->y)) {
            get_drug(map, hero);
            map->item_exist = 0;
        }

	// 敵移動(追跡型)
	if(enemy->life > 0 && enemy->type != 4){

		if(enemy->x > hero->x){
			
			enemy->x--;

		}
		else if(enemy->x < hero->x){

			enemy->x++;

		}
		else if(enemy->y > hero->y){

			enemy->y--;

		}
		else if(enemy->y < hero->y){

			enemy->y++;

		}

	}

	if(pos_compare(hero->x, hero->y, merchant->x, merchant->y)){
		shop(map, hero);
	}

        if (pos_compare(map->ent_x, map->ent_y, hero->x, hero->y)) {
	    if(map->floor % 5 == 0 && boss == 0){
		    flag++;
		    printf("\t  強力な気配がする...\n\n");
		    printf("\tBOSSと戦いますか(y/n) : ");
		    scanf("%c", &str);
		    fgets(trush, BUF, stdin);
		    if(str == 'y' && boss == 0){
			boss++;
		        enemy_create(enemy, map, hero);
		    }
		    else{
			hero->x = old_x;
			hero->y = old_y;
			enemy_create(enemy, map, hero);
			enemy->x = e_old_x;
			enemy->y = e_old_y;
		    }
	    }
	    else if(boss >= 1 || map->floor != 5){
                printf("\n階段があった。下に降りますか？ (y/n) : ");
	        flag++;
		scanf("%c", &str);
		fgets(trush, BUF, stdin);
		printf("\n");
                if (str == 'y') {
		    if(map->floor == 100){ //GameClear
			    gameclear(map, hero);
		    }
                    map_create(map);
                    enemy_create(enemy, map, hero);
                    current_point(hero, map);
		    fflag = 0;
		    sum = 0;
		    merchant_create(map, merchant);
                    printf("\n\t地下 %d 階に降りた。\n\n", map->floor);
		    flag = 0;
		    boss = 0;
                }else{
		    boss = 2;
		}
	    }
        }
        if (pos_compare(hero->x, hero->y, enemy->x, enemy->y)) {
	    int point = hero->point; 
	    e_defence = enemy->defence;
	    d_offence = hero->offence;
	    d_defence = hero->defence;
            fight(hero, enemy, map);
	    if(boss == 1){
		    boss = 2;
	    }
            if (point < hero->point) {
                enemy_create(enemy, map, hero);
            }
        }
	
	//見やすさ用
	printf("-----------------------------------------------------------------\n");

    }

}

void gameclear(struct Map *map, struct Hero *hero) {
    extern int old_offence;
    extern int d_defence;
    int a = 10;

    if(hero->status2 == 1){ //不屈のとき
	hero->offence = old_offence;
    }
    if(hero->status == 2){ //腐敗魔法差分
	hero->defence = d_defence;
    }
    printf("\n");
    printf("----------最終結果----------\n\n");
    printf("floor :  %d / 100\n", map->floor);
    printf("\nHP: %d/%d\tMP: %d/%d\t攻撃力: %d\t守備力: %d\n\n", hero->life, hero->life_upper,  hero->mp, hero->mp_upper, hero->offence, hero->defence);
    printf("魔法攻撃力: %d\t魔法防御力: %d\n\n", hero->m_offence, hero->m_defence);
    if(hero->job == 0){
	    printf("木こり ");
    }
    else if(hero->job == 1){
	    printf("戦士 ");
    }
    else if(hero->job == 2){
	    printf("魔法つかい ");
    }

    printf("Lv: %d\t", hero->level);
    for(int i = 0; i < a; i++){
	    if((float)hero->point / (float)hero->count > (float)i / a){
		    printf("■");
	    }
	    else{
		    printf("□");
	    }
    }
    printf("  %d / %d\n", hero->point, hero->count);
    printf("\n持ち物のレベル\n\n剣: %3d    盾: %3d    鎧: %3d    杖: %3d    ローブ: %d\n",
            hero->sword, hero->buckler, hero->armer, hero->staff, hero->robe);
    printf("\n薬の回復力: %d\n\n", hero->drug);
    printf("固有スキル : ");
    if(hero->origin == 1){
	printf("毎ターンHP自動回復(最大HPの%d％)", hero->ori_num);
    }
    else if(hero->origin == 2){

if(TURN <= hero->ori_num){
	    printf("死の宣告(%dターン後確率即死魔法)", hero->ori_num);
	}
	else{
	    printf("死の宣告(%dターン後確率即死魔法) ＜発動済＞", hero->ori_num);
	}
    }
    else if(hero->origin == 3){
	if(TURN <= hero->ori_num){
	    printf("かたいよろい(開幕%dターン無敵)", hero->ori_num);
	}
	else{
	    printf("かたいよろい(開幕%dターン無敵) ＜発動済＞", hero->ori_num);
	}
    }
    else{
	printf("なし");
    }
    printf("\n\n所持金 : %d 円", hero->money);
    printf("\n\n");
    printf("-----------------------------------------------------------------\n");
    printf("\n\n\tGAME CLEAR!!!\n\n");
    exit(0);
}

void map_create(struct Map *map) {
    map->floor++;
    map->wide_x = random() % (2 + map->floor) + 10;
    map->wide_y = random() % (2 + map->floor) + 10;
    map->ent_x = random() % map->wide_x;
    map->ent_y = random() % map->wide_y;
    map->item_exist = 1;
    map->item_strong = random() % (10 + 10 * map->floor) + 30;
    map->item_x = random() % map->wide_x;
    map->item_y = random() % map->wide_y;
    return;
}

void show_status(struct Hero *hero) {
    int a = 10;
    extern int TURN;
    if(hero->level == 0 && hero->job != 0){
	    hero->count = 20;
    }
    else if(hero->job == 0 && hero->level == 0){
	    hero->count = 10;
    }
    printf("-----------------------------------------------------------------");
    printf("\n現在の状況:\n");
    if(hero->status == 2){
        printf("\nHP: %d/%d\tMP: %d/%d\t攻撃力: %d\t守備力: %d ↓↓\n\n", hero->life, hero->life_upper,  hero->mp, hero->mp_upper, hero->offence, hero->defence);
    }
    else{
        printf("\nHP: %d/%d\tMP: %d/%d\t攻撃力: %d\t守備力: %d\n\n", hero->life, hero->life_upper,  hero->mp, hero->mp_upper, hero->offence, hero->defence);
    }
    printf("魔法攻撃力: %d\t魔法防御力: %d\n\n", hero->m_offence, hero->m_defence);
    printf("職業 : ");
    if(hero->job == 0){
	    printf("木こり   ");
    }
    else if(hero->job == 1){
	    printf("戦士     ");
    }
    else if(hero->job == 2){
	    printf("魔法つかい   ");
    }
    printf("Lv: %d    ", hero->level);
    for(int i = 0; i < a; i++){
	    if((float)hero->point / (float)hero->count > (float)i / a){
		    printf("■");
	    }
	    else{
		    printf("□");
	    }
    }
    printf("  %d / %d\n", hero->point, hero->count);
    printf("\n持ち物のレベル\n\n");
    printf("剣: %3d    盾: %3d    鎧: %3d    杖: %3d    ローブ: %3d\n", hero->sword, hero->buckler, hero->armer, hero->staff, hero->robe);
    printf("\n薬の回復力: %d\n\n", hero->drug);
    printf("状態: ");
    if(hero->status == 1){
        printf("毒(毎ターン継続ダメージ)\n");
    }
    else if(hero->status == 2){
	printf("腐敗(物理防御減少)\n");
    }
    else if(hero->status == 0 && hero->status2 == 0){
	printf("なし\n");
    }
    if(hero->status2 == 1){
	if(hero->status != 0){ //表示用
 	    printf("      不屈(1ターン後強制死亡、攻撃力20倍)\n");
	}
	else{
            printf("不屈(1ターン後強制死亡、攻撃力20倍)\n");
	}
    }
    printf("\n");
    printf("固有スキル : ");
    if(hero->origin == 1){
	printf("毎ターンHP自動回復(最大HPの%d％)", hero->ori_num);
    }
    else if(hero->origin == 2){
	if(TURN <= hero->ori_num){
	    printf("死の宣告(%dターン後確率即死魔法)", hero->ori_num);
	}
	else{
	    printf("死の宣告(%dターン後確率即死魔法) ＜発動済＞", hero->ori_num);
	}
    }
    else if(hero->origin == 3){
	if(TURN <= hero->ori_num){
	    printf("かたいよろい(開幕%dターン無敵)", hero->ori_num);
	}
	else{
	    printf("かたいよろい(開幕%dターン無敵) ＜発動済＞", hero->ori_num);
	}
    }
    else{
	printf("なし");
    }
    printf("\n\n");
    printf("職業スキル : ");
    if(hero->job == 0){
	    printf(" なし");
    }
    else if(hero->job == 1){
	    printf("物理攻撃威力増加 ↑↑");
    }
    else if(hero->job == 2){
	    printf("魔法消費MP半減 ↓↓");
    }
    printf("\n\n所持金 : %d 円", hero->money);
    printf("\n\n");
    printf("-----------------------------------------------------------------\n");
    return;
}

void hero_die(struct Map *map, struct Hero *hero) {
    extern int old_offence;
    extern int d_defence;
    int a = 10;
    hero->life = 0;

    if(hero->status2 == 1){ //不屈のとき
	hero->offence = old_offence;
    }
    if(hero->status == 2){ //腐敗魔法差分
	hero->defence = d_defence;
    }
    printf("\t\n   Heroはしんでしまった...！！\n\n");
    printf("----------最終結果----------\n\n");
    printf("floor :  %d / 100\n", map->floor);
    printf("\nHP: %d/%d\tMP: %d/%d\t攻撃力: %d\t守備力: %d\n\n", hero->life, hero->life_upper,  hero->mp, hero->mp_upper, hero->offence, hero->defence);
    printf("魔法攻撃力: %d\t魔法防御力: %d\n\n", hero->m_offence, hero->m_defence);
    if(hero->job == 0){
	    printf("木こり ");
    }
    else if(hero->job == 1){
	    printf("戦士 ");
    }
    else if(hero->job == 2){
	    printf("魔法つかい ");
    }
    printf("Lv: %d\t", hero->level);
    for(int i = 0; i < a; i++){
	    if((float)hero->point / (float)hero->count > (float)i / a){
		    printf("■");
	    }
	    else{
		    printf("□");
	    }
    }
    printf("  %d / %d\n", hero->point, hero->count);
    printf("\n持ち物のレベル\n\n剣: %3d    盾: %3d    鎧: %3d    杖: %3d    ローブ: %d\n",
            hero->sword, hero->buckler, hero->armer, hero->staff, hero->robe);
    printf("\n薬の回復力: %d\n\n", hero->drug);
    printf("固有スキル : ");
    if(hero->origin == 1){
	printf("毎ターンHP自動回復(最大HPの%d％)", hero->ori_num);
    }
    else if(hero->origin == 2){
	if(TURN <= hero->ori_num){
	    printf("死の宣告(%dターン後確率即死魔法)", hero->ori_num);
	}
	else{
	    printf("死の宣告(%dターン後確率即死魔法) ＜発動済＞", hero->ori_num);
	}
    }
    else if(hero->origin == 3){
	if(TURN <= hero->ori_num){
	    printf("かたいよろい(開幕%dターン無敵)", hero->ori_num);
	}
	else{
	    printf("かたいよろい(開幕%dターン無敵) ＜発動済＞", hero->ori_num);
	}
    }
    else{
	printf("なし");
    }
    printf("\n\n");
    printf("職業スキル : ");
    if(hero->job == 0){
	    printf(" なし");
    }
    else if(hero->job == 1){
	    printf("物理攻撃威力増加 ↑↑");
    }
    else if(hero->job == 2){
	    printf("魔法消費MP半減 ↓↓");
    }
    printf("\n\n所持金 : %d 円", hero->money);
    printf("\n\n");
    printf("-----------------------------------------------------------------\n\n");
    printf("\n\n\tGAME OVER...\n\n");
    exit(0);
}

void calc_parameter(struct Hero *hero, struct Enemy *enemy) {
    if (enemy->item_suit == 1) {
        hero->offence = hero->offence - hero->sword + enemy->item_strong;
        hero->sword = enemy->item_strong;
    }
    else if (enemy->item_suit == 2) {
        hero->defence = hero->defence - hero->buckler + enemy->item_strong;
        hero->buckler = enemy->item_strong;
    }
    else if (enemy->item_suit == 3) {
        hero->defence = hero->defence - hero->armer + enemy->item_strong;
        hero->armer = enemy->item_strong;
    }
    else if (enemy->item_suit == 4) {
	hero->m_offence = hero->m_offence - hero->staff + enemy->item_strong;
	hero->staff = enemy->item_strong;
    }
    else if (enemy->item_suit == 5) {
	hero->m_defence = hero->m_defence - hero->robe + enemy->item_strong;
	hero->robe = enemy->item_strong;
    }
    return;
}

void use_drug(struct Hero *hero) {
    if (hero->drug > 0) {
        hero->life += hero->drug;
        if (hero->life_upper < hero->life) {
            hero->life = hero->life_upper;
        }
        hero->drug = 0;
        printf("\n\t薬を使った。\b\n\n");
        show_status(hero);
    }
    else {
        printf("\n\t薬は今持っていない。\n\n");
    }
    return;
}

void get_enemyitem(struct Enemy *enemy, struct Hero *hero) {
    int st = 0;
    char pro;

    if (enemy->item_suit != 0) {
        show_status(hero);
        printf("   敵はアイテムを持っていた\n\n");
        if (enemy->item_suit == 1) {
            printf("\t剣: level %d\n\n", enemy->item_strong);
	    st = hero->sword;
        }
        else if (enemy->item_suit == 2) {
            printf("\t盾: level %d\n\n", enemy->item_strong);
	    st = hero->buckler;
        }
        else if (enemy->item_suit == 3) {
            printf("\t鎧: level %d\n\n", enemy->item_strong);
	    st = hero->armer;
        }
	else if (enemy->item_suit == 4) {
	    printf("\t杖: level %d\n\n", enemy->item_strong);
	    st = hero->staff;
	}
	else if (enemy->item_suit == 5) {
	     printf("\tローブ: level %d\n\n", enemy->item_strong);
	     st = hero->robe;
	}
	//装備確認
	if(st > enemy->item_strong){
            printf("\n＜注意！ この武器は現在の装備より弱いです！＞\n\n");
	}
        printf("\n装備しますか？(y or n) : ");
	scanf("%c", &pro);
	fgets(trush, BUF, stdin);
        if (pro == 'y') {
            printf("   ヒーローの状態が変わった。\b\n\n");
            calc_parameter(hero,enemy);
            show_status(hero);
        }
        else {
	    st = (enemy->item_strong * 30);
	    hero->money += st;
            printf("\n\tアイテムを捨てた！\n");
	    printf("\tアイテムを %d 円に換金した！\n\n", st);
        }
    }
    else {
        printf("  敵はアイテムを持っていなかった。\n\n");
    }
    return;
}

int escape() {
    int x, y;
    extern int boss;
    if(boss !=0){
	printf("\n\tBOSSでは逃げられない！！\n\n");
        return 0;
    }
    x = random() % 10;
    y = random() % 10;
    if (x < y) {
        return !0;
    }
    else {
	printf("\n\t逃げられなかった！\n\n");
        return 0;
    }
}


/* 敵は呼び出し元で生成すること */
void fight(struct Hero *hero, struct Enemy *enemy, struct Map *map) {
    int dum;
    int flag2 = 0;
    int enemy_max = 0;
    int deai = 0;
    char com;
    int help = 0;
    int drugflag = 0;
    int heal = 0;
    int bomb = 0;
    int enemy_com = 0;
    int magic = 0;
    int turn = 0;
    int huhai = 0;
    int old_mp;
    extern int old_offence;
    int hukutu = 0;
    extern int d_offence;
    extern int d_defence;
    int e_turn = 0;
    extern int e_defence;
    extern int TURN;
    int mon = 0;

    // printf("enemy_life : %d\n", enemy->life);

    if(flag2 == 0){

  	  enemy_max = enemy->life;
	  flag2++;

    }

    //debug
    //printf("enemy_max : %d\n", enemy_max);
    //printf("enemy->offence : %d\n", enemy->offence);

    for (;;) {
	printf("-----------------------------------------------------------------\n\n");

	if(deai == 0 && enemy->type != 4){
		
 		printf("\n  敵が現れた！！\n\n");
		deai++;
		TURN = 1;

	}
	else if(enemy->type == 4 && deai == 0){

		printf("\n  BOSSがあらわれた！！\n\n");
		deai++;
		TURN = 1;
	}

	printf("     ペン太\n");
	printf("     *******\n");
	printf("    *       *\n");
	printf("   *         *\n");
	printf("  *   >   <   *\n");
	printf("   *         *\n");
	printf("    *       *\n");
	printf("     *******\n");
	
	//HP表示
	printf(" HP ");
	int bar_len = 10;
	for (int i = 0; i < bar_len; i++) {
	  if ( (float)enemy->life / (float)enemy_max > (float)i / bar_len ) {
	    printf("■");
	  } else {
	    printf("□");
	  }
	}
	help = 0;
	drugflag = 0;
	//コマンド
	printf("\n\n");
        show_status(hero);
	printf("\n<<command>>\n\n");
     	printf("たたかう : a\n");
	if(hero->level >= 3 || hero->job != 0){
	    printf("強攻撃 : s\n");
	}
	if(hero->level >= 2 || hero->job != 0){
	    printf("まほう : m\n");
	}
	printf("くすり : d\n");
	if(hero->level >= 2){
	    printf("めいそう : y\n");
	}
	printf("にげる : e\n");
	printf("ヘルプ : h\n\n"); 
	if(hero->life <= 20){
	    printf("＜＜瀕死です！！＞＞\n\n");
	}
	printf("どうする？ : ");
	scanf("%c", &com);
	printf("\n");
	fgets(trush, BUF, stdin);
	//くすり
	if(com == 'd'){
		if(hero->drug > 0){
			hero->life += hero->drug;
			if(hero->life_upper < hero->life){
				hero->life = hero->life_upper;
			}
			hero->drug = 0;
			printf("\n\t薬を使った。\n\n");
		}
		else{
			printf("\n\t薬は持っていない。\n\n");
		}
		drugflag++;
	}
	else if(com == 'y'){
	    printf("\tHeroは静かにめいそうした。。。\n\n");
	    dum = (hero->mp_upper * 0.2);
	    hero->mp += dum;
	    if(hero->mp > hero->mp_upper){
		    hero->mp = hero->mp_upper;
	    }
	    printf("\tHeroのMPが %d 回復した！\n\n", dum);
	}
	else if (com == 'e' && escape() == 1 && boss == 0) { //にげる
            printf("\t敵から逃げました。\n\n");
	    hero->status = 0;
	    hero->status2 = 0;
	    printf("\tすべての状態異常が回復した！\n\n");
	    hero->defence = d_defence;
	    hero->offence = d_offence;
	    deai = 0;
	    enemy->status = 0;
	    TURN = 1;
            break;
        }
        else if(com == 'a') { //たたかう
            printf("\t敵と戦います。\n\n");
            printf("Hero の攻撃: ");
       	    dum = (hero->offence * (random() % 5)) / (enemy->defence + 1);
	    if(enemy->origin == 3 && TURN <= ori){ //敵無敵
		    dum = 0;
	    }
	    if(hero->status2 == 1){ //不屈
		    dum = (hero->offence / 100) * hero->level;
	    }
	    if(hero->job == 1){ //戦士
		    dum *= 1.5;
	    }
            enemy->life -= dum;
            printf("敵に %d のダメージ\n\n", dum);
	}
	else if((hero->job != 0 || hero->level >= 3) && com == 's'){ //強攻撃
		printf("\t強攻撃をした！\n\n");
		printf("Heroの攻撃: ");		
		dum = ((hero->offence * (random() % 5 + 5)) / (enemy->defence + 1)) + (hero->level * 2);
		if(enemy->origin ==3 && TURN <= ori){ //敵無敵
			dum = 0;
		}
		if(hero->status2 == 1){ //不屈
			dum = (hero->offence * hero->level) / 11;
		}
		if(hero->job == 1){ //戦士
			dum *= 1.7;
		}
                enemy->life -= dum;
                printf("敵に %d のダメージ！！\n\n", dum);
	}
	else if(com == 'h'){ //ヘルプ
		printf("\t   ヘルプ\n\n");
		printf("たたかう → 敵に物理攻撃をする。\n");
		printf("           敵の物理防御と自身の物理攻撃力によっては、ダメージが0になることがある。\n\n");
		printf("強攻撃 → 敵にやや強めの物理攻撃をする。(Lv.3以上で解禁)\n");
		printf("         通常攻撃「たたかう」より、Hero自身のレベルに依存する。\n\n");
		printf("まほう → 敵に魔法攻撃をしたり、自身を回復する魔法を使えたりする。(Lv.2以上で一部解禁)\n\n");
		printf("くすり → 持っている薬をすべて使用し、HPを回復できる。\n\n");
		printf("めいそう → 静かに瞑想してMPを最大値の20％回復する。\n\n");
		printf("にげる → 一定確率で敵から逃走する。失敗すると、敵から攻撃を受ける。\n\n");
		printf("状態異常 → 一部の行動には、状態異常を付与できるものがある。\n\n");
		help++;
	}
	else if(com == 'g'){ //製作者用チート
		enemy->life = 1;
		help++;
	}
	else if(com == 'm' && (hero->level >= 2 || hero->job != 0)){ //まほう
	        if(hero->level < 5 && hero->job == 0){
			if(hero->job == 2){
			    printf("\nヒール(消費MP 3 ↓↓) : 1\n\n");
			    printf("クリア(消費MP 2 ↓↓) : 2\n\n");
			    printf("チェック(消費MP 1 ↓↓) : 3\n");
			}
			else{
			    printf("\nヒール(消費MP 6) : 1\n\n");
			    printf("クリア(消費MP 4) : 2\n\n");
			    printf("チェック(消費MP 2) : 3\n");
			}
		}
		else if((hero->level >= 5 && hero->job == 0) || hero->job != 0){
			if(hero->job == 2){
		 	    printf("\nハイヒール(消費MP 5 ↓↓) : 1\n\n");
			    printf("ボム(消費MP 0) : 2\n\n");
			    printf("ハイクリア(消費MP 3 ↓↓) : 3\n\n");
			    printf("腐敗魔法(消費MP 3 ↓↓) : 4\n\n");
	 		    printf("不屈(消費MP 最大MPの7割): 5\n\n");
			    printf("アナライズ(消費MP 2 ↓↓) : 6\n\n");
			    if(hero->level >= 7){
			        printf("割合魔法(消費MP 現在MPすべて) : 7\n\n");
			        printf("マジックキャンセル(消費MP 25 ↓↓) : 8\n");
			    }
			}
			else{
			    printf("\nハイヒール(消費MP 9) : 1\n\n");
			    printf("ボム(消費MP 0) : 2\n\n");
			    printf("ハイクリア(消費MP 6) : 3\n\n");
			    printf("腐敗魔法(消費MP 6) : 4\n\n");
	 		    printf("不屈(消費MP 最大MPの7割): 5\n\n");
			    printf("アナライズ(消費MP 3) : 6\n\n");
			    if(hero->level >= 7){
			        printf("割合魔法(消費MP 現在MPすべて) : 7\n\n");
			        printf("マジックキャンセル(消費MP 50) : 8\n");
			    }
			}
		}
		printf("\n\nどの魔法をつかう？ : ");
		scanf("%c", &com);
		fgets(trush, BUF, stdin);
		printf("\n");
		old_mp = hero->mp;
		if(hero->level < 5 && hero->job == 0){
			if(com == '1'){
				if(hero->job == 2){
					hero->mp -= 3;
				}
				else{
				    hero->mp -= 6;
				}
				if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else{
				    printf("\tヒールを使った！\n");
				    heal = (random() % 5) + hero->m_offence;
				    hero->life += heal;
				    if(hero->life > hero->life_upper){
					    hero->life = hero->life_upper;
				    }
				    printf("\tHeroのHPが %d 回復した！\n\n", heal);
				}
			}
			else if(com == '2'){
				if(hero->job == 2){
					hero->mp -= 2;
				}
				else{
					hero->mp -= 4;
				}
                                if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else{
				    printf("\tクリアを使った！\n");
				    hero->status = 0;
				    turn = 0;
				    printf("\t状態異常が回復した！\n\n");
				}
			}
			else if(com == '3'){
				if(hero->job == 2){
					hero->mp -= 1;
				}
				else{
					hero->mp -= 2;
				}
				if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else{
				    printf("\tチェックを使った！\n\n");
				    printf("\t------敵情報------\n\n");
				    printf("\t敵タイプ : ");
				    if(enemy->type == 1){
			                printf("\tノーマル\n");
				    }
				    else if(enemy->type == 2){
					printf("\tHP(大)、物理防御(少)、魔法防御(少)\n");
				    }
				    else if(enemy->type == 3){
					printf("\tHP(少)、物理攻撃(大)、魔法攻撃(大)\n");
				    }
                                    printf("\tHP : %d / %d\n", enemy->life, enemy_max);
				    printf("\t物理攻撃 : %d\n", enemy->offence);
				    printf("\t物理防御 : %d\n", enemy->defence);
				    printf("\t魔法攻撃 : %d\n", enemy->m_offence);
				    printf("\t魔法防御 : %d\n", enemy->m_defence);
				    printf("\tアイテム : ");
				    if(enemy->item_suit == 1){
				        printf("\t剣");
				    }
				    else if(enemy->item_suit == 2){
					printf("\t盾");
				    }
				    else if(enemy->item_suit == 3){
					printf("\t鎧");
				    }
				    else if(enemy->item_suit == 4){
					printf("\t杖");
				    }
				    else if(enemy->item_suit == 5){
					printf("\tローブ");
				    }
				    else{
					printf("\tなし");
				    }
				    printf("\n\n");
			        }
			}
		}
		else if((hero->level >= 5 && hero->job == 0) || hero->job != 0){
			if(com == '1'){
				if(hero->job == 2){
					hero->mp -= 5;
				}
				else{
					hero->mp -= 9;
				}
                                if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else{
				    printf("\tハイヒールを使った！！\n");
				    heal = ((random() % 5) + hero->m_offence) + 20;
				    hero->life += heal;
				    if(hero->life > hero->life_upper){
					    hero->life = hero->life_upper;
				    }
				    printf("\tHeroのHPが %d 回復した！\n\n", heal);
				}
			}
			else if(com == '2'){
				printf("\tボムの魔法を使った！\n");
				bomb = ((hero->m_offence * (random() % 7)) / (enemy->m_defence + 1)) + 15;
				if(enemy->origin == 3 && TURN <= ori){
					bomb = 0;
				}
				enemy->life -= bomb;
				printf("\t敵に %d のダメージ！！\n\n", bomb);
			}
			else if(com == '3'){
				if(hero->job == 2){
					hero->mp -= 3;
				}
				else{
					hero->mp -= 6;
				}
                                if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else{
				    printf("\tハイクリアの魔法を使った！\n\n");
				    if(hero->status == 2){
					    hero->defence = d_defence;
			 	    }
				    hero->status = 0;
				    turn = 0;
				    dum = hero->life_upper * 0.1;
				    hero->life += dum;
				    if(hero->life > hero->life_upper){
					    hero->life = hero->life_upper;
				    }
				    printf("\t状態異常が回復した！\n");
				    printf("\tHeroのHPが %d 回復した！\n\n", dum);
				}
			}
			else if(com == '4'){
				if(hero->job == 2){
					hero->mp -= 3;
				}
				else{
					hero->mp -= 6;
				}
                                if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else if(huhai > map->floor || enemy->defence == 0){
					printf("これ以上、腐敗魔法は使えない！\n\n");
					help++;
				}
				else{
				    printf("\t腐敗魔法を使った！\n");
				    dum = enemy->defence * 0.3;
				    dum++;
				    enemy->defence -= dum;
				    if(enemy->defence < 0){
					    dum = 0;
					    enemy->defence = 0;
				    }
				    printf("\t敵の防御が %d 減少した！\n\n", dum);
				    huhai++;
				}
			}
			else if(com == '5'){
				dum = (hero->mp_upper * 0.7);
				hero->mp -= dum;
				if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n");
					help++;
				}
				else{
				    hukutu = 0;
				    old_offence = hero->offence;
				    hero->offence *= 20;
				    hero->status2 = 1;
				    printf("\t不屈魔法を使った！\n\n");
				    printf("\t状態: 不屈 になった！\n");
			 	    printf("    Heroの物理攻撃力が20倍になった！\n\n");
			       }
			}
                        else if(com == '6'){
				if(hero->job == 2){
					hero->mp -= 2;
				}
				else {
					hero->mp -= 3;
				}
				if(hero->mp < 0){
					hero->mp = old_mp;
					printf("MPがたりない！\n\n");
					help++;
				}
				else{
				    printf("\tアナライズを使った！\n\n");
				    printf("\t------敵情報------\n\n");
				    printf("\t敵タイプ : ");
				    if(enemy->type == 1){
			                printf("\tノーマル\n");
				    }
				    else if(enemy->type == 2){
					printf("\tHP(大)、物理防御(少)、魔法防御(少)\n");
				    }
				    else if(enemy->type == 3){
					printf("\tHP(少)、物理攻撃(大)、魔法攻撃(大)\n");
				    }
				    else if(enemy->type == 4){
					printf("\tBOSS\n");
				    }
				    printf("\t   HP    : %d / %d\n", enemy->life, enemy_max);
				    printf("\t物理攻撃 : %d\n", enemy->offence);
				    if(enemy->defence != e_defence){
				        printf("\t物理防御 : %d ↓↓\n", enemy->defence);
				    }
				    else{
					printf("\t物理防御 : %d\n", enemy->defence);
				    }
				    printf("\t魔法攻撃 : %d\n", enemy->m_offence);
				    printf("\t魔法防御 : %d\n", enemy->m_defence);
				    printf("\tアイテム : ");
				    if(enemy->item_suit == 1){
				        printf("\t剣");
				    }
				    else if(enemy->item_suit == 2){
					printf("\t盾");
				    }
				    else if(enemy->item_suit == 3){
					printf("\t鎧");
				    }
				    else if(enemy->item_suit == 4){
					printf("\t杖");
				    }
				    else if(enemy->item_suit == 5){
					printf("\tローブ");
				    }
				    else{
					printf("\tなし");
				    }
				    printf("\n\t  状態   : ");
				    if(enemy->defence != e_defence){
					printf("腐敗(防御減少)");
				    }
				    if(enemy->status == 1){
					if(enemy->defence != e_defence){
					    printf("\n\t           マジックキャンセル(魔法使用不可)"); //表示用
					}
					else{
					    printf("マジックキャンセル(魔法使用不可)");
					}
				    }
				    else if(e_defence == enemy->defence && enemy->status == 0){
					printf(" なし");
				    }
				    printf("\n      固有スキル : ");
				    if(enemy->origin == 1){
					printf("毎ターンHP自動回復(最大HPの%d％)", ori);
				    }
				    else if(enemy->origin == 2){
					printf("死の宣告(%dターン後確率即死魔法)", ori);
				    }
				    else if(enemy->origin == 3){
					printf("かたいよろい(開幕%dターン無敵)", ori);
				    }
				    else{
					printf(" なし");
				    }
				    printf("\n\n");
			        }
			}
			else if(com == '7' && hero->level >= 7){
			    hero->mp = 0;
			    dum = (old_mp * 6.5);
			    if(enemy->origin == 3 && TURN <= ori){
				    dum = 0;
			    }
			    enemy->life -= dum;
			    printf("\t割合魔法を使った！\n");
			    printf("\t敵に %d の割合ダメージ！！\n\n", dum);
			}
			else if(com == '8' && hero->level >= 7){
			    if(hero->job == 2){
				    hero->mp -= 25;
			    }
			    else{
			    	hero->mp -= 50;
			    }
			    if(hero->mp < 0){
				hero->mp = old_mp;
				printf("MPがたりない！\n\n");
				help++;
			    }
			    else{
				printf("\tマジックキャンセルを使った！\n\n");
				printf("\t敵の魔法を封印した！\n\n");
				enemy->status = 1;
				e_turn = 0;
		 	    }
			}

		}
		if(hero->life > hero->life_upper){
			hero->life = hero->life_upper;
		}
	}
        if(hero->origin == 2 && TURN == hero->ori_num && hero->life > 0){ //固有スキル(即死魔法)
            printf("\n\t固有スキル！！\n");
     	    printf("       即死魔法をとなえた！\n\n");
 	    dum = random() % 4;
	    if(dum == 0){
		enemy->life = 0;
	    }
	    else{
		printf("       なにもおきなかった！\n\n\n");
	    }
	}
	//敵の死亡確認
	if (enemy->life <= 0) {
		if(enemy->type != 4){
                    printf("\t敵を倒した！\b\n\n");
		}
		else{
		    printf("\tBOSSを倒した！！\b\n\n");
		}
		int fullst = 0;
		int hero_old_level = 0;
		if(hero->status2 == 1){ //不屈のとき
			hero->offence = old_offence;
		}
		if(hero->status == 2){ //腐敗魔法差分
			hero->defence = d_defence;
		}
		if(hero->status != 0){
			printf("      状態異常が回復した！\n\n");
		}
		hero->status = 0;
		hero->status2 = 0;
		if(hero->level == 0 && hero->job != 0){
			hero->count = 20;
		}
		else if(hero->job == 0 && hero->level == 0){
			hero->count = 10;
		}
		fullst = (((enemy_max + enemy->offence + enemy->defence + enemy->m_offence + enemy->m_defence) / 10) * map->floor); //敵の強さによって経験値変化
		if(boss >= 2){ //BOSSなら経験値２倍
			fullst *= 2;
		}
		mon = (fullst / 2);
		hero->money += mon;
                hero->point += fullst;
	        flag2 = 0;
	        deai = 0;
		huhai = 0;
		TURN = 1;
		printf("    %d の経験値を獲得！\b\n\n", fullst);
		printf("     %d 円のお金を獲得！\b\n\n", mon);
		while(hero->point >= hero->count){ //経験値がある限り
		if(hero->point >= hero->count){
			//超過分くりこし
			hero->point -= hero->count;
			if(hero->point < 0){
				hero->point *= (-1);
			}
	                hero->life_upper += 10;
			hero->mp_upper += 10;
			if(hero->job == 0){ //木こり
			    hero->offence += 1;
			    hero->defence += 1;
			    hero->m_offence += 1;
			    hero->m_defence += 1;
			}
			else if(hero->job == 1){ //戦士
			    hero->offence += 3;
			    hero->defence += 3;
			    hero->m_offence += 1;
			    hero->m_defence += 1;
			}
			else if(hero->job == 2){ //魔法つかい
			    hero->offence += 1;
			    hero->defence += 1;
			    hero->m_offence += 3;
			    hero->m_defence += 3;
			}
			hero->level += 1;
			hero->life = (hero->life + (hero->life_upper / 2));
			if(hero->life > hero->life_upper){
				hero->life = hero->life_upper;
			}
			hero->mp += (hero->mp_upper / 2);
			if(hero->mp > hero->mp_upper){
				hero->mp = hero->mp_upper;
			}
	                printf(" Heroのレベルがあがった！！\n\n");
			printf("    HPが50％回復した！\n");
			printf("    MPが50％回復した！\n\n");
			printf(" Hero : 最大HP +10\n");
			printf("        最大MP +10\n");
			if(hero->job == 0){ //木こり
			    printf("        物理攻撃力 +1\n");
			    printf("        物理防御力 +1\n");
			    printf("        魔法攻撃力 +1\n");
			    printf("        魔法防御力 +1\n\n");
			}
		        if(hero->job == 1){ //戦士
			    printf("        物理攻撃力 +3\n");
			    printf("        物理防御力 +3\n");
			    printf("        魔法攻撃力 +1\n");
			    printf("        魔法防御力 +1\n\n");
			}
                        if(hero->job == 2){ //魔法つかい
			    printf("        物理攻撃力 +1\n");
			    printf("        物理防御力 +1\n");
			    printf("        魔法攻撃力 +3\n");
			    printf("        魔法防御力 +3\n\n");
			}
			if(hero->job == 0){
				hero->count = 10;
			}
			else{
			    hero->count = 20;
			}
			for(int i = 0; i < hero->level; i++){
				hero->count *= 2;
			}
		}
		}
                if(hero->job == 0 && hero->level >= 5){
			printf("\t転職可能です！！\n\n");
			show_status(hero);
			printf("転職しない : 0\n\n");
			printf("   戦士    : 1\n\n");
			printf("魔法つかい : 2\n\n");
			printf("どうする？ : ");
			scanf("%c", &com);
			fgets(trush, BUF, stdin);
			printf("\n\n");
			if(com == '0'){
				hero->job = 0;
				printf("\t転職しなかった！\n\n");
			}
			else if(com == '1'){
				hero->job = 1;
				printf("\t戦士に転職した！\n\n");
				printf("\t   最大HP  : +20％\n"); 
				printf("\t  物理攻撃 : +5\n");
				printf("\t  物理防御 : +5\n");
				printf("\t＜物理攻撃威力増加＞\n\n");
				hero->level = 0;
				hero->point = 0;
				hero->count = 20;
				hero->offence += 5;
				hero->defence += 5;
				dum = (hero->life * 0.2);
				hero->life_upper += dum;
				hero->life += dum;
				show_status(hero);
			}
			else if(com == '2'){
				hero->job = 2;
				printf("\t魔法つかいに転職した！\n\n");
				printf("\t    最大MP  : +20％\n");
				printf("\t   魔法攻撃 : +5\n");
				printf("\t   魔法防御 : +5\n");
				printf("\t＜魔法消費MP半減＞\n\n");
				dum = (hero->mp * 0.2);
				hero->mp += dum;
				hero->mp_upper += dum;
				hero->m_offence += 5;
				hero->m_defence += 5;
				hero->level = 0;
				hero->point = 0;
				hero->count = 20;
				show_status(hero);
			}
		}
		printf(" 現在の経験値 : %d / %d\n\n", hero->point, hero->count);
		if(enemy->origin != 0){
			printf("\n敵の固有スキル : ");
		        if(enemy->origin == 1){
			    printf("毎ターンHP自動回復(最大HPの%d％)", ori);
		        }
			else if(enemy->origin == 2){
			    printf("死の宣告(%dターン後確率即死魔法)", ori);
			}
			else if(enemy->origin == 3){
			    printf("かたいよろい(開幕%dターン無敵)", ori);
			}
			printf("\n\n\n敵の固有スキルをうけとる？(y/n) : ");
			scanf("%c", &com);
			fgets(trush, BUF, stdin);
			if(com == 'y'){
			    hero->origin = enemy->origin;
			    hero->ori_num = ori;
			    printf("\n\n\t固有スキルをうけとった！\n\n");
			}
			else{
			    printf("\n\n\t固有スキルをすてた！\n\n");
			}
		}
                get_enemyitem(enemy, hero);
                break;
        }
	//敵行動(「ヘルプ」または「くすり」コマンドが使用されたときは攻撃しない)
	if(help == 0 && drugflag == 0){
                printf("敵の行動 : ");
		enemy_com = (random() % 2);
		if(enemy_com == 0 || map->floor == 1){ //物理攻撃
      		        dum = (((enemy->offence + (map->floor * 0.3)) * (random() % 10)) / (hero->defence + 1)) + (map->floor * 0.25);
			if(hero->origin == 3 && TURN <= hero->ori_num){
				dum = 0;
			}
        	        hero->life -= dum;
        	        printf("%d の物理ダメージを受けた！\n\n", dum);
		}
		else if(enemy_com != 0 && enemy->status == 0){ //魔法攻撃
			magic = random() % 3;
			if(enemy->life == enemy_max && magic == 1){ //HP全開時、回復魔法使用なし
			    magic = random() % 2;
			    if(magic == 0){
				    magic = 0;
			    }
			    else if(magic == 1){
				    magic = 2;
			    }
			}
			if(magic == 0 || hero->status != 0){
		            dum = (((enemy->m_offence + (map->floor * 0.3)) * (random() % 10)) / (hero->m_defence + 1)) + (map->floor * 0.25);
			    if(hero->origin == 3 && TURN <= hero->ori_num){
				    dum = 0;
			    }
			    hero->life -= dum;
			    printf("%d の魔法ダメージを受けた！\n\n", dum);
			}
			else if(magic == 1){ //回復魔法
			    dum = random() % 10 + enemy->m_offence;
			    enemy->life += dum;
			    if(enemy_max < enemy->life){
				    enemy->life = enemy_max;
			    }
			    printf("回復魔法！\n\n");
			    printf("          %d 回復された！\n\n", dum);
			}
			else if(magic == 2){ //特殊魔法
			    dum = random() % 3 + 1;
		            if(dum == 1 && map->floor >= 10){
			        dum = (hero->life_upper * 0.2);
				if(hero->origin == 3 && TURN <= hero->ori_num){
					dum = 0;
				}
			        hero->life -= dum;
			        printf("割合魔法！ \n\n");
			        printf("          %d のダメージを受けた！\n\n", dum);
		  	    }
			    else if(dum == 2 && map->floor >= 5){
			         printf("毒魔法！ \n");
			         printf("\n    状態異常: 毒 になった\n\n");
			         hero->status = 1;
			         turn = 0;
		            }
			    else if(dum == 3){
				 printf("腐敗魔法！\n\n");
				 dum = hero->defence * 0.2;
				 hero->defence -= dum;
				 printf("  Heroの物理防御が %d 減少した！\n\n", dum);
				 hero->status = 2;
				 turn = 0;
		            }
			    else{
				 printf("ようすを見ている...\n\n");
			    }
		       }    
		}
		else if(enemy_com != 0 && enemy->status == 1){
			printf("魔法がつかえない！！\n\n");
		}
		if(hero->status == 1 && turn == 5){ //５ターンで毒解除
			printf("\t毒が治った！\n\n");
			hero->status = 0;
		}
		if(hero->status == 2 && turn >= 3){ // ３ターン以上経過でいつか解除
			dum = random() % 3;
			if(dum == 0){
				hero->status = 0;
				hero->defence = d_defence;
				printf("    Heroの物理防御がもどった！\n\n");
			}
		}
		if(hero->status == 1){ //状態異常 毒
			dum = (hero->life_upper * 0.03);
			hero->life -= dum;
			printf("    毒のダメージを %d うけた！\n\n", dum);
		}
		if(hero->life <= 0 && hero->status2 != 1){ //Hero死亡
			hero_die(map, hero);
		}
		if(hukutu == 1 && hero->status2 == 1){
			printf("   不屈によってしんででしまった...\n\n");
			hero_die(map, hero);
		}
		if(enemy->origin == 2 && TURN == ori){
			printf("\n\t敵の固有スキル！！\n");
			printf("\t即死魔法をとなえた！\n\n");
			dum = random() % 4;
			if(dum == 0){
				printf("\t即死魔法でしんでしまった...\n\n");
				hero_die(map, hero);
			}
			else{
				printf("\tなにもおきなかった！\n\n");
			}
		}
		if(hero->status2 == 1 && hero->life <= 0){
			printf("\t不屈が発動した！！\n");
			printf("\tHP1で耐えた！\n\n");
			hero->life = 1;
		}
		if(enemy->status == 1 && e_turn >= 5){
			dum = random() % 3;
			if(dum == 0){
			    printf("\t敵のマジックキャンセルがとけた！\n\n");
			    e_turn = 0;
			    enemy->status = 0;
			}
		}
		if(hero->origin == 1 && help == 0){
			printf("\n\t 固有スキル！！\n\n");
			dum = (hero->life_upper * hero->ori_num) / 100;
			hero->life += dum;
			if(hero->life > hero->life_upper){
				hero->life = hero->life_upper;
			}
			printf("\tHeroのHPが%d回復した！\n\n", dum);
		}
		if(enemy->origin == 1 && help == 0){
			printf("\n\t敵の固有スキル！！\n\n");
			dum = (enemy_max * ori) / 100; 
			enemy->life += dum;
			if(enemy->life > enemy_max){
			    enemy->life = enemy_max;
			}
			printf("\t敵のHPが%d回復した！\n\n", dum);
		}
		if(hero->origin == 3 && TURN == hero->ori_num){
			printf("\n    Heroの固有スキルがとけた！！\n\n");
		}
		if(enemy->origin == 3 && TURN == ori){
			printf("\n    敵の固有スキルがとけた！！\n\n");
		}
		turn++;
		hukutu++;
		e_turn++;
		TURN++;
        }
   }
   return;
}

void hero_create(struct Hero *hero) {
    hero->point = 0;
    hero->life_upper = 100 + random() % 10;
    hero->life = hero->life_upper;
    hero->x = 0; hero->y = 0;
    hero->offence = random() % 6 + 5;
    hero->defence = random() % 6 + 5;
    hero->drug = 0;
    hero->sword = random() % 3 + 1;
    hero->buckler = random() % 3;
    hero->armer = random() % 2 + 1;
    hero->m_offence = random() % 5 + 5;
    hero->m_defence = random() % 5 + 5;
    hero->staff = random() % 3 + 1;
    hero->robe = random() % 3 + 1;
    hero->status = 0;
    hero->mp_upper = 10 + random() % 10;
    hero->mp = hero->mp_upper;
    hero->status2 = 0;
    hero->origin = 0;
    hero->ori_num = 0;
    hero->job = 0;
    hero->money = 0;
    return;
}

void enemy_create(struct Enemy *enemy, struct Map *map, struct Hero *hero) {
    int i;
    int dec = 0;
    int sum = 0;
    int ran = 0;
    int ff = 0;
    extern int boss;
    extern int ori;
    sum = random() % 3 + 1;
    ff = random() % 8;
    if(ff == 0 || boss == 1){ //固有スキル(BOSSは確定)
	sum = random() % 3;
	if(map->floor == 100){ //ラスボスは確定自動回復
		sum = 0;
	}
	if(sum == 0){ //毎ターン自動回復
            enemy->origin = 1;
	    ori = random() % 4 + 1;
	}
	else if(sum == 1){ //死の宣告(数ターン後確率で即死)
	    enemy->origin = 2;
	    ori = random() % 5 + 10;
	}
	else if(sum == 2){ //開幕数ターン無敵
	    enemy->origin = 3;
	    ori = random() % 3 + 1;
	}
    }
    else{
	enemy->origin = 0;
    }
    enemy->x = random() % map->wide_x;
    enemy->y = random() % map->wide_y;
    enemy->status = 0;
    if(sum == 1){ //ノーマル敵
        enemy->life = random() % 100 + map->floor * 40;
        enemy->offence = random() % 10 + map->floor;
        enemy->defence = random() % (map->floor + 2);
        enemy->m_offence = random() % 10 + map->floor;
        enemy->m_defence = random() % (map->floor + 2);
	enemy->type = 1;
    }
    else if(sum == 2){ //HP(大)、物理防御(少)、魔法防御(少)
	enemy->life = random() % 100 + map->floor * 40;
	dec = (enemy->life * 0.2);
	enemy->life += dec;
	enemy->offence = random() % 10 + map->floor;
	enemy->defence = random() % (map->floor + 2);
	dec = (enemy->defence * 0.4);
	enemy->defence -= dec;
        enemy->m_offence = random() % 10 + map->floor;
        enemy->m_defence = random() % (map->floor + 2);
	dec = (enemy->m_defence * 0.4);
	enemy->m_defence -= dec;
	enemy->type = 2;
    }
    else if(sum == 3){ //HP(少)、物理攻撃(大)、魔法攻撃(大)
        enemy->life = random() % 100 + map->floor * 40;
	dec = (enemy->life * 0.4);
	enemy->life -= dec;
        enemy->offence = random() % 10 + map->floor;
	dec = (enemy->offence * 0.5);
	enemy->offence += dec;
        enemy->defence = random() % (map->floor + 2);
        enemy->m_offence = random() % 10 + map->floor;
	dec = (enemy->m_offence * 0.5);
	enemy->m_offence += dec;
        enemy->m_defence = random() % (map->floor + 2);
	enemy->type = 3;
    }
    if(map->floor % 5 == 0 && boss == 1){ //BOSSキャラ(ステータス特大)
	enemy->x = map->ent_x;
	enemy->y = map->ent_y;
        enemy->life = random() % 100 + map->floor * 45;
        enemy->offence = random() % 10 + (map->floor * 2);
        enemy->defence = random() % map->floor * 2;
        enemy->m_offence = random() % 10 + (map->floor * 2);
        enemy->m_defence = random() % map->floor * 2;
	enemy->type = 4;
	if(map->floor % 10 == 0){ //１０階ごとに強化
	    enemy->offence *= 1.5;
	    enemy->defence *= 1.5;
	    enemy->m_offence *= 1.5;
	    enemy->m_defence *= 1.5;
	}
	if(map->floor == 100){ //ラスボスはさらに強化
	    enemy->life *= 2;
	    enemy->offence *= 1.5;
	    enemy->defence *= 1.5;
	    enemy->m_offence *= 1.5;
	    enemy->m_defence *= 1.5;
	}
    }
    ran = random() % 2;
    if(map->floor % 5 == 0 && boss == 1){
	    ran = 1;
    }
    if (ran == 1) {   /* item を持っている時 */
        i = random() % 5 + 1;
        enemy->item_suit = i;
        if (i == 1) {
            enemy->item_strong = random() % (10 + map->floor);
        }
        else if (i == 2) {
            enemy->item_strong = random() % (5 + map->floor);
        }
        else {
            enemy->item_strong = random() % (7 + map->floor);
        }
    }
    else {                  /* item を持っていない時 */
        enemy->item_suit = 0;
        enemy->item_strong = 0;
    }
    if(map->floor % 5 == 0 && boss == 1){
	 enemy->item_strong = (random() % 10 + (map->floor * 1.3));
    }
    return;
}

void command(struct Hero *hero, struct Map *map, struct Enemy *enemy, struct Merchant *merchant) {
    char input;
    extern int flag;
    extern int ori;
    extern int boss;
    int d_mp;
    extern int a;
    extern int strong;
    extern int drug;
    extern int sum;
    extern int money1;
    extern int money2;
    extern int fflag;

    printf("\n＜コマンド＞\n\n");
    printf("              上 : k\n");
    printf("                 ↑\n");
    //printf("            |\n");
    printf("左 : h ←-    ステータス : s    -→ 右 : l\n");
    //printf("          |");
    printf("                 ↓\n");
    printf("              下 : j\n\n");
    printf(" 薬  : d        終了(セーブする)  : q\n");
    printf("魔法 : m        終了(セーブしない): z\n");
    for (;;) {
        printf("\ncommand input => ");
        if (scanf("%c", &input) != 1) {
            printf("Error:");
            fgets(trush, BUF, stdin);
        }
        else {
            fgets(trush, BUF, stdin);
	    if(input == 'g'){
		    hero->mp = hero->mp_upper;
		    break;
	    }
            if (input == 'd') {
                use_drug(hero); break;
            }
            else if (input == 's') {
                show_status(hero); break;
            }
            else if (input == 'j' || input == 'k' || input == 'l' || input == 'h') {
                walk_arround(input, hero, map); break;
            }
            else if (input == 'q') {
		printf("\n\tデータを保存して終了します。\n\n");
		//データ保存
		FILE *fp = fopen("Savefile", "w");
		if( fp == NULL ) {
                        printf("\nファイルオープンエラー\n");
			exit(0);
                }
                else{
                        fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", hero->point, hero->x, hero->y, hero->life, hero->life_upper,
					                hero->offence, hero->defence, hero->drug, hero->sword, hero->buckler, hero->armer,
							hero->m_offence, hero->m_defence, hero->level, hero->count, hero->staff, hero->robe, hero->status, hero->mp,
							hero->mp_upper, hero->status2, hero->origin, hero->ori_num, hero->job, hero->money);
			fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d\n", enemy->x, enemy->y, enemy->life, enemy->offence, enemy->defence, 
					                      enemy->item_suit, enemy->item_strong, enemy->m_offence, enemy->m_defence, enemy->type, enemy->status, enemy->origin);
			fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", map->wide_x, map->wide_y, map->ent_x, map->ent_y, map->floor,
					                map->item_exist, map->item_strong, map->item_x, map->item_y);
			fprintf(fp, "%d %d %d %d %d %d %d %d %d %d\n", boss, flag, ori, a, strong, drug, sum, money1, money2, fflag);
			fprintf(fp, "%d %d\n", merchant->x, merchant->y);
                        fclose(fp);
                }

                exit(0);
            }
	    else if(input == 'z'){
		    printf("\n\t終了します。\n\n");
		    exit(0);
	    }
	    else if(input == 'm'){
		    show_status(hero);
		    d_mp = hero->mp;
		    printf("\n");
		    printf("使わない : 0\n\n");
		    if(hero->job == 2){
		        printf("ワープ(階段)(消費MP 50 ↓↓) : 1\n\n");
		        printf("リクリエイト(消費MP 20 ↓↓) : 2\n\n");
		    }
		    else{
                        printf("ワープ(階段)(消費MP 100) : 1\n\n");
		        printf("リクリエイト(消費MP 40) : 2\n\n");
	 	    }
		    printf("どの魔法を使う？ : ");
		    scanf("%c", &input);
		    fgets(trush, BUF, stdin);
		    printf("\n\n");
		    if(input == '0'){
			    break;
		    }
		    if(input == '1'){
			    if(hero->job == 2){
			        hero->mp -= 50;
			    }
			    else{
				hero->mp -= 100;
			    }
			    if(hero->mp < 0){
				    printf("\tMPがたりない！！\n");
				    hero->mp = d_mp;
			    }
			    else{
			        printf("\tHeroはワープ(階段)をつかった！\n\n");
			        hero->x = map->ent_x;
				hero->y = map->ent_y;
				printf("\t  Heroは階段へ移動した！\n");
			    }
			    break;
		    }
                    if(input == '2'){
			    if(hero->job == 2){
			        hero->mp -= 20;
			    }
			    else{
				hero->mp -= 40;
			    }
			    if(hero->mp < 0){
				    printf("\tMPがたりない！！\n");
				    hero->mp = d_mp;
			    }
			    else{
			        printf("\tHeroはリクリエイトをつかった！\n\n");
				enemy->life = 0;
				enemy_create(enemy, map, hero);
				printf("\t   敵が再配置された！\n");
			    }
			    break;
		    }
	    }
        }
    }
    printf("\n");
    return;
}

/* move  lower j:  left l: right h: upper k*/
void walk_arround(char move, struct Hero *hero, struct Map *map) {
    if (move == 'l') {
        if (hero->x == map->wide_x) {
            printf("その方向には歩けません!\n");
            return;
        }
        hero->x++;
    }
    else if (move == 'k') {
        if (hero->y == map->wide_y) {
            printf("その方向には歩けません!\n");
            return;
        }
        hero->y++;
    }
    else if (move == 'h') {
        if (hero->x == 0) {
            printf("その方向には歩けません!\n");
            return;
        }
        hero->x--;
    }
    else if (move == 'j') {
        if (hero->y == 0) {
            printf("その方向には歩けません!\n");
            return;
        }
        hero->y--;
    }
    return;
}


int pos_compare(int a_x, int a_y, int b_x, int b_y) {
    if (a_x == b_x && a_y == b_y) {
        return !0;
    }
    else {
        return 0;
    }
}

void get_drug(struct Map *map, struct Hero *hero) {
    if (map->item_exist == 1) {
        printf("\n\t薬(%d)を拾った！\b\n\n", map->item_strong);
        hero->drug += map->item_strong;
    }
    return;
}

void current_point(struct Hero *hero, struct Map *map) {
    hero->x = random() % map->wide_x;
    hero->y = random() % map->wide_y;
    return;
}


int yesno() {
    char input;
    for (;;) {
        printf("yes or no (y/n) => ");
        if (scanf("%c", &input) != 1) {
            printf("Error:");
            fgets(trush, BUF, stdin);
        }
        else {
            fgets(trush, BUF, stdin);
            if (input == 'y' || input == 'Y') {
                return !0;
            }
            else if (input == 'n' || input == 'N') {
                return 0;
            }
        }
    }
}

void merchant_create(struct Map *map, struct Merchant *merchant) {
    int ran = 0;
    ran = random() % 3;

    if(ran == 0 && map->floor >= 2){ //存在する
        merchant->x = random() % map->wide_x;
	merchant->y = random() % map->wide_x;
    }
    else{ //存在しない
	merchant->x = map->wide_x + 1;
	merchant->y = map->wide_y + 1;
    }
    if(merchant->x == map->ent_x && merchant->y == map->ent_y){ //階段と同じだと処理がめんどくさいので再抽選
	merchant->x = random() % map->wide_x;
	merchant->y = random() % map->wide_y;
    }

}

void shop(struct Map *map, struct Hero *hero) {
    extern int a;
    extern int strong;
    char str;
    extern int drug;
    extern int sum;
    extern int money1;
    extern int money2;
    int old_money;
    int old_floor;
    extern int fflag;
    int da;
    if(fflag == 0 && old_floor != map->floor){
    old_floor = map->floor;
    drug = random() % (10 + 10 * map->floor) + 30;
    a = random() % 5;
    strong = (random() % 3 + 1) * (map->floor / 2);
    if(strong < map->floor){
        strong *= 1.5;
    }
    money1 = strong * 500;
    money2 = drug * 15;
    fflag++;
    }
    printf("\n商人 : ようこそ\n\n");
    while(1){
	show_status(hero); 
	printf("\n\n商人 : お好きなものをどうぞ\n");
	old_money = hero->money;
	printf("\n\n買わない : 0\n\n");
        if(a == 0 && sum == 0){
	    printf("剣(Lv. %d)(%d 円) : 1\n\n", strong, money1);
	    da = hero->sword;
        }
	else if(a == 1 && sum == 0){
		printf("盾(Lv. %d)(%d 円) : 1\n\n", strong, money1);
		da = hero->buckler;
	}
	else if(a == 2 && sum == 0){
		printf("鎧(Lv. %d)(%d 円) : 1\n\n", strong, money1);
		da = hero->armer;
	}
	else if(a == 3 && sum == 0){
		printf("杖(Lv. %d)(%d 円) : 1\n\n", strong, money1);
		da = hero->staff;
	}
	else if(a == 4 && sum == 0){
		printf("ローブ(Lv. %d)(%d 円) : 1\n\n", strong, money1);
		da = hero->robe;
	}
	printf("薬(回復量 %d)(%d 円) : 2\n\n\n", drug, money2);
	printf("どうする？ : ");
	scanf("%c", &str);
	fgets(trush, BUF, stdin);
	if(str == '0'){
		printf("\n\n商人 : またどうぞ\n\n");
		break;
	}
	else if(str == '1' && sum == 0){
		hero->money -= money1;
		if(hero->money < 0){
			printf("\tお金がたりません！！\n\n");
			hero->money = old_money;
		}
		else if(strong <= da){
			printf("\n   現在の装備より弱い装備は購入できません！\n\n");
			hero->money = old_money;
		}
		else{
		printf("\n\t装備を新調しました！\n\n");
		if(a == 0){
		    hero->offence = hero->offence - hero->sword + strong;
      		    hero->sword = strong;
        	}
		else if(a == 1){ 
		    hero->defence = hero->defence - hero->buckler + strong;
		    hero->buckler = strong;
		}
		else if(a == 2){
		    hero->defence = hero->defence - hero->armer + strong;
		    hero->armer = strong;
		}
		else if(a == 3){
		    hero->m_offence = hero->m_offence - hero->staff + strong;
		    hero->staff = strong;
		}
		else if(a == 4){ 
		    hero->m_defence = hero->m_defence - hero->robe + strong;
		    hero->robe = strong;
		}
		sum++;
		}
	}
	else if(str == '2'){
		hero->money -= money2;
		if(hero->money < 0){
			printf("\tお金がたりません！！\n\n");
			hero->money = old_money;
		}
		else{
			printf("\n\t薬(回復量 %d)を購入した！\n\n", drug);
			hero->drug += drug;
		}
	}
	else{
		continue;
	}
    }
}
