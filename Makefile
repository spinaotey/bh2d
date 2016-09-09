COMPILER	=	gcc
CFLAGS		=	-O2 -fopenmp
LFLAGS		=	-lm
OBJECTS		=	bh2d.o tree_lib.o 
INCLUDES	=	tree_lib.h

bh2d:		$(OBJECTS)
	$(COMPILER) $(CFLAGS) -o bh2d $(OBJECTS) $(LFLAGS)

bh2d.o:	$(INCLUDES) bh2d.c
	$(COMPILER) $(CFLAGS) -c bh2d.c $(LFLAGS)

tree_lib.o:		tree_lib.c $(INCLUDES)
	$(COMPILER) $(CFLAGS) -c tree_lib.c $(LFLAGS)

clean:
	rm -f *.o *~

realclean: clean
	rm -f bh2d
