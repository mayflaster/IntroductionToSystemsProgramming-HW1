CC = gcc
OBJS = mtm_elections.o city.o citizen.o preference.o uniqueOrderedList.o mtm_print.o
EXEC = mtmElections
DEBUG = -DNDEBUG
FLAGS = -std=c99 -Wall -Werror -pedantic-errors $(DEBUG)
LIBs = -L. -lmtm 

$(EXEC) : $(OBJS)
	$(CC) -c $(DEBUG) $(OBJS) $(LIBS) -o $@

mtm_elections.o: mtm_elections.c mtm_elections.h \
 uniqueOrderedList/uniqueOrderedList.h set.h city.h citizen.h \
 preference.h mtm_print.h
	$(CC) -c $(FLAGS) $(LIBS) $*.c
 	
city.o: city.c city.h citizen.h uniqueOrderedList/uniqueOrderedList.h \
 preference.h set.h
	$(CC) -c $(FLAGS) $(LIBS) $*.c
	
citizen.o: citizen.c citizen.h uniqueOrderedList/uniqueOrderedList.h \
 preference.h
	$(CC) -c $(FLAGS) $(LIBS) $*.c
	
preference.o: preference.c preference.h
	$(CC) -c $(FLAGS) $*.c
	
uniqueOrderedList.o : uniqueOrderedList/uniqueOrderedList.c uniqueOrderedList/uniqueOrderedList.h
	$(CC) -c $(FLAGS) uniqueOrderedList/uniqueOrderedList.c

mtm_print.o : mtm_print.c mtm_print.h
	$(CC) -c $(FLAGS) $*.c
	
clean:
	rm -f $(OBJS) $(EXEC)