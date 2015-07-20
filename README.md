# Gson
Gson is a tool to handler  Json data by C in resouce limited environment.

gson generator design by me.
gson parser modify from jsmn.
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
*/
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
