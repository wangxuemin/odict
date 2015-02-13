/** 
* @file 	mass_data_save_test.cpp
* @brief	测试字典在大数据量时，能否安全save;
* @author 	konghui, konghui@baidu.com
* @version 	1.0.1
* @date 	2011-11-22
* Copyright (c) 2011, Baidu, Inc. All rights reserved.
*/


#include "ul_sign.h"
#include "odict.h"

int hash_num	= 2000000000;	/**< 桶大小 */
int iterm_num	= 1000000000;	/**< 数据数量 */

void print_usage(const char* program) {
	printf("Usage: %s -h hash_num -i iterm_num\n", program);
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		print_usage(argv[0]);
		return 1;
	}

	int opt;
	opterr = 0;
	while ( (opt = getopt(argc, argv, "h:i:")) != -1) {
		switch(opt) {
		case 'h': 
			hash_num = atoi(optarg);
			break;
		case 'i': 
			iterm_num= atoi(optarg);
			break;
		default:
				printf("unkown option -%c: %s\n", opt, optarg);
				print_usage(argv[0]);
				return 1;
		}
	}
	

	sodict_build_t *sdb;
	sodict_search_t *ssdb;

	printf("Create a odict dictionary object ... \n");
	sdb = odb_creat(hash_num);
	if (sdb == NULL) {
		printf("Create odict FAIL\n");
		return 1;
	}
	printf("Create odict OK, hash_num:[%u]\n", odb_get_hashnum(sdb));

	printf("Insert nodes into odict dictionary object ... \n");
	sodict_snode_t snode;
	int ret;
	for (int i=1; i <= iterm_num; i++) {
		snode.sign1 = i;	
		snode.sign2 = i;
		snode.cuint1 = 28;
		snode.cuint2 = 0xffffffff;

		ret = odb_add(sdb, &snode, 1);
		if (ODB_ADD_OK != ret && ODB_ADD_OVERWRITE != ret) {
			printf("Add [%d]th node to odict FAIL\n", i);
			return 1;
		}
	}

	printf("Now save the dictionary to file test_dict ...\n");
	if (ODB_SAVE_OK != odb_save(sdb, "./", "test_dict")) {
		printf("Save odict FAIL\n");
		return 1;
	}
	printf("Save odict OK\n");
	odb_destroy(sdb);

	printf("Now load the dictionary from file ...\n");
	ssdb = odb_load_search("./", "test_dict");
	if (NULL == ssdb) {
		printf("load odict FAIL\n");
		return 1;
	}
	printf("load odict OK\n");
	
	snode.sign1 = 10000;
	snode.sign2 = 10000;
	printf("Now seek node from the dictionary to check value...\n");
	if( ODB_SEEK_OK != odb_seek_search(ssdb, &snode)) {
		printf("Seek Check FAIL\n");
		return 1;
	}
	if (28 != snode.cuint1 || 0xffffffff != snode.cuint2) {
		printf("Seek Check FAIL\n");
		return 1;
	}
	printf("Seek Check OK: snode.cuint1[%u], snode.cuint2[%u]\n", snode.cuint1, snode.cuint2);

	odb_destroy_search(ssdb);
	return 0;
}
