# Declaration of variables
#WIN32 cross compile
CC = i586-mingw32msvc-g++
CC_FLAGS = -w `/usr/local/i586-mingw32msvc/bin/wx-config --cxxflags --libs all` -static-libgcc -static-libstdc++
EXEC = avrrfconfigurator.exe
#LINUX
#~ CC = g++
#~ CC_FLAGS = -w `wx-config --cxxflags --libs all`
#~ EXEC = avrrfconfigurator.a

# Sources and Objects
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(CC_FLAGS) -o $(EXEC)

# To obtain object files
%.o: %.cpp %.h
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

install:
	@echo You must be root to install
