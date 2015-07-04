/*
 *	Author:	LaoGuo
 *	Data:	2015-7-3
 *	Description:	this is a test project to explain 
 *					how to create and parse a json string by Gson. 
 *	
*/
#include <stdio.h>

#include "Gson.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main(){
	
	char *temp,str[300];
	temp=str;
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; //We expect no more than 128 tokens 
	//create
	// {type:"Gson",num:123,arr:[{name:"xiaoming",age:12},{name:"xiaohong",age:14}]}
	temp=JSON_START(temp);
	temp=jsonBufInsertByS2M(temp,JSON_STRING,JSON_OBJECT,"type","Gson");
	temp=jsonBufInsertByS2M(temp,JSON_PRIMITIVE,JSON_OBJECT,"num","123");
	temp=JSON_START_ARRAY(temp,"arr");
	temp=JSON_START_OBJECT(temp);
	temp=jsonBufInsertByS2M(temp,JSON_STRING,JSON_ARRAY,"name","xiaoming");
	temp=jsonBufInsertByS2M(temp,JSON_PRIMITIVE,JSON_ARRAY,"age","12");
	temp=JSON_END_OBJECT(temp);
	temp=JSON_START_OBJECT(temp);
	temp=jsonBufInsertByS2M(temp,JSON_STRING,JSON_ARRAY,"name","xiaohong");
	temp=jsonBufInsertByS2M(temp,JSON_PRIMITIVE,JSON_ARRAY,"age","13");
	temp=JSON_END_OBJECT(temp);
	temp=JSON_END_ARRAY(temp);
	temp=JSON_END_OBJECT(temp);
	temp=JSON_END(temp);
	
	printf("%s\n",str);
	
	//parse
	jsmn_init(&p);
	r = jsmn_parse(&p, str, strlen(str), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	//Assume the top-level element is an object 
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf(" r=%d,type=%d,Object expected\n",r,t[0].type);
		return 1;
	}

	// Loop over all keys of the root object 
	for (i = 1; i < r; i++) {
		if (jsoneq(str, &t[i], "type") == 0) {
			// We may use strndup() to fetch string value 
			printf("- type: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (jsoneq(str, &t[i], "num") == 0) {
			// We may additionally check if the value is either "true" or "false" 
			printf("- num: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (jsoneq(str, &t[i], "arr") == 0) {
			int j;
			printf("- arr:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; // We expect groups to be an array of strings 
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2+j*4];
				printf("  * %.*s\n", g->end - g->start, str + g->start);
			}
			//4 is the array contain tokens
			i += t[i+1].size*4+t[i+1].size+1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					str + t[i].start);
		}
	}
	
	return 0;
}
