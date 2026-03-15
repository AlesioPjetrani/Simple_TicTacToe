CC            := clang
CCFLAGS       := -Wall -Wextra -Wtype-limits -pedantic -std=c17 -g -lm
ASSIGNMENT    := tictactoe

.DEFAULT_GOAL := default
.PHONY: default clean bin all run help simple

default: help

clean: reset          ## cleans up project folder
	@printf '[\e[0;36mINFO\e[0m] Cleaning up ...\n'
	rm -f $(ASSIGNMENT)


bin:                  ## compiles project to executable binary
	@printf '[\e[0;36mINFO\e[0m] Compiling binary...\n'
	$(CC) $(CCFLAGS) -o $(ASSIGNMENT) *.c
	chmod +x $(ASSIGNMENT)

reset:                ## resets the reports folder
	@printf "[\e[0;36mINFO\e[0m] Resetting config files (Nothing to reset so far)...\n"

all: clean bin        ## executes 'clean' and 'bin'

help:                 ## prints the help text
	@printf "Usage: make \e[0;36m<TARGET>\e[0m\n"
	@printf "Available targets:\n"
	@awk -F':.*?##' '/^[a-zA-Z_-]+:.*?##.*$$/{printf "  \033[36m%-10s\033[0m%s\n", $$1, $$2}' $(MAKEFILE_LIST)

run: all              ## runs the project
	@printf '[\e[0;36mINFO\e[0m] Executing binary...\n'
	./$(ASSIGNMENT)

simple: clean bin    ## cleans, compiles, clears terminal then executes
	@clear
	@./$(ASSIGNMENT)