cmd_drivers/of/built-in.o :=  mips-openwrt-linux-uclibc-ld  -m elf32btsmip   -r -o drivers/of/built-in.o drivers/of/base.o drivers/of/fdt.o drivers/of/address.o drivers/of/irq.o drivers/of/device.o drivers/of/platform.o drivers/of/gpio.o drivers/of/of_i2c.o drivers/of/of_net.o drivers/of/of_spi.o drivers/of/selftest.o drivers/of/of_mdio.o drivers/of/of_pci.o drivers/of/of_pci_irq.o 
