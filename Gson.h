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
*/
#ifndef _GSON_H_
#define _GSON_H_

#include <stdio.h>
#include <string.h>

#include "debug.h"

#define GSON_DEBUG_GENERATOR_ON	0x01
#define GSON_DEBUG_PARSER_ON	0x02
#define GSON_DEBUG_ON			0x03
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
	/* Invalid character inside JSON string */
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
	int parent;
}gsontok_t;

/**
 * GSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {
	unsigned int pos; 		//offset in the GSON string 
	unsigned int toknext; 	// next token to allocate ,point to unallocated tokens.
	int toksuper; 			//superior token node, e.g parent object or array 
}gson_parser;



/**
 * Creates a new generator based over a given  buffer with an array of tokens
 * available.
 */
void gson_init_generator(char *str,int str_size);

gsonerr_t GSON_START();

gsonerr_t GSON_END();

gsonerr_t GSON_START_OBJECT();

gsonerr_t GSON_END_OBJECT();

gsonerr_t GSON_START_ARRAY(char *name);

gsonerr_t GSON_END_ARRAY();

gsonerr_t gsonInsertKV(gsontype_t stype,char *key,char *value);

gsonerr_t gsonInsertK(char *key);

gsonerr_t gsonInsertV(gsontype_t stype,char *value);


/**
 * Create GSON parser over an array of tokens
 */
void gson_init_parser(gson_parser *parser);

/**
 * Run GSON parser. It parses a GSON data string into and array of tokens, each describing
 * a single GSON object.
 */
gsonerr_t gson_parse(gson_parser *parser, const char *js,
		gsontok_t *tokens, unsigned int num_tokens);

int gsonCheck(const char *gson, gsontok_t *tok, const char *s);

void gsonCopy(const char *gson, gsontok_t *tok,char *str);

#endif

