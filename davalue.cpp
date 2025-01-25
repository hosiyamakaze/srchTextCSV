static char sccsid[] =
"@(#)$Workfile: davalue.cpp $$Revision: 3 $$Date: 25/01/25 15:05 $$NoKeywords: $";
/*********************************************************************
 *
 *     Data Area Value Processor : 値をメモリ上に配置する
 *
 *********************************************************************/

/* -----	includes */
#include	<stdlib.h>
#include	<stdio.h>
#include    <string.h>
#include    <malloc.h>
#include 	"davalue.h"

/* -----	 define */

/* -----	 static */

/* -----	processor */
static void da_error();

/*
*****************************************************************************
*	da_init:	DataAreaValueStructureを初期化する
*****************************************************************************
*/
struct da_value_s *	/* Data Area Value Structure */
da_init(
int deltaSize)		/* データ領域を拡張する場合の増分(バイト) */
{
    register struct da_value_s *da;

/*-----    procedure*/

	da = (struct da_value_s *)malloc(sizeof(struct da_value_s));
	if(da==NULL) da_error();
	memset(da,0x00,sizeof(struct da_value_s));

	da->deltaSize = deltaSize;

    return(da);  /* Data Area Value Structure */
}

/*
*****************************************************************************
*	da_put:	DataAreaにValueを追加する
*****************************************************************************
*/
void
da_put(
struct da_value_s *da,	/* Data Area Value Structure */
char			  *str)	/* データ(string) */
{

	int	reqLength;	/* 連結後に必要なサイズ */

/* -----	procedure */

	/* 現在のデータ領域が不足する場合は拡張する */
	for(reqLength = da->bindSize + strlen(str) + 1;
		da->currentSize < reqLength;
		da->currentSize += da->deltaSize){

		da->dataArea = (char *)realloc(da->dataArea,da->currentSize + da->deltaSize);
		if(da->dataArea==NULL) da_error();
		memset(da->dataArea+da->currentSize,0x00,da->deltaSize);
	}

	/* データをデータ領域に追加する */
	strcpy_s(da->dataArea+da->bindSize,da->currentSize - da->bindSize,str);
	da->bindSize = reqLength;
	da->fieldCount ++;

}

/*
*****************************************************************************
*	da_fin:	DataAreaのindexを作成する
*****************************************************************************
*/
void
da_fin(
struct da_value_s *da)	/* Data Area Value Structure */
{

	int		i;
	char	*p;

/* -----	procedure */

	if(da->index != NULL) free((char *)da->index);
	da->index = (char **)malloc((da->fieldCount+1)*sizeof(char *));
	if(da->index==NULL) da_error();
	for(p= *da->index =da->dataArea,i=1;i<da->fieldCount;i++){
		*(da->index + i) = p + strlen(p) + 1;
		p = *(da->index + i);
	}
	*(da->index + da->fieldCount) = NULL;

}

/*
*****************************************************************************
*	da_reset:	DataAreaValueStructureを再利用のためにリセットする
*****************************************************************************
*/
void
da_reset(
struct da_value_s *da)	/* Data Area Value Structure */
{
/* -----	procedure */
	if(da->index != NULL){free((char *)da->index); da->index = NULL;}
	da->fieldCount	= 0;
	da->bindSize	= 0;

}

/*
*****************************************************************************
*	da_kill:	DataAreaValueStructureを開放する
*****************************************************************************
*/
void
da_kill(
struct da_value_s *da)	/* Data Area Value Structure */
{
/* -----	procedure */
	if(da == NULL) goto exit_procedure;

	if(da->index!=NULL){free((char *)da->index); da->index = NULL;}
	if(da->dataArea!=NULL){free(da->dataArea);da->dataArea = NULL;}
	if(da!=NULL) free(da);

exit_procedure:;
}

/*
*****************************************************************************
*	da_error:	DataAreaValueProcessorを終了する
*****************************************************************************
*/
static void
da_error()
{
/*-----    procedure */
    perror("ERROR davalue:malloc/realloc returns NULL pointer. Going abort.");
	abort();
}

/*
*****************************************************************************
*	da_getVN:	nameとvalueのDataAreaValueStructureから値を得る(getValueByName)
*****************************************************************************
*/
char *
da_getVN(
struct da_value_s *da_name,	/* Data Area Value Structure */
struct da_value_s *da_value,/* Data Area Value Structure */
const char	*name,				/* 名前 */
const char	*defaultVal)		/* 名前が存在しない場合の値 */
{
	char *p = NULL;
	char **ppName;
	char **ppVal;

/* -----	procedure */
	for(ppName=da_name->index,ppVal=da_value->index;
		*ppName != NULL && *ppVal != NULL; ppName ++,ppVal ++){
		if(strcmp(*ppName,name)==0){
			p = *ppVal;
			break;
		}
	}

	if(p == NULL){
		p = (char *)defaultVal;
	}

	return(p);
}

