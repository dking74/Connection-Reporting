PROG = citrix
CC = g++
FLAGS = -std=c++11 -Wall -g

LIBS = -lcurl -lxlsxwriter -lquickmail
LIB_PATH = -L/usr/local/lib
INCLUDE_PATH = -I/usr/local/include/rapidjson/include -I/usr/local/include -I./include -I/usr/local/include/xlsxwriter

OBJECTS = objects/main.o     \
	  objects/timing.o   \
          objects/requests.o \
          objects/daemon.o   \
          objects/internal.o \
          objects/excel.o    \
          objects/mail.o

$(PROG): $(OBJECTS)
	@mkdir -p bin
	$(CC) -o bin/$@ $? $(FLAGS) $(INCLUDE_PATH) $(LIB_PATH) $(LIBS)

objects/%.o: src/%.cpp
	@mkdir -p objects
	$(CC) -o $@ -c $? $(FLAGS) $(INCLUDE_PATH)

clean:
	rm -rf bin/ objects/ *.o citrix Last_Month_Connections.xlsx
