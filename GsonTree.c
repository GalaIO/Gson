/*
 *
 *	Author:			GalaIO
 *	Date:			2015-7-18 9:29 PM
 *	Description:	-build a tree for JSON generator and parser. 
 *					-add json_type struct as super class.
 *					-add json_pair json_object json_array as sub-class.
 *					-add quick macro for build JSON tree. 
 *
 *
 *	Author:			GalaIO
 *	Date:			2015-7-19 12:57 AM
 *	Description:	-add GSONTREE_OBJECT_KV and GSONTREE_OBJECT for build object data.
 *				
 *				
 *	 
 *	Author:			GalaIO
 *	Date:			2015-7-20  5:40 PM
 *	Description:	-simplify the use of macros.
 *					-add more notes.
 *					
 *				
 *	 
 *	Author:			GalaIO
 *	Date:			2015-7-20  5:40 PM
 *	Description:	-simplify the use of macros.
 *					-add more notes.
 *				
 *				 			
 *	 
 *	Author:			GalaIO
 *	Date:			2015-7-20  10:14 PM
 *	Description:	-add two JSON parser,GSON_PARSER  and GSON_OBJECT_PARSER.
 *					-i just want say "how hard to write code... but fun...".
 *
**/

#include "GsonTree.h"

///
///
/////JSON build code area.
///
///

/*
	build a json data autoly.
	@param  json_str	input the the string of json, witch will be filled autoly.
	@param  json_size	ipput the max length of the string.
	@param  generator	the handler of json generator.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_BUILD(char *json_str,int json_size,gson_generator *generator,const gsontree_object object){
	int i;
	
	if(object.count < 1 || object.type != GSON_OBJECT || object.current_tree == NULL){
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD: the object with the error initial!!\n"));
		return GSON_ERROR_PART;
	}
	//init gson_generator.
	gson_init_generator(generator,json_str,json_size);
	
	//object start.
	GSON_START(generator);
	
	for(i = 0;i < object.count;i++){
		//GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD : type is %d,i = %d.\n",(object.current_tree[i].pair_type)->type,i));
		//traverse the entir object.
		if((object.current_tree[i].pair_type)->type == GSON_PRIMITIVE || (object.current_tree[i].pair_type)->type == GSON_STRING){
			//key-value pair handler.
			gsontree_kv *kv=(gsontree_kv *)(object.current_tree[i].pair_type);
			if(kv->handler!=NULL){
				gsonInsertK(generator,object.current_tree[i].name);
				kv->handler(generator,GET_value);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD : no callback in %d pair !!\n",i));
			}
		}else if((object.current_tree[i].pair_type)->type == GSON_OBJECT){
			//object in object.
			//type the object name.
			gsontree_object *obj=(gsontree_object *)(object.current_tree[i].pair_type);
			if(obj->count > 0){
				gsonInsertK(generator,object.current_tree[i].name);
				GSON_OBJECT_BUILD(generator,*(obj));
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD : no more token object in %d pair !!\n",i));
			}
		}else if((object.current_tree[i].pair_type)->type == GSON_ARRAY){
			//array in object.
			//type the array name.
			gsontree_array *arr=(gsontree_array *)(object.current_tree[i].pair_type);
			if(arr->handler!=NULL){
				//gsonInsertK(generator,object.current_tree[i].name);
				GSON_START_ARRAY(generator,object.current_tree[i].name);
				arr->handler(generator,GET_value);
				GSON_END_ARRAY(generator);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD : no callback in %d pair !!\n",i));
			}
		}else{
			GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD: no a invild type in current object!!\n"));
			return GSON_ERROR_PART;
		}
	}
	
	//end od object.
	GSON_END(generator);
	return GSON_ERROR_NONE;
}

/*
	build a json object autoly.
	@param  generator	the handler of json generator.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_OBJECT_BUILD(gson_generator *generator,const gsontree_object object){
	int i;
	if(object.count < 1 || object.type != GSON_OBJECT || object.current_tree == NULL){
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD: the object with the error initial!!-%d-%d\n",object.count ,object.type));
		return GSON_ERROR_PART;
	}
	//object start.
	GSON_START_OBJECT(generator);
	
	for(i = 0;i < object.count;i++){
		//traverse the entir object.
		if((object.current_tree[i].pair_type)->type == GSON_PRIMITIVE || (object.current_tree[i].pair_type)->type == GSON_STRING){
			//key-value pair handler.
			gsontree_kv *kv=(gsontree_kv *)(object.current_tree[i].pair_type);
			if(kv->handler!=NULL){
				gsonInsertK(generator,object.current_tree[i].name);
				kv->handler(generator,GET_value);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no callback in %d pair !!\n",i));
			}
		}else if((object.current_tree[i].pair_type)->type == GSON_OBJECT){
			//object in object.
			//type the object name.
			gsontree_object *obj=(gsontree_object *)(object.current_tree[i].pair_type);
			if(obj->count > 0){
				gsonInsertK(generator,object.current_tree[i].name);
				GSON_OBJECT_BUILD(generator,*(obj));
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no more token object in %d pair !!\n",i));
			}
		}else if((object.current_tree[i].pair_type)->type == GSON_ARRAY){
			//array in object.
			//type the array name.
			gsontree_array *arr=(gsontree_array *)(object.current_tree[i].pair_type);
			if(arr->handler!=NULL){
				//gsonInsertK(generator,object.current_tree[i].name);
				GSON_START_ARRAY(generator,object.current_tree[i].name);
				arr->handler(generator,GET_value);
				GSON_END_ARRAY(generator);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no callback in %d pair !!\n",i));
			}
		}else{
			GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD: no a invild type in current object!!\n"));
			return GSON_ERROR_PART;
		}
	}
	
	//end od object.
	GSON_END_OBJECT(generator);
	return GSON_ERROR_NONE;
}



///
///
/////JSON parser code area.
///
///

/*
	parser a json String autoly.
	@param  json_data 	input a complete JSON data String.
	@param 	tok			input the gsontok_t array.
	@param  tok_size	input the number of gsontok_t.
	@param  parser		the handler of json parser.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_PARSER(char *json_data,gsontok_t *tok,int tok_size,gson_parser *parser,const gsontree_object object){
	int i,r,j;
	
	gson_init_parser(parser);
	//init other members.
	parser->json_buf = json_data;
	parser->type = GSON_PRIMITIVE;
	parser->tok = NULL;
	parser->tok_c = -1;
	
	r = gson_parse(parser, json_data, tok, tok_size);
	if (r < 0) {
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER: Failed to parse GSON: %d\n", r));
		return GSON_ERROR_PART;
	}

	//Assume the top-level element is an object 
	if (r < 1 || tok[0].type != GSON_OBJECT) {
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER: r=%d,type=%d,Object expected\n",r,tok[0].type));
		return GSON_ERROR_PART;
	}
	GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER: the total token-size is :%d\n",r));
	// Loop over all keys of the root object 
	//i = 1,point to the first token in the ogject.
	for (i = 1; i < r ;) {
		for(j = 0;j < object.count;j++){
			if(GSON_ERROR_NONE == gsonCheck(parser->json_buf,&tok[i],object.current_tree[j].name)){
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER: match successful -%s-!!!\n",object.current_tree[j].name));
				//match the key.
				if(object.current_tree[j].pair_type->type == GSON_PRIMITIVE || object.current_tree[j].pair_type->type == GSON_STRING){
					//key-value pair handler.
					gsontree_kv *kv=(gsontree_kv *)(object.current_tree[j].pair_type);
					//afferent type.
					parser->type = object.current_tree[j].pair_type->type;
					//afferent tok.
					parser->tok = &tok[i+1];
					//afferent the number of tok.
					parser->tok_c = tok[i].size;
					if(kv->handler!=NULL){
						//invoking the callback.
						kv->handler(parser,SET_value);
					}else{
						GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER : no callback in %d pair !!\n",i));
					}
					//update i.
					i+=2;
				}else if(object.current_tree[j].pair_type->type == GSON_ARRAY){
					//array handler.
					gsontree_array *arr=(gsontree_array *)(object.current_tree[j].pair_type);
					//afferent type.
					parser->type = GSON_ARRAY;
					//afferent tok.
					parser->tok = &tok[i+2];
					//afferent the number of tok.
					parser->tok_c = tok[i+1].length;
					if(arr->handler!=NULL){
						//invoking the callback.
						arr->handler(parser,SET_value);
					}else{
						GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER : no callback in %d pair !!\n",i));
					}
					//update i.
					i+=1+1+tok[i+1].length;
					
				}else if(object.current_tree[j].pair_type->type == GSON_OBJECT){
					//object handler.
					gsontree_object *obj=(gsontree_object *)(object.current_tree[j].pair_type);
					//afferent type.
					parser->type = GSON_OBJECT;
					//afferent tok.
					parser->tok = &tok[i+1];
					//afferent the number of tok.
					parser->tok_c = tok[i+1].length;
					if(obj->count > 0){
						GSON_OBJECT_PARSER(parser,*(obj));
					}else{
						GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER : no more token object in %d pair !!\n",i));
					}
					//update i.
					i+=1+1+tok[i+1].length;
				}else{
					if(i+1 < r && (tok[i+1].type == GSON_PRIMITIVE || tok[i+1].type == GSON_STRING)){
						//update i.
						i+=2;
					}else if(i+1 < r && (tok[i+1].type == GSON_ARRAY || tok[i+1].type == GSON_OBJECT)){
						//update i.
						i+=1+1+tok[i+1].length;
					}else{
						i+=2;
					}
				}
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_PARSER: match failed !!!\n"));
				if(i+1 < r && (tok[i+1].type == GSON_PRIMITIVE || tok[i+1].type == GSON_STRING)){
					//update i.
					i+=2;
				}else if(i+1 < r && (tok[i+1].type == GSON_ARRAY || tok[i+1].type == GSON_OBJECT)){
					//update i.
					i+=1+1+tok[i+1].size;
				}else{
					i+=2;
				}	
			}
		}
	}
	
	return GSON_ERROR_NONE;
}


/*
	parser a json String autoly.
	@param  parser		the handler of json parser.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_OBJECT_PARSER(gson_parser *parser,const gsontree_object object){
	int i,j;
	gsontok_t	*tok = parser->tok;
	int r = parser->tok_c;
	GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_PARSER: the total token-size is :%d\n",r));
	// Loop over all keys of the root object 
	//i = 1,point to the first token in the ogject.
	for (i = 1; i < r ;) {
		for(j = 0;j < object.count;j++){
			if(GSON_ERROR_NONE == gsonCheck(parser->json_buf,&tok[i],object.current_tree[j].name)){
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_PARSER: match successful -%s-!!!\n",object.current_tree[j].name));
				//match the key.
				if(object.current_tree[j].pair_type->type == GSON_PRIMITIVE || object.current_tree[j].pair_type->type == GSON_STRING){
					//key-value pair handler.
					gsontree_kv *kv=(gsontree_kv *)(object.current_tree[j].pair_type);
					//afferent type.
					parser->type = object.current_tree[j].pair_type->type;
					//afferent tok.
					parser->tok = &tok[i+1];
					//afferent the number of tok.
					parser->tok_c = tok[i].size;
					if(kv->handler!=NULL){
						//invoking the callback.
						kv->handler(parser,SET_value);
					}else{
						GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_PARSER : no callback in %d pair !!\n",i));
					}
					//update i.
					i+=2;
				}else if(object.current_tree[j].pair_type->type == GSON_ARRAY){
					//array handler.
					gsontree_array *arr=(gsontree_array *)(object.current_tree[j].pair_type);
					//afferent type.
					parser->type = GSON_ARRAY;
					//afferent tok.
					parser->tok = &tok[i+2];
					//afferent the number of tok.
					parser->tok_c = tok[i+1].length;
					if(arr->handler!=NULL){
						//invoking the callback.
						arr->handler(parser,SET_value);
					}else{
						GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_PARSER : no callback in %d pair !!\n",i));
					}
					//update i.
					i+=1+1+tok[i+1].length;
					
				}else if(object.current_tree[j].pair_type->type == GSON_OBJECT){
					//object handler.
					gsontree_object *obj=(gsontree_object *)(object.current_tree[j].pair_type);
					//afferent type.
					parser->type = GSON_OBJECT;
					//afferent tok.
					parser->tok = &tok[i+1];
					//afferent the number of tok.
					parser->tok_c = tok[i+1].length;
					if(obj->count > 0){
						GSON_OBJECT_PARSER(parser,*(obj));
					}else{
						GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_PARSER : no more token object in %d pair !!\n",i));
					}
					//update i.
					i+=1+1+tok[i+1].length;
				}else{
					if(i+1 < r && (tok[i+1].type == GSON_PRIMITIVE || tok[i+1].type == GSON_STRING)){
						//update i.
						i+=2;
					}else if(i+1 < r && (tok[i+1].type == GSON_ARRAY || tok[i+1].type == GSON_OBJECT)){
						//update i.
						i+=1+1+tok[i+1].length;
					}else{
						i+=2;
					}
				}
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_PARSER: match failed !!!\n"));
				if(i+1 < r && (tok[i+1].type == GSON_PRIMITIVE || tok[i+1].type == GSON_STRING)){
					//update i.
					i+=2;
				}else if(i+1 < r && (tok[i+1].type == GSON_ARRAY || tok[i+1].type == GSON_OBJECT)){
					//update i.
					i+=1+1+tok[i+1].size;
				}else{
					i+=2;
				}	
			}
		}
	}
	
	return GSON_ERROR_NONE;
}

