CFLAGS=-Werror -Wall
GTKFLAGS=`pkg-config --cflags gtk+-3.0`
GTKLIBS=`pkg-config --libs gtk+-3.0`
FILES=wav_analyzer.c wav_reader.c
OUTPUT=wav_analyzer

all: wav_analyzer.c wav_reader.c
	gcc $(CFLAGS) $(GTKFLAGS) $(FILES) -o $(OUTPUT) $(GTKLIBS)
