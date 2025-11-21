CC := g++
SRCS := AccountManager.cpp MailManager.cpp socketfunctions.cpp TaskManager.cpp utils.cpp
OBJS := $(patsubst %.cpp,%.o, $(SRCS))
TARGET := server scheduler
LIB := libscheduling.so
CXXFLAGS := -Wall -std=c++17 -fPIC
LDFLAGS := -lscheduling -L .


all: $(TARGET)

server: server.cpp $(LIB)
	$(CC) server.cpp -o server -lsfml-network -lsfml-system -ltinyxml2 $(LDFLAGS)

$(LIB): $(OBJS)
	$(CC) -shared -o $(LIB) $(OBJS) -ltinyxml2 -L .

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

scheduler: scheduler.cpp $(LIB)
	$(CC) scheduler.cpp -o scheduler $(LDFLAGS) -lsfml-network -lsfml-system -ltinyxml2 

clean:
	rm *.o *.so $(TARGET) *.pid


