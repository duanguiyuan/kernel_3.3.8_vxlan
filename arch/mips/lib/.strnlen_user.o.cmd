cmd_arch/mips/lib/strnlen_user.o := mips-openwrt-linux-uclibc-gcc -Wp,-MD,arch/mips/lib/.strnlen_user.o.d  -nostdinc -isystem /home/duanguiyuan/SpiderNet/TA361/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/gcc/mips-openwrt-linux-uclibc/4.6.3/include -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/include/linux/kconfig.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80060000" -D"DATAOFFSET=0" -D__ASSEMBLY__  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-ath79 -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-generic -gdwarf-2        -c -o arch/mips/lib/strnlen_user.o arch/mips/lib/strnlen_user.S

source_arch/mips/lib/strnlen_user.o := arch/mips/lib/strnlen_user.S

deps_arch/mips/lib/strnlen_user.o := \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/sgidefs.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/regdef.h \

arch/mips/lib/strnlen_user.o: $(deps_arch/mips/lib/strnlen_user.o)

$(deps_arch/mips/lib/strnlen_user.o):
