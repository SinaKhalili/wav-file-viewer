CFLAGS=-Werror -Wall
GTKFLAGS=`pkg-config --cflags gtk+-3.0`
GTKLIBS=`pkg-config --libs gtk+-3.0`
NAME=main.c
OUTPUT=main

file_picker: file_picker.c
	gcc $(CFLAGS) $(GTKFLAGS) file_picker.c -o file_picker $(GTKLIBS)
	./file_picker

wav: wav_reader.c
	gcc $(CFLAGS) wav_reader.c -o wav_reader
	./wav_reader wav_files/LEOPARD.wav
