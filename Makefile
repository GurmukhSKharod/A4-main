myls: myls.c
	gcc -g -o myls myls.c -lm

clean:
	rm -f myls  *.o
