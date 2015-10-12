/*
==============================================================================

This file is part of the T.R.P. Engine
Copyright (c) 2015 - Dominique Dumont

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
#ifndef __APPLICATION_P_H__
#define __APPLICATION_P_H__

#include "ScriptManager.h"
#include "TweenManager.h"

class Application_p
{
#ifdef TRP_USE_BINDING
	FunctionEntry		*on_init_func;
	FunctionEntry		*on_update_func;
	FunctionEntry		*on_render_func;
#endif

public:
	Application_p();
	void Init();
	void Update(unsigned int elapsed);

};

#endif