
# Installation

T.R.P. n'a pas à proprement parler besoin d'être installé cependant, il vient avec une suite d'outils qui, elle, 
a besoin de l'être;
Ces outils sont optionnels mais T.R.P. a été pensé pour les utiliser donc votre experience sera bien meilleure
si vous le faites.



## ShoeBox


Shoebox is a freeware tool(made with the Adobe Air SDK) for generating sprites sheets.
T.R.P. use it a lot for sprites and animations.

To install it, you must first install the adobe air environment (if you don't already have it of course)
Just go to
[http://get.adobe.com/fr/air/](http://get.adobe.com/fr/air/)
and follow the instructions.

When the air environment is installed , take a version of Shoebox directly on the ShoeBox's website :

[http://renderhjs.net/shoebox/](http://renderhjs.net/shoebox/)


## Tiled

Tiled is a well known tile editor.
If you want to do a tile based game or just instantiate 2d objects with Tiled,
take a version of Tiled directly on their website :

[http://www.mapeditor.org](http://www.mapeditor.org)


## SublimeText T.R.P. Plugin

T.R.P.'s scripts (*.rsc files) can be edited with any text editor but if you use Sublime Text 3 you will benefit of the T.R.P. plugin to ease your life.
This plugin is included in the Tools directory.
It offers snippets and code generators that can enhance your T.R.P. experience.

[http://www.sublimetext.com/3](http://www.sublimetext.com/3)



## Configuration

You can edit the file settings.xml to configure T.R.P.
For example, you can set the initial position of the window, it's scale, the default text editor, etc...

win32 sample : 

	<settings>
	<position x = "50" y = "100"/>
	<size w = "768" h="576"/>
	<editor url="D:\tools\Sublime Text Build 3 3021 x64\sublime_text.exe" args="%s:%d:%d"/>
    <datafolder directory="gamedata-shooter"/>
	<autorestart value="0"/>
	<allowdebug value="1"/>
    <verbose value="0"/>
    <logtofile value="1"/>
    <server ip="192.168.0.1"/>
	</settings>

OSX Sample :

	<settings>
	<position x = "50" y = "100"/>
	<size w = "768" h="576"/>
	<editor url="/usr/local/bin/subl" args="%s:%d:%d"/>
    <datafolder directory="mygamedata"/>
	<autorestart value="1"/>
	<allowdebug value="0"/>    
    <verbose value="0"/>
    <logtofile value="1"/>
    <server ip="192.168.0.1"/>
	</settings>

Here are the different options available : 

* position      : window's position in screen coordinates
* size          : window's size in screen coordinates
* editor        : url of your favorite text editor
* datafolder    : folder name of your gamedata.
* autorestart   : if value != 0 T.R.P. will restart automatically if the gamedata folder changed (add/modify/remove files)
* allowdebug    : if value != 0 you can attach a debugger
* verbose       : Set T.R.P. in verbose mode or not
* logtofile     : Every T.R.P. log is written to a file called trp.log
* server        : IP Address of the T.R.P. server (only if you want to connect to it as a client)

## How to see Debug Logs

### Pro way

If you want to see all logs (included TRP internal logs), the best option is to
see OutputDebugString (and eventually turn on the verbose mode)


* Under Windows

Use the application Dbgview.exe located in the Tools directory

[Download it Here](http://technet.microsoft.com/en-us/sysinternals/bb896647.aspx)

* Under OSX

Use the OSX Console Application located in Utility/Console.

### Easy way

If you only want to see your logs, it's easier to turn on the logtofile option
and use a file logger to see trp.log modifications in real time.
For example you can use the free version of BareTail

[Download it Here](http://www.baremetalsoft.com/baretail/index.php)


or Glogg 


[Download it Here](http://glogg.bonnefon.org/)


