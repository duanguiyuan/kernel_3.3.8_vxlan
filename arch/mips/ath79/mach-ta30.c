
/*
 * Custom TA30 board support
 *
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "pci.h"


#define TA30_GPIO_LED_SYSTEM	15  /*SYS_LED   1-on*/

#define TA30_GPIO_RST_U2		11  /*U2 RST  0-on*/
#define TA30_GPIO_RST_U3		16  /*U3 RST  0-on*/
#define TA30_GPIO_RST_U4		17  /*U4 RST  0-on*/



#define TA30_GPIO_BTN_RESET     15  /*SW_RST*/ 

#define TA30_KEYS_POLL_INTERVAL	20	/* msecs */
#define TA30_KEYS_DEBOUNCE_INTERVAL	(3 * TA30_KEYS_POLL_INTERVAL)

#define TA30_MAC0_OFFSET		0
#define TA30_MAC1_OFFSET		6
#define TA30_WMAC_CALDATA_OFFSET	0x1000

static struct gpio_led ta30_leds_gpio[] __initdata = {
	{
		.name		= "ta30:green:system",
		.gpio		= TA30_GPIO_LED_SYSTEM,
		.active_low	= 0,
	},
};

static struct gpio_keys_button ta30_gpio_keys[] __initdata = {
	{
		.desc		= "reset button",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = TA30_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TA30_GPIO_BTN_RESET,
		.active_low	= 0,
	},
};

static void __init ta30_gpio_led_setup(void)
{
	ath79_gpio_direction_select(TA30_GPIO_LED_SYSTEM, true);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ta30_leds_gpio),
			ta30_leds_gpio);
	ath79_register_gpio_keys_polled(-1, TA30_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(ta30_gpio_keys),
			ta30_gpio_keys);
}

static void __init ta30_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);

	ta30_gpio_led_setup();

	ath79_register_usb();
	ath79_register_pci();

	ath79_register_wmac(art + TA30_WMAC_CALDATA_OFFSET, NULL);

	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + TA30_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + TA30_MAC1_OFFSET, 0);

	/* WAN port */
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.speed = SPEED_100;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_register_eth(0);

	
	ath79_setup_ar933x_phy4_switch(1,0);
	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_100;
	ath79_eth1_data.duplex = DUPLEX_FULL;
	ath79_switch_data.phy_poll_mask |= BIT(0);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_CUSTOM_TA30, "ta30", "Custom TA30 board",
	     ta30_setup);
