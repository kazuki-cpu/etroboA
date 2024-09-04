mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
	Color.o\
	Calibration.o\
	Tracer.o\
	Odometry.o\
	Grid.o\

APPL_LIBS += -lm

APPL_DIRS +=\
	$(mkfile_path)Color\
	$(mkfile_path)Calibration\
	$(mkfile_path)Tracer\
	$(mkfile_path)Odometry\
	$(mkfile_path)Grid\


INCLUDES += \
	-I$(mkfile_path)Color\
	-I$(mkfile_path)Calibration\
	-I$(mkfile_path)Tracer\
	-I$(mkfile_path)Odometry\
	-I$(mkfile_path)Grid\
