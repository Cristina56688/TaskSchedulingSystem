CXX = g++
CXXFLAGS = -std=c++17 -Wall -fPIC
QTFLAGS = `pkg-config --cflags --libs Qt5Widgets Qt5Core Qt5Gui`
SFML_LIBS = -lsfml-network -lsfml-system

SRC = client.cpp MainWindow.cpp serialize.cpp moc_MainWindow.cpp
TARGET = client
UI_HEADER = ui_MainWindow.h

all: $(TARGET)

$(UI_HEADER): MainWindow.ui
	uic MainWindow.ui -o $(UI_HEADER)

moc_MainWindow.cpp: MainWindow.h
	moc MainWindow.h -o moc_MainWindow.cpp

$(TARGET): $(SRC) $(UI_HEADER)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(QTFLAGS) $(SFML_LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) moc_MainWindow.cpp ui_MainWindow.h
	rm -f *.o

.PHONY: all clean run
