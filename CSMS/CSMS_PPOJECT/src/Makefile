CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -Wno-reorder
LDFLAGS =
TARGET = CSMS

SOURCES = Product.cpp Customer.cpp Transaction.cpp InventoryManager.cpp Main.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
