
IMAGEMAKER=../script/ROMImage/out/artifacts/ROMImage_jar/ROMImage.jar

LOADER=eeprom
LOPTION=-path="$(CURDIR)/$(FILE).bin" -action=write

START=32768

# Make ROM image and load
il: image load

# Only make ROM image
image:
	java -jar $(IMAGEMAKER) -path="$(CURDIR)/$(FILE).65b" -out="$(CURDIR)/$(FILE).bin" -start=$(START)

# Only load ROM image
load:
	$(LOADER) $(LOPTION)