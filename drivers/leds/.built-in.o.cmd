cmd_drivers/leds/built-in.o :=  mips-openwrt-linux-uclibc-ld  -m elf32btsmip   -r -o drivers/leds/built-in.o drivers/leds/led-core.o drivers/leds/led-class.o drivers/leds/led-triggers.o 
