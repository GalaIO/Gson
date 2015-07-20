/*
 *	Author:	LaoGuo
 *	Data:	2015-7-3
 *	Description:	this is a test project to explain 
 *					how to create and parse a gson string by Gson. 
 *
 * 	Author:	LaoGuo
 *	Data:	2015-7-3
 *	Description:	add a new test way. 
 *
 *
 *	Author:			GalaIO
 *	Date:			2015-7-19 12:57 AM
 *	Description:	-test a new way to construct a JSON str,you must build serval callback handler.
 *					-这个版本比较 失败 不能出现嵌套结构的 json。 
*/
#include <stdio.h>

#include "Gson.h"

#include "GsonTree.h"


///
///
///the third way to build a json.
///
///


gsonerr_t Num(void *temp,gsonHandler_t sog){
	printf("1\n");
	if(sog == GET_value){
		GSON_INERT_PRIMITIVE(temp,"1504121988");
	}else{
		
	}
	return 0;	
}
GSONTREE_PRIMITIVE(Num,Num);
gsonerr_t name(void *temp,gsonHandler_t sog){
	printf("2\n");	
	if(sog == GET_value){
		GSON_INERT_STRING(temp,"pro.yang");
	}else{
		
	}
	return 0;	
}
GSONTREE_STRING(name,name);

GSONTREE_OBJECT_KV(stu)={
	GSONTREE_PAIR(Num),
	GSONTREE_PAIR(name)
	};
GSONTREE_OBJECT(stu,2);


gsonerr_t  age(void *temp,gsonHandler_t sog){
	printf("3\n");
	if(sog == GET_value){
		GSON_INERT_PRIMITIVE(temp,"12");
	}else{
		
	}	
	return 0;	
}
GSONTREE_PRIMITIVE(age,age);

gsonerr_t  grade(void *temp,gsonHandler_t sog){
	printf("4\n");
	if(sog == GET_value){
		GSON_INERT_PRIMITIVE(temp,"98");
		GSON_INERT_PRIMITIVE(temp,"96");
		GSON_INERT_PRIMITIVE(temp,"88");
		GSON_INERT_PRIMITIVE(temp,"78");
		GSON_INERT_PRIMITIVE(temp,"58");
	}else{
		
	}	
	return 0;	
}
GSONTREE_ARRAY(grade,grade);

GSONTREE_KV(temp)={
	GSONTREE_PAIR(age),
	GSONTREE_PAIR(stu),
	GSONTREE_PAIR(grade)
};
GSONTREE(temp,3);


int main(){
	
	
	//alloc var for generating a JSON string. 
	int j;
	gson_generator generator;
	char str[1000];
	
	//
	//
	//generator code area.
	//
	//
	
	gson_init_generator(&generator,str,1000);
	GSON_BUILD(&generator,temp);
	
	printf("%s\n",str);
	
	return 0;
}


/*
///
///
///the second way to build a json.
///
///
void led_handler(void *temp,gsonHandler_t sog){
	if(sog==GET_h){
		gson_generator *generator=temp;
		gsonInsertKV(generator,GSON_PRIMITIVE,"sturas","234");
	}else{
		gsontok_p	*p=temp;
		printf("%.*s --- %.*s\n", p->sizeK,p->buf+p->startK,p->sizeV,p->buf+p->startV);
	}
}

GSONTREE_KV(hah)= {
	GSONTREE_PRIMITIVE((void *)led_handler),
	GSONTREE_PRIMITIVE((void *)led_handler)
	};
GSONTREE(hah,2);

int main(){
	
	//alloc var for generating a JSON string. 
	int j;
	gson_generator generator;
	char str[1000];
	//alloc var for parsing a JSON string. 
	int i;
	int r;
	gson_parser p;
	gsontok_t t[128]; //We expect no more than 128 tokens 
	
	//
	//
	//generator code area.
	//
	//
	gson_init_generator(&generator,str,1000);
	GSON_START(&generator);
	for(j=0;j<hah.count;j++){
		if(((gsontree_pair*)(hah.current_tree+j))->handler!=NULL){
			((gsontree_pair*)(hah.current_tree+j))->handler(&generator,GET_h);
		}
	}
	GSON_END(&generator);
	printf("-%d-%d-%d\n",hah.type,hah.count,hah.current_tree[0].type);
	printf("%s\n",str);
	
	
	//
	//
	//parser code area.
	//
	//
	gson_init_parser(&p);
	r = gson_parse(&p, str, t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse GSON: %d\n", r);
		return 1;
	}

	//Assume the top-level element is an object 
	if (r < 1 || t[0].type != GSON_OBJECT) {
		printf(" r=%d,type=%d,Object expected\n",r,t[0].type);
		return 1;
	}
	printf("the total token-size is :%d\n",r);
	// Loop over all keys of the root object 
	for (i = 1,j=0; i < r && j<hah.count; i+=2,j++) {
		gsontok_p pp;
		pp.buf=str;
		pp.startK=t[i].start;
		pp.sizeK=t[i].end - t[i].start;
		pp.startV=t[i+1].start;
		pp.sizeV=t[i+1].end - t[i+1].start;
		if(((gsontree_pair*)(hah.current_tree+j))->handler!=NULL){
			((gsontree_pair*)(hah.current_tree+j))->handler(&pp,SET_h);
		}
	}
	return 0;
}*/

/*
///
///
///the origin way to build a json.
///
///
#define TEST_SIZE_KB	1

int main(){
	gson_generator generator;
	char *temp,str[TEST_SIZE_KB*1100];
	int i;
	int r;
	gson_parser p;
	gsontok_t t[12*TEST_SIZE_KB*17]; //We expect no more than 128 tokens 
	//create
	// {type:"Gson",num:123,arr:[{name:"xiaoming",age:12},{name:"xiaohong",age:14}]}
	gson_init_generator(&generator,str,TEST_SIZE_KB*1100);
	GSON_START(&generator);
	for(i=1;i<12*TEST_SIZE_KB;i++){
		gsonInsertKV(&generator,GSON_STRING,"type","Gson");
		gsonInsertKV(&generator,GSON_PRIMITIVE,"num","123");
		GSON_START_ARRAY(&generator,"arr");
		GSON_START_OBJECT(&generator);
		gsonInsertKV(&generator,GSON_STRING,"name","xiaoming");
		gsonInsertKV(&generator,GSON_PRIMITIVE,"age","12");
		GSON_END_OBJECT(&generator);
		GSON_START_OBJECT(&generator);
		gsonInsertKV(&generator,GSON_STRING,"name","xiaohong");
		gsonInsertKV(&generator,GSON_PRIMITIVE,"age","13");
		GSON_END_OBJECT(&generator);
		GSON_END_ARRAY(&generator);
	}
	GSON_END(&generator);
	
	printf("%s*****%d\n",str,strlen(str));
	
	//parse
	gson_init_parser(&p);
	r = gson_parse(&p, str, t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse GSON: %d\n", r);
		return 1;
	}

	//Assume the top-level element is an object 
	if (r < 1 || t[0].type != GSON_OBJECT) {
		printf(" r=%d,type=%d,Object expected\n",r,t[0].type);
		return 1;
	}
	printf("the total token-size is :%d\n",r);
	// Loop over all keys of the root object 
	for (i = 1; i < r; i++) {
		if (gsonCheck(str, &t[i], "type") == 0) {
			// We may use strndup() to fetch string value 
			printf("- type: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (gsonCheck(str, &t[i], "num") == 0) {
			// We may additionally check if the value is either "true" or "false" 
			printf("- num: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (gsonCheck(str, &t[i], "arr") == 0) {
			int j;
			printf("- arr:\n");
			if (t[i+1].type != GSON_ARRAY) {
				continue; // We expect groups to be an array of strings 
			}
			for (j = 0; j < t[i+1].size; j++) {
				//2 is element's distance to current token.
				//4 is element's length.
				gsontok_t *g = &t[i+2+j+j*4];
				printf("  * %.*s\n", g->end - g->start, str + g->start);
			}
			//4 is the array contain tokens
			i += t[i+1].size*4+t[i+1].size+1;
		} else {
			printf("Unexpected key: %.*s-------size: %d----type: %d\n", t[i].end-t[i].start,
					str + t[i].start,t[i].size,t[i].type);
		}
	}
	
	return 0;
}*/
