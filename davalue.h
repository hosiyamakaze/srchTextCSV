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
	char	*dataArea;	/* 値を格納するデータ領域 */
	char	**index;	/* データ領域の値へのインデックス */
	int		fieldCount;	/* データ領域の値の数 */
	int		deltaSize;	/* データ領域を拡張する場合の増分(バイト) */
	int		currentSize;/* 現在のデータ領域({re}malloc()済み)のサイズ(バイト) */
	int		bindSize;	/* データ領域の値によって使用されたサイズ(バイト) */
};

/* -----	processor */
struct da_value_s	*da_init(int deltaSizer);
void 				 da_put(struct da_value_s *da,char *str);
void 				 da_fin(struct da_value_s *da);
void				 da_reset(struct da_value_s *da);
void				 da_kill(struct da_value_s *da);
char *da_getVN(struct da_value_s *da_name, struct da_value_s *da_value, const char *name, const char *defaultVal);

#endif	/* _DAVALUE_H */
