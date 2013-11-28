# Gruppe 2-8
# Matthias Hansert 791744
# Max Knodel 798479
# Niklas Knauer 798163

OBJEKTE = ueb1.o velocity.o
LINKOPT :=  -Wall -O0 `pkg-config --cflags vmc` `pkg-config --libs vmc` `pkg-config --libs log4cpp` -lpthread

velocity: $(OBJEKTE)
	c++ -g $(LINKOPT) $(OBJEKTE) -o $@ 

%.o: %.cpp robi.h 
	c++ -g -Wall -O0 `pkg-config --cflags vmc` `pkg-config --libs vmc` `pkg-config --libs log4cpp` -lpthread  -c $<

clean:
	rm velocity -f
	rm *.o -f
	rm *.txt -f
	rm *.png -f

run:
	./velocity
