CC=g++
UNAME_S := $(shell uname -s)

OBJ_DIR=Obj
BIN_DIR=bin
RELEASE_DIR=$(BIN_DIR)/Release
DEBUG_DIR=$(BIN_DIR)/Debug
OBJ_DEBUG_DIR=$(OBJ_DIR)/Debug
OBJ_RELEASE_DIR=$(OBJ_DIR)/Release
PROJECT_DIR=s3ntineL-irc

IRC_CHANNEL?=changemepinga
IRC_CHANNEL_NAME='"\#$(IRC_CHANNEL)"'

# CHANGEMEEEEEEEEE
# PUBLIC_KEY='"-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAv5mynmP9yOJxBVjwsw4Z7V4nPQrKvczED1Mhq2FrlDR0y4JgwYr00oA91G4Y1tERM78VFCIDOcINVO7n0bRTDL24dDmmvtabI0iNveDFKJEr6RnshoKWtmHPsU1nf3chjbDipH1C/PL5aCNRvFQrYrg9RMCskOGGoZEFw4C+xmyCTEsEhT74xBiHrlfstxe/kkmuKAHGL9njtOCxeH/DpS3rFk9Anfyewgs6204jiszNdFpjJXxpkA8qHOpi9A7wfFmn10Lmc9fNUv2kTcVgu5UGIkHqURMDFa7cKD2bPUC5FvztWVB0rJHiw2gAXux/nJDddyqhNxgtvH70sJ4U6wIDAQAB\n-----END PUBLIC KEY-----\n"'

# DYNAMIC_PUBLIC_KEY_FLAG=-DPUB_KEY=$(PUBLIC_KEY)
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
# Explicit object file, because we want to add the dynamic public key
# $(OBJ_DIR)/crypto.o: $(PROJECT_DIR)/src/crypto.c
#					 $(CC) $(DYNAMIC_PUBLIC_KEY_FLAG) $< -c -o $@

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
