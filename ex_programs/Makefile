
IMAGEMAKER=../script/ROMImage/out/artifacts/ROMImage_jar/ROMImage.jar

LOADER=../script/eeprom.jar
LOPTION=--file "$(CURDIR)/$(FILE).bin" --write --port /dev/ttyUSB0 --validate

START=32768

# Make ROM image and load
il: image load

# Only make ROM image
image:
	java -jar $(IMAGEMAKER) -path="$(CURDIR)/$(FILE).65b" -out="$(CURDIR)/$(FILE).bin" -start=$(START)

# Only load ROM image
load:
	java -jar $(LOADER) $(LOPTION)
	
# La variabile CC contiene il compilatore (in questo caso non avendo creato
# una variabile d'ambiente bisogna passare il percorso assoluto)

# Compilatori:
CPATH=../compiler/vasm_unix/
# CC=vasm6502_oldstyle
 CC=vasm6502_std

# La variabile COPTION invece contiene le opzioni per il compilatore
# -Fbin permette di creare un file puramente binario
COPTION=-Fbin

# La variabile $(FILE) contiene il valore passato come argomento durante la chiamata di
# "make FILE=prog" quindi in questo caso $(FILE) = prog
FILEINPUTPATH=$(FILE).65s
# FILEINPUTPATH=$(FILE).asm
FILEOUTPUTPATH=$(FILE).bin

compile:
	$(CPATH)$(CC) $(COPTION) -o $(FILEOUTPUTPATH) $(FILEINPUTPATH)