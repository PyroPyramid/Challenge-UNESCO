all : main

main4: main4.o lectureFichiers.o site.o haversine.o itineraires.o opti.o
	gcc -o main4 main4.o itineraires.o lectureFichiers.o site.o haversine.o opti.o -lm

main4.o: main4.c site.h itineraires.h haversine.h lectureFichiers.h opti.h
	gcc -c main4.c -lm

main3: main3.o lectureFichiers.o site.o haversine.o itineraires.o
	gcc -o main3 main3.o itineraires.o lectureFichiers.o site.o haversine.o -lm

main3.o: main3.c site.h itineraires.h haversine.h lectureFichiers.h
	gcc -c main3.c -lm

main2: main2.o lectureFichiers.o site.o haversine.o itineraires.o opti.o
	gcc -o main2 main2.o itineraires.o lectureFichiers.o site.o haversine.o opti.o -lm

main2.o: main2.c site.h itineraires.h haversine.h lectureFichiers.h opti.h
	gcc -c main2.c -lm

main: main.o lectureFichiers.o site.o haversine.o itineraires.o
	gcc -o main main.o itineraires.o lectureFichiers.o site.o haversine.o -lm

main.o: main.c site.h haversine.h lectureFichiers.h 
	gcc -c main.c -lm

opti.o: opti.c opti.h site.h itineraires.h
	gcc -c opti.c -lm

itineraires.o: itineraires.c site.h itineraires.h
	gcc -c itineraires.c -lm

lectureFichiers.o: lectureFichiers.c lectureFichiers.h
	gcc -c lectureFichiers.c -lm

site.o: site.c site.h lectureFichiers.h haversine.h
	gcc -c site.c -lm

haversine.o: haversine.c haversine.h
	gcc -c haversine.c -lm
 
clean:
	rm -f *.o main main2 main3 main4