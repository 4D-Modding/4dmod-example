TARGET_NAME = 4DMod

CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -std=c++20 -s -O2 -I./4dm.h -I./4dm.h/networking/include -march=x86-64 -municode -m64 -mwindows -fpermissive -w

NETLIBS=$(wildcard 4dm.h/networking/lib/*.lib)
NETLIBS:=$(filter-out %steamwebrtc.lib,$(NETLIBS))
NETLIBS:=$(filter-out %webrtc-lite.lib,$(NETLIBS))

LDFLAGS = -shared -L. $(patsubst %.lib,-l%,$(NETLIBS)) -lopengl32 -L./4dm.h/ -Wl,-Bdynamic -lglew32 -lglfw3

TARGET = "out/$(TARGET_NAME).dll"

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

default: compile

compile: $(OBJS) out
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(TARGET) $(OBJS)

out:
	mkdir out
