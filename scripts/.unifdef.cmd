cmd_scripts/unifdef := gcc -Wp,-MD,scripts/.unifdef.d -O2 -I/home/duanguiyuan/SpiderNet/TA361/staging_dir/host/include -Wall -Wmissing-prototypes -Wstrict-prototypes   -I/home/duanguiyuan/SpiderNet/TA361/staging_dir/host/include -I/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/tools/include  -o scripts/unifdef scripts/unifdef.c  

source_scripts/unifdef := scripts/unifdef.c

deps_scripts/unifdef := \
  /usr/include/stdc-predef.h \
  /usr/include/sys/types.h \
  /usr/include/features.h \
  /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-64.h \
  /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h \
  /usr/include/time.h \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h \
  /home/duanguiyuan/SpiderNet/TA361/staging_dir/host/include/endian.h \
  /home/duanguiyuan/SpiderNet/TA361/staging_dir/host/include/byteswap.h \
  /usr/include/byteswap.h \
  /usr/include/bits/byteswap.h \
  /usr/include/bits/byteswap-16.h \
  /usr/include/endian.h \
  /usr/include/bits/endian.h \
  /usr/include/sys/select.h \
  /usr/include/bits/select.h \
  /usr/include/bits/sigset.h \
  /usr/include/bits/time.h \
  /home/duanguiyuan/SpiderNet/TA361/staging_dir/host/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h \
  /usr/include/sys/stat.h \
  /usr/include/bits/stat.h \
  /usr/include/ctype.h \
  /usr/include/xlocale.h \
  /usr/include/err.h \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h \
  /usr/include/errno.h \
  /usr/include/bits/errno.h \
  /usr/include/linux/errno.h \
  /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdbool.h \
  /usr/include/stdio.h \
  /usr/include/libio.h \
  /usr/include/_G_config.h \
  /usr/include/wchar.h \
  /usr/include/bits/stdio_lim.h \
  /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h \
  /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h \
  /usr/include/alloca.h \
  /usr/include/bits/stdlib-float.h \
  /usr/include/string.h \
  /usr/include/bits/string.h \
  /usr/include/bits/string2.h \
  /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h \
  /usr/include/bits/environments.h \
  /usr/include/bits/confname.h \
  /usr/include/getopt.h \

scripts/unifdef: $(deps_scripts/unifdef)

$(deps_scripts/unifdef):
