# This is the Makefile to use for Homework Assignment #3
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
#	make public		# installs assignment into public area

DIR=$(HOME)/../public/hw2

all:	driver1 driver2

driver1:hw2.o
	gcc -Wall -Wno-long-long -pedantic -o driver1 hw2.o

driver2:hw2.java 
	javac -g hw2.java
	echo 'java hw2 $$*' > driver2
	chmod ug+rx driver2

hw2.o:	hw2.c
	gcc -Wall -Wno-long-long -pedantic -c -g hw2.c

clean:
	rm -f *.o *.class driver1 driver2 core $(HOME)/core

new:
	make clean
	make

backup:
	cp *.[ch] *.java Makefile ./backup

directories:
	rm -f $(HOME)/public
	ln -s $(HOME)/../public $(HOME)/public
	mkdir $(HOME)/hw2
	mkdir $(HOME)/hw2/backup

install:
	cp $(DIR)/.cproject $(DIR)/.project $(DIR)/.hw2.sln \
	$(DIR)/.hw2.vcproj $(DIR)/hw2.c.empty $(DIR)/hw2.java.empty \
	$(DIR)/Makefile $(HOME)/hw2

public:
	make new
	# lock public directory
	chmod go-rwx $(DIR)
	#
	# remove old files from directory
	rm -r $(DIR)
	mkdir $(DIR)
	#
	# make java executable
	gcj -c *.java
	gcj -o driver2 --main=hw2 hw2.o

	# copy all files to directory
	cp .cproject .project .hw2.sln .hw2.vcproj Makefile \
	    hw2.c.empty hw2.java.empty driver1 driver2 $(DIR)
	#
	# strip executable of debugging information
	strip $(DIR)/driver1 $(DIR)/driver2
	#
	# set correct permissions on source files 
	chmod 644 $(DIR)/Makefile $(DIR)/*.empty $(DIR)/.project \
	    $(DIR)/.cproject $(DIR)/.hw2.sln $(DIR)/.hw2.vcproj
	#
	# set correct permissions on executable
	chmod 711 $(DIR)/driver1 $(DIR)/driver2
	#
	rm -rf $(HOME)/../cs12xzz/hw2/*
	scp Makefile hw2.c $(DIR)/driver1 cs12xzz@ieng6-201:~/hw2
	#
	# unlock public directory for tutors
	chmod 750 $(DIR)
	#
	# move html files to public_html directory
	rm -r $(HOME)/public_html/hw2/
	mkdir $(HOME)/public_html/hw2
	cp *html $(HOME)/public_html/hw2
	chmod 744 $(HOME)/public_html/hw2/*html
	chmod 750 $(HOME)/public_html/hw2/
	#
	# unlock public directory
	chmod go+rx $(DIR)
	chmod go+rx $(HOME)/public_html/hw2/


tabs2spaces:
	make backup
	expand --tabs=4 hw2.c > .hw2_c$
	mv .hw2_c$ hw2.c
