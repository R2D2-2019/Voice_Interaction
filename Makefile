.PHONY: build run

build:
	gcc -o main.out main.c \
	-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" \
	`pkg-config --cflags --libs pocketsphinx sphinxbase`
run:
	./main.out