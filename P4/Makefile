.SUFFIXES:

prueba: main.o y.tab.o
	gcc -o prueba main.o y.tab.o
y.tab.o: y.tab.c
	gcc -c y.tab.c
main.o: main.c
	gcc -c main.c
y.tab.c: ejemplosyntax.y lex.yy.c
	bison -d -v -o y.tab.c ejemplosyntax.y
lex.yy.c: analizadorlexico.lex
	flex analizadorlexico.lex
limpia:
	rm -f prueba main.o y.tab.o y.tab.c lex.yy.c
todo:
	make limpia
	make prueba
