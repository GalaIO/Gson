/*
	Date:			2014-12-26	15:39
	Author:			LaoGuo
	Describtion:	the gson-creator's code!!!
					and with test code!!
					structure:
						start<obj>
							insert...singal 2 multiple
							insert...
							...
						end<obj>

	Date:			2015-2-6 
	Author:			LaoGuo
	Describtion:	update the code of gson-creator
					we can creator-gson data on one buf!!!
					but only case by case,,,,,
					
					same api  rather than  different usal.


	Date:			2015-7-4 am 
	Author:			LaoGuo
	Describtion:	-add a insert function for array as a sequenced value.
					-redesign gson and gson type
					-redesign gson code


	Date:			2015-7-4 10:18 pm 
	Author:			LaoGuo
	Describtion:	-fix GSON_PARENT_LINKS bug.

	Date:			2015-7-5 7:23 pm 
	Author:			LaoGuo
	Describtion:	-no bug.
					-we should use rebuild rather than compile.


	Date:			2015-7-5 7:25 pm 
	Author:			LaoGuo
	Describtion:	-use parent link and strict mode default.
					-test and fix bug.

	Date:			2015-7-6 8:30 pm 
	Author:			LaoGuo
	Describtion:	-add err info for json generator.
					-handler buf point autoly.	
									
	Date:			2015-7-6 11:09 pm 
	Author:			LaoGuo
	Describtion:	-add debug info for gson,it can use conveniently.

	Date:			2015-7-19 7:41 pm 
	Author:			GalaIO
	Describtion:	-add more extern macro.	


	Date:			2015-7-20 5:25 pm 
	Author:			GalaIO
	Describtion:	-add gsontype_t attribute to struct gson_generator.
					-modefy few function for operating type_c in gson_generator.
					 and it indicate the location where you are inserd data , eg. GSON_OBJECT or GSON_ARRAY.
					 it's important for gsoninsertV . it's different when array or object is inserted.
					-simplify the use of macros GSON_INERT_PRIMITIVE and GSON_INERT_STRING.
					-add more notes.


	Date:			2015-7-20 6:00 pm 
	Author:			GalaIO
	Describtion:	-add more gson_parser's members for gson_parser.
					-reserved the old apis for old code.


	Date:			2015-7-20 10:06 pm 
	Author:			GalaIO
	Describtion:	-add a member for gsontok_t,this member can only update in GSON_OBJECT or GSON_ARRAY.
					 it becomes more bloated,but it is worth,then possible to delete member size.
					-add two more extern macro GSON_PARSER_PRIMITIVE and GSON_PARSER_STRING.


*/
#ifndef _GSON_H_
#define _GSON_H_

#include <stdio.h>
#include <string.h>

#include "debug.h"

#define GSON_DEBUG_GENERATOR_ON	0x01
#define GSON_DEBUG_PARSER_ON	0x02
#define GSON_DEBUG_GSONTREE_ON	0x04
#define GSON_DEBUG_ON			0x07
#define GSON_DEBUG_OFF			0x00

/*
 **************************************************************************
 *							How to debug gson.
 **************************************************************************
*/
//if you should open gson_debug all.
//#define GSON_DEBUG	GSON_DEBUG_ON
//if you should open gson_debug GENERATOR only.
//#define GSON_DEBUG	GSON_DEBUG_GENERATOR_ON
//if you should open gson_debug PARSER only.
//#define GSON_DEBUG	GSON_DEBUG_PARSER_ON
//shut down gson_debug.
//#define GSON_DEBUG	GSON_DEBUG_OFF
#define GSON_DEBUG	GSON_DEBUG_OFF


#ifdef GSON_DEBUG
	#define GSON_DEBUG_DIA(assert,dia)  DEBUG_ASSERT_DIA(!(GSON_DEBUG&(assert)),dia)
#else
	#define GSON_DEBUG_DIA(assert,dia)
#endif

/**
 * GSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
	GSON_PRIMITIVE = 0,  //"name":value.
	GSON_OBJECT = 1,      //{****}.
	GSON_ARRAY = 2,      //"ARRAY":[****].
	GSON_STRING = 3      //"name":"value".
} gsontype_t;

typedef enum {		   
	/* no error */
	GSON_ERROR_NONE = 0,
	/* Not enough tokens were provided */
	GSON_ERROR_NOMEM = -1,
	/* Invalid character or operation inside JSON string */
	GSON_ERROR_INVAL = -2,
	/* The string is not a full JSON packet, more bytes expected */
	GSON_ERROR_PART = -3,
	/* check the string with token as the string type  */
	GSON_ERROR_CHECK = -4
} gsonerr_t;

/**
 * GSON token description.
 * @param		type	type (object, array, string etc.)
 * @param		start	start position in GSON data string
 * @param		end		end position in GSON data string
 *
 *  it can retain json data perfectly. it means the operator cannot destry the ative data.
 */
typedef struct {
	gsontype_t type;
	int start;
	int end;
	int size;
	// only {} or [] has length .
	int length;
	int parent;
}gsontok_t;

/**
 * GSON token description for json_pair entire info.
 *
 * reserved the gsontok_p for old code.
 */
typedef struct {
	char *buf;
	gsontype_t type;
	int startK;
	int sizeK;
	int startV;
	int sizeV;
}gsontok_p;

/*
 *
 *defien a genarator struct.
 *
**/
//this bak_fg indicates the truth that the json data cannot nesting over 100 object or array.
//that enough for oridary json data.
#define GSON_BACK_FLAG		100
typedef struct {
	char *buf;
	int size; 
	//type_C  indicate the current muti-type whitch you will insert. eg. array,object...
	gsontype_t	type_c; 
	//this bak_fg indicates the truth that the json data cannot nesting over 100 object or array.
	//that enough for oridary json data.
	char bak_fg[GSON_BACK_FLAG+2];
}gson_generator; 

/**
 * GSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {
	unsigned int pos; 		//offset in the GSON string 
	unsigned int toknext; 	// next token to allocate ,point to unallocated tokens.
	int toksuper; 			//superior token node, e.g parent object or array 
	
	//add more gson_parser's members.
	char *json_buf;
	gsontype_t type;
	gsontok_t  *tok;
	int 	   tok_c;
}gson_parser;



/**
 * Creates a new generator based over a given  buffer with an array of tokens
 * available.
 */
void gson_init_generator(gson_generator *generator,char *str,int str_size);

gsonerr_t GSON_START(gson_generator *generator);

gsonerr_t GSON_END(gson_generator *generator);

gsonerr_t GSON_START_OBJECT(gson_generator *generator);

gsonerr_t GSON_END_OBJECT(gson_generator *generator);

gsonerr_t GSON_START_ARRAY(gson_generator *generator,char *name);

gsonerr_t GSON_END_ARRAY(gson_generator *generator);

gsonerr_t gsonInsertKV(gson_generator *generator,gsontype_t stype,char *key,char *value);

gsonerr_t gsonInsertK(gson_generator *generator,char *key);

gsonerr_t gsonInsertV(gson_generator *generator,gsontype_t stype,char *value);

//extern macro
#define GSON_INERT_PRIMITIVE(generator,value)	gsonInsertV((gson_generator *)generator,GSON_PRIMITIVE,value)
#define GSON_INERT_STRING(generator,value)		gsonInsertV((gson_generator *)generator,GSON_STRING,value)

#define GSON_PARSER_PRIMITIVE(parser,str)		gsonCopy(((gson_parser *)parser)->json_buf, ((gson_parser *)parser)->tok++,str)     
#define GSON_PARSER_STRING(parser,str)			gsonCopy(((gson_parser *)parser)->json_buf, ((gson_parser *)parser)->tok++,str)         

/**
 * Create GSON parser over an array of tokens
 */
void gson_init_parser(gson_parser *parser);

/**
 * Run GSON parser. It parses a GSON data string into and array of tokens, each describing
 * a single GSON object.
 */
int gson_parse(gson_parser *parser, const char *js,
		gsontok_t *tokens, unsigned int num_tokens);

gsonerr_t gsonCheck(const char *gson, gsontok_t *tok, const char *s);

void gsonCopy(const char *gson, gsontok_t *tok,char *str);

#endif

