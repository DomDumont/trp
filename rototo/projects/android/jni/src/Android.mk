LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2-2.0.3
SDL_MIXER_PATH := ../SDL2_mixer-2.0.0
SDL_IMAGE_PATH := ../SDL2_image
SDL_TTF_PATH := ../SDL2_ttf
SDL_NET_PATH := ../SDL2_net-2.0.0
SDL_GFX_PATH := ../SDL2_gfx-1.0.0
ANGELSCRIPT_PATH := ../AngelScript-2.29.0/sdk/angelscript/include
ANGELSCRIPT_ADDON_PATH := ../AngelScript-2.29.0/sdk/add_on
TINYXML_PATH := ../tinyxml2
ROTOTO_PATH := ../../../../src
BOX2D_PATH := ../Box2D-2.3.1

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_MIXER_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_IMAGE_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_TTF_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_NET_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_GFX_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ANGELSCRIPT_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ANGELSCRIPT_ADDON_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(TINYXML_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(BOX2D_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ROTOTO_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(ROTOTO_PATH)/Main.cpp \
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
	$(ROTOTO_PATH)/TWeenManager.cpp\
	$(ROTOTO_PATH)/Utils.cpp\
	$(ROTOTO_PATH)/WatchManager.cpp\
	$(ROTOTO_PATH)/Font.cpp\
	$(ROTOTO_PATH)/Label.cpp\
	$(ROTOTO_PATH)/Button.cpp\
	$(ROTOTO_PATH)/TextBox.cpp\
	$(ROTOTO_PATH)/Primitive.cpp\
	$(ROTOTO_PATH)/Widget.cpp\
	$(ROTOTO_PATH)/Vector2D.cpp\
	$(ROTOTO_PATH)/Debug.cpp\
	$(ROTOTO_PATH)/Particles.cpp\
	$(SDL_GFX_PATH)/SDL2_gfxPrimitives.c\
	$(SDL_GFX_PATH)/SDL2_rotozoom.c\
	$(ANGELSCRIPT_ADDON_PATH)/scriptstdstring/scriptstdstring.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scripthandle/scripthandle.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptbuilder/scriptbuilder.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptmath/scriptmath.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptdictionary/scriptdictionary.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/scriptarray/scriptarray.cpp\
	$(ANGELSCRIPT_ADDON_PATH)/debugger/debugger.cpp\
	$(ROTOTO_PATH)/ListBox.cpp\
	$(ROTOTO_PATH)/ComboBox.cpp\
	$(TINYXML_PATH)/tinyxml2.cpp

LOCAL_CFLAGS += -DTRP_ANDROID
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_net SDL2_image SDL2_mixer SDL2_ttf
LOCAL_STATIC_LIBRARIES  := angelscript box2d
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
