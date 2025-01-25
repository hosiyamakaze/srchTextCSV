static char sccsid[] =
"@(#)$Workfile: gdlist.cpp $$Revision: 3 $$Date: 25/01/25 11:54 $$NoKeywords: $";
/********************************************************************************
 *
 *	Generic Data List Processor : 汎用的なデータを双方向リスト状に管理します
 *
 ********************************************************************************/

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <ctype.h>
#include        <malloc.h>
#include        <memory.h>
#include        <iostream>
#include        <iomanip>
#include        "gdlist.h"

 /* -----	 define */
#define	bzero(p,l)	memset(p,0x00,l)
#define	bcopy(src,dst,l)	memcpy(dst,src,l)

 /* -----	 static */

 /* -----	processor */
void	gd_error();


/*
*****************************************************************************
* gd_init: 双方向リストを初期化します。
*****************************************************************************
*/
struct gd_list *
gd_init(
int flg)
{
    register struct gd_list *head;

/*-----    procedure*/
    head       = (struct gd_list *)malloc(sizeof(struct gd_list));
	if(head == NULL) gd_error();
    bzero(head,sizeof(struct gd_list));
    head->next = head;
    head->prev = head;
    head->key  = (int)NULL;/* dg_*()により任意の値がセットされる*/
    head->dsize= 1;
    head->data = (unsigned char *)malloc(head->dsize);/* gd_*()により使用される*/
	if(head->data == NULL) gd_error();
    *head->data=(flg==U_DUP)?'d':'u';
    
    return head;  /*この要素はダミーである*/
}


/*
*****************************************************************************
* gd_insert: 相対的なkeyの位置にdataを挿入します。keyが存在すればデータを更新する
*****************************************************************************
*/
struct gd_list *
gd_insert(
struct gd_list *head,
int key,
unsigned char *data,
int size)
{
    register struct gd_list *where,
                            *newer;
    char     flg;

/*-----    procedure*/

    head->key  = key;
    flg = *head->data;

    /*keyの昇順の方向に位置を捜します*/
    for(where = head->next; where->key < key ;where = where->next)
        continue;
    if((where == head)||(where->key > key)||(flg=='d')){
        /*whereの前に挿入する*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where;
		newer->prev = where->prev;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpyとは転送方向が逆である*/
		newer->dsize= size;
        where->prev->next = newer;
        where->prev = newer;
    }else{
        /*whereのデータをdataに更新する*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currentlyな要素を返す*/
}


/*
*****************************************************************************
* gd_append:	相対的なkeyの位置にdataを挿入します。keyが存在すればデータを更新する
*****************************************************************************
*/
struct gd_list *
gd_append(
struct gd_list *head,
int key,
unsigned char *data,
int size)
{
    register struct gd_list *where,
                            *newer;
    char     flg;

/*-----    procedure*/

    /*head->key  = key;*/
    flg = *head->data;

    /*keyの昇順の方向に位置を捜します*/
    for(where = head->next;
		(where!=head) && (flg=='d'?(where->key <= key):(where->key < key));
		where = where->next)
        continue;
    if((where == head)||(where->key > key)||(flg=='d')){
        /*whereの前に挿入する*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where;
		newer->prev = where->prev;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpyとは転送方向が逆である*/
		newer->dsize= size;
        where->prev->next = newer;
        where->prev = newer;
    }else{
        /*whereのデータをdataに更新する*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currentlyな要素を返す*/
}


/*
*****************************************************************************
* :
*****************************************************************************
*/
struct gd_list *     /*keyの位置の要素を削除します。keyが存在しなければ0を返す */
gd_delete(
struct gd_list *head,
int key)
{
    register struct gd_list *where,
                            *current;

/*-----    procedure*/
    head->key = key;
    /*keyの昇順の方向に位置を捜します*/
    for(where = head->next; where->key != key ;where = where->next)
        continue;
    if(where != head){
        /*リストポインタを更新する*/
        where->next->prev = where->prev;
        where->prev->next = where->next;
        current           = where->next;
        /*whereを削除する*/
        free(where->data);
        free((char *)where);
    }else{
        /*該当のデータはない*/
        current = NULL;
    }

    return current;/*currentlyな要素を返す*/
}



/*
*****************************************************************************
* gd_find:	keyの位置の要素を返します。keyが存在しなければ0を返す
*****************************************************************************
*/
struct gd_list *
gd_find(
struct gd_list *head,
struct gd_list *current,
int key)
{
    register struct gd_list *where;

/*-----    procedure*/
    /*keyの昇順の方向に位置を捜します*/
    for(where = current; where != head ;where = where->next){
        if(where->key == key) return where;
    }
    return NULL;
}


/*
*****************************************************************************
* _gd_kill:	dg_listが確保した領域を解放する
*****************************************************************************
*/
void
_gd_kill(
struct gd_list **headp)
{
    register struct gd_list *head,
                            *where,
                            *next;

/*-----    procedure*/
    head = *headp;
    /*keyの昇順の方向に沿って解放する*/
    for(where = head->next; where != head ;where = next){
        next =where->next;
        /*whereの領域を解放する*/
        free(where->data);
        free((char *)where);
    }
    /*headの領域を解放する*/
    free(head->data);
    free((char *)head);
    *headp = (struct gd_list *)NULL;/*headの値をNULLクリアする*/
}


/*
*****************************************************************************
* gd_browser:	dg_listが確保した現在の内容をstderrに出力します(DEBUG用)
*****************************************************************************
*/
void
gd_browser(
struct gd_list *head,
char *msg,
void   (*fnc)(struct gd_list* where))
{
    register struct gd_list *where;

/*-----    procedure*/

    std::cerr << std::endl << msg << std::endl;
    std::cerr << "GD_LIST:head=0x" << std::hex << std::setw(8) << std::setfill('0') << head << std::endl;
    /*keyの昇順の方向に沿って出力する*/
    for(where = head->next; where != head ;where = where->next){
        std::cerr << "GD_LIST:addr=0x" << std::hex << (uintptr_t)where
            << ",next=0x" << (uintptr_t)where->next
            << ",prev=0x" << (uintptr_t)where->prev
            << ",key=" << std::dec << where->key
            << ",dsize=" << where->dsize
            << ",data=0x" << std::hex << (uintptr_t)where->data
            << std::endl;

        std::cerr << "GD_LIST:data=";
        if (fnc == NULL) {
			std::cerr << where->data << std::endl;
		}else{
			(*fnc)(where);
        }

    }
}


/*
*****************************************************************************
* gd_loop:	headリストのcurrentが指す要素以降にfncを適用する
*****************************************************************************
*/
void
gd_loop(
struct gd_list *head,
struct gd_list *current,
void           (*fnc)(struct gd_list *where))
{

     struct gd_list *where;

/*-----    procedure*/
    /*keyの昇順の方向に沿って適用する*/
    for(where = current; where != head ;where = where->next){
        (*fnc)(where);
    }
}


/*
*****************************************************************************
* gd_findf:	headリストにfncを適用しTRUEとなった要素を返す
*****************************************************************************
*/
struct gd_list *
gd_findf(
struct gd_list *head,
struct gd_list *current,
int            (*fnc)(struct gd_list *where))
{

     struct gd_list *where;

/*-----    procedure*/
    /*keyの昇順の方向に沿って適用する*/
    for(where = current; where != head ;where = where->next){
        if((*fnc)(where)) return where;
    }
    return NULL;
}


/*
*****************************************************************************
* gd_insert_force:	itemの直前にdataを挿入します。keyが存在すればデータを更新する
*****************************************************************************
*/
struct gd_list *
gd_insert_force(
struct gd_list *head,
struct gd_list *item,
unsigned char  *data,
int             size)
{
    register struct gd_list *where,
                            *newer;
    int      key;
    char     flg;

/*-----    procedure*/

    where = item;
    flg = *head->data;

    key = where->key -1;
    if(item->prev != head){
       if(key <= where->prev->key){
          key = where->key;
       }
    }
    if((flg=='d') || key < where->key){
        /*whereの前に挿入する*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where;
		newer->prev = where->prev;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpyとは転送方向が逆である*/
		newer->dsize= size;
        where->prev->next = newer;
        where->prev = newer;
    }else{
        /*whereのデータをdataに更新する*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currentlyな要素を返す*/
}


/*
*****************************************************************************
* gd_append_force:	itemの直後にdataを追加します。keyが存在すればデータを更新する
*****************************************************************************
*/
struct gd_list *
gd_append_force(
struct gd_list *head,
struct gd_list *item,
unsigned char  *data,
int             size)
{
    register struct gd_list *where,
                            *newer;
    int      key;
    char     flg;

/*-----    procedure*/

    where = item;
    flg = *head->data;

    key = where->key + 1;
    if(where->next != head){
       if(key >= where->next->key){
          key = where->key;
       }
    }
    if((flg=='d') || key > where->key){
        /*whereの直後に追加する*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where->next;
		newer->prev = where;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpyとは転送方向が逆である*/
		newer->dsize= size;
        where->next->prev = newer;
        where->next = newer;
    }else{
        /*whereのデータをdataに更新する*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currentlyな要素を返す*/
}


/*
*****************************************************************************
* gd_delete_force:	*itemを削除します
*****************************************************************************
*/
struct gd_list *
gd_delete_force(
struct gd_list *head,
struct gd_list *item)
{
    register struct gd_list *where,
                            *current;

/*-----    procedure*/
    where = item;
    if(where != head){
        /*リストポインタを更新する*/
        where->next->prev = where->prev;
        where->prev->next = where->next;
        current           = where->next;
        /*whereを削除する*/
        free(where->data);
        free((char *)where);
    }else{
        /*該当のデータはない*/
        current = NULL;
    }

    return current;/*currentlyな要素を返す*/
}


/*
*****************************************************************************
* gd_qsort:	リスト中の要素をfncの順にソートする
*****************************************************************************
*/
void
gd_qsort(
struct gd_list *head,
int    (*fnc)(struct gd_list *, struct gd_list *))
{
	register struct gd_list *object;
	struct gd_list *w;
	struct gd_list *w_object_next;
	struct gd_list *w_item_prev;

/*-----    procedure*/

	GD_LOOP(head,GD_FIRST(head))
		object = GD_LAST(head);
    	while(item!=object){
       		if((*fnc)(item,object)<0){
          		/*objectとitemを入れ換える*/
          		/*-----    objectの前後の要素のポインタ*/
          		if(object->prev!=item)
             		object->prev->next = item;/*same as item->next*/
          		object->next->prev = item;
          		/*-----    itemの前後の要素のポインタ*/
          		if(item->next!=object)
             		item->next->prev  = object;/*same as object->prev*/
          		item->prev->next  = object;
          		/*-----    object,itemのポインタ*/
          		w_object_next = object->next;
          		w_item_prev = item->prev;
          		object->next = (object->prev==item)?item :item->next;
          		item->prev  = (item->next==object)?object:object->prev;
          		object->prev = w_item_prev;
          		item->next  = w_object_next;
#ifdef DEBUG_off
   	gd_browser(head,(char *)"gd_qsort:exchanged",NULL);
#endif
          		w = object;                 /*objectとitem自体を入れ換える*/
          		object = item;
          		item  = w;
       		}
			object = object->prev;
    	}
	GD_END_LOOP
}


/*
*****************************************************************************
* _gd_concat:	head1の後ろにhead2を連結する
*****************************************************************************
*/
struct gd_list *
_gd_concat(
struct gd_list **head1p,
struct gd_list **head2p)
{
    struct gd_list *head1,
                   *head2,
                   *new_head;

/*-----    procedure*/

    head1 = *head1p;
    head2 = *head2p;

    if(GD_IS_EMPTY(head1) && GD_IS_EMPTY(head2)){
          /*ともにNULLリストなら単にhead1を返す*/
          new_head = head1;
          gd_kill(head2);
    }else{
       if(GD_IS_EMPTY(head2)){
          /*head1は要素あり,head2はNULLリストなら、head1を返す*/
          new_head = head1;
          gd_kill(head2);
       }else{
          /*head1,2ともに要素ありなら、head1の後にhead2を連結する*/
          /*順方向のリンクを作成する*/
          GD_NEXT(GD_LAST(head1))    = GD_FIRST(head2);
          GD_NEXT(GD_LAST(head2))    = head1;
          /*逆方向のリンクを作成する*/
          GD_PREV(GD_FIRST(head2))   = GD_LAST(head1);
          GD_PREV(head1)             = GD_LAST(head2);
          new_head = head1;
          /*head2のハンドル領域をfreeする*/
          free(head2->data);
          free((char *)head2);
       }
    }
    *head1p = (struct gd_list *)NULL;
    *head2p = (struct gd_list *)NULL;
    return(new_head);
}

/*
*****************************************************************************
* gd_error:	Going abort.
*****************************************************************************
*/
void
gd_error()
{
/*-----    procedure */
    perror("ERROR gdlist:Malloc returns NULL pointer. Going abort.");
	abort();
}


/*     -----    gd_qsort stuff*/
int
gd_data_str_order(
	struct gd_list *item1,
	struct gd_list *item2)
{
	/*-----    procedure*/
	return(strcmp((const char *)item1->data, (const char *)item2->data) * -1);
}


int
gd_key_order_asc(
	struct gd_list *item1,
	struct gd_list *item2)
{
	/*-----    procedure*/
	return((item1->key == item2->key) ? (int)NULL : (item1->key < item2->key ? 1 : (-1)));
}


int
gd_key_order_dec(
	struct gd_list *item1,
	struct gd_list*item2)
{
	/*-----    procedure*/
	return((item1->key == item2->key) ? (int)NULL : (item1->key > item2->key ? 1 : (-1)));
}
