#!/usr/bin/env -S zsh --no-rcs --no-globalrcs


# declare hash table
local -A GCC

GCC[-fdiagnostics-color]='           colorize diagnostics'
GCC[-fdiagnostics-generate-patch]='  print fix-it hints to stderr in unified diff format'
GCC[-fdiagnostics-parseable-fixits]='print fixit hints in machine-readable form'
GCC[-fdiagnostics-show-caret]='      show the source line with a caret indicating the column'
GCC[-fdiagnostics-show-location]='   how often to emit source location at the beginning of line-wrapped diagnostics'
GCC[-fdiagnostics-show-option]='     amend appropriate diagnostic messages with the command line option that controls them'
GCC[-fdirectives-only]='             preprocess directives only'


echo '\x1b[31m[GCC]\x1b[0m'
# print all keys and values
for key value in ${(kv)GCC}; do
	echo '\x1b[32m'$key'\x1b[0m' $value
done

# declare hash table
local -A CLANG

CLANG[-fdiagnostics-absolute-paths]='         print absolute paths in diagnostics'
CLANG[-fdiagnostics-color]='                  colorize diagnostics'
CLANG[-fdiagnostics-fixit-info]='             supply fixit into with diagnostic messages'
CLANG[-fdiagnostics-format]='                 diagnostics format'
CLANG[-fdiagnostics-generate-patch]='         print fix-it hints to stderr in unified diff format'
CLANG[-fdiagnostics-hotness-threshold]='      prevent optimization remarks from being output if they do not meet threshold'
CLANG[-fdiagnostics-parseable-fixits]='       print fixit hints in machine-readable form'
CLANG[-fdiagnostics-print-source-range-info]='print source range spans in numeric form'
CLANG[-fdiagnostics-show-caret]='             show the source line with a caret indicating the column'
CLANG[-fdiagnostics-show-category]='          diagnostics show category'
CLANG[-fdiagnostics-show-hotness]='           enable profile hotness information in diagnostic line'
CLANG[-fdiagnostics-show-location]='          how often to emit source location at the beginning of line-wrapped diagnostics'
CLANG[-fdiagnostics-show-note-include-stack]='display include stacks for diagnostic notes'
CLANG[-fdiagnostics-show-option]='            amend appropriate diagnostic messages with the command line option that controls them'
CLANG[-fdiagnostics-show-template-tree]='     print a template comparison tree for differing templates'


echo '\x1b[31m[CLANG]\x1b[0m'
# print all keys and values
for key value in ${(kv)CLANG}; do
	echo '\x1b[32m'$key'\x1b[0m' $value
done










