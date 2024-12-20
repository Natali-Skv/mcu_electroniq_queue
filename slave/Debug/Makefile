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
#  C:/Users/nat-s/AppData/Local/Temp/VSM Studio/9386fd69260a454d9807591f48c7f180/ATMEGA8515_2/Debug/Debug.elf: TOOL=LD
#  Post: avr-objcopy -O ihex -R .eeprom "./Debug.elf" "./Debug.hex"
#  Post: avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Debug.elf" "./Debug.eep" || exit 0 
#     C:/Users/nat-s/AppData/Local/Temp/VSM Studio/9386fd69260a454d9807591f48c7f180/ATMEGA8515_2/Debug/main.o: TOOL=CC
#        C:/Users/nat-s/AppData/Local/Temp/VSM Studio/9386fd69260a454d9807591f48c7f180/ATMEGA8515_2/main.c

# Build tree:
all:	Debug

Debug:	Debug.elf

Debug.elf:	main.o 
	$(LD) $(LDFLAGS)  -o "./Debug.elf" "main.o"
	avr-objcopy -O ihex -R .eeprom "./Debug.elf" "./Debug.hex"
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Debug.elf" "./Debug.eep" || exit 0 

main.o:	../main.c 
	$(CC) $(CCFLAGS)  -o "main.o" -c "../main.c"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf ./main.d

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.elf
	rm -rf main.o
