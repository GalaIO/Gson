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
						
	Date:			2015-2-6 
	Author:			LaoGuo
	Describtion:	update the code of json-creator
					we can creator-json data on one buf!!!
					but only case by case,,,,,
					
					same api  rather than  different usal.
*/

#include "Gson.h"
//version 1
/*
#define JSON_STR_CPY(des,src)				{for(;*src!='\0';des++,src++) *des=*src;*des='\0';}

#define JSON_START_OBJECT(buf)				{*(buf++)='{';*(buf)='}';}
#define JSON_END_OBJECT(buf)				{*(++buf)='\0';}


int JSON_START_ARRAY_handler(char *buf, char *name){
	char *bufTmp=buf;
	*(buf++)='\"';
	JSON_STR_CPY(buf,name);
	*(buf++)='\"';
	*(buf++)=':';
	*(buf++)='[';
	*buf=']';
	return buf-bufTmp;
}
#define JSON_START_ARRAY(buf,name)			{buf+=JSON_START_ARRAY_handler(buf,name);}
#define JSON_END_ARRAY(buf)  				{*(++buf)='\0';}


//the JSON_MULTI_TYPE_t indicate the type whitch you are creating.
//the JSON_SINGAL_TYPE_t indicate the type whitch you will be inserted in JSON_MULTI_TYPE_t's type.
//key-value is the type of the value pair.

//buf is the alright index of the new inserted type in json String, and point to } or ].
char *jsonBufInsertByS2M(char *buf,JSON_SINGAL_TYPE_t stype,
			JSON_MULTI_TYPE_t mtype,char *key,char *value){
		
	char indL,indR;		
	if(mtype==JSON_OBJECT){
		indL='{';
		indR='}';
	}else if(mtype==JSON_ARRAY){
		indL='[';
		indR=']';
	}else{
//		return -1;  //type in the wrong JSON_MULTI_TYPE_t
		return buf;
	}
	
	//judge !! is there has a , ??
	if(*buf==indR){
		if(*(buf-1)!=indL)
			*(buf++)=',';
	}else{
//		return -2;	//wrong buf??
		return buf;
	}
	
	if(stype==JSON_STRING){
		*(buf++)='\"';
		JSON_STR_CPY(buf,key);
		*(buf++)='\"';
		*(buf++)=':';
		*(buf++)='\"';
		JSON_STR_CPY(buf,value);
		*(buf++)='\"';
		*buf=indR;
		return buf;
	}else if(stype==JSON_PRIMITIVE){
		*(buf++)='\"';
		JSON_STR_CPY(buf,key);
		*(buf++)='\"';
		*(buf++)=':';
		JSON_STR_CPY(buf,value);
		*buf=indR;
		return buf;
	}else{
		
		return buf;
	}
	
}

char *jsonBufInsertByM2M(char *buf,JSON_MULTI_TYPE_t stype,
			JSON_MULTI_TYPE_t mtype,char *buff){
		
	char indL,indR;	
	if(mtype==stype){
//		return 0;//illegal input
		return buf;
	}	
	if(mtype==JSON_OBJECT){
		indL='{';
		indR='}';
	}else if(mtype==JSON_ARRAY){
		indL='[';
		indR=']';
	}else{
//		return -1;  //type in the wrong JSON_MULTI_TYPE_t
		return buf;
	}
	
	//judge !! is there has a , ??
	if(*buf==indR){
		if(*(buf-1)!=indL)
			*(buf++)=',';
	}else{
//		return -2;	//wrong buf??
		return buf;
	}
	
	if(stype==JSON_OBJECT){
		JSON_STR_CPY(buf,buff);
		*buf=indR;
		return buf;
	}else if(stype==JSON_ARRAY){
		JSON_STR_CPY(buf,buff);
		*buf=indR;
		return buf;
	}else{
		
		return buf;
	}
	
}
*/

//version 2
#define JSON_STR_CPY(des,src)				{for(;*src!='\0';des++,src++) *des=*src;*des='\0';}

#define JSON_BACK_FLAG		100
static char bak_fg[JSON_BACK_FLAG+2];

#define JSON_BAK(buf)						{for(bak_fg[JSON_BACK_FLAG+1]=JSON_BACK_FLAG;(*buf=='}'||*buf==']')&&bak_fg[JSON_BACK_FLAG+1];bak_fg[JSON_BACK_FLAG+1]--,buf--){bak_fg[bak_fg[JSON_BACK_FLAG+1]]=*buf;}}
#define JSON_BAK_RE(buf)					{for(bak_fg[JSON_BACK_FLAG+1]+=1;bak_fg[JSON_BACK_FLAG+1]<=JSON_BACK_FLAG;bak_fg[JSON_BACK_FLAG+1]++,buf++){*buf=bak_fg[bak_fg[JSON_BACK_FLAG+1]];}}

char* JSON_START(char *buf){

	buf[0]='{';
	buf[1]=',';
	buf[2]='}';
	buf[3]='\0';
	return buf+3;
}

char* JSON_END(char *buf){

	if(*buf=='\0'){
		buf--;
	}
	if(*buf==','){
		*buf='\0';
		return buf;
	}
	return buf+1;
}

char *JSON_START_OBJECT(char *buf){
	
	if(*buf=='\0'){
		buf--;
	}
	JSON_BAK(buf);
	if(*buf!=','){
		buf++;
		JSON_BAK_RE(buf);
		*buf='\0';
		return buf;
	}
	if(*(buf-1)=='{'||*(buf-1)=='['){
		*(buf)='{';
		*(++buf)=',';
		*(++buf)='}';
	}else{
		*(++buf)='{';
		*(++buf)=',';
		*(++buf)='}';
	}
	++buf;
	JSON_BAK_RE(buf);
	*(buf)='\0';
	return buf;
	
}
char *JSON_END_OBJECT(char *buf){
	char *tmp;
	if(*buf=='\0'){
		buf--;
	}
	JSON_BAK(buf);
	if(*buf!=','){
		buf++;
		JSON_BAK_RE(buf);
		*buf='\0';
		return buf;
	}
	tmp=buf;
	*(buf++)=',';
	JSON_BAK_RE(buf);
	*(buf)='\0';
	tmp[0]='}';
	tmp[1]=',';	
	return buf;	
}

/*
int JSON_START_ARRAY_handler(char *buf, char *name){
	char *bufTmp=buf;
	*(buf++)=',';
	*(buf++)='\"';
	JSON_STR_CPY(buf,name);
	*(buf++)='\"';
	*(buf++)=':';
	*(buf++)='[';
	*buf=']';
	return buf-bufTmp;
}*/
char *JSON_START_ARRAY(char *buf,char *name){
	if(*buf=='\0'){
		buf--;
	}
	JSON_BAK(buf);
	if(*buf!=','){
		buf++;
		JSON_BAK_RE(buf);
		*buf='\0';
		return buf;
	}
	
	if(*(buf-1)!='{'&&*(buf-1)!='['){
		buf++;
	}
	
	*(buf++)='\"';
	JSON_STR_CPY(buf,name);
	*(buf++)='\"';
	*(buf++)=':';
	*(buf++)='[';
	*(buf++)=',';
	*(buf++)=']';
	JSON_BAK_RE(buf);
	*(buf)='\0';
	return buf;
}


char *JSON_END_ARRAY(char *buf){
	char *tmp;
	if(*buf=='\0'){
		buf--;
	}
	JSON_BAK(buf);
	if(*buf!=','){
		buf++;
		JSON_BAK_RE(buf);
		*buf='\0';
		return buf;
	}
	tmp=buf;
	*(buf++)=',';
	JSON_BAK_RE(buf);
	*(buf)='\0';
	tmp[0]=']';
	tmp[1]=',';	
	return buf;	
}


//the JSON_MULTI_TYPE_t indicate the type whitch you are creating.
//the JSON_SINGAL_TYPE_t indicate the type whitch you will be inserted in JSON_MULTI_TYPE_t's type.
//key-value is the type of the value pair.

//buf is the alright index of the new inserted type in json String, and point to } or ].
char *jsonBufInsertByS2M(char *buf,JSON_SINGAL_TYPE_t stype,
			JSON_MULTI_TYPE_t mtype,char *key,char *value){
	
	if(*buf=='\0'){
		buf--;
	}	
	JSON_BAK(buf);
	if(*buf!=','){
		buf++;
		JSON_BAK_RE(buf);
		*buf='\0';
		return buf;
	}
	if(*(buf-1)!='{'&&*(buf-1)!='['){
		buf++;
	}
	if(stype==JSON_STRING){
		*(buf++)='\"';
		JSON_STR_CPY(buf,key);
		*(buf++)='\"';
		*(buf++)=':';
		*(buf++)='\"';
		JSON_STR_CPY(buf,value);
		*(buf++)='\"';
	}else if(stype==JSON_PRIMITIVE){
		*(buf++)='\"';
		JSON_STR_CPY(buf,key);
		*(buf++)='\"';
		*(buf++)=':';
		JSON_STR_CPY(buf,value);
	}
	*(buf++)=',';
	JSON_BAK_RE(buf);
	*buf='\0';
	return buf;
}

/*
***********************************************************************************************************************
--------------------------------------jsmn code------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
***********************************************************************************************************************
*/

/**
 * Allocates a fresh unused token from the token pull.
 */
static jsmntok_t *jsmn_alloc_token(jsmn_parser *parser,
		jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *tok;
	if (parser->toknext >= num_tokens) {
		return NULL;
	}
	tok = &tokens[parser->toknext++];
	tok->start = tok->end = -1;
	tok->size = 0;
#ifdef JSMN_PARENT_LINKS
	tok->parent = -1;
#endif
	return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(jsmntok_t *token, jsmntype_t type,
                            int start, int end) {
	token->type = type;		
	token->start = start;	
	token->end = end;
	token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static jsmnerr_t jsmn_parse_primitive(jsmn_parser *parser, const char *js,
		size_t len, jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *token;
	int start;

	start = parser->pos;

	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		switch (js[parser->pos]) {
#ifndef JSMN_STRICT
			/* In strict mode primitive must be followed by "," or "}" or "]" */
			case ':':
#endif
			case '\t' : case '\r' : case '\n' : case ' ' :
			case ','  : case ']'  : case '}' :
				goto found;
		}
		if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
			parser->pos = start;
			return JSMN_ERROR_INVAL;
		}
	}
#ifdef JSMN_STRICT
	/* In strict mode primitive must be followed by a comma/object/array */
	parser->pos = start;
	return JSMN_ERROR_PART;
#endif

found:
	if (tokens == NULL) {
		parser->pos--;
		return JSMN_ERROR_NONE;
	}
	token = jsmn_alloc_token(parser, tokens, num_tokens);
	if (token == NULL) {
		parser->pos = start;
		return JSMN_ERROR_NOMEM;
	}
	jsmn_fill_token(token, JSMN_PRIMITIVE, start, parser->pos);
#ifdef JSMN_PARENT_LINKS
	token->parent = parser->toksuper;
#endif
	parser->pos--;
	return JSMN_ERROR_NONE;
}

/**
 * Filsl next token with JSON string.
 */
static jsmnerr_t jsmn_parse_string(jsmn_parser *parser, const char *js,
		size_t len, jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *token;

	int start = parser->pos;

	parser->pos++;

	/* Skip starting quote */
	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		char c = js[parser->pos];

		/* Quote: end of string */
		if (c == '\"') {
			if (tokens == NULL) {
				return JSMN_ERROR_NONE;
			}
			token = jsmn_alloc_token(parser, tokens, num_tokens);
			if (token == NULL) {
				parser->pos = start;
				return JSMN_ERROR_NOMEM;
			}
			jsmn_fill_token(token, JSMN_STRING, start+1, parser->pos);
#ifdef JSMN_PARENT_LINKS
			token->parent = parser->toksuper;
#endif
			return JSMN_ERROR_NONE;
		}

		/* Backslash: Quoted symbol expected */
		if (c == '\\' && parser->pos + 1 < len) {
			int i;
			parser->pos++;
			switch (js[parser->pos]) {
				/* Allowed escaped symbols */
				case '\"': case '/' : case '\\' : case 'b' :
				case 'f' : case 'r' : case 'n'  : case 't' :
					break;
				/* Allows escaped symbol \uXXXX */
				case 'u':
					parser->pos++;
					for(i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0'; i++) {
						/* If it isn't a hex character we have an error */
						if(!((js[parser->pos] >= 48 && js[parser->pos] <= 57) || /* 0-9 */
									(js[parser->pos] >= 65 && js[parser->pos] <= 70) || /* A-F */
									(js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
							parser->pos = start;
							return JSMN_ERROR_INVAL;
						}
						parser->pos++;
					}
					parser->pos--;
					break;
				/* Unexpected symbol */
				default:
					parser->pos = start;
					return JSMN_ERROR_INVAL;
			}
		}
	}
	parser->pos = start;
	return JSMN_ERROR_PART;
}

/**
 * Parse JSON string and fill tokens.
 * return error info or the count of tokens
 */
jsmnerr_t jsmn_parse(jsmn_parser *parser, const char *js, size_t len,
		jsmntok_t *tokens, unsigned int num_tokens) {
	jsmnerr_t r;
	int i;
	jsmntok_t *token;
	int count = 0;
	
	//
	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		char c;
		jsmntype_t type;
		
		c = js[parser->pos];
		switch (c) {
			case '{': case '[':
				count++;
				if (tokens == NULL) {
					break;
				}
				token = jsmn_alloc_token(parser, tokens, num_tokens);
				if (token == NULL)
					return JSMN_ERROR_NOMEM;
				if (parser->toksuper != -1) {
					tokens[parser->toksuper].size++;
#ifdef JSMN_PARENT_LINKS
					token->parent = parser->toksuper;
#endif
				}
				token->type = (c == '{' ? JSMN_OBJECT : JSMN_ARRAY);
				token->start = parser->pos;
				parser->toksuper = parser->toknext - 1;
				break;
			case '}': case ']':
				if (tokens == NULL)
					break;
				type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
#ifdef JSMN_PARENT_LINKS
				if (parser->toknext < 1) {
					return JSMN_ERROR_INVAL;
				}
				token = &tokens[parser->toknext - 1];
				for (;;) {
					if (token->start != -1 && token->end == -1) {
						if (token->type != type) {
							return JSMN_ERROR_INVAL;
						}
						token->end = parser->pos + 1;
						parser->toksuper = token->parent;
						break;
					}
					if (token->parent == -1) {
						break;
					}
					token = &tokens[token->parent];
				}
#else
				for (i = parser->toknext - 1; i >= 0; i--) {
					token = &tokens[i];
					if (token->start != -1 && token->end == -1) {
						if (token->type != type) {
							return JSMN_ERROR_INVAL;
						}
						parser->toksuper = -1;
						token->end = parser->pos + 1;
						break;
					}
				}
				/* Error if unmatched closing bracket */
				if (i == -1) return JSMN_ERROR_INVAL;
				for (; i >= 0; i--) {
					token = &tokens[i];
					if (token->start != -1 && token->end == -1) {
						parser->toksuper = i;
						break;
					}
				}
#endif
				break;
			case '\"':
				r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
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
						tokens[parser->toksuper].type != JSMN_ARRAY &&
						tokens[parser->toksuper].type != JSMN_OBJECT) {
#ifdef JSMN_PARENT_LINKS
					parser->toksuper = tokens[parser->toksuper].parent;
#else
					for (i = parser->toknext - 1; i >= 0; i--) {
						if (tokens[i].type == JSMN_ARRAY || tokens[i].type == JSMN_OBJECT) {
							if (tokens[i].start != -1 && tokens[i].end == -1) {
								parser->toksuper = i;
								break;
							}
						}
					}
#endif
				}
				break;
#ifdef JSMN_STRICT
			/* In strict mode primitives are: numbers and booleans */
			case '-': case '0': case '1' : case '2': case '3' : case '4':
			case '5': case '6': case '7' : case '8': case '9':
			case 't': case 'f': case 'n' :
				/* And they must not be keys of the object */
				if (tokens != NULL) {
					jsmntok_t *t = &tokens[parser->toksuper];
					if (t->type == JSMN_OBJECT ||
							(t->type == JSMN_STRING && t->size != 0)) {
						return JSMN_ERROR_INVAL;
					}
				}
#else
			/* In non-strict mode every unquoted value is a primitive */
			default:
#endif
				r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
				if (r < 0) return r;
				count++;
				if (parser->toksuper != -1 && tokens != NULL)
					tokens[parser->toksuper].size++;
				break;

#ifdef JSMN_STRICT
			/* Unexpected char in strict mode */
			default:
				return JSMN_ERROR_INVAL;
#endif
		}
	}

	for (i = parser->toknext - 1; i >= 0; i--) {
		/* Unmatched opened object or array */
		if (tokens[i].start != -1 && tokens[i].end == -1) {
			return JSMN_ERROR_PART;
		}
	}

	return count;
	//return JSMN_ERROR_NONE;
}

/**
 * Creates a new parser based over a given  buffer with an array of tokens
 * available.
 */
void jsmn_init(jsmn_parser *parser) {
	parser->pos = 0;
	parser->toknext = 0;
	parser->toksuper = -1;
}

/*
	json check from numberous results.
*/
int jsonCheck(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
