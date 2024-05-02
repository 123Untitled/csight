
# -- S E T T I N G S ----------------------------------------------------------

# set default target
.DEFAULT_GOAL := all

# silent mode
.SILENT:

# use one shell for all commands
.ONESHELL:

# delete intermediate files on error
.DELETE_ON_ERROR:

# set shell program
override SHELL := $(shell which zsh)

# shell flags
.SHELLFLAGS := -d -f -c -e -o pipefail -u


# -- O P E R A T I N G  S Y S T E M -------------------------------------------

# Detect operating system
override OS := $(shell uname -s)

# check for macosx
ifeq ($(OS), Darwin)
override THREAD   := $(shell sysctl -n hw.ncpu)
# check for linux
else ifeq ($(OS), Linux)
override THREAD   := $(shell nproc)
# unsupported os
else
$(error $(shell echo "unsupported os: \x1b[31m$(OS)\x1b[0m"))
endif


# set make flags
override MAKEFLAGS += --warn-undefined-variables --no-builtin-rules \
					  --jobs=$(THREAD) --output-sync=target \
					  --no-print-directory



# -- T A R G E T S ------------------------------------------------------------

# project name
override PROJECT := compile_sight

# main executable
override EXEC := exec_$(PROJECT)

# compile commands database
override COMPILE_DB := compile_commands.json


# -- D I R E C T O R I E S ----------------------------------------------------

# root directory
override ROOT_DIR := $(shell pwd)

# source directory
override SRC_DIR := sources

# include directory
override INC_DIR := includes


# -- S O U R C E S ------------------------------------------------------------

# source files
override SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')

# number of source files
override NSRCS := $(words $(SRCS))

# object files
override OBJS := $(SRCS:%.cpp=%.o)

# dependency files
override DEPS := $(OBJS:%.o=%.d)


# -- T O O L S ----------------------------------------------------------------

# make directory if not exists
override MKDIR := mkdir -p

# remove recursively force
override RM := rm -rvf


# -- C O M P I L E R ----------------------------------------------------------

# compiler
override CXX := $(shell which clang++)

# compiler standard
override STD := -std=c++2a

# compiler optimization
override OPT := -O0

# debug level
override DBG := -g3

# compiler flags
override FLAGS := -Wall -Wextra -Werror -Wpedantic -Weffc++ -Winline \
				  -Wno-unused -Wno-unused-variable -Wno-unused-parameter \
				  -Wconversion -Wsign-conversion -Wfloat-conversion -Wnarrowing \
				  -Wshadow \
				  -fno-rtti

# cxx flags
override CXXFLAGS := $(STD) $(DBG) $(OPT) $(FLAGS) -I$(INC_DIR)

# linker flags
override LDFLAGS ?=

# dependency flags
override DEPFLAGS = -MT $@ -MMD -MP -MF $*.d


# -- P H O N Y ----------------------------------------------------------------

# phony targets
.PHONY: all clean fclean re rm

# not parallel
.NOTPARALLEL: $(COMPILE_DB)


# -- T A R G E T S ------------------------------------------------------------

all: $(EXEC) $(COMPILE_DB)
	echo '[done]'

$(EXEC): $(OBJS)
	echo '\033[32m>\033[0m    linking' $@
	$(CXX) $^ -o $@ $(LDFLAGS)

-include $(DEPS)
%.o: %.cpp Makefile
	echo '\033[33m>\033[0m  compiling' $<
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(COMPILE_DB): $(SRCS) Makefile
	echo '\033[34m>\033[0m generating' $@
	$(call GENERATE_CDB)

clean:
	$(RM) $(OBJS) $(DEPS) $(COMPILE_DB) .cache

fclean: clean
	$(RM) $(EXEC)

re: fclean all

rm: fclean


# generate compile_commands.json
define GENERATE_CDB
CONTENT='[\n'
for FILE in $(SRCS); do
CONTENT+='\t{\n\t\t"directory": "'$$(pwd)'",\n\t\t"file": "'$$FILE'",\n\t\t"output": "'$${FILE%.cpp}.o'",\n\t\t"arguments": [\n\t\t\t"$(CXX)",\n'
	for FLAG in $(CXXFLAGS); do
		CONTENT+='\t\t\t"'$$FLAG'",\n'
	done
	CONTENT+='\t\t\t"-c",\n\t\t\t"'$$FILE'",\n\t\t\t"-o",\n\t\t\t"'$${FILE%.cpp}'.o"\n\t\t]\n\t},\n'
done
echo $${CONTENT%',\n'}'\n]' > $@
endef
