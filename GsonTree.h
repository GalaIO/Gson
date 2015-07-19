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
**/
#ifndef _GSONTREE_H_
#define _GSONTREE_H_

#include "gson.h"
//as a arg for gson pair that it indicate the way of get or set for json-tree. 
typedef enum{
	GET_h=0,
	SET_h
}gsonHandler_t;

//a super class ,it's pointer can quote sub-class.
typedef struct gsontree_type{
	gsontype_t 	type;
	void*		empty;	//declre a void * for warning.
}gsontree_type; 

//the key-value type.
typedef struct gsontree_pair{
	gsontype_t 	type;
	//callback protype.
	// @param	temp  can cast to (gson_generator *) or (gsontok_p *) for getter and setter.
	// @param   sog	  indicate the handler is set or get.
	gsonerr_t 	(*handler)(void *temp,gsonHandler_t sog);		
}gsontree_pair;

//the object.
typedef struct gsontree_object{
	gsontype_t 		type;
	#define 		GSONTREE_OBJECT_NAME_SIZE	10
	char			name[GSONTREE_OBJECT_NAME_SIZE];
	int		  		count;
	gsontree_type 	*current_tree;
}gsontree_object;

//the array.
typedef struct gsontree_array{
	gsontype_t 		type;
	#define 		GSONTREE_ARRAY_NAME_SIZE	10
	char			name[GSONTREE_ARRAY_NAME_SIZE];
	//callback protype.
	// @param	temp  can cast to (gson_generator *) or (gsontok_p *) for getter and setter.
	// @param   sog	  indicate the handler is set or get.
	gsonerr_t 	(*handler)(void *temp,gsonHandler_t sog);
}gsontree_array;

#define GSONTREE_STRING(handler)		{GSON_STRING,(handler)}
#define GSONTREE_PRIMITIVE(handler)		{GSON_PRIMITIVE,(handler)}

#define GSONTREE_KV(name)				static gsontree_type pair_##name[]    
#define GSONTREE(name,count)			static gsontree_object name = { GSON_OBJECT,"",count, pair_##name}

#define GSONTREE_OBJECT_KV(name)		gsontree_type pair_##name[]    
#define GSONTREE_OBJECT(name,count)		gsontree_object name = { GSON_OBJECT,#name,count, pair_##name}
   
#define GSONTREE_ARRAY(name,handler)	{GSON_ARRAY,#name,(handler)}


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

#endif

