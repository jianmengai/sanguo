LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame



LOCAL_SRC_FILES := $(call all-subdir-cpp-files)

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/utils/GameUtils.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/Army.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/AutoFindPath.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/Building.cpp\
				   $(LOCAL_PATH)/../../../Classes/game/BulletManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/GameBattle.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/GameConfig.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/GameObject.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/GameObjectManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/GameScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/GameUILayer.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/MapManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/Soldier.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/SoundManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/SpecialEffectManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/TeamManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/TextureSprite.cpp \
				   $(LOCAL_PATH)/../../../Classes/game/WarFogLayer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
					$(LOCAL_PATH)/../../../Classes/utils\
					$(LOCAL_PATH)/../../../Classes/game

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
