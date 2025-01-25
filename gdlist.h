#pragma	comment( user,"@(#)$Workfile: gdlist.h $$Revision: 3 $$Date: 25/01/25 11:54 $$NoKeywords: $")
/*********************************************************************
 *
 *     #include <gdlist.h> : 双方向リスト管理(GD_LIST processor)
 *
 *********************************************************************/

#ifndef _GDLIST_H
#define	_GDLIST_H

/*
*   このリストでは、各要素ごとにkey(int型)を割り付けて管理します
*   このリストでは、順方向の要素の並びがkeyの昇順になるよう管理します
*   keyの一意モードを使えば(U_UNIQ)、key値による簡便な扱いができます
*   keyの重複モードを使えば(U_DUP)、柔軟な双方向リストとして使用できます
*
*
*     struct gd_list *
*     gd_init(flg)
*          int   flg;
*
*          双方向リストを初期化します
*          リストのハンドル(head)を返します
*          flgはkeyがU_DUP(重複あり),U_UNIQ(一意)のいずれかを示します
*          この要素はダミーです(GD_LIST processorが使用します)
*          headの値は以降のハンドリングのために必要です
*
*
*     struct gd_list *
*     gd_insert(head,key,data,size) 
*          struct gd_list *head;
*          int             key;
*          caddr_t         data;
*          int             size;
*
*          相対的なkeyの位置(昇順)にdataを挿入します
*          keyが存在すればその要素(ら)の前に挿入します(U_DUPの場合)     
*          keyが存在すればその要素のデータを更新します(U_UNIQの場合)     
*          挿入または更新された要素を返します     
*          sizeは sizeof(型) によって得られるバイト数です
*          keyの値についてはuser次第です
*          内部的には、sizeバイトのエリアをmallocし、bcopyによりdataを複製しています
*
*
*     struct gd_list *
*     gd_append(head,key,data,size) 
*          struct gd_list *head;
*          int             key;
*          caddr_t         data;
*          int             size;
*
*          相対的なkeyの位置(昇順)にdataを挿入します
*          keyが存在すればその要素(ら)の後に挿入します(U_DUPの場合)     
*          keyが存在すればその要素のデータを更新します(U_UNIQの場合)     
*          挿入または更新された要素を返します     
*          sizeは sizeof(型) によって得られるバイト数です
*          keyの値についてはuser次第です
*          内部的には、sizeバイトのエリアをmallocし、bcopyによりdataを複製しています
*
*
*     struct gd_list *
*     gd_delete(head,key)
*          int             key;
*
*          keyの位置の要素を１つだけ削除します(U_DUPの場合注意)
*          keyの位置の次の要素を返します
*          keyが存在しなければ0を返します
*          keyが最大値なら(その要素が最後の要素なら)headを返します
*
*
*     struct gd_list *
*     gd_find(head,current,key)
*          struct gd_list *head,
*                         *current;
*          int             key;
*
*          headが指すリストのcurrent以降の要素でkeyが一致する要素を返します
*          先頭の要素はGD_FIRST(head)です
*          keyが複数存在する場合は最初に一致した要素を返します
*          keyが存在しなければ0を返します
*
*
*     void
*     gd_kill(head)
*          struct gd_list *head;
*
*          dg_list processorが確保した領域を解放します
*          リストが不要になった時点で解放するべきです
*          headの値は(struct gd_list *)NULLになります
*
*
*     void
*     gd_browser(head,msg,fnc)
*          struct gd_list *head;
*          char *msg;
*		   void   (*fnc)();
*
*          メッセージ(msg)に続けて現在のリストの状態をstderrに出力します
*          fncは要素一つ一つに適用する関数で、次のように定義します
*               void
*               fnc(item)
*               struct gd_list *item;
*               {
*                  (  処理
*                  (*例えばstderrに出力します*)
*				   cerr << ((struct data_s *)item)->num << ","
*						<< ((struct data_s *)item)->str << endl;
*                  )
*               }
*
*
*     void
*     gd_loop(head,current,fnc)
*          struct gd_list *head,
*                         *current;
*          void   (*fnc)();
*
*          headが指すリストのcurrent以降の要素にfncを適用します
*          先頭の要素はGD_FIRST(head)です
*          fncは要素一つ一つに適用する関数で、次のように定義します
*               void
*               fnc(item)
*               struct gd_list *item;
*               {
*                  (  処理  
*                  (*例えばstderrに出力します*)
*				   cerr << ((struct data_s *)item)->num << ","
*						<< ((struct data_s *)item)->str << endl;
*                  )
*               }
*
*
*     struct gd_list *
*     gd_findf(head,current,fnc)
*          struct gd_list *head,
*                         *current;
*          int    (*fnc)();
*
*          headが指すリストのcurrent以降の要素にfncを適用し
*          戻り値がTRUEとなった要素を返します
*          先頭の要素はGD_FIRST(head)です
*          TRUEとなる要素が複数存在する場合は最初にTRUEとなった要素を返します
*          TRUEとなる要素が存在しなければ0を返します
*          fncを適用する要素は順方向の要素の並びに沿って(keyの昇順に)選ばれます
*          fncは要素一つ一つに適用する関数で、次のように定義します
*               int
*               fnc(item)
*               struct gd_list *item;
*               {
*                  (  処理  
*                     return TRUE;
*                     return FALSE;
*                  )
*               }
*
*
*
*     struct gd_list *
*     gd_insert_force(head,item,data,size)
*          struct gd_list *head,
*                         *item;
*          caddr_t         data;
*          int             size;
*
*          itemの直前にdataを挿入します
*          リストがU_UNIQの場合、挿入するkey値がないとitemのdataを更新します
*          リストがU_DUPの場合、挿入するkey値がないとitemのkey値が重複します
*          挿入または更新された要素を返します     
*          sizeは sizeof(型) によって得られるバイト数です
*
*
*     struct gd_list *
*     gd_append_force(head,item,data,size)
*          struct gd_list *head,
*                         *item;
*          caddr_t         data;
*          int             size;
*
*          itemの直後にdataを追加します
*          リストがU_UNIQの場合、追加するkey値がないとitemのdataを更新します
*          リストがU_DUPの場合、追加するkey値がないとitemのkey値が重複します
*          追加または更新された要素を返します     
*          sizeは sizeof(型) によって得られるバイト数です
*
*
*     struct gd_list *
*     gd_delete_force(head,item)
*          struct gd_list *head,
*                         *item;
*
*          itemを削除します
*          itemの位置の次の要素を返します
*          itemが最後の要素ならheadを返します
*
*
*     void
*     gd_qsort(head,fnc)
*          struct gd_list *head;
*          int    (*fnc)();
*
*          headリストの要素をfncの評価にしたがってソートします
*          keyの値は、ソート前の状態が保持されます。
*          順方向の要素の並びとkeyの昇順は、多くの場合、対応を失います。
*          fncは隣あう２つの要素へのポインタを引数として呼び出される
*          比較ルーチンの名前です。
*          比較ルーチンが負の値を返すとき入れ替えが起こります。
*            次の関数はその例です
*              int
*              gd_data_str_order(item1,item2)(*データ(string)の昇順*)
*              struct gd_list *item1,
*                             *item2;
*              {
*              (*-----    procedure*)
*                  return(strcmp(item1->data,item2->data) * -1);
*              }
*          注意: 現在はqsortアルゴリズムにはなっていません。(将来も)
*                比較ルーチンの評価が等しいときは0を返しましょう。
*
*
*     struct gd_list *
*     gd_concat(head1,head2)
*          struct gd_list *head1,
*                         *head2;
*
*          head1リストの後ろにhead2リストを連結します
*          連結された新しいリストのハンドル(head)を返します
*          head1,2のふたつのリストはgd_killされ、head1,2の値はNULLになります
*          keyの値は、連結前の状態が保持されます
*          順方向の要素の並びとkeyの昇順は、多くの場合、対応を失います。
*/

#define U_DUP  0  /*キー値の重複を認めます*/
#define U_UNIQ 1  /*キー値の重複を認めません*/

#ifndef TRUE
#define TRUE   1
#define FALSE  0
#endif

#define GD_FIRST(head)         ((head)->next)
#define GD_SECOND(head)        ((head)->next->next)
#define GD_NEXT(item)          ((item)->next)
#define GD_END(head)           (head)
#define GD_LAST(head)          ((head)->prev)
#define GD_PREV(item)          ((item)->prev)
#define GD_IS_EMPTY(head)      (((head)->next==(head))?TRUE:FALSE)
#define GD_IS_LAST(head,item)  (((item)->next==(head))?TRUE:FALSE)

/*
typedef char *   caddr_t;
*/

struct gd_list {                /*General Data List structure*/
          struct gd_list *next; /*一つ後の要素、または、headへのポインタ*/
          struct gd_list *prev; /*一つ前の要素、または、headへのポインタ*/
          int             key;  /*gd_insertなどで示されたキー値*/
          int             dsize;/*gd_insertなどで示されたデータサイズ*/
          unsigned char   *data; /*gd_insertなどで格納されたデータへのポインタ*/
          unsigned char   *client_data;/*for user*/
};


struct gd_list *gd_init(int flg);      /*processor*/
struct gd_list *gd_insert(struct gd_list *head,int key,unsigned char *data,int size);
struct gd_list *gd_append(struct gd_list *head, int key, unsigned char *data, int size);
struct gd_list *gd_delete(struct gd_list *head, int key);
struct gd_list *gd_find(struct gd_list *head, struct gd_list *curr, int key);
#define gd_kill(head) _gd_kill(&(head))
void            _gd_kill(struct gd_list **headp);
void            gd_browser(struct gd_list *head,char *msg, void(*fnc)(struct gd_list* where));
void            gd_loop(struct gd_list *head, struct gd_list *curr, void(*fnc)(struct gd_list *where));
struct gd_list *gd_findf(struct gd_list *head, struct gd_list *curr, int(*fnc)(struct gd_list *where));
struct gd_list *gd_insert_force(struct gd_list *head, struct gd_list *curr, unsigned char *data, int size);/*強制的にリストを操作します*/
struct gd_list *gd_append_force(struct gd_list *head, struct gd_list *curr, unsigned char *data, int size);
struct gd_list *gd_delete_force(struct gd_list *head, struct gd_list *curr);
void            gd_qsort(struct gd_list *head, int(*fnc)(struct gd_list *, struct gd_list *));
#define gd_concat(head1,head2)  _gd_concat(&(head1),&(head2))
struct gd_list *_gd_concat(struct gd_list **head1, struct gd_list **head2);

int             gd_data_str_order(struct gd_list *item1, struct gd_list *item2);/* data_strの昇順  gd_qsort staff */
int             gd_key_order_asc(struct gd_list *item1, struct gd_list *item2); /* keyの昇順 */
int             gd_key_order_dec(struct gd_list *item1, struct gd_list *item2); /* keyの降順 */

/*
*      macroによるgd_loopのもうひとつの使い方です
*
*          GD_LOOP(head,current)
*             (* body *)
*          GD_END_LOOP
*
*          bodyの中では変数名itemとして要素を参照します
*          itemの型は(struct gd_list *)です
*          head,currentの型はいずれも(struct gd_list *)です
*          例えば、次のように使います。
*              {int i=1; (*keyの値をone-basedで整理します*)
*               GD_LOOP(head,GD_FIRST(head))
*                  item->key = i++;
*               GD_END_LOOP
*              }
*
*          GD_LOOPR(head,current)	は、逆方向に要素を選びます
*             (* body *)
*          GD_END_LOOP
*
*/
#define GD_LOOP(head,current)  {struct gd_list *item;\
								/*[ GD_LOOP        head ]*/\
                                for(item=current;item!=head;item=item->next){

#define GD_LOOPR(head,current) {struct gd_list *item;/*逆方向に要素を選びます*/\
                                for(item=current;item!=head;item=item->prev){

#define GD_END_LOOP             }\
                               }

/*
*      macroによるgd_findfのもうひとつの使い方です
*
*          GD_FINDF(find,head,current,condition)
*
*          見つかった要素は、findに代入されます。
*          conditionの中では変数名itemとして要素を参照します
*          itemの型は(struct gd_list *)です
*
*          find,head,currentは、いずれも(struct gd_list *)型です。
*          conditionは、if(condition) として展開されます。
*          例えば、次のように使います。
*              GD_FINDF(find,keito_list,GD_FIRST(keito_list),
*                       (strcmp(((struct keitom *)item->data)->keito,
*                               input_keito)
*                        ==NULL))(*入力と一致する単系統を探します*)
*
*          GD_FINDFR(find,head,current,condition)	は、逆方向に要素を選びます
*
*/
#define GD_FINDF(find,head,current,condition) {struct gd_list *gd;\
											   gd = NULL;\
											   GD_LOOP(head,current)\
												 if(condition){\
												   gd = item;\
												   break;\
												 }\
											   GD_END_LOOP\
											   find = gd;\
											  }
#define GD_FINDFR(find,head,current,condition) {struct gd_list *gd;\
											   gd = NULL;\
											   GD_LOOPR(head,current)\
												 if(condition){\
												   gd = item;\
												   break;\
												 }\
											   GD_END_LOOP\
											   find = gd;\
											  }

#endif	/* _GDLIST_H */
