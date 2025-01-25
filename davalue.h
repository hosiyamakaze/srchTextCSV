#pragma	comment( user,"@(#)$Workfile: $$Revision: $$Date: $$NoKeywords: $")
/**********************************************************************
 *
 *     #include <davalue.h> : Data Area Value Processor
 *
 **********************************************************************/

#ifndef _DAVALUE_H
#define	_DAVALUE_H

/* -----	data structure */
struct da_value_s {	/* Data Area Value Structure */
	char	*dataArea;	/* �l���i�[����f�[�^�̈� */
	char	**index;	/* �f�[�^�̈�̒l�ւ̃C���f�b�N�X */
	int		fieldCount;	/* �f�[�^�̈�̒l�̐� */
	int		deltaSize;	/* �f�[�^�̈���g������ꍇ�̑���(�o�C�g) */
	int		currentSize;/* ���݂̃f�[�^�̈�({re}malloc()�ς�)�̃T�C�Y(�o�C�g) */
	int		bindSize;	/* �f�[�^�̈�̒l�ɂ���Ďg�p���ꂽ�T�C�Y(�o�C�g) */
};

/* -----	processor */
struct da_value_s	*da_init(int deltaSizer);
void 				 da_put(struct da_value_s *da,char *str);
void 				 da_fin(struct da_value_s *da);
void				 da_reset(struct da_value_s *da);
void				 da_kill(struct da_value_s *da);
char *da_getVN(struct da_value_s *da_name, struct da_value_s *da_value, const char *name, const char *defaultVal);

#endif	/* _DAVALUE_H */
