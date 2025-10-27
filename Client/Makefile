
QT_INC = /usr/include/x86_64-linux-gnu/qt5
QT_LIBS = -lQt5Widgets -lQt5Core -lQt5Gui
SFML_LIBS = -lsfml-network -lsfml-system
CXX = g++
CXXFLAGS = -I$(QT_INC) -I$(QT_INC)/QtWidgets -I$(QT_INC)/QtCore -I$(QT_INC)/QtGui -I. -fPIC -std=c++17
SRC = main.cpp MainWindow.cpp PasswordDialog.cpp ClientLogic.cpp MessageFactory.cpp moc_MainWindow.cpp moc_PasswordDialog.cpp
TARGET = client


all: moc $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(QT_LIBS) $(SFML_LIBS)

moc_MainWindow.cpp: MainWindow.h
	moc MainWindow.h -o moc_MainWindow.cpp

moc_PasswordDialog.cpp: PasswordDialog.h
	moc PasswordDialog.h -o moc_PasswordDialog.cpp

moc: moc_MainWindow.cpp moc_PasswordDialog.cpp


clean:
	rm -f $(TARGET) moc_MainWindow.cpp moc_PasswordDialog.cpp
