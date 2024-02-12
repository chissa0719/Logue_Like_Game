/* enemy */
struct Enemy {

    int x;            /* x 座標 */
    int y;            /* y 座標 */
    int life;         /* 生命力 */
    int offence;      /* 攻撃力 */
    int defence;      /* 守備力 */
    int item_suit;    /* アイテムの種類 */
    int item_strong;  /* アイテムのレベル */
    int m_offence;    /* 魔法攻撃力 */
    int m_defence;    /* 魔法防御力 */
    int type;         /* 敵の個性 */
    int status;       /* 敵の状態異常 */
    int origin;       /* 固有スキル */
};

/* map */
struct Map {

    int wide_x;         /* 横の広さ */
    int wide_y;         /* 縦の広さ */
    int ent_x;          /* 階段の x 座標 */
    int ent_y;          /* 階段の y 座標 */
    int floor;          /* 階 */
    int item_exist;     /* アイテムの有無 */
    int item_strong;    /* アイテムのレベル*/
    int item_x;         /* アイテムの場所 x */
    int item_y;         /* アイテムの場所 y */

};

/* hero */
struct Hero {

    int point;         /* 経験値 */
    int x;             /* x 座標 */
    int y;             /* y 座標 */
    int life;          /* 生命力 */
    int life_upper;    /* 生命力の上限値 */
    int offence;       /* 攻撃力 */
    int defence;       /* 守備力 */
    int drug;          /* 薬の回復力 */
    int sword;         /* 剣のレベル */
    int buckler;       /* 盾のレベル */
    int armer;         /* 鎧のレベル */
    int m_offence;     /* 魔法攻撃力 */
    int m_defence;     /* 魔法防御力 */
    int level;         /* レベル */
    int count;         /* 経験値レート */
    int staff;         /* 杖のレベル */
    int robe;          /* ローブのレベル */
    int status;        /* 状態異常 */
    int mp;            /* MP */
    int mp_upper;      /* MPの上限値 */
    int status2;       /* バフ */
    int origin;        /* 固有スキル */
    int ori_num;       /* 固有スキル値 */
    int job;           /* 職業 */
    int money;         /* 所持金 */
};

/* 商人 */
struct Merchant {

    int x;             /* x 座標 */
    int y;             /* y 座標 */

};

void map_create(struct Map*);

void show_status(struct Hero*);

void hero_die(struct Map*, struct Hero*);

void calc_parameter(struct Hero*, struct Enemy*);

void use_drug(struct Hero*);

void get_enemyitem(struct Enemy*, struct Hero*);

int escape(void);

void fight(struct Hero*, struct Enemy*, struct Map*);

void hero_create(struct Hero*);

void enemy_create(struct Enemy*, struct Map*, struct Hero*);

void command(struct Hero*, struct Map*, struct Enemy*, struct Merchant*);

void walk_arround(char, struct Hero*, struct Map*);

int  pos_compare(int, int, int, int);

void get_drug(struct Map*, struct Hero*);

void current_point(struct Hero*, struct Map*);

int yesno(void);

void gameclear(struct Map*, struct Hero*);

void merchant_create(struct Map*, struct Merchant*);

void shop(struct Map*, struct Hero*);
