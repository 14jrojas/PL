.SUFFIXES:

prueba: y.tab.o
	gcc -o prueba y.tab.o
y.tab.o: y.tab.c
	gcc -c y.tab.c
y.tab.c: ejemplosyntax.y lex.yy.c
	bison -d -v -o y.tab.c ejemplosyntax.y
lex.yy.c: analizadorlexico.lex
	flex analizadorlexico.lex
limpia:
	rm -f prueba y.tab.o y.tab.c lex.yy.c
todo:
	make limpia
	make prueba
