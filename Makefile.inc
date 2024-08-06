mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
	Tracer.o\
	Odometry.o\
	Grid.o\

APPL_LIBS += -lm

APPL_DIRS +=\
	$(mkfile_path)Tracer\
	$(mkfile_path)Odometry\
	$(mkfile_path)Grid\


INCLUDES += \
	-I$(mkfile_path)Tracer\
	-I$(mkfile_path)Odometry\
	-I$(mkfile_path)Grid\
