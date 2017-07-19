cmd_arch/mips/mm/uasm.o := mips-openwrt-linux-uclibc-gcc -Wp,-MD,arch/mips/mm/.uasm.o.d  -nostdinc -isystem /home/duanguiyuan/SpiderNet/TA361/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/gcc/mips-openwrt-linux-uclibc/4.6.3/include -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include -Iarch/mips/include/generated -Iinclude  -include /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/include/linux/kconfig.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80060000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -fno-caller-saves -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-ath79 -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -Werror    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(uasm)"  -D"KBUILD_MODNAME=KBUILD_STR(uasm)" -c -o arch/mips/mm/uasm.o arch/mips/mm/uasm.c

source_arch/mips/mm/uasm.o := arch/mips/mm/uasm.c

deps_arch/mips/mm/uasm.o := \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/64bit.h) \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/duanguiyuan/SpiderNet/TA361/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/gcc/mips-openwrt-linux-uclibc/4.6.3/include/stdarg.h \
  include/linux/linkage.h \
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
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/types.h \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/posix_types.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/sgidefs.h \
  include/linux/bitops.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/bmips.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/cpu.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-ath79/cpu-feature-overrides.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/mach-ath79/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/inst.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/n32.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/compat.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/bugs.h \
  include/linux/bug.h \
  include/linux/delay.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/delay.h \
  include/linux/param.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/errno.h \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/const.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/arch/mips/include/asm/uasm.h \
    $(wildcard include/config/export/uasm.h) \

arch/mips/mm/uasm.o: $(deps_arch/mips/mm/uasm.o)

$(deps_arch/mips/mm/uasm.o):
