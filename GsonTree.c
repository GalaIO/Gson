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
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD: the object with the error initial!!\n"));
		return GSON_ERROR_PART;
	}
	//object start.
	GSON_START(generator);
	
	for(i = 0;i < object.count;i++){
		//traverse the entir object.
		if(object.current_tree[i].type == GSON_PRIMITIVE || object.current_tree[i].type == GSON_STRING){
			//key-value pair handler.
			if(((gsontree_pair*)(object.current_tree+i))->handler!=NULL){
				((gsontree_pair*)(object.current_tree+i))->handler(generator,GET_h);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no callback in %d pair !!\n",i));
			}
		}else if(object.current_tree[i].type == GSON_OBJECT){
			//object in object.
			//type the object name.
			if(((gsontree_object*)(object.current_tree+i))->name[0]!='\0'){
				gsonInsertK(generator,((gsontree_object*)(object.current_tree+i))->name);
			}
			GSON_OBJECT_BUILD(generator,*((gsontree_object*)(object.current_tree+i)));
		}else if(object.current_tree[i].type == GSON_ARRAY){
			//array in object.
			//type the array name.
			if(((gsontree_object*)(object.current_tree+i))->name[0]!='\0'){
				GSON_START_ARRAY(generator,((gsontree_object*)(object.current_tree+i))->name);
			}
			//array handler.
			if(((gsontree_pair*)(object.current_tree+i))->handler!=NULL){
				((gsontree_pair*)(object.current_tree+i))->handler(generator,GET_h);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no callback in array !!\n",i));
			}
			GSON_END_ARRAY(generator);
		}else{
			GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD: no a invild type in current object!!\n"));
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
		GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD: the object with the error initial!!\n"));
		return GSON_ERROR_PART;
	}
	//object start.
	GSON_START_OBJECT(generator);
	
	for(i = 0;i < object.count;i++){
		//traverse the entir object.
		if(object.current_tree[i].type == GSON_PRIMITIVE || object.current_tree[i].type == GSON_STRING){
			//key-value pair handler.
			if(((gsontree_pair*)(object.current_tree+i))->handler!=NULL){
				((gsontree_pair*)(object.current_tree+i))->handler(generator,GET_h);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no callback in %d pair !!\n",i));
			}
		}else if(object.current_tree[i].type == GSON_OBJECT){
			//object in object.
			//type the object name.
			if(((gsontree_object*)(object.current_tree+i))->name[0]!='\0'){
				gsonInsertK(generator,((gsontree_object*)(object.current_tree+i))->name);
			}
			GSON_OBJECT_BUILD(generator,*((gsontree_object*)(object.current_tree+i)));
		}else if(object.current_tree[i].type == GSON_ARRAY){
			//array in object.
			//type the array name.
			if(((gsontree_object*)(object.current_tree+i))->name[0]!='\0'){
				GSON_START_ARRAY(generator,((gsontree_object*)(object.current_tree+i))->name);
			}
			//array handler.
			if(((gsontree_pair*)(object.current_tree+i))->handler!=NULL){
				((gsontree_pair*)(object.current_tree+i))->handler(generator,GET_h);
			}else{
				GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD : no callback in array !!\n",i));
			}
			GSON_END_ARRAY(generator);
		}else{
			GSON_DEBUG_DIA(GSON_DEBUG_GSONTREE_ON,("-GSONTREE in GSON_OBJECT_BUILD: no a invild type in current object!!\n"));
			return GSON_ERROR_PART;
		}
	}
	
	//end od object.
	GSON_END_OBJECT(generator);
	return GSON_ERROR_NONE;
}


