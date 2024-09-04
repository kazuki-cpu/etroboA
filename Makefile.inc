mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
	Color.o\
	Calibration.o\
	LineTracer.o\
	Odometry.o\
	Manager.o\
	Pid.o\
	Motor.o\


APPL_LIBS += -lm

APPL_DIRS +=\
	$(mkfile_path)Color\
	$(mkfile_path)Calibration\
	$(mkfile_path)LineTracer\
	$(mkfile_path)Odometry\
	$(mkfile_path)Manager\
	$(mkfile_path)Pid\
	$(mkfile_path)Motor\

	

INCLUDES += \
	-I$(mkfile_path)Color\
	-I$(mkfile_path)Calibration\
	-I$(mkfile_path)LineTracer\
	-I$(mkfile_path)Odometry\
	-I$(mkfile_path)Manager\
	-I$(mkfile_path)Pid\
	-I$(mkfile_path)Motor\

