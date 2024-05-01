CPPFLAGS = -I include
VPATH = src include

test_BME_280_Simulator: BME_280_Simulator.cpp BME_280_Simulator.h
	g++ test/test_BME_280_Simulator.cpp $< -I include -fno-access-control