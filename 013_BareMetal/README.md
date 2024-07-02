openocd -f /usr/share/openocd/scripts/board/<yourboard>.cfg

gdb> target remote localhost:3333

gdb> monitor reset init (every time on connection)

gdb> monitor reset halt

gdb> monitor resume

To keep break point:

gdb> monitor bp <address_of_function>

To flash:

gdb> monitor flash write_image erase <image.elf>

To read a memory address: 

gdb> monitor mdw 0x20000000 4

To enable semihosting

gdb> monitor arm enable semihosting