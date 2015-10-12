#ifndef __COMBOBOX_P_H__
#define __COMBOBOX_P_H__

#ifdef TRP_USE_BINDING
#include <angelscript.h>
#include "binding/scripthandle.h"
#endif

class ComboBox_p
{
#ifdef TRP_USE_BINDING
	asIScriptFunction *		onSelectionChangedHandler_script;
	CScriptHandle			userData_script;
	CScriptHandle			sender_script;
#endif
	ComboBox_p();
};

#endif