static char sccsid[] =
"@(#)$Workfile: ex_davalue.cpp $$Revision: 1 $$Date: 25/01/25 15:03 $$NoKeywords: $";
/********************************************************************************
 *
 *	Data Area Value Processor : 値をメモリ上に配置する exsample
 *
 ********************************************************************************/

 /* -----	includes */
#include <iostream>
#include <string>
#include "davalue.h"

using namespace std;

/* -----	 define */

/* -----	 static */
static std::string words[] = {
	"直進",
	"左折",
	"右折",
	"直進または左折",
	"直進または右折",
	"左折または右折",
	"右折専用",
	"左折専用",
	"Uターン",
	"一方通行",
	"進入禁止",
	"駐車禁止",
	"駐停車禁止",
	"自転車専用",
	"歩行者専用",
	"車両通行止め",
	"大型車通行止め",
	"自転車通行止め",
	"徒歩者通行止め",
	"徐行",
	"停止",
	"優先道路",
	"一時停止",
	"横断歩道",
	"学校",
	"保育園",
	"病院",
	"信号機",
	"交差点",
	"踏切",
	"急カーブ",
	"急勾配",
	"下り坂",
	"上り坂",
	"トンネル",
	"高架橋",
	"橋",
	"危険",
	"落石",
	"凍結",
	"対向車注意",
	"追越し禁止",
	"追越し可能",
	"追越し専用",
	"横風注意",
	"狭路",
	"幅員狭小",
	"狭小",
	"落下物注意",
	"工事中",
	"迂回",
	"迂回路",
	"通行止め",
	"規制解除",
	"制限速度",
	"最高速度",
	"最低速度",
	"速度制限解除",
	"優先",
	"非優先",
	"進路変更禁止",
	"車線変更禁止",
	"車線変更可能",
	"車線増",
	"車線減",
	"追い越し車線",
	"走行車線",
	"バス停",
	"バス専用",
	"タクシー乗り場",
	"トラック専用",
	"自転車道",
	"歩道",
	"歩道橋",
	"地下道",
	"駅",
	"空港",
	"駐車場",
	"自転車駐車場",
	"非常駐車帯",
	"駐輪場",
	"自転車横断帯",
	"横断自転車道",
	"横断歩道",
	"信号待ち",
	"信号無視",
	"信号機設置",
	"信号機撤去",
	"信号機前",
	"信号機後",
	"一時停車",
	"一時停車禁止",
	"一時停止線",
	"一時停止位置",
	"交差点手前",
	"交差点直前",
	"交差点付近",
	"交差点内",
	"交差点出口",
	"交差点通過"
};

/* -----	processor */

int
main()
{
	char *p;
	struct da_value_s* da;
	int i;

	/* -----	procedure */

	cerr << "da_init" << endl;
	da = da_init(240);

    for (i = 0;i < 60; i++) {
		da_put(da, (char *)words[i].c_str());
		cerr << "word:\t" << words[i] 
			<< "\tlength:\t" << strlen(words[i].c_str())
			<< "\tda->fieldCount:\t" << da->fieldCount
			<< "\tda->deltaSize:\t" << da->deltaSize
			<< "\tda->bindSize:\t" << da->bindSize
			<< "\tda->currentSize:\t" << da->currentSize
			<< endl;
	}
	cerr << "da_fin" << endl;
	da_fin(da);

	for (i = 0, p = *(da->index + i); i < da->fieldCount;i++, p = *(da->index + i)) {
		cerr << "word\t" << i << "\t" << p <<"\tstrcmp=" << strcmp(p, (char*)words[i].c_str()) << endl;
	}

	cerr << "da_kill" << endl;
	da_kill(da);
}

