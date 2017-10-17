# This is the Makefile to use for Homework Assignment #1
# To use, at the prompt, type:
#
# 	make <RET>		# This will make executable
# or
#	make new		# This will safely remove old stuff, and
#				# will then "make" again.
#
#	make directories	# makes correct directories
#	make install		# copies files to correct place
#	make clean		# cleans up directory
#	make install		# installs assignment into public area

DIR=$(HOME)/../public/hw1

all:	driver1 driver2

purify:	hw1.o
	purify gcc -Wall -pedantic -g -o driver1 hw1.o

driver1:hw1.o
	gcc -Wall -pedantic -o driver1 hw1.o

hw1.o:	hw1.c
	gcc -Wall -pedantic -c -g hw1.c

driver2: hw1.java
	javac -g hw1.java
	echo 'java hw1 $$*' > driver2
	chmod ug+rx driver2

clean:
	rm -f *.class *.o driver1 driver2 core $(HOME)/core

new:
	make clean
	make

backup:
	cp *.[ch] Makefile ./backup

directories:
	rm -f $(HOME)/public
	ln -s $(HOME)/../public $(HOME)/public
	mkdir $(HOME)/hw1
	mkdir $(HOME)/hw1/backup

install:
	cp ~/public/_vimrc.student $(HOME)/.vimrc
	cp $(DIR)/.cproject $(DIR)/.project $(DIR)/.hw1.sln \
	$(DIR)/.hw1.vcproj $(DIR)/hw1.c.empty $(DIR)/hw1.java.empty \
	$(DIR)/Makefile $(HOME)/hw1

public:
	make new
	# lock public directory
	chmod go-rwx $(DIR)
	#
	# remove old files from directory
	rm -r $(DIR)
	mkdir $(DIR)

       # produce java executables
	gcj -c *.java
	gcj -o driver2 --main=hw1 hw1.o

	#
	# copy all files to directory
	cp .cproject .project .hw1.sln .hw1.vcproj Makefile \
	    hw1.c.empty hw1.java.empty driver1 driver2 $(DIR)
	#
	# strip executable of debugging information
	strip $(DIR)/driver1 $(DIR)/driver2
	#
	# set correct permissions on source files 
	chmod 644 $(DIR)/Makefile $(DIR)/*.empty $(DIR)/.project \
	    $(DIR)/.cproject $(DIR)/.hw1.sln $(DIR)/.hw1.vcproj
	#
	# set correct permissions on executable
	chmod 711 $(DIR)/driver1 $(DIR)/driver2
	#
	rm -rf $(HOME)/../cs12xzz/hw1/*
	scp Makefile hw1.c hw1.java $(DIR)/driver1 $(DIR)/driver2 cs12xzz@ieng6-201:~/hw1
	#
	# unlock public directory for tutors
	chmod 750 $(DIR)
	# 
	# move html files to public_html directory
	rm -r $(HOME)/public_html/hw1/
	mkdir $(HOME)/public_html/hw1
	cp *html $(HOME)/public_html/hw1
	chmod 744 $(HOME)/public_html/hw1/*html
	chmod 750 $(HOME)/public_html/hw1/
	#
	# unlock public directory
	chmod go+rx $(DIR)
	chmod go+rx $(HOME)/public_html/hw1/
