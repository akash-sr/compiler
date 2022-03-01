input_file = "input.txt"

all: compile
	./compile $(input_file)

compile:
	gcc -o compile driver.c parser.c lexer.c hashtable.c setADT.c stackADT.c treeADT.c

clean:
	rm -rf compile
