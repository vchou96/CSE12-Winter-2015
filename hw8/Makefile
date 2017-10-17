# This is the Makefile to use for Homework Assignment #6
# To use, at the prompt, type:
#
#       make Calc               # This will make executable calc
# or
#       make Driver             # This will make executable driver
# or
#       make clean              # This will safely remove old stuff

DIR=$(HOME)/../public/hw8

all:	Driver Calc

Calc:	Calc.o Decin.o Main.o Namein.o
	g++ -o Calc -Wall -pedantic -g Calc.o Decin.o Main.o Namein.o

Calc.o:	Calc.c Calc.h Decin.h Tree.c Tree.h List.c List.h Stack.c Stack.h SymTab.h
	g++ -Wall -pedantic -g -c Calc.c

Driver:	Driver.o 
	g++ -o Driver -Wall -pedantic -g Driver.o 

Driver.o:	Driver.c Tree.c Tree.h
	g++ -Wall -pedantic -g -c Driver.c

Decin.o:	Decin.c Decin.h
	g++ -Wall -pedantic -g -c Decin.c

Main.o:	Main.c Calc.h Tree.c Tree.h List.c List.h Stack.c Stack.h SymTab.h
	g++ -Wall -pedantic -g -c Main.c

Namein.o:	Namein.h Namein.c
	g++ -Wall -pedantic -g -c Namein.c

valgrind_Calc: Calc
	valgrind --leak-check=full --read-var-info=yes \
	    --show-reachable=yes ./Calc $(DEBUG)

valgrind_Driver: Driver
	valgrind --leak-check=full --read-var-info=yes \
	    --show-reachable=yes ./Driver $(DEBUG)

clean:
	rm -f *.o Driver Calc

new:
	make clean
	make

backup:	Makefile *.[ch]
	cp Makefile *.[ch] backup

directories:
	mkdir $(HOME)/hw8
	mkdir $(HOME)/hw8/backup

install:
	cp $(DIR)/Makefile $(DIR)/Main.c \
	$(DIR)/Decin.c $(DIR)/Stack.c $(DIR)/Calc.c \
	$(DIR)/Namein.c $(DIR)/Tree.c.empty $(DIR)/List.c \
	$(DIR)/Driver.h.empty $(DIR)/Driver.c \
	$(DIR)/alphabet $(DIR)/alphabet.rev \
	$(DIR)/Decin.h $(DIR)/Stack.h $(DIR)/Calc.h \
	$(DIR)/Namein.h $(DIR)/List.h $(DIR)/SymTab.h \
	$(DIR)/.cproject $(DIR)/.project $(DIR)/.hw8.vcproj $(DIR)/.hw8.sln \
	$(DIR)/Tree.h $(HOME)/hw8

public:
	make new
	chmod 700 $(DIR)
	#
	cp -p .cproject .project .hw8.vcproj .hw8.sln alphabet* \
	Makefile Calc Driver *.h *.c *.empty $(DIR)
	rm -f $(DIR)/Tree.c
	rm -f $(DIR)/Driver.h
	#
	strip $(DIR)/Calc $(DIR)/Driver
	# mpage -1 -P- hw8 > $(DIR)/hw8.1.ps
	# mpage -4 -P- hw8 > $(DIR)/hw8.4.ps
	#
	chmod 600 $(DIR)/*
	chmod 711 $(DIR)/Calc $(DIR)/Driver
	chmod 644 $(DIR)/*.[ch] $(DIR)/*.empty \
		$(DIR)/.cproject $(DIR)/.project \
		$(DIR)/.hw8.vcproj $(DIR)/.hw8.sln \
		$(DIR)/Makefile \
		$(DIR)/alphabet $(DIR)/alphabet.rev
	#
	#rm -rf $(HOME)/../cs12xzz/hw8/*
	#scp -p Makefile *.[ch] $(DIR)/Calc $(DIR)/Driver \
#		cs12xzz@ieng6-201:~/hw8

	# move html files to public_html directory
	rm -r $(HOME)/public_html/hw8/
	mkdir $(HOME)/public_html/hw8
	cp *html $(HOME)/public_html/hw8
	chmod 744 $(HOME)/public_html/hw8/*html
	chmod 755 $(HOME)/public_html/hw8/

	# Uncomment below when hw has been handed out
	chmod 755 $(DIR)
