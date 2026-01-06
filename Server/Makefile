CC := g++
SRCS := AccountManager.cpp MailManager.cpp socketfunctions.cpp TaskManager.cpp utils.cpp scheduler.cpp executor.cpp server.cpp main.cpp
OBJS := $(patsubst %.cpp,%.o, $(SRCS))
TARGET := task_scheduler
CXXFLAGS := -Wall -std=c++17 -fPIC

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lsfml-network -lsfml-system -ltinyxml2 -lpthread

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.so $(TARGET) *.pid

