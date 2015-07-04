
#ifndef _JSON_HANDLER_H_
#define _JSON_HANDLER_H_

/*
	Date:			2014-12-26	15:39
	Author:			LaoGuo
	Describtion:	the json-creator's code!!!
					and with test code!!
					structure:
						start<obj>
							insert...singal 2 multiple
							insert...
							...
						end<obj>
						
					2015-2-6 update the code of json-creator
					we can creator-json data on one buf!!!
					but only case by case,,,,,
					
					same api  rather than  different usal.
*/

#include <stdio.h>

#include <string.h>


typedef enum JSON_SINGAL_TYPE_t{	//the simple type of json.
	JSON_PRIMITIVE ,		//"name":value.
	JSON_STRING 			//"name":"value".
}JSON_SINGAL_TYPE_t;

typedef enum JSON_MULTI_TYPE_t{	//the complex object  of json.
	JSON_ARRAY,				//"ARRAY":[****].
	JSON_OBJECT            //{****}.
}JSON_MULTI_TYPE_t;

/*
#define JSON_START_OBJECT(buf)				{*(buf++)='{';*(buf)='}';}
#define JSON_END_OBJECT(buf)				{*(++buf)='\0';}


extern int JSON_START_ARRAY_handler(char *buf, char *name);

#define JSON_START_ARRAY(buf,name)			{buf+=JSON_START_ARRAY_handler(buf,name);}
#define JSON_END_ARRAY(buf)  				{*(++buf)='\0';}


//the JSON_MULTI_TYPE_t indicate the type whitch you are creating.
//the JSON_SINGAL_TYPE_t indicate the type whitch you will be inserted in JSON_MULTI_TYPE_t's type.
//key-value is the type of the value pair.

//buf is the alright index of the new inserted type in json String, and point to } or ].
extern char *jsonBufInsertByS2M(char *buf,JSON_SINGAL_TYPE_t stype,
			JSON_MULTI_TYPE_t mtype,char *key,char *value);

extern char *jsonBufInsertByM2M(char *buf,JSON_MULTI_TYPE_t stype,
			JSON_MULTI_TYPE_t mtype,char *buff);

*/

//version  2

char* JSON_START(char *buf);

char* JSON_END(char *buf);

char *JSON_START_OBJECT(char *buf);

char *JSON_END_OBJECT(char *buf);

char *JSON_START_ARRAY(char *buf,char *name);

char *JSON_END_ARRAY(char *buf);


//the JSON_MULTI_TYPE_t indicate the type whitch you are creating.
//the JSON_SINGAL_TYPE_t indicate the type whitch you will be inserted in JSON_MULTI_TYPE_t's type.
//key-value is the type of the value pair.

//buf is the alright index of the new inserted type in json String, and point to } or ].
char *jsonBufInsertByS2M(char *buf,JSON_SINGAL_TYPE_t stype,
			JSON_MULTI_TYPE_t mtype,char *key,char *value);

/*
***********************************************************************************************************************
--------------------------------------jsmn code------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
***********************************************************************************************************************
*/

/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
	JSMN_PRIMITIVE = 0,
	JSMN_OBJECT = 1,
	JSMN_ARRAY = 2,
	JSMN_STRING = 3
} jsmntype_t;

typedef enum {		   
	/* no error */
	JSMN_ERROR_NONE = 0,
	/* Not enough tokens were provided */
	JSMN_ERROR_NOMEM = -1,
	/* Invalid character inside JSON string */
	JSMN_ERROR_INVAL = -2,
	/* The string is not a full JSON packet, more bytes expected */
	JSMN_ERROR_PART = -3
} jsmnerr_t;

/**
 * JSON token description.
 * @param		type	type (object, array, string etc.)
 * @param		start	start position in JSON data string
 * @param		end		end position in JSON data string
 */
typedef struct {
	jsmntype_t type;
	int start;
	int end;
	int size;
#ifdef JSMN_PARENT_LINKS
	int parent;
#endif
}jsmntok_t;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {
	unsigned int pos; /* offset in the JSON string */
	unsigned int toknext; /* next token to allocate */
	int toksuper; /* superior token node, e.g parent object or array */
} jsmn_parser;

/**
 * Create JSON parser over an array of tokens
 */
extern void jsmn_init(jsmn_parser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each describing
 * a single JSON object.
 */
extern jsmnerr_t jsmn_parse(jsmn_parser *parser, const char *js, size_t len,
		jsmntok_t *tokens, unsigned int num_tokens);

extern int jsonCheck(const char *json, jsmntok_t *tok, const char *s);

#endif

