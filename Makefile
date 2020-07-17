ifndef PROJECT_DIR
    PROJECT_DIR = $(PANDORA_DIR)/ExampleContent
    PROJECT_LIBRARY_DIR = $(PANDORA_DIR)/lib
else
    PROJECT_LIBRARY_DIR = $(PROJECT_DIR)/lib
endif

CC = g++
CFLAGS = -c -g -fPIC -O2 -Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing -std=c++11
ifdef BUILD_32BIT_COMPATIBLE
    CFLAGS += -m32
endif

LIBS = -L$(PANDORA_DIR)/lib -lPandoraSDK
ifdef MONITORING
    LIBS += $(shell root-config --glibs --evelibs)
    LIBS += -lPandoraMonitoring
endif
ifdef BUILD_32BIT_COMPATIBLE
    LIBS += -m32
endif

PROJECT_INCLUDE_DIR = $(PROJECT_DIR)/include/
PROJECT_LIBRARY = $(PROJECT_LIBRARY_DIR)/libExampleContent.so
PROJECT_BINARY = $(PROJECT_DIR)/bin/PandoraExample

INCLUDES  = -I$(PROJECT_INCLUDE_DIR)
INCLUDES += -I$(PANDORA_DIR)/PandoraSDK/include
ifdef MONITORING
    INCLUDES += -I $(shell root-config --incdir)
    INCLUDES += -I$(PANDORA_DIR)/PandoraMonitoring/include
endif

ifdef MONITORING
    DEFINES = -DMONITORING=1
endif

SOURCES  = $(wildcard $(PROJECT_DIR)/src/*.cc)
SOURCES += $(wildcard $(PROJECT_DIR)/src/ExampleAlgorithms/*.cc)
SOURCES += $(wildcard $(PROJECT_DIR)/src/ExampleAlgorithmTools/*.cc)
SOURCES += $(wildcard $(PROJECT_DIR)/src/ExampleHelpers/*.cc)
SOURCES += $(wildcard $(PROJECT_DIR)/src/ExamplePlugins/*.cc)
OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = $(OBJECTS:.o=.d)

TEST_SOURCES = $(wildcard $(PROJECT_DIR)/test/*.cc)
TEST_OBJECTS = $(TEST_SOURCES:.cc=.o)
TEST_DEPENDS = $(TEST_OBJECTS:.o=.d)

all: library binary

library: $(SOURCES) $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -shared -o $(PROJECT_LIBRARY)

binary: $(TEST_SOURCES) $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(LIBS) -L$(PROJECT_LIBRARY_DIR) -lExampleContent -o $(PROJECT_BINARY)

-include $(DEPENDS)
-include $(TEST_DEPENDS)

%.o:%.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -MP -MMD -MT $*.o -MT $*.d -MF $*.d -o $*.o $*.cc

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	rm -f $(PROJECT_LIBRARY)
	rm -f $(TEST_OBJECTS)
	rm -f $(TEST_DEPENDS)
	rm -f $(PROJECT_BINARY)

install:
ifdef INCLUDE_TARGET
	rsync -r --include '*/' --include '*.h' --exclude '*' --prune-empty-dirs $(PROJECT_DIR)/ ${INCLUDE_TARGET}
endif
ifdef LIB_TARGET
	cp $(PROJECT_LIBRARY) ${LIB_TARGET}
endif
