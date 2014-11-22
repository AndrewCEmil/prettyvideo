CC = clang++
CFLAGS = -pipe -fomit-frame-pointer -ggdb -std=c++11 -stdlib=libc++

images: EasyBMP.o ppix.o dagcontainer.o triops.o triopgenerator.o daggenerator.o
	$(CC) $(CFLAGS) -g EasyBMP.o ppix.o dagcontainer.o triops.o triopgenerator.o daggenerator.o -o ppix

EasyBMP.o: EasyBMP.cpp EasyBMP*.h
	$(CC) $(CFLAGS) -c EasyBMP.cpp

ppix.o: ppix.cpp
	$(CC) $(CFLAGS) -c ppix.cpp

dagcontainer.o: ppix.cpp
	$(CC) $(CFLAGS) -c dagcontainer.cpp

triops.o: ppix.cpp
	$(CC) $(CFLAGS) -c triops.cpp

triopgenerator.o: ppix.cpp
	$(CC) $(CFLAGS) -c triopgenerator.cpp

daggenerator.o: ppix.cpp
	$(CC) $(CFLAGS) -c daggenerator.cpp

clean:
	rm -f *.o
	rm ppix
	rm img*.bmp
