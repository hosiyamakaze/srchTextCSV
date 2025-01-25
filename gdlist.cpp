static char sccsid[] =
"@(#)$Workfile: gdlist.cpp $$Revision: 3 $$Date: 25/01/25 11:54 $$NoKeywords: $";
/********************************************************************************
 *
 *	Generic Data List Processor : �ėp�I�ȃf�[�^��o�������X�g��ɊǗ����܂�
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
* gd_init: �o�������X�g�����������܂��B
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
    head->key  = (int)NULL;/* dg_*()�ɂ��C�ӂ̒l���Z�b�g�����*/
    head->dsize= 1;
    head->data = (unsigned char *)malloc(head->dsize);/* gd_*()�ɂ��g�p�����*/
	if(head->data == NULL) gd_error();
    *head->data=(flg==U_DUP)?'d':'u';
    
    return head;  /*���̗v�f�̓_�~�[�ł���*/
}


/*
*****************************************************************************
* gd_insert: ���ΓI��key�̈ʒu��data��}�����܂��Bkey�����݂���΃f�[�^���X�V����
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

    /*key�̏����̕����Ɉʒu��{���܂�*/
    for(where = head->next; where->key < key ;where = where->next)
        continue;
    if((where == head)||(where->key > key)||(flg=='d')){
        /*where�̑O�ɑ}������*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where;
		newer->prev = where->prev;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpy�Ƃ͓]���������t�ł���*/
		newer->dsize= size;
        where->prev->next = newer;
        where->prev = newer;
    }else{
        /*where�̃f�[�^��data�ɍX�V����*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currently�ȗv�f��Ԃ�*/
}


/*
*****************************************************************************
* gd_append:	���ΓI��key�̈ʒu��data��}�����܂��Bkey�����݂���΃f�[�^���X�V����
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

    /*key�̏����̕����Ɉʒu��{���܂�*/
    for(where = head->next;
		(where!=head) && (flg=='d'?(where->key <= key):(where->key < key));
		where = where->next)
        continue;
    if((where == head)||(where->key > key)||(flg=='d')){
        /*where�̑O�ɑ}������*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where;
		newer->prev = where->prev;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpy�Ƃ͓]���������t�ł���*/
		newer->dsize= size;
        where->prev->next = newer;
        where->prev = newer;
    }else{
        /*where�̃f�[�^��data�ɍX�V����*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currently�ȗv�f��Ԃ�*/
}


/*
*****************************************************************************
* :
*****************************************************************************
*/
struct gd_list *     /*key�̈ʒu�̗v�f���폜���܂��Bkey�����݂��Ȃ����0��Ԃ� */
gd_delete(
struct gd_list *head,
int key)
{
    register struct gd_list *where,
                            *current;

/*-----    procedure*/
    head->key = key;
    /*key�̏����̕����Ɉʒu��{���܂�*/
    for(where = head->next; where->key != key ;where = where->next)
        continue;
    if(where != head){
        /*���X�g�|�C���^���X�V����*/
        where->next->prev = where->prev;
        where->prev->next = where->next;
        current           = where->next;
        /*where���폜����*/
        free(where->data);
        free((char *)where);
    }else{
        /*�Y���̃f�[�^�͂Ȃ�*/
        current = NULL;
    }

    return current;/*currently�ȗv�f��Ԃ�*/
}



/*
*****************************************************************************
* gd_find:	key�̈ʒu�̗v�f��Ԃ��܂��Bkey�����݂��Ȃ����0��Ԃ�
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
    /*key�̏����̕����Ɉʒu��{���܂�*/
    for(where = current; where != head ;where = where->next){
        if(where->key == key) return where;
    }
    return NULL;
}


/*
*****************************************************************************
* _gd_kill:	dg_list���m�ۂ����̈���������
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
    /*key�̏����̕����ɉ����ĉ������*/
    for(where = head->next; where != head ;where = next){
        next =where->next;
        /*where�̗̈���������*/
        free(where->data);
        free((char *)where);
    }
    /*head�̗̈���������*/
    free(head->data);
    free((char *)head);
    *headp = (struct gd_list *)NULL;/*head�̒l��NULL�N���A����*/
}


/*
*****************************************************************************
* gd_browser:	dg_list���m�ۂ������݂̓��e��stderr�ɏo�͂��܂�(DEBUG�p)
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
    /*key�̏����̕����ɉ����ďo�͂���*/
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
* gd_loop:	head���X�g��current���w���v�f�ȍ~��fnc��K�p����
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
    /*key�̏����̕����ɉ����ēK�p����*/
    for(where = current; where != head ;where = where->next){
        (*fnc)(where);
    }
}


/*
*****************************************************************************
* gd_findf:	head���X�g��fnc��K�p��TRUE�ƂȂ����v�f��Ԃ�
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
    /*key�̏����̕����ɉ����ēK�p����*/
    for(where = current; where != head ;where = where->next){
        if((*fnc)(where)) return where;
    }
    return NULL;
}


/*
*****************************************************************************
* gd_insert_force:	item�̒��O��data��}�����܂��Bkey�����݂���΃f�[�^���X�V����
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
        /*where�̑O�ɑ}������*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where;
		newer->prev = where->prev;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpy�Ƃ͓]���������t�ł���*/
		newer->dsize= size;
        where->prev->next = newer;
        where->prev = newer;
    }else{
        /*where�̃f�[�^��data�ɍX�V����*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currently�ȗv�f��Ԃ�*/
}


/*
*****************************************************************************
* gd_append_force:	item�̒����data��ǉ����܂��Bkey�����݂���΃f�[�^���X�V����
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
        /*where�̒���ɒǉ�����*/
		newer = (struct gd_list *)malloc(sizeof(struct gd_list));
		if(newer == NULL) gd_error();
        bzero(newer,sizeof(struct gd_list));
		newer->next = where->next;
		newer->prev = where;
		newer->key  = key;
		newer->data = (unsigned char *)malloc(size);
		if(newer->data == NULL) gd_error();
        bcopy(data, newer->data,size);/*strcpy�Ƃ͓]���������t�ł���*/
		newer->dsize= size;
        where->next->prev = newer;
        where->next = newer;
    }else{
        /*where�̃f�[�^��data�ɍX�V����*/
        if(size > where->dsize){
           free(where->data);
           where->data = (unsigned char *)malloc(size);
		   if(where->data == NULL) gd_error();
           where->dsize= size;
        }
        bcopy(data,where->data,size);
		newer = where;
    }

    return newer;/*currently�ȗv�f��Ԃ�*/
}


/*
*****************************************************************************
* gd_delete_force:	*item���폜���܂�
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
        /*���X�g�|�C���^���X�V����*/
        where->next->prev = where->prev;
        where->prev->next = where->next;
        current           = where->next;
        /*where���폜����*/
        free(where->data);
        free((char *)where);
    }else{
        /*�Y���̃f�[�^�͂Ȃ�*/
        current = NULL;
    }

    return current;/*currently�ȗv�f��Ԃ�*/
}


/*
*****************************************************************************
* gd_qsort:	���X�g���̗v�f��fnc�̏��Ƀ\�[�g����
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
          		/*object��item����ꊷ����*/
          		/*-----    object�̑O��̗v�f�̃|�C���^*/
          		if(object->prev!=item)
             		object->prev->next = item;/*same as item->next*/
          		object->next->prev = item;
          		/*-----    item�̑O��̗v�f�̃|�C���^*/
          		if(item->next!=object)
             		item->next->prev  = object;/*same as object->prev*/
          		item->prev->next  = object;
          		/*-----    object,item�̃|�C���^*/
          		w_object_next = object->next;
          		w_item_prev = item->prev;
          		object->next = (object->prev==item)?item :item->next;
          		item->prev  = (item->next==object)?object:object->prev;
          		object->prev = w_item_prev;
          		item->next  = w_object_next;
#ifdef DEBUG_off
   	gd_browser(head,(char *)"gd_qsort:exchanged",NULL);
#endif
          		w = object;                 /*object��item���̂���ꊷ����*/
          		object = item;
          		item  = w;
       		}
			object = object->prev;
    	}
	GD_END_LOOP
}


/*
*****************************************************************************
* _gd_concat:	head1�̌���head2��A������
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
          /*�Ƃ���NULL���X�g�Ȃ�P��head1��Ԃ�*/
          new_head = head1;
          gd_kill(head2);
    }else{
       if(GD_IS_EMPTY(head2)){
          /*head1�͗v�f����,head2��NULL���X�g�Ȃ�Ahead1��Ԃ�*/
          new_head = head1;
          gd_kill(head2);
       }else{
          /*head1,2�Ƃ��ɗv�f����Ȃ�Ahead1�̌��head2��A������*/
          /*�������̃����N���쐬����*/
          GD_NEXT(GD_LAST(head1))    = GD_FIRST(head2);
          GD_NEXT(GD_LAST(head2))    = head1;
          /*�t�����̃����N���쐬����*/
          GD_PREV(GD_FIRST(head2))   = GD_LAST(head1);
          GD_PREV(head1)             = GD_LAST(head2);
          new_head = head1;
          /*head2�̃n���h���̈��free����*/
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
