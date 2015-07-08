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
*/
#include <stdio.h>

#include "Gson.h"
 
#define TEST_SIZE_KB	4

int main(){
	
	char *temp,str[TEST_SIZE_KB*1100];
	int i;
	int r;
	gson_parser p;
	gsontok_t t[12*TEST_SIZE_KB*17]; //We expect no more than 128 tokens 
	//create
	// {type:"Gson",num:123,arr:[{name:"xiaoming",age:12},{name:"xiaohong",age:14}]}
	gson_init_generator(str);
	GSON_START();
	for(i=1;i<12*TEST_SIZE_KB;i++){
		gsonInsertKV(GSON_STRING,"type","Gson");
		gsonInsertKV(GSON_PRIMITIVE,"num","123");
		GSON_START_ARRAY("arr");
		GSON_START_OBJECT();
		gsonInsertKV(GSON_STRING,"name","xiaoming");
		gsonInsertKV(GSON_PRIMITIVE,"age","12");
		GSON_END_OBJECT();
		GSON_START_OBJECT();
		gsonInsertKV(GSON_STRING,"name","xiaohong");
		gsonInsertKV(GSON_PRIMITIVE,"age","13");
		GSON_END_OBJECT();
		GSON_END_ARRAY();
	}
	GSON_END();
	
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
}
