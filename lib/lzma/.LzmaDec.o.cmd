cmd_lib/lzma/LzmaDec.o := mips-openwrt-linux-uclibc-gcc -Wp,-MD,lib/lzma/.LzmaDec.o.d  -nostdinc -isystem /home/duanguiyuan/SpiderNet/TA361/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/gcc/mips-openwrt-linux-uclibc/4.6.3/include -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/include/linux/kconfig.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80060000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -fno-caller-saves -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-ath79 -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -Iinclude/linux -Iinclude/linux/lzma -include types.h    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(LzmaDec)"  -D"KBUILD_MODNAME=KBUILD_STR(lzma_decompress)" -c -o lib/lzma/LzmaDec.o lib/lzma/LzmaDec.c

source_lib/lzma/LzmaDec.o := lib/lzma/LzmaDec.c

deps_lib/lzma/LzmaDec.o := \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/posix_types.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/sgidefs.h \
  include/linux/lzma/LzmaDec.h \
  include/linux/lzma/Types.h \
  include/linux/stddef.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/linux/types.h \
  /home/duanguiyuan/SpiderNet/TA361/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/gcc/mips-openwrt-linux-uclibc/4.6.3/include/stdarg.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/string.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/r3000.h) \

lib/lzma/LzmaDec.o: $(deps_lib/lzma/LzmaDec.o)

$(deps_lib/lzma/LzmaDec.o):
