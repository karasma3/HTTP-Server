# HTTP-Server

## Popis
HTTP-Server
- /examples -ukázka stránek
- /src -zdrojový kód
- Doxyfile -nastavení dokumentace
- Makefile -makefile
- README.md -specifikace

## Konfigurace servera
- konfigurace servera se nachází v /src/config.txt, kde je potřeba nastavit root, port, soubour logu a adresu

## MakeFile, spuštení servera a generovaní dokumentace
- make compile -zkompiluje
- make doc -vygeneruje dokumentace *Doxygen required*
- make all -zkompiluje a vygeneruje dokumentace
- make run -spustí server
- make clean -smaže spustitelný soubor a všechny objektové soubory

### Dokumentace
- po vygenerovaní dokumentace se vytvoří adresář /doc který obsahuje veškerú dokumentaci k zdrojovému kódu

