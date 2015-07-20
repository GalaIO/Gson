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
**/
#ifndef _GSONTREE_H_
#define _GSONTREE_H_

#include "gson.h"
//as a arg for gson pair that it indicate the way of get or set for json-tree. 
typedef enum{
	GET_value=0,
	SET_value
}gsonHandler_t;

//a super class ,it's pointer can quote sub-class.
typedef struct gsontree_type{
	gsontype_t 	type;
	//void*		empty;	//declre a void * for warning.
}gsontree_type; 

//the temp pair.
typedef struct gsontree_pair{
	char *name;
	gsontree_type *pair_type;		
}gsontree_pair;

//the key-value type.
typedef struct gsontree_kv{
	gsontype_t 	type;
	//callback protype.
	// @param	temp  can cast to (gson_generator *) or (gsontok_p *) for getter and setter.
	// @param   sog	  indicate the handler is set or get.
	gsonerr_t 	(*handler)(void *temp,gsonHandler_t sog);		
}gsontree_kv;

//the object.
typedef struct gsontree_object{
	gsontype_t 		type;
	int		  		count;
	gsontree_pair 	*current_tree;
}gsontree_object;

//the array.
typedef struct gsontree_array{
	gsontype_t 		type;
	//callback protype.
	// @param	temp  can cast to (gson_generator *) or (gsontok_p *) for getter and setter.
	// @param   sog	  indicate the handler is set or get.
	gsonerr_t 	(*handler)(void *temp,gsonHandler_t sog);
}gsontree_array;

/*
#define GSONTREE_STRING(name,handler)			const gsontree_kv pair_##name={GSON_STRING,(handler)}
#define GSONTREE_PRIMITIVE(name,handler)		const gsontree_kv pair_##name={GSON_PRIMITIVE,(handler)}
#define GSONTREE_ARRAY(name,handler)			const gsontree_array pair_##name={GSON_ARRAY,(handler)}
*/
//modify th correspoing macro.
//just put the callback handler name in it.
#define GSONTREE_STRING(name)			const gsontree_kv pair_##name={GSON_STRING,(name)}
#define GSONTREE_PRIMITIVE(name)		const gsontree_kv pair_##name={GSON_PRIMITIVE,(name)}
#define GSONTREE_ARRAY(name)			const gsontree_array pair_##name={GSON_ARRAY,(name)}

//quote above packaged handler.
#define GSONTREE_PAIR(name)				{#name , (gsontree_type*)&pair_##name}

//quote above packaged pair.
#define GSONTREE_KV(name)				const gsontree_pair pairARR_##name[]    
//build a json tree.
#define GSONTREE(name,count)			const gsontree_object name = { GSON_OBJECT,count, (gsontree_pair *)pairARR_##name}

//quote above packaged pair.
#define GSONTREE_OBJECT_KV(name)		const gsontree_pair pairARR_##name[]    
//build a json object tree.    
#define GSONTREE_OBJECT(name,count)		const gsontree_object pair_##name = { GSON_OBJECT,count, (gsontree_pair *)pairARR_##name}
   


//
//
//function in GsonTree
//
//

/*
	build a json object autoly.
	@param  generator	the handler of json generator.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_OBJECT_BUILD(gson_generator *generator,const gsontree_object object);

/*
	build a json data autoly.
	@param  generator	the handler of json generator.
	@param  object		the info of the object,can indicate the struct of json tree.
*/
gsonerr_t	GSON_BUILD(gson_generator *generator,const gsontree_object object);

#endif

