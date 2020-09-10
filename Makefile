CC=g++

OBJ_DIR=Obj
BIN_DIR=bin
RELEASE_DIR=$(BIN_DIR)/Release
DEBUG_DIR=$(BIN_DIR)/Debug
PROJECT_DIR=s3ntineL-irc

# CHANGEMEEEEEEEEE
# PUBLIC_KEY='"-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAv5mynmP9yOJxBVjwsw4Z7V4nPQrKvczED1Mhq2FrlDR0y4JgwYr00oA91G4Y1tERM78VFCIDOcINVO7n0bRTDL24dDmmvtabI0iNveDFKJEr6RnshoKWtmHPsU1nf3chjbDipH1C/PL5aCNRvFQrYrg9RMCskOGGoZEFw4C+xmyCTEsEhT74xBiHrlfstxe/kkmuKAHGL9njtOCxeH/DpS3rFk9Anfyewgs6204jiszNdFpjJXxpkA8qHOpi9A7wfFmn10Lmc9fNUv2kTcVgu5UGIkHqURMDFa7cKD2bPUC5FvztWVB0rJHiw2gAXux/nJDddyqhNxgtvH70sJ4U6wIDAQAB\n-----END PUBLIC KEY-----\n"'

# DYNAMIC_PUBLIC_KEY_FLAG=-DPUB_KEY=$(PUBLIC_KEY)
STATIC_COMPILE_CFLAGS=-static -static-libgcc
# Optimizations:
#	-O3: 3Â° optimization (faster run)
#	-pipe: Use pipes rather than temporary files for communication between the various stages of compilation. (faster compilation.)
#	-fno-math-errno: Do not check the errno after calling math functions
RELEASE_CFLAGS=-O3 -pipe -fno-math-errno
DEBUG_CFLAGS=-Wall -gdwarf-2 -g -O0 -DDEBUG

OBJ = $(OBJ_DIR)/s3ntineL-irc.o $(OBJ_DIR)/irc.o $(OBJ_DIR)/net.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/command.o

# Create directories if not exists
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(RELEASE_DIR))
$(shell mkdir -p $(DEBUG_DIR))

all: $(DEBUG_DIR)/s3ntineL-irc $(RELEASE_DIR)/s3ntineL-irc

$(RELEASE_DIR)/s3ntineL-irc: $(OBJ)
			     $(CC) $(RELEASE_CFLAGS) $^ -o $@

$(DEBUG_DIR)/s3ntineL-irc: $(OBJ)
			   $(CC) $^ $(DEBUG_CFLAGS) -o $@

# Explicit object file, because all content is in src
$(OBJ_DIR)/s3ntineL-irc.o: $(PROJECT_DIR)/s3ntineL-irc.cpp
			   $(CC) $< -c -o $@

# Explicit object file, because we want to add the dynamic public key
# $(OBJ_DIR)/crypto.o: $(PROJECT_DIR)/src/crypto.c
#					 $(CC) $(DYNAMIC_PUBLIC_KEY_FLAG) $< -c -o $@

# General way to map object files with src/ ones
$(OBJ_DIR)/%.o: $(PROJECT_DIR)/%.cpp
		$(CC) $< -c -o $@


.PHONY: clean
clean:
	rm -f $(OBJ) $(RELEASE_DIR)/s3ntineL-irc-unix $(DEBUG_DIR)/s3ntineL-irc-unix
	rm -rf $(RELEASE_DIR) $(DEBUG_DIR) $(BIN_DIR) $(OBJ_DIR)

