
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
override PROJECT := csight

# main executable
override EXEC := $(PROJECT)

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

# object files
override OBJS := $(SRCS:%.cpp=%.o)

# dependency files
override DEPS := $(OBJS:%.o=%.d)

# header files
override HDRS := $(shell find $(INC_DIR) -type f -name '*.hpp')

# precompiled header
override PCHS := $(HDRS:%.hpp=%.pch)

# include directories
override INCS := $(shell find $(INC_DIR) -mindepth 1 -type d)

# log files
override LOGS := $(OBJS:%.o=%.log)


# -- T O O L S ----------------------------------------------------------------

# make directory if not exists
override MKDIR := mkdir -p

# remove recursively force
override RM := rm -vfr


# -- C O M P I L E R ----------------------------------------------------------

# compiler
override CXX := $(shell which clang++)
#override CXX := /opt/homebrew/Cellar/llvm/18.1.4/bin/clang++
#override CXX := /opt/homebrew/Cellar/gcc/13.2.0/bin/g++-13

# compiler standard
override STD := -std=c++2a

# compiler optimization
override OPT := -O0

# debug level
override DBG := -g3

# compiler flags
override FLAGS := -Wall -Wextra -Werror \
				  -Wpedantic -Weffc++ -Winline \
				  -Wno-unused -Wno-unused-variable -Wno-unused-parameter \
				  -Wconversion -Wsign-conversion -Wfloat-conversion -Wnarrowing \
				  -Wshadow \
				  -fno-rtti \
				  -fdiagnostics-color=always



#-fdiagnostics-absolute-paths           -- print absolute paths in diagnostics
#-fdiagnostics-color                    -- colorize diagnostics
#-fdiagnostics-fixit-info               -- supply fixit into with diagnostic messages
#-fdiagnostics-format                   -- diagnostics format
#-fdiagnostics-generate-patch           -- print fix-it hints to stderr in unified diff format
#-fdiagnostics-hotness-threshold        -- prevent optimization remarks from being output if they do not meet threshold
#-fdiagnostics-parseable-fixits         -- print fixit hints in machine-readable form
#-fdiagnostics-print-source-range-info  -- print source range spans in numeric form
#-fdiagnostics-show-caret               -- show the source line with a caret indicating the column
#-fdiagnostics-show-category            -- diagnostics show category
#-fdiagnostics-show-hotness             -- enable profile hotness information in diagnostic line
#-fdiagnostics-show-location            -- how often to emit source location at the beginning of line-wrapped diagnostics
#-fdiagnostics-show-note-include-stack  -- display include stacks for diagnostic notes
#-fdiagnostics-show-option              -- amend appropriate diagnostic messages with the command line option that controls them
#-fdiagnostics-show-template-tree       -- print a template comparison tree for differing templates

#-fno-elide-type -fdiagnostics-show-template-tree


#-Wfatal-errors 

#-Werror \

# cxx flags
override CXXFLAGS := $(STD) $(DBG) $(OPT) $(FLAGS) -I$(INC_DIR)

# pch include
override PCH_INCLUDES := $(addprefix -include-pch ,$(PCHS))

# linker flags
override LDFLAGS ?=

# dependency flags
override DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# number of compiled files
NSRC := 0

# number of compiled headers
NHDR := 0


# -- P H O N Y ----------------------------------------------------------------

# phony targets
.PHONY: all ascii clean fclean re .WAIT

# not parallel
.NOTPARALLEL: $(COMPILE_DB) re fclean clean ascii


# -- T A R G E T S ------------------------------------------------------------

all: ascii $(COMPILE_DB) .WAIT $(EXEC)
	echo '[done]\n'
	echo '\033[35m'$(NHDR) 'headers precompiled\033[0m'
	echo '\033[32m'$(NSRC) 'sources compiled\033[0m'
	echo

$(EXEC): $(OBJS)
	echo '\n\033[32mexe\033[0m' $@'\n'
	$(CXX) $^ -o $@ $(LDFLAGS)

-include $(DEPS)
%.o: %.cpp Makefile | $(PCHS)
	echo '\033[33mobj\033[0m' $@
	$(CXX) -c $< -o $@ $(PCH_INCLUDES) $(CXXFLAGS) $(DEPFLAGS)
	$(eval NSRC := $(shell echo $$(($(NSRC) + 1))))

%.pch: %.hpp Makefile
	echo '\033[35mpch\033[0m' $@
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@
	$(eval NHDR := $(shell echo $$(($(NHDR) + 1))))

$(COMPILE_DB): $(SRCS) Makefile
	echo '\033[34mcdb\033[0m' $@
	$(call GENERATE_CDB)

clean:
	$(RM) $(EXEC) $(OBJS) $(DEPS) $(PCHS) $(COMPILE_DB) .cache | wc -l | awk '{print $$1 " files removed"}'

fclean: clean

re: fclean all

ascii:
	echo '\033[32m\n'\
		' ▄████▄   ▒█████   ███▄ ▄███▓ ██▓███   ██▓ ██▓    ▓█████      ██████  ██▓  ▄████  ██░ ██ ▄▄▄█████▓\n'\
		'▒██▀ ▀█  ▒██▒  ██▒▓██▒▀█▀ ██▒▓██░  ██▒▓██▒▓██▒    ▓█   ▀    ▒██    ▒ ▓██▒ ██▒ ▀█▒▓██░ ██▒▓  ██▒ ▓▒\n'\
		'▒▓█    ▄ ▒██░  ██▒▓██    ▓██░▓██░ ██▓▒▒██▒▒██░    ▒███      ░ ▓██▄   ▒██▒▒██░▄▄▄░▒██▀▀██░▒ ▓██░ ▒░\n'\
		'▒▓▓▄ ▄██▒▒██   ██░▒██    ▒██ ▒██▄█▓▒ ▒░██░▒██░    ▒▓█  ▄      ▒   ██▒░██░░▓█  ██▓░▓█ ░██ ░ ▓██▓ ░ \n'\
		'▒ ▓███▀ ░░ ████▓▒░▒██▒   ░██▒▒██▒ ░  ░░██░░██████▒░▒████▒   ▒██████▒▒░██░░▒▓███▀▒░▓█▒░██▓  ▒██▒ ░ \n'\
		'░ ░▒ ▒  ░░ ▒░▒░▒░ ░ ▒░   ░  ░▒▓▒░ ░  ░░▓  ░ ▒░▓  ░░░ ▒░ ░   ▒ ▒▓▒ ▒ ░░▓   ░▒   ▒  ▒ ░░▒░▒  ▒ ░░   \n'\
		'  ░  ▒     ░ ▒ ▒░ ░  ░      ░░▒ ░      ▒ ░░ ░ ▒  ░ ░ ░  ░   ░ ░▒  ░ ░ ▒ ░  ░   ░  ▒ ░▒░ ░    ░    \n'\
		'░        ░ ░ ░ ▒  ░      ░   ░░        ▒ ░  ░ ░      ░      ░  ░  ░   ▒ ░░ ░   ░  ░  ░░ ░  ░      \n'\
		'░ ░          ░ ░         ░             ░      ░  ░   ░  ░         ░   ░        ░  ░  ░  ░         \n'\
		'░                                                                                                 \n'\
		'\033[0m'

	#echo '\033[32m'\
	#	'                   _ _            _     _   _   \n'\
	#	' ___ ___ _____ ___|_| |___    ___|_|___| |_| |_ \n'\
	#	'|  _| . |     | . | | | -_|  |_ -| | . |   |  _|\n'\
	#	'|___|___|_|_|_|  _|_|_|___|  |___|_|_  |_|_|_|  \n'\
	#	'              |_|                  |___|        \n'\
	#	'\033[0m'




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
