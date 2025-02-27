CXX = clang++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/opt/sfml@2/include
LDFLAGS = -L/opt/homebrew/opt/sfml@2/lib
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp Game.cpp Champ.cpp Bullet.cpp Enemy.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

cleanObjs:
	rm -f $(OBJS)

re: clean all

