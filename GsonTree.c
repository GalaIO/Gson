/*
 *
 *	Author:			GalaIO
 *	Date:			2015-7-18 9:29 PM
 *	Description:	-add two api GSON_OBJECT_BUILD() GSON_ARRAY_BUILD() for construct json autoly.
 *
 *
 *			
 *	
**/

#include "GsonTree.h"


/*
	build a json data autoly.
	@param  generator	the handler of json generator.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_BUILD(gson_generator *generator,const gsontree_object object){
	int i;
	
	if(object.count < 1 || object.type != GSON_OBJECT || object.current_tree == NULL){
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_BUILD: the object with the error initial!!\n"));
		return GSON_ERROR_PART;
	}
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
				GSON_OBJECT_BUILD(generator,*((gsontree_object*)(object.current_tree+i)));
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


