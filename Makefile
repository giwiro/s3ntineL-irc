CC=g++
UNAME_S := $(shell uname -s)

OBJ_DIR=Obj
BIN_DIR=bin
RELEASE_DIR=$(BIN_DIR)/Release
DEBUG_DIR=$(BIN_DIR)/Debug
OBJ_DEBUG_DIR=$(OBJ_DIR)/Debug
OBJ_RELEASE_DIR=$(OBJ_DIR)/Release
PROJECT_DIR=s3ntineL-irc

IRC_CHANNEL?=changeme1234
IRC_CHANNEL_NAME='"$(IRC_CHANNEL)"'

STATIC_COMPILE_CFLAGS=-static -static-libgcc
# Optimizations:
#	-O3: 3Â° optimization (faster run)
#	-pipe: Use pipes rather than temporary files for communication between the various stages of compilation. (faster compilation.)
#	-fno-math-errno: Do not check the errno after calling math functions
COMMON_CFLAGS=-DIRC_CHANNEL=$(IRC_CHANNEL_NAME)
RELEASE_CFLAGS=-O3 -pipe -fno-math-errno
DEBUG_CFLAGS=-Wall -gdwarf-2 -g -O0 -DDEBUG

OBJ_DEBUG = $(OBJ_DEBUG_DIR)/s3ntineL-irc.o $(OBJ_DEBUG_DIR)/irc.o $(OBJ_DEBUG_DIR)/net.o $(OBJ_DEBUG_DIR)/logger.o $(OBJ_DEBUG_DIR)/command.o
OBJ_RELEASE = $(OBJ_RELEASE_DIR)/s3ntineL-irc.o $(OBJ_RELEASE_DIR)/irc.o $(OBJ_RELEASE_DIR)/net.o $(OBJ_RELEASE_DIR)/logger.o $(OBJ_RELEASE_DIR)/command.o

all: clean create_folders $(DEBUG_DIR)/s3ntineL-irc $(RELEASE_DIR)/s3ntineL-irc

$(RELEASE_DIR)/s3ntineL-irc: $(OBJ_RELEASE)
			$(CC) $^ $(COMMON_CFLAGS) $(RELEASE_CFLAGS) -o $@
ifeq ($(UNAME_S),Linux)
			strip -s $@ -o $@
endif
ifeq ($(UNAME_S),Darwin)
			strip -S -x $@ -o $@
endif

$(DEBUG_DIR)/s3ntineL-irc: $(OBJ_DEBUG)
			$(CC) $^ $(COMMON_CFLAGS) $(DEBUG_CFLAGS) -o $@

# Explicit object file, because all content is in src
$(OBJ_DEBUG_DIR)/s3ntineL-irc.o: $(PROJECT_DIR)/s3ntineL-irc.cpp
			$(CC) $< $(COMMON_CFLAGS) $(DEBUG_CFLAGS) -c -o $@

$(OBJ_RELEASE_DIR)/s3ntineL-irc.o: $(PROJECT_DIR)/s3ntineL-irc.cpp
			$(CC) $< $(COMMON_CFLAGS) $(RELEASE_CFLAGS) -c -o $@

# General way to map object files with src/ ones
$(OBJ_DEBUG_DIR)/%.o: $(PROJECT_DIR)/%.cpp
		$(CC) $< $(COMMON_CFLAGS) $(DEBUG_CFLAGS) -c -o $@

$(OBJ_RELEASE_DIR)/%.o: $(PROJECT_DIR)/%.cpp
		$(CC) $< $(COMMON_CFLAGS) $(RELEASE_CFLAGS) -c -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(RELEASE_DIR)/s3ntineL-irc $(DEBUG_DIR)/s3ntineL-irc
	rm -rf $(RELEASE_DIR) $(DEBUG_DIR) $(BIN_DIR) $(OBJ_DIR) $(OBJ_DEBUG_DIR) $(OBJ_RELEASE_DIR)


.PHONY: create_folders
create_folders:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(DEBUG_DIR)
	mkdir -p $(OBJ_DEBUG_DIR)
	mkdir -p $(OBJ_RELEASE_DIR)
