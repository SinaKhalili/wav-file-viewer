CFLAGS=-Werror -Wall
GTKFLAGS=`pkg-config --cflags gtk+-3.0`
GTKLIBS=`pkg-config --libs gtk+-3.0`
FILES=file_picker.c wav_reader.c
OUTPUT=main

all:
	gcc $(CFLAGS) $(GTKFLAGS) $(FILES) -o wav_analyzer $(GTKLIBS)
	./wav_analyzer

file_picker: file_picker.c
	gcc $(CFLAGS) $(GTKFLAGS) file_picker.c -o file_picker $(GTKLIBS)
	./file_picker

wav: wav_reader.c
	gcc $(CFLAGS) wav_reader.c -o wav_reader
	./wav_reader wav_files/LEOPARD.wav
