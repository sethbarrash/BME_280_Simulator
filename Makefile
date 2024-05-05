CPPFLAGS = -I include
VPATH = src include

test_simulator_internal: BME_280_Simulator.cpp BME_280_Simulator.h
	g++ test/test_BME_280_Simulator_internal.cpp $< -I include -fno-access-control