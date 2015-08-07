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
					 and modify few code for real-time update length.
					-add two more extern macro GSON_PARSER_PRIMITIVE and GSON_PARSER_STRING.


*/

#include "Gson.h"

/*
 * gson genrator part.
*/
#define GSON_STR_CPY(des,src)				{for(;*src!='\0';des++,src++) *des=*src;*des='\0';}

//backup data.
#define GSON_BAK(buf)						{for(generator->bak_fg[GSON_BACK_FLAG+1]=GSON_BACK_FLAG;(*buf=='}'||*buf==']')&&generator->bak_fg[GSON_BACK_FLAG+1];generator->bak_fg[GSON_BACK_FLAG+1]--,buf--){generator->bak_fg[generator->bak_fg[GSON_BACK_FLAG+1]]=*buf;}}
//recover json data.
#define GSON_BAK_RE(buf)					{for(generator->bak_fg[GSON_BACK_FLAG+1]+=1;generator->bak_fg[GSON_BACK_FLAG+1]<=GSON_BACK_FLAG;generator->bak_fg[GSON_BACK_FLAG+1]++,buf++){*buf=generator->bak_fg[generator->bak_fg[GSON_BACK_FLAG+1]];}}


/**
 * Creates a new generator based over a given  generator.buffer with an array of tokens
 * available.
 */
void gson_init_generator(gson_generator *generator,char *str,int str_size) {
	generator->buf=str;
	generator->size=str_size;
	generator->type_c=GSON_PRIMITIVE;
}

gsonerr_t GSON_START(gson_generator *generator){
	//generator.buf must remain more 4 char.
	if(generator->buf==NULL||(generator->size=generator->size-4)<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_START: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	generator->type_c=GSON_OBJECT;
	generator->buf[0]='{';
	generator->buf[1]=',';
	generator->buf[2]='}';
	generator->buf[3]='\0';
	generator->buf+=3;
	return GSON_ERROR_NONE;
}

gsonerr_t GSON_END(gson_generator *generator){
	if(generator->buf==NULL){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_END: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	GSON_END_OBJECT(generator);
	if(*generator->buf=='\0'){
		generator->buf--;
	}
	if(*generator->buf==','){
		*generator->buf='\0';
		//if there is a ',' yes,that we want.
		//we should end json by the character.
		return GSON_ERROR_NONE;
	}
	generator->buf+=1;
	GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_END: end with a unexpected result.\n"));
	return GSON_ERROR_PART;
}

gsonerr_t GSON_START_OBJECT(gson_generator *generator){
	//generator.buf must remain more 3 char.
	if(generator->buf==NULL||(generator->size=generator->size-3)<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_START_OBJECT: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	
	generator->type_c=GSON_OBJECT;
	if(*generator->buf=='\0'){
		generator->buf--;
	}
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_START_OBJECT: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	if(*(generator->buf-1)=='{'||*(generator->buf-1)=='['||*(generator->buf-1)==':'){
		*(generator->buf)='{';
		*(++generator->buf)=',';
		*(++generator->buf)='}';
	}else{
		*(++generator->buf)='{';
		*(++generator->buf)=',';
		*(++generator->buf)='}';
	}
	++generator->buf;
	GSON_BAK_RE(generator->buf);
	*(generator->buf)='\0';
	return GSON_ERROR_NONE;
	
}
gsonerr_t GSON_END_OBJECT(gson_generator *generator){
	char *tmp;
	if(generator->buf==NULL){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_END_OBJECT: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	if(*generator->buf=='\0'){
		generator->buf--;
	}
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_END_OBJECT: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	tmp=generator->buf;
	*(generator->buf++)=',';
	GSON_BAK_RE(generator->buf);
	*(generator->buf)='\0';
	tmp[0]='}';
	tmp[1]=',';	
	return GSON_ERROR_NONE;	
}


gsonerr_t GSON_START_ARRAY(gson_generator *generator,char *name){
	//generator.buf must remain more 5+strlen(name) char.
	if(generator->buf==NULL||(generator->size=generator->size-5-strlen(name))<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_START_ARRAY: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	generator->type_c=GSON_ARRAY;
	if(*generator->buf=='\0'){
		generator->buf--;
	}
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_START_ARRAY: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	
	if(*(generator->buf-1)!='{'&&*(generator->buf-1)!='['){
		generator->buf++;
	}
	
	*(generator->buf++)='\"';
	GSON_STR_CPY(generator->buf,name);
	*(generator->buf++)='\"';
	*(generator->buf++)=':';
	*(generator->buf++)='[';
	*(generator->buf++)=',';
	*(generator->buf++)=']';
	GSON_BAK_RE(generator->buf);
	*(generator->buf)='\0';
	return GSON_ERROR_NONE;
}


gsonerr_t GSON_END_ARRAY(gson_generator *generator){
	char *tmp;
	//generator.buf must remain more 1 char.
	if(generator->buf==NULL||(generator->size=generator->size-1)<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_END_ARRAY: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	if(*generator->buf=='\0'){
		generator->buf--;
	}
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in GSON_END_ARRAY: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	tmp=generator->buf;
	*(generator->buf++)=',';
	GSON_BAK_RE(generator->buf);
	*(generator->buf)='\0';
	tmp[0]=']';
	tmp[1]=',';	
	return GSON_ERROR_NONE;	
}


/*
 *inert the key-value to object .
 *	object
 *	    { }
 *	    { members }
 *	members
 *	    pair
 *	    pair , members
 *	pair
 *	    string : value
 *	array
 *	    [ ]
 *	    [ elements ]
 *	elements
 *	    value
 *	    value , elements
 *	value
 *	    string
 *	    number
 *	    object
 * 	    array
 *	    true
 *	    false
 *	    null
*/
gsonerr_t gsonInsertKV(gson_generator *generator,gsontype_t stype,char *key,char *value){
//key-value is the type of the value pair.
//generator.buf is the alright index of the new inserted type in gson String, and point to } or ].
	
	//generator.buf must remain more 4+strlen(key)+strlen(value) char.
	if(generator->buf==NULL||(generator->size=generator->size-4-strlen(key)-strlen(value))<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertKV: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	if(generator->type_c != GSON_OBJECT){
		//key:value can't inserted to array
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertKV: not a corret operation for JSON!!!\n"));
		return GSON_ERROR_INVAL;
	}
	if(*generator->buf=='\0'){
		generator->buf--;
	}	
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertKV: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	if(*(generator->buf-1)!='{'&&*(generator->buf-1)!='['){
		generator->buf++;
	}
	*(generator->buf++)='\"';
	GSON_STR_CPY(generator->buf,key);
	*(generator->buf++)='\"';
	*(generator->buf++)=':';
	if(stype==GSON_STRING){
		*(generator->buf++)='\"';
		GSON_STR_CPY(generator->buf,value);
		*(generator->buf++)='\"';
	}else if(stype==GSON_PRIMITIVE){
		GSON_STR_CPY(generator->buf,value);
	}
	*(generator->buf++)=',';
	GSON_BAK_RE(generator->buf);
	*generator->buf='\0';
	return GSON_ERROR_NONE;
}

/*
 *insert ' "name": ' to json for object.
*/
gsonerr_t gsonInsertK(gson_generator *generator,char *key){
//key-value is the type of the value pair.
//generator.buf is the alright index of the new inserted type in gson String, and point to } or ].
	
	//generator.buf must remain more 3+strlen(key) char.
	if(generator->buf==NULL||(generator->size=generator->size-3-strlen(key))<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertK: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	if(generator->type_c != GSON_OBJECT){
		//key:value can't inserted to array
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertK: not a corret operation for JSON!!!\n"));
		return GSON_ERROR_INVAL;
	}
	if(*generator->buf=='\0'){
		generator->buf--;
	}	
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertK: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	if(*(generator->buf-1)!='{'&&*(generator->buf-1)!='['){
		generator->buf++;
	}
	*(generator->buf++)='\"';
	GSON_STR_CPY(generator->buf,key);
	*(generator->buf++)='\"';
	*(generator->buf++)=':';
	*(generator->buf++)=',';
	GSON_BAK_RE(generator->buf);
	*generator->buf='\0';
	return GSON_ERROR_NONE;
}

/*
 *insert ' "value" ' to json for array.
*/
gsonerr_t gsonInsertV(gson_generator *generator,gsontype_t stype,char *value){
//key-value is the type of the value pair.
//generator.buf is the alright index of the new inserted type in gson String, and point to } or ].
	
	//generator.buf must remain more 2+strlen(value) char.
	if(generator->buf==NULL||(generator->size=generator->size-2-strlen(value))<0){
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertV: sorry,generator.buf is NULL!!! or no more space!!!\n"));
		return GSON_ERROR_PART;
	}
	if(*generator->buf=='\0'){
		generator->buf--;
	}	
	GSON_BAK(generator->buf);
	if(*generator->buf!=','){
		generator->buf++;
		GSON_BAK_RE(generator->buf);
		*generator->buf='\0';
		//not a insertable format.
		GSON_DEBUG_DIA(GSON_DEBUG_GENERATOR_ON,("-GENERATOR in gsonInsertV: not a insertable format.\n"));
		return GSON_ERROR_PART;
	}
	if(*(generator->buf-1)!='{' &&*(generator->buf-1)!='['&&generator->type_c == GSON_ARRAY){
		generator->buf++;
	}
	if(stype==GSON_STRING){
		*(generator->buf++)='\"';
		GSON_STR_CPY(generator->buf,value);
		*(generator->buf++)='\"';
	}else if(stype==GSON_PRIMITIVE){
		GSON_STR_CPY(generator->buf,value);
	}
	*(generator->buf++)=',';
	GSON_BAK_RE(generator->buf);
	*generator->buf='\0';
	return GSON_ERROR_NONE;
}

/*
 * gson parse part.
*/

/**
 * Creates a new parser based over a given  generator.buffer with an array of tokens
 * available.
 */
void gson_init_parser(gson_parser *parser) {
	parser->pos = 0;
	parser->toknext = 0;
	parser->toksuper = -1;
}
/*
 *assign a unallocated token for parser and init its info.
*/
static gsontok_t *gson_alloc_token(gson_parser *parser,
		gsontok_t *tokens, size_t num_tokens) {
	gsontok_t *tok;
	if (tokens == NULL || parser->toknext >= num_tokens) {
		GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_alloc_token: no more token mem.\n"));
		return NULL;
	}
	tok = &tokens[parser->toknext++];
	tok->start = tok->end = -1;
	tok->size = 0;
	tok->length=0;
	tok->parent = -1;
	return tok;
}

/**
 * Fills token type and boundaries.
 */
static void gson_fill_token(gsontok_t *token, gsontype_t type,
                            int start, int end) {
	token->type = type;		
	token->start = start;	
	token->end = end;
	token->size = 0;
}

/**
 * Fills next available token with GSON primitive.
 */
static gsonerr_t gson_parse_primitive(gson_parser *parser, const char *js,
		gsontok_t *tokens, size_t num_tokens) {
	gsontok_t *token;
	int start;

	start = parser->pos;

	for (;js[parser->pos] != '\0'; parser->pos++) {
		switch (js[parser->pos]) {
			case '\t' : case '\r' : case '\n' : case ' ' :
			case ','  : case ']'  : case '}' :
				goto found;
		}
		//exclude the control char and external char
		if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
			parser->pos = start;
			GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse_primitive: Invalid character inside JSON string.\n"));
			return GSON_ERROR_INVAL;
		}
	}
	/* In strict mode primitive must be followed by a comma/object/array */
	parser->pos = start;
	GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse_primitive: The string is not a full JSON packet, more bytes expected.\n"));
	return GSON_ERROR_PART;

found:
	token = gson_alloc_token(parser, tokens, num_tokens);
	if (token == NULL) {
		parser->pos = start;
		return GSON_ERROR_NOMEM;
	}
	gson_fill_token(token, GSON_PRIMITIVE, start, parser->pos);
	token->parent = parser->toksuper;
	parser->pos--;
	return GSON_ERROR_NONE;
}

/**
 * Filsl next token with GSON string.
 */
static gsonerr_t gson_parse_string(gson_parser *parser, const char *js,
		gsontok_t *tokens, size_t num_tokens) {
	gsontok_t *token;

	int start = parser->pos;

	// Skip starting quote 
	parser->pos++;

	for (; js[parser->pos] != '\0'; parser->pos++) {
		char c = js[parser->pos];

		// Quote: end of string 
		if (c == '\"') {
			token = gson_alloc_token(parser, tokens, num_tokens);
			if (token == NULL) {
				return GSON_ERROR_NOMEM;
			}
			gson_fill_token(token, GSON_STRING, start+1, parser->pos);
			token->parent = parser->toksuper;
			return GSON_ERROR_NONE;
		}

		// Backslash: Quoted symbol expected 
		// how to handler Escape character perfectly.
		if (c == '\\' && js[parser->pos + 1] != '\0') {
			int i;
			parser->pos++;
			switch (js[parser->pos]) {
				// Allowed escaped symbols 
				case '\"': case '/' : case '\\' : case 'b' :
				case 'f' : case 'r' : case 'n'  : case 't' :
					break;
				// Allows escaped symbol \uXXXX 
				case 'u':
					parser->pos++;
					for(i = 0; i < 4 && js[parser->pos] != '\0'  && js[parser->pos] != '\0'; i++) {
						/* If it isn't a hex character we have an error */
						if(!((js[parser->pos] >= 48 && js[parser->pos] <= 57) || /* 0-9 */
									(js[parser->pos] >= 65 && js[parser->pos] <= 70) || /* A-F */
									(js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
							GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse_string: 1 Invalid character inside JSON string.\n"));
							return GSON_ERROR_INVAL;
						}
						parser->pos++;
					}
					parser->pos--;
					break;
				// Unexpected symbol 
				default:
					GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse_string: 2 Invalid character inside JSON string.\n"));
					return GSON_ERROR_INVAL;
			}
		}
	}
	GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse_string: The string is not a full JSON packet, more bytes expected.\n"));
	return GSON_ERROR_PART;
}

/**
 * Parse GSON string and fill tokens.
 * return error info or the count of tokens
 */
int gson_parse(gson_parser *parser, const char *js,
		gsontok_t *tokens, unsigned int num_tokens) {
	gsonerr_t r;
	int i;
	gsontok_t *token;
	int count = 0;
	//traverse the whole json data.
	for (; js[parser->pos] != '\0'; parser->pos++) {
		char c;
		gsontype_t type;
		
		c = js[parser->pos];
		switch (c) {
			//the start of object or array.
			//its end must at a distance.
			case '{': case '[':
				count++;
				token = gson_alloc_token(parser, tokens, num_tokens);
				if (token == NULL)
					return GSON_ERROR_NOMEM;
				//update the token's parent size and link to parent.
				if (parser->toksuper != -1) {
					tokens[parser->toksuper].size++;
					token->parent = parser->toksuper;
				}
				token->type = (c == '{' ? GSON_OBJECT : GSON_ARRAY);
				token->start = parser->pos;
				//update gsontok_t count.
				token->length = count;
				//update current toksuper.
				//because the { or [ is the parent of following tokens.
				parser->toksuper = parser->toknext - 1;
				break;
			//end of object or array.
			case '}': case ']':
				if (tokens == NULL)
					break;
				type = (c == '}' ? GSON_OBJECT : GSON_ARRAY);
				//like {} or []
				if (parser->toknext < 1) {
					GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse: 1 Invalid character inside JSON string.\n"));
					return GSON_ERROR_INVAL;
				}
				//find the correspondence token.
				token = &tokens[parser->toksuper];
				for (;;) {
					//find the most near token with same type (e.g { or [).
					if (token->start != -1 && token->type == type) {
						token->end = parser->pos + 1;
						//update end.
						token->length = count - token->length;
						parser->toksuper = token->parent;
						break;
					}
					//nowhere for finding.
					if (token->parent == -1) {
						GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse: 2 Invalid character inside JSON string.\n"));
						return GSON_ERROR_INVAL;
					}
					//upstair by parent link.
					//more fast.
					token = &tokens[token->parent];
				}
				break;
			case '\"':
				r = gson_parse_string(parser, js, tokens, num_tokens);
				if (r < 0) return r;
				count++;
				if (parser->toksuper != -1 && tokens != NULL)
					tokens[parser->toksuper].size++;
				break;
			case '\t' : case '\r' : case '\n' : case ' ':
				break;
			case ':':
				parser->toksuper = parser->toknext - 1;
				break;
			case ',':
				if (tokens != NULL &&
						tokens[parser->toksuper].type != GSON_ARRAY &&
						tokens[parser->toksuper].type != GSON_OBJECT) {
					parser->toksuper = tokens[parser->toksuper].parent;

				}
				break;
			/* In strict mode primitives are: numbers and booleans */
			case '-': case '0': case '1' : case '2': case '3' : case '4':
			case '5': case '6': case '7' : case '8': case '9':
			case 't': case 'f': case 'n' :
				/* And they must not be keys of the object */
				if (tokens != NULL) {
					gsontok_t *t = &tokens[parser->toksuper];
					if (t->type == GSON_OBJECT ||
							(t->type == GSON_STRING && t->size != 0)) {
						GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse: 3 Invalid character inside JSON string.\n"));
						return GSON_ERROR_INVAL;
					}
				}
				r = gson_parse_primitive(parser, js, tokens, num_tokens);
				if (r < 0) return r;
				count++;
				if (parser->toksuper != -1 && tokens != NULL)
					tokens[parser->toksuper].size++;
				break;

			/* Unexpected char in strict mode */
			default:
				GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse: 4 Invalid character inside JSON string.\n"));
				return GSON_ERROR_INVAL;
		}
	}

	for (i = parser->toknext - 1; i >= 0; i--) {
		/* Unmatched opened object or array */
		if (tokens[i].start != -1 && tokens[i].end == -1) {
			GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gson_parse: The string is not a full JSON packet, more bytes expected.\n"));
			return GSON_ERROR_PART;
		}
	}

	return count;
}


/*
 *	gson check from numberous results.
*/
gsonerr_t gsonCheck(const char *gson, gsontok_t *tok, const char *s) {
	if (tok->type == GSON_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(gson + tok->start, s, tok->end - tok->start) == 0) {
		return GSON_ERROR_NONE;
	}
	GSON_DEBUG_DIA(GSON_DEBUG_PARSER_ON,("-PARSER in gsonCheck: check the string with token as the string type filed as \"%s\" with \"%.*s\".\n",s,tok->end - tok->start,gson + tok->start));
	return GSON_ERROR_CHECK;
}
/*
 *	gson copy from numberous results to str.
*/
void gsonCopy(const char *gson, gsontok_t *tok,char *str) {
	int i,j;
	for(i=tok->start,j=0;i<tok->end;i++,j++){
		str[j]=gson[i];
	}
	str[j]='\0';
}
