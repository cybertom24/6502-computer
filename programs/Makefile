# La variabile CC contiene il compilatore (in questo caso non avendo creato
# una variabile d'ambiente bisogna passare il percorso assoluto
CC=C:\Users\savol\Documents\CyberIndustries\Programmi\vasm\vasm6502_oldstyle.exe
# La variabile COPTION invece contiene le opzioni per il compilatore
# -Fbin permette di creare un file puramente binario
COPTION=-Fbin

# La variabile $(FILE) contiene il valore passato come argomento durante la chiamata di
# "make FILE=prog" quindi in questo caso $(FILE) = prog
FILEINPUTPATH=$(FILE).asm
FILEOUTPUTPATH=$(FILE).bin

LOADER=eeprom
LOPTION=-path="$(CURDIR)/$(FILEOUTPUTPATH)" -action=write

cl: compile load

compile:
	$(CC) $(COPTION) -o $(FILEOUTPUTPATH) $(FILEINPUTPATH)

load:
	$(LOADER) $(LOPTION)