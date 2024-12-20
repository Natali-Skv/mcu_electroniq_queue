###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = ATmega8515
COMPILER = "WinAVR"
TARGET = Debug
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=-Wall -gdwarf-2 -fsigned-char -MD -MP -DF_CPU=1000000 -O1 -mmcu=atmega8515
LDFLAGS=-mmcu=atmega8515

## Processing Tools
CC=avr-gcc.exe
ASM=avr-gcc.exe
LD=avr-gcc.exe
#  C:/Users/nat-s/AppData/Local/Temp/VSM Studio/e97e4778b30c48869381980961a51652/ATMEGA8515_1/Debug/Debug.elf: TOOL=LD
#  Post: avr-objcopy -O ihex -R .eeprom "./Debug.elf" "./Debug.hex"
#  Post: avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Debug.elf" "./Debug.eep" || exit 0 
#     C:/Users/nat-s/AppData/Local/Temp/VSM Studio/e97e4778b30c48869381980961a51652/ATMEGA8515_1/Debug/lcd.o: TOOL=CC
#        C:/Users/nat-s/AppData/Local/Temp/VSM Studio/e97e4778b30c48869381980961a51652/ATMEGA8515_1/lcd.c
#     C:/Users/nat-s/AppData/Local/Temp/VSM Studio/e97e4778b30c48869381980961a51652/ATMEGA8515_1/Debug/main.o: TOOL=CC
#        C:/Users/nat-s/AppData/Local/Temp/VSM Studio/e97e4778b30c48869381980961a51652/ATMEGA8515_1/main.c

# Build tree:
all:	Debug

Debug:	Debug.elf

Debug.elf:	lcd.o i2c.o main.o 
	$(LD) $(LDFLAGS)  -o "./Debug.elf" "lcd.o" "i2c.o" "main.o"
	avr-objcopy -O ihex -R .eeprom "./Debug.elf" "./Debug.hex"
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Debug.elf" "./Debug.eep" || exit 0 

lcd.o:	../lcd.c 
	$(CC) $(CCFLAGS)  -o "lcd.o" -c "../lcd.c"

i2c.o:	../i2c.c 
	$(CC) $(CCFLAGS)  -o "i2c.o" -c "../i2c.c"

main.o:	../main.c 
	$(CC) $(CCFLAGS)  -o "main.o" -c "../main.c"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf ./lcd.d
	rm -rf ./i2c.d
	rm -rf ./main.d

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.elf
	rm -rf lcd.o
	rm -rf main.o
	rm -rf i2c.o
