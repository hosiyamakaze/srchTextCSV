#pragma	comment( user,"@(#)$Workfile: gdlist.h $$Revision: 3 $$Date: 25/01/25 11:54 $$NoKeywords: $")
/*********************************************************************
 *
 *     #include <gdlist.h> : �o�������X�g�Ǘ�(GD_LIST processor)
 *
 *********************************************************************/

#ifndef _GDLIST_H
#define	_GDLIST_H

/*
*   ���̃��X�g�ł́A�e�v�f���Ƃ�key(int�^)������t���ĊǗ����܂�
*   ���̃��X�g�ł́A�������̗v�f�̕��т�key�̏����ɂȂ�悤�Ǘ����܂�
*   key�̈�Ӄ��[�h���g����(U_UNIQ)�Akey�l�ɂ��ȕւȈ������ł��܂�
*   key�̏d�����[�h���g����(U_DUP)�A�_��ȑo�������X�g�Ƃ��Ďg�p�ł��܂�
*
*
*     struct gd_list *
*     gd_init(flg)
*          int   flg;
*
*          �o�������X�g�����������܂�
*          ���X�g�̃n���h��(head)��Ԃ��܂�
*          flg��key��U_DUP(�d������),U_UNIQ(���)�̂����ꂩ�������܂�
*          ���̗v�f�̓_�~�[�ł�(GD_LIST processor���g�p���܂�)
*          head�̒l�͈ȍ~�̃n���h�����O�̂��߂ɕK�v�ł�
*
*
*     struct gd_list *
*     gd_insert(head,key,data,size) 
*          struct gd_list *head;
*          int             key;
*          caddr_t         data;
*          int             size;
*
*          ���ΓI��key�̈ʒu(����)��data��}�����܂�
*          key�����݂���΂��̗v�f(��)�̑O�ɑ}�����܂�(U_DUP�̏ꍇ)     
*          key�����݂���΂��̗v�f�̃f�[�^���X�V���܂�(U_UNIQ�̏ꍇ)     
*          �}���܂��͍X�V���ꂽ�v�f��Ԃ��܂�     
*          size�� sizeof(�^) �ɂ���ē�����o�C�g���ł�
*          key�̒l�ɂ��Ă�user����ł�
*          �����I�ɂ́Asize�o�C�g�̃G���A��malloc���Abcopy�ɂ��data�𕡐����Ă��܂�
*
*
*     struct gd_list *
*     gd_append(head,key,data,size) 
*          struct gd_list *head;
*          int             key;
*          caddr_t         data;
*          int             size;
*
*          ���ΓI��key�̈ʒu(����)��data��}�����܂�
*          key�����݂���΂��̗v�f(��)�̌�ɑ}�����܂�(U_DUP�̏ꍇ)     
*          key�����݂���΂��̗v�f�̃f�[�^���X�V���܂�(U_UNIQ�̏ꍇ)     
*          �}���܂��͍X�V���ꂽ�v�f��Ԃ��܂�     
*          size�� sizeof(�^) �ɂ���ē�����o�C�g���ł�
*          key�̒l�ɂ��Ă�user����ł�
*          �����I�ɂ́Asize�o�C�g�̃G���A��malloc���Abcopy�ɂ��data�𕡐����Ă��܂�
*
*
*     struct gd_list *
*     gd_delete(head,key)
*          int             key;
*
*          key�̈ʒu�̗v�f���P�����폜���܂�(U_DUP�̏ꍇ����)
*          key�̈ʒu�̎��̗v�f��Ԃ��܂�
*          key�����݂��Ȃ����0��Ԃ��܂�
*          key���ő�l�Ȃ�(���̗v�f���Ō�̗v�f�Ȃ�)head��Ԃ��܂�
*
*
*     struct gd_list *
*     gd_find(head,current,key)
*          struct gd_list *head,
*                         *current;
*          int             key;
*
*          head���w�����X�g��current�ȍ~�̗v�f��key����v����v�f��Ԃ��܂�
*          �擪�̗v�f��GD_FIRST(head)�ł�
*          key���������݂���ꍇ�͍ŏ��Ɉ�v�����v�f��Ԃ��܂�
*          key�����݂��Ȃ����0��Ԃ��܂�
*
*
*     void
*     gd_kill(head)
*          struct gd_list *head;
*
*          dg_list processor���m�ۂ����̈��������܂�
*          ���X�g���s�v�ɂȂ������_�ŉ������ׂ��ł�
*          head�̒l��(struct gd_list *)NULL�ɂȂ�܂�
*
*
*     void
*     gd_browser(head,msg,fnc)
*          struct gd_list *head;
*          char *msg;
*		   void   (*fnc)();
*
*          ���b�Z�[�W(msg)�ɑ����Č��݂̃��X�g�̏�Ԃ�stderr�ɏo�͂��܂�
*          fnc�͗v�f���ɓK�p����֐��ŁA���̂悤�ɒ�`���܂�
*               void
*               fnc(item)
*               struct gd_list *item;
*               {
*                  (  ����
*                  (*�Ⴆ��stderr�ɏo�͂��܂�*)
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
*          head���w�����X�g��current�ȍ~�̗v�f��fnc��K�p���܂�
*          �擪�̗v�f��GD_FIRST(head)�ł�
*          fnc�͗v�f���ɓK�p����֐��ŁA���̂悤�ɒ�`���܂�
*               void
*               fnc(item)
*               struct gd_list *item;
*               {
*                  (  ����  
*                  (*�Ⴆ��stderr�ɏo�͂��܂�*)
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
*          head���w�����X�g��current�ȍ~�̗v�f��fnc��K�p��
*          �߂�l��TRUE�ƂȂ����v�f��Ԃ��܂�
*          �擪�̗v�f��GD_FIRST(head)�ł�
*          TRUE�ƂȂ�v�f���������݂���ꍇ�͍ŏ���TRUE�ƂȂ����v�f��Ԃ��܂�
*          TRUE�ƂȂ�v�f�����݂��Ȃ����0��Ԃ��܂�
*          fnc��K�p����v�f�͏������̗v�f�̕��тɉ�����(key�̏�����)�I�΂�܂�
*          fnc�͗v�f���ɓK�p����֐��ŁA���̂悤�ɒ�`���܂�
*               int
*               fnc(item)
*               struct gd_list *item;
*               {
*                  (  ����  
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
*          item�̒��O��data��}�����܂�
*          ���X�g��U_UNIQ�̏ꍇ�A�}������key�l���Ȃ���item��data���X�V���܂�
*          ���X�g��U_DUP�̏ꍇ�A�}������key�l���Ȃ���item��key�l���d�����܂�
*          �}���܂��͍X�V���ꂽ�v�f��Ԃ��܂�     
*          size�� sizeof(�^) �ɂ���ē�����o�C�g���ł�
*
*
*     struct gd_list *
*     gd_append_force(head,item,data,size)
*          struct gd_list *head,
*                         *item;
*          caddr_t         data;
*          int             size;
*
*          item�̒����data��ǉ����܂�
*          ���X�g��U_UNIQ�̏ꍇ�A�ǉ�����key�l���Ȃ���item��data���X�V���܂�
*          ���X�g��U_DUP�̏ꍇ�A�ǉ�����key�l���Ȃ���item��key�l���d�����܂�
*          �ǉ��܂��͍X�V���ꂽ�v�f��Ԃ��܂�     
*          size�� sizeof(�^) �ɂ���ē�����o�C�g���ł�
*
*
*     struct gd_list *
*     gd_delete_force(head,item)
*          struct gd_list *head,
*                         *item;
*
*          item���폜���܂�
*          item�̈ʒu�̎��̗v�f��Ԃ��܂�
*          item���Ō�̗v�f�Ȃ�head��Ԃ��܂�
*
*
*     void
*     gd_qsort(head,fnc)
*          struct gd_list *head;
*          int    (*fnc)();
*
*          head���X�g�̗v�f��fnc�̕]���ɂ��������ă\�[�g���܂�
*          key�̒l�́A�\�[�g�O�̏�Ԃ��ێ�����܂��B
*          �������̗v�f�̕��т�key�̏����́A�����̏ꍇ�A�Ή��������܂��B
*          fnc�ׂ͗����Q�̗v�f�ւ̃|�C���^�������Ƃ��ČĂяo�����
*          ��r���[�`���̖��O�ł��B
*          ��r���[�`�������̒l��Ԃ��Ƃ�����ւ����N����܂��B
*            ���̊֐��͂��̗�ł�
*              int
*              gd_data_str_order(item1,item2)(*�f�[�^(string)�̏���*)
*              struct gd_list *item1,
*                             *item2;
*              {
*              (*-----    procedure*)
*                  return(strcmp(item1->data,item2->data) * -1);
*              }
*          ����: ���݂�qsort�A���S���Y���ɂ͂Ȃ��Ă��܂���B(������)
*                ��r���[�`���̕]�����������Ƃ���0��Ԃ��܂��傤�B
*
*
*     struct gd_list *
*     gd_concat(head1,head2)
*          struct gd_list *head1,
*                         *head2;
*
*          head1���X�g�̌���head2���X�g��A�����܂�
*          �A�����ꂽ�V�������X�g�̃n���h��(head)��Ԃ��܂�
*          head1,2�̂ӂ��̃��X�g��gd_kill����Ahead1,2�̒l��NULL�ɂȂ�܂�
*          key�̒l�́A�A���O�̏�Ԃ��ێ�����܂�
*          �������̗v�f�̕��т�key�̏����́A�����̏ꍇ�A�Ή��������܂��B
*/

#define U_DUP  0  /*�L�[�l�̏d����F�߂܂�*/
#define U_UNIQ 1  /*�L�[�l�̏d����F�߂܂���*/

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
          struct gd_list *next; /*���̗v�f�A�܂��́Ahead�ւ̃|�C���^*/
          struct gd_list *prev; /*��O�̗v�f�A�܂��́Ahead�ւ̃|�C���^*/
          int             key;  /*gd_insert�ȂǂŎ����ꂽ�L�[�l*/
          int             dsize;/*gd_insert�ȂǂŎ����ꂽ�f�[�^�T�C�Y*/
          unsigned char   *data; /*gd_insert�ȂǂŊi�[���ꂽ�f�[�^�ւ̃|�C���^*/
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
struct gd_list *gd_insert_force(struct gd_list *head, struct gd_list *curr, unsigned char *data, int size);/*�����I�Ƀ��X�g�𑀍삵�܂�*/
struct gd_list *gd_append_force(struct gd_list *head, struct gd_list *curr, unsigned char *data, int size);
struct gd_list *gd_delete_force(struct gd_list *head, struct gd_list *curr);
void            gd_qsort(struct gd_list *head, int(*fnc)(struct gd_list *, struct gd_list *));
#define gd_concat(head1,head2)  _gd_concat(&(head1),&(head2))
struct gd_list *_gd_concat(struct gd_list **head1, struct gd_list **head2);

int             gd_data_str_order(struct gd_list *item1, struct gd_list *item2);/* data_str�̏���  gd_qsort staff */
int             gd_key_order_asc(struct gd_list *item1, struct gd_list *item2); /* key�̏��� */
int             gd_key_order_dec(struct gd_list *item1, struct gd_list *item2); /* key�̍~�� */

/*
*      macro�ɂ��gd_loop�̂����ЂƂ̎g�����ł�
*
*          GD_LOOP(head,current)
*             (* body *)
*          GD_END_LOOP
*
*          body�̒��ł͕ϐ���item�Ƃ��ėv�f���Q�Ƃ��܂�
*          item�̌^��(struct gd_list *)�ł�
*          head,current�̌^�͂������(struct gd_list *)�ł�
*          �Ⴆ�΁A���̂悤�Ɏg���܂��B
*              {int i=1; (*key�̒l��one-based�Ő������܂�*)
*               GD_LOOP(head,GD_FIRST(head))
*                  item->key = i++;
*               GD_END_LOOP
*              }
*
*          GD_LOOPR(head,current)	�́A�t�����ɗv�f��I�т܂�
*             (* body *)
*          GD_END_LOOP
*
*/
#define GD_LOOP(head,current)  {struct gd_list *item;\
								/*[ GD_LOOP        head ]*/\
                                for(item=current;item!=head;item=item->next){

#define GD_LOOPR(head,current) {struct gd_list *item;/*�t�����ɗv�f��I�т܂�*/\
                                for(item=current;item!=head;item=item->prev){

#define GD_END_LOOP             }\
                               }

/*
*      macro�ɂ��gd_findf�̂����ЂƂ̎g�����ł�
*
*          GD_FINDF(find,head,current,condition)
*
*          ���������v�f�́Afind�ɑ������܂��B
*          condition�̒��ł͕ϐ���item�Ƃ��ėv�f���Q�Ƃ��܂�
*          item�̌^��(struct gd_list *)�ł�
*
*          find,head,current�́A�������(struct gd_list *)�^�ł��B
*          condition�́Aif(condition) �Ƃ��ēW�J����܂��B
*          �Ⴆ�΁A���̂悤�Ɏg���܂��B
*              GD_FINDF(find,keito_list,GD_FIRST(keito_list),
*                       (strcmp(((struct keitom *)item->data)->keito,
*                               input_keito)
*                        ==NULL))(*���͂ƈ�v����P�n����T���܂�*)
*
*          GD_FINDFR(find,head,current,condition)	�́A�t�����ɗv�f��I�т܂�
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
