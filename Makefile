#TODO(axp) 随便找了个，之后再改
#Modified By JHX (2018-04-30 8:26)
ROOTDIR = .

cc = g++

EXE_DIR = ./bin
CFLAGS = -g -Wall -std=c++11
LFLAGS = 

objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
executables := $(patsubst %.cpp,%,$(wildcard *.cpp))

all : $(objects)
$(objects) :%.o : %.cpp
	@mkdir -p ./bin
	$(cc) -c $(CFLAGS) $< -o $@
	$(cc) $(CFLAGS) $< -o $(subst .o, ,$(EXE_DIR)/$@) $(LFLAGS) $(LIBS)
test:
	@mkdir -p ./log
	@./bin/vector_demo > ./log/vector_demo.log
clean:
	@rm -f *.o
	@rm -f $(executables)
	@rm -r ./bin ./log
distclean: clean
