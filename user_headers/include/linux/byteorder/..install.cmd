cmd_/home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/user_headers/include/linux/byteorder/.install := perl scripts/headers_install.pl /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/include/linux/byteorder /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/user_headers/include/linux/byteorder mips big_endian.h little_endian.h; perl scripts/headers_install.pl /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/include/linux/byteorder /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/user_headers/include/linux/byteorder mips ; perl scripts/headers_install.pl /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/include/generated/linux/byteorder /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/user_headers/include/linux/byteorder mips ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/user_headers/include/linux/byteorder/$$F; done; touch /home/duanguiyuan/SpiderNet/TA361/build_dir/linux-ar71xx_generic/linux-3.3.8/user_headers/include/linux/byteorder/.install
