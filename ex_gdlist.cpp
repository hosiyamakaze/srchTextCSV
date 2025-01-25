#include <iostream>
#include <string>
#include <vector>

#include "gdlist.h"

using namespace std;

struct Poem_s {
	int number;
	std::string sentence;
};

void
printList(struct gd_list* DataItem) {	// �f�[�^�̍\���ɉ����ē��e��W���G���[�o�͂���
	cerr << ((struct Poem_s *)DataItem->data)->number << "," 
		 << ((struct Poem_s *)DataItem->data)->sentence << endl;
}

int 
main() {

	struct gd_list* DataList;	// mode:DUP|UNIQ,key:NULL,int(SEQ)�̑g�������Ƃ̓���������
	struct gd_list* currItem;	// append_force,insert_force�̂��߂̃J�����g�A�C�e��

	Poem_s poems[] = {
		{0,"�t�̖K��A�Ԃ��炭"},
		{1,"�Ă̖��ɋP���A�����̖�"},
		{2,"�H�̕����A�t�𕑂킹��"},
		{3,"��̍~���A�S�͐Â�"},
		{4,"�Î�̒��A���������"},
		{5,"���̌��A���ʂɉf��"},
		{6,"���̂����₫�A���ɓ͂�"},
		{7,"�Ă̏����A���������h���"},
		{8,"�R�̖��A�_���s��"},
		{9,"�[���̋�A�F�ʖL��"}
	};

 
	DataList = gd_init(U_DUP);
    for (int i = 0; i < 3; i++) {
		gd_append(DataList, NULL, (unsigned char*)&poems[i], sizeof(struct Poem_s));
    }
	gd_browser(DataList, (char*)"=====	sample 1(Recommended). mode:U_DUP,key:NULL(0),data:struct Poem_s", printList);

	gd_insert(DataList, NULL, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"-----	insert '3...' before '0...' ", printList);

	gd_append(DataList, NULL, (unsigned char*)&poems[4], sizeof(struct Poem_s));
	gd_browser(DataList, (char*)"-----	append '4...' after '2...' ", printList);
	gd_kill(DataList);
		


	DataList = gd_init(U_DUP);
	for (int i = 0; i < 3; i++) {
		gd_append(DataList, i, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"=====	sample 2(Recommended). mode:DUP,key:int(SEQ),data:struct Poem_s", printList);

	gd_insert(DataList,1, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"-----	insert '3...' before '1...' ", printList);

	gd_append(DataList, 0, (unsigned char*)&poems[4], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"-----	append '4...' after '0...' ", printList);
	gd_kill(DataList);
		

	DataList = gd_init(U_UNIQ);
	for (int i = 0; i < 3; i++) {
		gd_append(DataList, i, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"=====	sample 3(Recommended). mode:U_UNIQ,key:int(SEQ),data:struct Poem_s", printList);

	gd_insert(DataList, 1, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"-----	insert '3...' replace '1...' ", printList);

	gd_append(DataList, 0, (unsigned char*)&poems[4], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"-----	append '4...' replace '0...' ", printList);
	gd_kill(DataList);


	DataList = gd_init(U_DUP);
	currItem = gd_append(DataList, 3, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_append(DataList, 7, (unsigned char*)&poems[7], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"=====	sample 4(Recommended). mode:U_DUP,key:int(SEQ),data:struct Poem_s with append_force", printList);

	for (int i = 4; i < 10; i++) {
		currItem = gd_append_force(DataList, currItem, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"-----	3,7  append after 3", printList);
	gd_kill(DataList);


	DataList = gd_init(U_DUP);
	currItem = gd_insert(DataList, 7, (unsigned char*)&poems[7], sizeof(struct Poem_s));
	gd_insert(DataList, 3, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"=====	sample 5(Recommended). mode:U_DUP,key:int(SEQ),data:struct Poem_s with insert_force", printList);

	for (int i = 4; i < 10; i++) {
		currItem = gd_insert_force(DataList, currItem, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"-----	3,7  insert before 7", printList);
	gd_kill(DataList);


	DataList = gd_init(U_DUP);
	currItem = gd_append(DataList, NULL, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_append(DataList, NULL, (unsigned char*)&poems[7], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"=====	sample 6(Recommended). mode:U_DUP,key:NULL(0),data:struct Poem_s with append_force", printList);

	for (int i = 4; i < 10; i++) {
		currItem = gd_append_force(DataList, currItem, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"-----	3,7  append after 3", printList);
	gd_kill(DataList);


	DataList = gd_init(U_DUP);
	currItem = gd_insert(DataList, NULL, (unsigned char*)&poems[7], sizeof(struct Poem_s));
	gd_insert(DataList, NULL, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"=====	sample 7(Recommended). mode:U_DUP,key:NULL(0),data:struct Poem_s with insert_force", printList);

	for (int i = 4; i < 10; i++) {
		currItem = gd_insert_force(DataList, currItem, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"-----	3,7  insert before 7", printList);
	gd_kill(DataList);


	DataList = gd_init(U_UNIQ);
	currItem = gd_append(DataList, 3, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_append(DataList, 7, (unsigned char*)&poems[7], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"=====	sample 8(Not recommended). mode:U_UNIQ,key:int(SEQ),data:struct Poem_s with append_force", printList);

	for (int i = 4; i < 10; i++) {
		currItem = gd_append_force(DataList, currItem, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"-----	3,7  append after 3", printList);
	gd_kill(DataList);


	DataList = gd_init(U_UNIQ);
	currItem = gd_insert(DataList, 7, (unsigned char*)&poems[7], sizeof(struct Poem_s));
	gd_insert(DataList, 3, (unsigned char*)&poems[3], sizeof(struct Poem_s));
	gd_browser(DataList, (char *)"=====	sample 9(Not recommended). mode:U_UNIQ,key:int(SEQ),data:struct Poem_s with insert_force", printList);

	for (int i = 4; i < 10; i++) {
		currItem = gd_insert_force(DataList, currItem, (unsigned char*)&poems[i], sizeof(struct Poem_s));
	}
	gd_browser(DataList, (char *)"-----	3,7  insert before 7", printList);
	gd_kill(DataList);


    return 0;
}
