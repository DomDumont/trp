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

#include "Global.h"



#include "ScriptManager.h"
#include "Application.h"
#include "Utils.h"

#include "Atlas.h" //Registration
#include "Sprite.h" //Registration
#include "TiledMap.h" //Registration
#include "Animation.h" //Registration

#include "Button.h" //Registration
#include "Font.h" //Registration
#include "Label.h" //Registration
#include "ListBox.h" //Registration
#include "ComboBox.h" //Registration
#include "TextBox.h" //Registration
#include "Primitive.h" //Registration

#include "Vector2D.h" //Registration

#include "Particles.h" //Registration

#include "scriptstdstring/scriptstdstring.h"
#include "scripthandle/scripthandle.h"
#include "scriptbuilder/scriptbuilder.h"
#include "scriptmath/scriptmath.h"
#include "scriptarray/scriptarray.h"
#include "Debug.h" // New one based on the angelscript add_on called debugger
#include "scriptdictionary/scriptdictionary.h"


#include <iostream>  // cout
#include <sstream> // stringstream
#include <stdlib.h> // atoi

using namespace std;

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


int MyIncludeCallback(const char *include, const char *from, CScriptBuilder *builder, void *userParam)
{
	ScriptManager *pThis;
	pThis = (ScriptManager *) userParam;
	(void) pThis; // Avoid unused warning
	std::string pLoadedScript;
	if ((include[0] == '~') && (include[1] == '/'))
		{
		pLoadedScript = LoadTextFile(&include[2],FULLPATH|BOTH);
		}
	else
		{
		pLoadedScript = LoadTextFile(include,GAMEDATA|BOTH);
		}
	builder->AddSectionFromMemory(include,pLoadedScript.c_str(),(unsigned int) pLoadedScript.length());
	return 0;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ScriptManager::Init()
{

	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine->SetMessageCallback(asMETHOD(ScriptManager, MessageCallback), this, asCALL_THISCALL);
	
	RegisterStdString(engine);
	RegisterScriptHandle(engine);
	RegisterScriptMath(engine);
	RegisterScriptArray(engine,false);
	RegisterScriptDictionary(engine);
	
	hasCompileErrors = false;
	


	this->Prepare();

	//this->ctx = engine->CreateContext();
	for( unsigned int n = 0; n < 10; n++ )
		{
		contexts.push_back(engine->CreateContext());
		}

	// Create the debugger instance
	this->mydbg = new CDebug();

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"CScriptManager Init Ok");

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ScriptManager::Prepare()
{
	int r;

	RegisterUtils();

	RegisterSoundManager();
	RegisterMusic();
	RegisterSound();

	RegisterAtlas();

	RegisterSprite();
	RegisterTiledMap();
	RegisterAnimation();






	r = engine->RegisterObjectType("TweenedFloat", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_FACTORY, "TweenedFloat@ f()", asFUNCTION(TweenedFloat_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_ADDREF, "void f()", asMETHOD(TweenedFloat,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_RELEASE, "void f()", asMETHOD(TweenedFloat,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = engine->RegisterObjectProperty("TweenedFloat", "float value", asOFFSET(TweenedFloat,value)); SDL_assert( r >= 0 );
	r = engine->RegisterObjectProperty("TweenedFloat", "float targetValue", asOFFSET(TweenedFloat,targetValue)); SDL_assert( r >= 0 );
	r = engine->RegisterObjectProperty("TweenedFloat", "float initialValue", asOFFSET(TweenedFloat,initialValue)); SDL_assert( r >= 0 );


	r = engine->RegisterObjectType("Tween", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Tween", asBEHAVE_FACTORY, "Tween@ f()", asFUNCTION(Tween_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Tween", asBEHAVE_ADDREF, "void f()", asMETHOD(Tween,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Tween", asBEHAVE_RELEASE, "void f()", asMETHOD(Tween,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = engine->RegisterObjectProperty("Tween", "CallbackHandler @onComplete", asOFFSET(Tween, onCompleteHandler)); SDL_assert( r >= 0 );
	r = engine->RegisterObjectProperty("Tween", "ref @userData", asOFFSET(Tween, userData)); SDL_assert( r >= 0 );

	this->RegisterClassMethod("Tween","void AddProp(TweenedFloat @)", asMETHOD(Tween, AddProp));
	this->RegisterClassMethod("Tween","void Init(float _duration,int _effect,int _easeMode)", asMETHOD(Tween, Init));

	r = engine->RegisterGlobalFunction("void TWN_AddTween(Tween @)", asMETHOD(TweenManager,AddTween), asCALL_THISCALL_ASGLOBAL, g_app->tweenManager);SDL_assert(r>0);
	r = engine->RegisterGlobalFunction("void TWN_CancelTweens()", asMETHOD(TweenManager,Shutdown), asCALL_THISCALL_ASGLOBAL, g_app->tweenManager);SDL_assert(r>0);

	RegisterVector2D();
	
	//GUI Related
	RegisterWidget();
	RegisterFont();
	RegisterButton();
	RegisterLabel();
	RegisterListBox();
	RegisterComboBox();
	RegisterTextBox();
	RegisterPrimitive();

	r = engine->RegisterGlobalFunction("void GUI_AddWidget(Widget @)", asMETHOD(GUIManager,AddWidget), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);SDL_assert(r>0);
	r = engine->RegisterGlobalFunction("void GUI_RemoveWidget(Widget @)", asMETHOD(GUIManager,RemoveWidget), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);SDL_assert(r>0);
	r = engine->RegisterGlobalFunction("void GUI_LoadTheme(string &in _file)", asMETHOD(GUIManager,LoadTheme), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);SDL_assert(r>0);
	r = engine->RegisterGlobalFunction("void GUI_UnLoadTheme()", asMETHOD(GUIManager,UnLoadTheme), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);SDL_assert(r>0);

	r = engine->RegisterObjectBehaviour("Widget", asBEHAVE_REF_CAST, "Button@ f()", asFUNCTION((refCast<Widget,Button>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Button", asBEHAVE_IMPLICIT_REF_CAST, "Widget@ f()", asFUNCTION((refCast<Button,Widget>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );

	r = engine->RegisterObjectBehaviour("Widget", asBEHAVE_REF_CAST, "Label@ f()", asFUNCTION((refCast<Widget,Label>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Label", asBEHAVE_IMPLICIT_REF_CAST, "Widget@ f()", asFUNCTION((refCast<Label,Widget>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );

	r = engine->RegisterObjectBehaviour("Widget", asBEHAVE_REF_CAST, "ListBox@ f()", asFUNCTION((refCast<Widget,ListBox>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ListBox", asBEHAVE_IMPLICIT_REF_CAST, "Widget@ f()", asFUNCTION((refCast<ListBox,Widget>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );

	r = engine->RegisterObjectBehaviour("Widget", asBEHAVE_REF_CAST, "ComboBox@ f()", asFUNCTION((refCast<Widget,ComboBox>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_IMPLICIT_REF_CAST, "Widget@ f()", asFUNCTION((refCast<ComboBox,Widget>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );
	
	r = engine->RegisterObjectBehaviour("Widget", asBEHAVE_REF_CAST, "TextBox@ f()", asFUNCTION((refCast<Widget,TextBox>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("TextBox", asBEHAVE_IMPLICIT_REF_CAST, "Widget@ f()", asFUNCTION((refCast<TextBox,Widget>)), asCALL_CDECL_OBJLAST); SDL_assert( r >= 0 );

	//Physics Related

	RegisterBody();

	///sect:PHYSIC
	///glob:void PHY_ShowDebugDraw(bool value)
	r = engine->RegisterGlobalFunction("void PHY_ShowDebugDraw(bool _value)", asMETHOD(PhysicsManager,ShowDebugDraw), asCALL_THISCALL_ASGLOBAL, g_app->physicsManager);SDL_assert(r>0);
	///glob:void PHY_SetGravity(float x,float y)
	r = engine->RegisterGlobalFunction("void PHY_SetGravity(float _x,float _y)", asMETHOD(PhysicsManager,SetGravity), asCALL_THISCALL_ASGLOBAL, g_app->physicsManager);SDL_assert(r>0);
	r = engine->RegisterGlobalFunction("Body @ PHY_GetContactA()", asMETHOD(PhysicsManager,GetContactA), asCALL_THISCALL_ASGLOBAL, g_app->physicsManager);SDL_assert(r>0);
	r = engine->RegisterGlobalFunction("Body @ PHY_GetContactB()", asMETHOD(PhysicsManager,GetContactB), asCALL_THISCALL_ASGLOBAL, g_app->physicsManager);SDL_assert(r>0);

	RegisterEmitter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ScriptManager::Shutdown()
{
	// Free Functions
	int nbFunctions = (int) functions.size();
	for(int n = 0; n < nbFunctions; n++ )
		{
		delete functions[n];
		}
	
	functions.clear();

	// Free Contexts
	int nbContexts = (int) contexts.size();
	for(int n = 0; n < nbContexts; n++ )
		{
		contexts[n]->Release();
		}
	
	contexts.clear();
	engine->Release();

	if( mydbg )
		{
		delete mydbg;
		mydbg = NULL;
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


int ScriptManager::RunCallback(asIScriptFunction * _callback,CScriptHandle *_sender,CScriptHandle *_userData)
{


	int r;
	int returnValue;

	asIScriptContext* tempCtx = this->NewContext();

	if( g_app->settings_allowdebug != 0 )
		{

		SDL_assert(mydbg);

		// Allow the user to initialize the debugging before moving on
		mydbg->TakeCommands(tempCtx);

		// Set the line callback for the debugging
		tempCtx->SetLineCallback(asMETHOD(CDebug, LineCallback), mydbg, asCALL_THISCALL);

		}



	r = tempCtx->Prepare(_callback);
	if (r<0)
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Couldn't Prepare callback: %s\n",SDL_GetError());
	r = tempCtx->SetArgObject(0,_sender); SDL_assert( r >= 0 );
	r = tempCtx->SetArgObject(1,_userData); SDL_assert( r >= 0 );
	r = tempCtx->Execute();
	if( r != asEXECUTION_FINISHED )
		{
		// The execution didn't complete as expected. Determine what happened.
		if( r == asEXECUTION_EXCEPTION )
			{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			SDL_Log("An exception '%s' occurred. Please correct the code and try again.\n", tempCtx->GetExceptionString());
			}
		}
	returnValue = tempCtx->GetReturnByte();
	//Test
	//Do not release the context as we have only one ctx->Release();
	this->FreeContext(tempCtx);


	return returnValue;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


FunctionEntry * ScriptManager::RegisterScript(const std::string& _prototype, char * _fmt)
{
	if (module != NULL)
		{
		FunctionEntry *pTempEntry = new FunctionEntry();

		pTempEntry->func = module->GetFunctionByDecl(_prototype.c_str());
		strcpy(pTempEntry->argsFormat,_fmt);

		this->functions.push_back(pTempEntry);

		return pTempEntry;
		}
	else
		{
		SDL_Log("Cannot register script %s because module is NULL \n",_prototype.c_str());
		return NULL;
		}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ScriptManager::RunFunctionEntry(FunctionEntry * _fe, ...)
{



	

	// Create our context, prepare it, and then execute
	// Now ctx is created at init
	asIScriptContext* tempCtx = this->NewContext();


	//Debug try
	if( g_app->settings_allowdebug != 0 )
		{


		if( mydbg )
			{
			// Allow the user to initialize the debugging before moving on
			mydbg->TakeCommands(tempCtx);

			// Set the line callback for the debugging
			tempCtx->SetLineCallback(asMETHOD(CDebug, LineCallback), mydbg, asCALL_THISCALL);
			}

		}

	if ((_fe == NULL) || (_fe->func == NULL))
		return;


	tempCtx->Prepare(_fe->func);

	va_list ap;
	int currentArg = 0;
	char * _fmt = _fe->argsFormat;
	va_start(ap, _fe);
	while (*_fmt)
		{
		switch (*_fmt ++)
			{

			case 'd':
				{
				int tempInt = va_arg (ap, int);
				tempCtx->SetArgDWord(currentArg++, tempInt);
				}
				break;

			case 'L':
				{
				Uint64 tempInt = va_arg (ap, Uint64);
				tempCtx->SetArgQWord(currentArg++, tempInt);
				}
				break;


			case 'f':
				{
				double tempFloat = va_arg (ap, double);
				tempCtx->SetArgDouble(currentArg++, tempFloat);
				}
				break;

			}
		}
	va_end(ap);





	int r = tempCtx->Execute();
	if( r != asEXECUTION_FINISHED )
		{
		// The execution didn't complete as expected. Determine what happened.
		if( r == asEXECUTION_EXCEPTION )
			{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			SDL_Log("An exception '%s' occurred. Please correct the code and try again.\n", tempCtx->GetExceptionString());
			}
		}

	//Test
	//Do not release the context as we have only one ctx->Release();
	this->FreeContext(tempCtx);



}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ScriptManager::RunScript(const std::string& _prototype, char * _fmt, ...)
{

	if (module == NULL)
		return;




	asIScriptFunction *func = module->GetFunctionByDecl(_prototype.c_str());
	if( func == 0 )
		{

		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		//SDL_Log("Scripts must have the function %s . Please add it and try again.\n",_prototype.c_str());
		//SDL_assert(0);
		return;

		}
	
	// Create our context, prepare it, and then execute
	// Now ctx is created at init
	asIScriptContext* tempCtx = this->NewContext();


	//Debug try
	if( g_app->settings_allowdebug != 0 )
		{

		SDL_assert(mydbg != NULL);

		// Allow the user to initialize the debugging before moving on
		mydbg->TakeCommands(tempCtx);

		// Set the line callback for the debugging
		tempCtx->SetLineCallback(asMETHOD(CDebug, LineCallback), mydbg, asCALL_THISCALL);

		}



	tempCtx->Prepare(func);

	va_list ap;
	int currentArg = 0;
	va_start(ap, _fmt);
	while (*_fmt)
		{
		switch (*_fmt ++)
			{
					
			case 'd':
				{
				int tempInt = va_arg (ap, int);
				tempCtx->SetArgDWord(currentArg++, tempInt);
				}
				break;

			case 'L':
				{
				Uint64 tempInt = va_arg (ap, Uint64);
				tempCtx->SetArgQWord(currentArg++, tempInt);
				}
				break;


			case 'f':
				{
				double tempFloat = va_arg (ap, double);
				tempCtx->SetArgDouble(currentArg++, tempFloat);
				}
				break;
			
			}
		}
	va_end(ap);




	int r = tempCtx->Execute();
	if( r != asEXECUTION_FINISHED )
		{
		// The execution didn't complete as expected. Determine what happened.
		if( r == asEXECUTION_EXCEPTION )
			{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			SDL_Log("An exception '%s' occurred. Please correct the code and try again.\n", tempCtx->GetExceptionString());
			}
		}

	//Test
	//Do not release the context as we have only one ctx->Release();
	this->FreeContext(tempCtx);


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ScriptManager::CompileScript(const std::string& _file)
{
	module = engine->GetModule("module", asGM_ALWAYS_CREATE);
	std::string pLoadedScript = LoadTextFile(_file,GAMEDATA|BOTH);
	module->AddScriptSection(_file.c_str(), pLoadedScript.c_str());
	int r = module->Build();
	if( r < 0 )
		{
		SDL_Log("Build() failed\n");
		}
	else
		{
		SDL_Log("%s Built successfully\n",_file.c_str());
		}
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ScriptManager::CompileScriptViaBuilder(const std::string& _file)
{

	std::stringstream s;
	s <<"[COMPILATION_START]"<<endl;
	LogToOutputAndNetwork(s.str());

	CScriptBuilder builder;


	builder.SetIncludeCallback(MyIncludeCallback,this);
	#ifdef TRP_OSX
		builder.DefineWord("OSX");
	#endif

	#ifdef TRP_IOS
		builder.DefineWord("IOS");
	#endif
	#ifdef WIN32
	builder.DefineWord("WIN32");
	#endif
	#ifdef TRP_ANDROID
	builder.DefineWord("ANDROID");
	#endif

	int r;
	r = builder.StartNewModule(engine, "MyModule"); 
	if( r < 0 ) 
		{
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		SDL_Log("Cannot Create My Module\n");
		}
	std::string pLoadedScript = LoadTextFile(_file,GAMEDATA|BOTH);
	builder.AddSectionFromMemory(_file.c_str(),pLoadedScript.c_str(),pLoadedScript.length());
	r = builder.BuildModule();
	if( r < 0 ) 
		{
		std::stringstream s;
		s <<"[COMPILATION_ERROR]"<<endl;
		LogToOutputAndNetwork(s.str());

		module = NULL;
		}
	else
		{
		std::stringstream s;
		s <<"[COMPILATION_OK]"<<endl;
		LogToOutputAndNetwork(s.str());

		module = builder.GetModule();
		}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ScriptManager::MessageCallback(const asSMessageInfo &msg)
{

	if( msg.type == asMSGTYPE_WARNING )
		{
		std::stringstream s;

		s <<"[WARN]:"<<msg.section<<":"<<msg.row<<":"<<msg.col<<":"<<msg.message<<endl;
			//LogToOutputAndNetwork("%s (%d,%d) type : %s msg : %s \n",msg.section,msg.row,msg.col,type.c_str(),msg.message);
		LogToOutputAndNetwork(s.str());
		}
	else if( msg.type == asMSGTYPE_INFORMATION )
		{
			std::stringstream s;

			s <<"[INFO]:"<<msg.section<<":"<<msg.row<<":"<<msg.col<<":"<<msg.message<<endl;

			//LogToOutputAndNetwork("%s (%d,%d) type : %s msg : %s \n",msg.section,msg.row,msg.col,type.c_str(),msg.message);
		LogToOutputAndNetwork(s.str());
		}
	else
		{
		std::stringstream s;

		s <<"[ERR]:"<<msg.section<<":"<<msg.row<<":"<<msg.col<<":"<<msg.message<<endl;


		//LogToOutputAndNetwork("%s (%d,%d) type : %s msg : %s \n",msg.section,msg.row,msg.col,type.c_str(),msg.message);
		LogToOutputAndNetwork(s.str());

#if 0 //defined WIN32 || defined TRP_OSX
		char tempString[256];
		sprintf(tempString,"%s (%d,%d) type : %s msg : %s \n",msg.section,msg.row,msg.col,type.c_str(),msg.message);		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"TRP Error",tempString,NULL);
#endif

#if 0 //defined WIN32 || defined TRP_OSX
		//Launch sublime Text on osx
		std::string cmd;
		std::string tempFile;
		tempFile = ".//" ;
		tempFile += g_app->settings_gamedataURL;
		tempFile += "//";
		tempFile += msg.section;
		sprintf(tempString,"\"%s\" ",g_app->settings_editorURL.c_str()); //,tempFile.c_str(),msg.row,msg.col);
		cmd = tempString;
		sprintf(tempString,g_app->settings_editorArgs.c_str(),tempFile.c_str(),msg.row,msg.col);
		cmd += tempString;
		system(cmd.c_str());
#endif		
		} // End of if err
	
	
	
	if( msg.type == asMSGTYPE_ERROR )
		{
		hasCompileErrors = true;
		}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

// There are many ways in AngelScript to register your classes, this is one of them
template <class class_type>
bool ScriptManager::RegisterClass(const std::string& class_name)
{
	if (engine->RegisterObjectType(class_name.c_str(), sizeof(class_type), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/)<0)
		{
		SDL_Log("Error - engine::register_class(): Can't register class %s \n",class_name.c_str());
		return false;
		}
	return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

// This is how you register a C++ class method in AngelScript
void ScriptManager::RegisterClassMethod(const std::string& class_name, const std::string& function_definition, const asSFuncPtr &funcPointer)
{

	int r = engine->RegisterObjectMethod(class_name.c_str(), function_definition.c_str(), funcPointer, asCALL_THISCALL);
	SDL_assert(r>0);

}
