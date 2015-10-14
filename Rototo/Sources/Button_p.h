#ifndef __BUTTON_P_H__
#define __BUTTON_P_H__

#ifdef TRP_USE_BINDING
#include "angelscript.h"
#include "binding/scripthandle.h"
#endif

class Button_p
{
public:
#ifdef TRP_USE_BINDING
	asIScriptFunction *	on_click_handler_script;
	CScriptHandle		user_data_script;
	CScriptHandle		sender_script;
#endif

public:
	Button_p();


};


#endif