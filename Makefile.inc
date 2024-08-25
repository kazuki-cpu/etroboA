mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
	Grid.o\
	Odometry.o\

APPL_LIBS += -lm

APPL_DIRS +=\
	$(mkfile_path)Grid\
	$(mkfile_path)Odometry\


INCLUDES += \
	-I$(mkfile_path)Grid\
	-I$(mkfile_path)Odometry\
