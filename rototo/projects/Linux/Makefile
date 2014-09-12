#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

# define the C compiler to use
CC = gcc
CXX= g++

# define any compile-time flags
CFLAGS = -Wall -g -DTRP_LINUX
CXXFLAGS = -Wall -g -DTRP_LINUX

# define any directories containing header files other than /usr/include
#
#INCLUDES = -I/home/newhall/include  -I../include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =  -L../../extern/SDL2-2.0.3/build/.libs -L../../extern/SDL2_mixer-2.0.0/build/.libs \
	-L../../extern/SDL2_ttf-2.0.12/.libs -L../../extern/SDL2_image-2.0.0/.libs -L../../extern/SDL2_net-2.0.0/.libs \
	-L ../../extern/AngelScript-2.28.2/sdk/angelscript/lib/ -L../../extern/Box2D-2.3.1/Build/gmake/bin/Debug \
	-L../../extern/SDL2_gfx-1.0.0/.libs -Wl,-rpath=./libs

LFLAGS2 = -Wl,-R,'$$ORIGIN'


# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS =  -lSDL2 -lSDL2_mixer -lSDL2_net -lSDL2_ttf -lSDL2_image -lSDL2_gfx -langelscript -lBox2D -lpthread 


SDL_PATH := ../../extern/SDL2-2.0.3
SDL_MIXER_PATH := ../../extern/SDL2_mixer-2.0.0
SDL_IMAGE_PATH := ../../extern/SDL2_image-2.0.0
SDL_TTF_PATH := ../../extern/SDL2_ttf-2.0.12
SDL_NET_PATH := ../../extern/SDL2_net-2.0.0
SDL_GFX_PATH := ../../extern/SDL2_gfx-1.0.0
ANGELSCRIPT_PATH := ../../extern/AngelScript-2.28.2/sdk/angelscript/include
ANGELSCRIPT_ADDON_PATH := ../../extern/AngelScript-2.28.2/sdk/add_on
BOX2D_PATH := ../../extern/Box2D-2.3.1


ROTOTO_PATH := ../../src
TINYXML_PATH := ../../extern/tinyxml2



INCLUDES := -I$(SDL_PATH)/include
INCLUDES += -I$(SDL_MIXER_PATH)
INCLUDES += -I$(SDL_IMAGE_PATH)
INCLUDES += -I$(SDL_TTF_PATH)
INCLUDES += -I$(SDL_NET_PATH)
INCLUDES += -I$(SDL_GFX_PATH)
INCLUDES += -I$(ANGELSCRIPT_PATH)
INCLUDES += -I$(ANGELSCRIPT_ADDON_PATH)
INCLUDES += -I$(TINYXML_PATH)
INCLUDES += -I$(BOX2D_PATH)
INCLUDES += -I$(ROTOTO_PATH)


# define the C source files
SRCS = $(ROTOTO_PATH)/Main.cpp \
	$(ROTOTO_PATH)/Application.cpp \
	$(ROTOTO_PATH)/Animation.cpp\
	$(ROTOTO_PATH)/Atlas.cpp\
	$(ROTOTO_PATH)/Client.cpp\
	$(ROTOTO_PATH)/NetworkManager.cpp\
	$(ROTOTO_PATH)/ResourceManager.cpp\
	$(ROTOTO_PATH)/ScriptManager.cpp\
	$(ROTOTO_PATH)/PhysicsManager.cpp\
	$(ROTOTO_PATH)/Server.cpp\
	$(ROTOTO_PATH)/SoundManager.cpp\
	$(ROTOTO_PATH)/Sprite.cpp\
	$(ROTOTO_PATH)/GUIManager.cpp\
	$(ROTOTO_PATH)/TiledMap.cpp\
	$(ROTOTO_PATH)/TextManager.cpp\
	$(ROTOTO_PATH)/TweenManager2.cpp\
	$(ROTOTO_PATH)/Utils.cpp\
	$(ROTOTO_PATH)/WatchManager.cpp\
	$(ROTOTO_PATH)/Font.cpp\
	$(ROTOTO_PATH)/Label.cpp\
	$(ROTOTO_PATH)/Button.cpp\
	$(ROTOTO_PATH)/TextBox.cpp\
	$(ROTOTO_PATH)/Primitive.cpp\
	$(ROTOTO_PATH)/Particles.cpp\
	$(ROTOTO_PATH)/Widget.cpp\
	$(ROTOTO_PATH)/Vector2D.cpp\
	$(ROTOTO_PATH)/Debug.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptstdstring/scriptstdstring.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scripthandle/scripthandle.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptbuilder/scriptbuilder.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptmath/scriptmath.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptdictionary/scriptdictionary.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptarray/scriptarray.cpp\
	$(ROTOTO_PATH)/ListBox.cpp\
	$(TINYXML_PATH)/tinyxml2.cpp


# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)
OBJS = $(SRCS:.cpp=.o)
# define the executable file 
MAIN = rototo

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
		@echo  ROTOTO has been compiled

$(MAIN): $(OBJS) 
		@echo "Linking ..."
		$(SILENT) $(CXX) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
		@echo $(notdir $<)
		$(SILENT) $(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

.cpp.o:
		@echo $(notdir $<)
		$(SILENT) $(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		@echo "Cleaning ..."
		$(SILENT) $(RM) $(OBJS) *~ $(MAIN)


depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
