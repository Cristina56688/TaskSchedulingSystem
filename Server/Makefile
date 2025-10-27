# CC := g++
# SRCS := add_mac.cpp
# OBJS := $(patsubst %.cpp,%.o, $(SRCS))

# TARGET := server
 
# server: server.cpp 
# 	$(CC) server.cpp -o server -lsfml-network -lsfml-system -lscheduling

# libscheduling.so: $(OBJS)
# 	$(CC) -fPIC -shared -o libscheduling.so $(OBJS)

# %.o: %.cpp 
# 	$(CC) -c $< -o $@

# clean:
# 	rm *.o *.so server

# Compilatorul
CC := g++

# Fișiere sursă pentru biblioteca
SRCS := add_mac.cpp

# Fișiere obiect
OBJS := $(patsubst %.cpp,%.o, $(SRCS))

# Executabilul final
TARGET := server

# Biblioteca dinamică
LIB := libscheduling.so

# Opțiuni de compilare
CXXFLAGS := -Wall -std=c++17 -fPIC
LDFLAGS := -lscheduling -L .

# Regulă implicită
all: $(TARGET)

# Construiește serverul și linkează cu biblioteca
$(TARGET): server.cpp $(LIB)
	$(CC) server.cpp -o $(TARGET) -lsfml-network -lsfml-system -ltinyxml2 $(LDFLAGS)

# Construiește biblioteca dinamică
$(LIB): $(OBJS)
	$(CC) -shared -o $(LIB) $(OBJS) -ltinyxml2 -L .

# Regulă pentru fișiere obiect
%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o *.so $(TARGET)


