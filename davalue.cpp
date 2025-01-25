static char sccsid[] =
"@(#)$Workfile: davalue.cpp $$Revision: 3 $$Date: 25/01/25 15:05 $$NoKeywords: $";
/*********************************************************************
 *
 *     Data Area Value Processor : �l����������ɔz�u����
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
*	da_init:	DataAreaValueStructure������������
*****************************************************************************
*/
struct da_value_s *	/* Data Area Value Structure */
da_init(
int deltaSize)		/* �f�[�^�̈���g������ꍇ�̑���(�o�C�g) */
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
*	da_put:	DataArea��Value��ǉ�����
*****************************************************************************
*/
void
da_put(
struct da_value_s *da,	/* Data Area Value Structure */
char			  *str)	/* �f�[�^(string) */
{

	int	reqLength;	/* �A����ɕK�v�ȃT�C�Y */

/* -----	procedure */

	/* ���݂̃f�[�^�̈悪�s������ꍇ�͊g������ */
	for(reqLength = da->bindSize + strlen(str) + 1;
		da->currentSize < reqLength;
		da->currentSize += da->deltaSize){

		da->dataArea = (char *)realloc(da->dataArea,da->currentSize + da->deltaSize);
		if(da->dataArea==NULL) da_error();
		memset(da->dataArea+da->currentSize,0x00,da->deltaSize);
	}

	/* �f�[�^���f�[�^�̈�ɒǉ����� */
	strcpy_s(da->dataArea+da->bindSize,da->currentSize - da->bindSize,str);
	da->bindSize = reqLength;
	da->fieldCount ++;

}

/*
*****************************************************************************
*	da_fin:	DataArea��index���쐬����
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
*	da_reset:	DataAreaValueStructure���ė��p�̂��߂Ƀ��Z�b�g����
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
*	da_kill:	DataAreaValueStructure���J������
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
*	da_error:	DataAreaValueProcessor���I������
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
*	da_getVN:	name��value��DataAreaValueStructure����l�𓾂�(getValueByName)
*****************************************************************************
*/
char *
da_getVN(
struct da_value_s *da_name,	/* Data Area Value Structure */
struct da_value_s *da_value,/* Data Area Value Structure */
const char	*name,				/* ���O */
const char	*defaultVal)		/* ���O�����݂��Ȃ��ꍇ�̒l */
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

