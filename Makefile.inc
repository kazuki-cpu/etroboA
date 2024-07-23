 mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

 APPL_CXXOBJS += \
 	util.o \
	MathFnc.o\
  	Walker.o\
	ServoCtrl.o\
	Log.o\
	ColorMonit.o\
	ArmCtrl.o\
	DistanceMonit.o\


SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile
# include += -I$(ETROBO_HRP3_WORKSPACE)/KenkiRobo2023A

endif

 APPL_DIRS += $(mkfile_path)app $(mkfile_path)aplmid $(mkfile_path)lib

 INCLUDES += \
 	-I$(mkfile_path)app \
	-I$(mkfile_path)aplmid \
 	-I$(mkfile_path)lib \


# COPTS += -fno-use-cxa-atexit
