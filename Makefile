#Author: Damien Edward Blasko
#Date: 29 Jan 2020

#this is a very simple makefile that will work with
#clang or gcc

#calling 'make' will create an executable called 'langtons_ant.out'
#calling 'make clean' will delete the executable without touching the source code

main:
	gcc langtons_ant.c -o langtons_ant.out
clean:
	rm langtons_ant.out