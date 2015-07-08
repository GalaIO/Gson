/*
*Author:		LaoGuo
*Date:			2015-5-1 22:12
*Descripton:	debug macro.
*
*
*Author:		LaoGuo
*Date:			2015-7-6 9:33 pm
*Descripton:	add debug function for GalaIO ware.
*
*/

#ifndef _DEBUG_H
#define _DEBUG_H

//define some quickly format type
//you can use it just like:
//printf("hi it's is"U8_D", also"X32_D"!!!",'1',238);
#define U8_D  "%c"
#define S8_D  "%c"
#define X8_D  "%x"
#define U16_D "%u"
#define S16_D "%d"
#define X16_D "%x"
#define U32_D "%u"
#define S32_D "%d"
#define X32_D "%x"

#if 1

//out put format string ,but format_str must include by ()
//eg. DEBUG_DIA(("%d",10))---->os_printf("%d",10)
#define DEBUG_DIA(format_str)  (printf format_str)

//out put just str,not need to included by ()
//eg. DEBUG_STR("121212")---->printf("121212")
#define DEBUG_STR(str)  (printf(str))

//assert for the expression
#define DEBUG_ASSERT(assert,message) {if(!(assert)){\
	DEBUG_STR(message);}

//assert the expression  and handler it
#define DEBUG_ERROR(expression,message,handler) {if(!(expression)){\
	DEBUG_STR(message);handler;}

//assert for the expression,message must be included by ()
#define DEBUG_ASSERT_DIA(assert,message) {if(!(assert)){\
	DEBUG_DIA(message);}}

//assert the expression  and handler it,message must be included by ()
#define DEBUG_ERROR_DIA(expression,message,handler) {if(!(expression)){\
	DEBUG_DIA(message);handler;}


#else

#define DEBUG_DIA(format_str)
#define DEBUG_STR(str)
#define DEBUG_ASSERT(message,assert)
#define DEBUG_ERROR(message,expression,handler) 
#define DEBUG_ASSERT_DIA(message,assert) 
#define DEBUG_ERROR_DIA(message,expression,handler) 


#endif




/*
 ***************************************************************************
 *							user_debug_define
 ***************************************************************************
 *
*/


#endif


