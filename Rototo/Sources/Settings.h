#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>

class Settings
{

public :

	std::string		editorURL; //TODO put all settings in a settings class
	std::string		editorArgs;
	std::string		gamedataURL;
	std::string		configURL;
	int				autorestart;
	int				allowdebug;
	int				verbose;
	int				logtofile;
	std::string		serverIP;
	int				winpos_x;
	int				winpos_y;
	int				winsize_w;
	int				winsize_h;

public:
	void Read();
	void Write();
};

#endif
