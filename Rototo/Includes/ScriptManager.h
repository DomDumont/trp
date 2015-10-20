/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2014 - Dominique Dumont

   Permission is granted to use this software under the terms of either:
   a) the GPL v3 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   T.R.P. is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   -----------------------------------------------------------------------------

   To release a closed-source product which uses T.R.P., commercial licenses are
   available: visit veed.fr for more information.

  ==============================================================================
*/

#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#ifdef TRP_USE_BINDING

#include <angelscript.h>
#include "binding/scripthandle.h"
#include <string>
#include <vector>



	

class FunctionEntry
	{

	public:
		asIScriptFunction *func;
		char	argsFormat[16]; //TODO change this hardcoded value
	};

class CDebug;

class ScriptManager
{
public:
	static ScriptManager& Get();

	~ScriptManager();

	void Init();
	void Prepare();
	void Shutdown();
	void MessageCallback(const asSMessageInfo &msg);
	
	void CompileScript(const std::string& _file);
	void CompileScriptViaBuilder(const std::string& _file);
	FunctionEntry * RegisterScript(const std::string& _prototype, char * _fmt);


	void RunScript(const std::string& _prototype, char * _fmt, ...);
	void RunFunctionEntry(FunctionEntry * _fe, ...);
	int RunCallback(asIScriptFunction * _callback,CScriptHandle *_sender,CScriptHandle *_userData);

	template <class class_type>
	bool RegisterClass(const std::string& class_name);
	void RegisterObjectProperty(const std::string& class_name, const std::string& function_definition, int byteOffset);
	void RegisterGlobalFunction(const std::string& function_definition, const asSFuncPtr &funcPointer,asDWORD callConv, void * objFOrThisCall = 0);


	



public:
	asIScriptEngine		*engine;
private:
	
	ScriptManager();

	// Contexts is what you use to call AngelScript functions and methods. They say you must pool them to avoid overhead. So I do as they say.
	asIScriptContext* NewContext();
	void FreeContext(asIScriptContext *ctx);


	asIScriptModule		*module;
	
	std::vector<asIScriptContext*> contexts;
	std::vector<FunctionEntry*> functions;
	bool				hasCompileErrors;

	CDebug			*mydbg;
	

	
};
#endif

#endif
