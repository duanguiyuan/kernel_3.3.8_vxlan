/*
 * Atheros TA361 reference board support
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
#include <linux/gpio.h>

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

#define TA361_GPIO_LED_WLAN		12
#define TA361_GPIO_LED_WPS		13
#define TA361_GPIO_LED_STATUS		13

#define TA361_GPIO_LED_WAN		4
#define TA361_GPIO_LED_LAN1		16
#define TA361_GPIO_LED_LAN2		15
#define TA361_GPIO_LED_LAN3		14
#define TA361_GPIO_LED_LAN4		11

#define TA361_GPIO_BTN_WPS		17

#define TA361_KEYS_POLL_INTERVAL	20	/* msecs */
#define TA361_KEYS_DEBOUNCE_INTERVAL	(3 * TA361_KEYS_POLL_INTERVAL)

#define TA361_MAC0_OFFSET		0
#define TA361_MAC1_OFFSET		6
#define TA361_WMAC_CALDATA_OFFSET	0x1000

#define TA361_GPIO_EXPANDER_GPIO_SPI_CS     16  /* EXPANDER GPIO SPI CS */ 
#define TA361_GPIO_4P2V_EN                  11  //high is enable

#define TA361_GPIO_4P2V_EN_VAL            1

#if 0
static struct gpio_led ta361_leds_gpio[] __initdata = {
	{
		.name		= "ta361:green:status",
		.gpio		= TA361_GPIO_LED_STATUS,
		.active_low	= 1,
	},
	{
		.name		= "ta361:green:wlan",
		.gpio		= TA361_GPIO_LED_WLAN,
		.active_low	= 1,
	}
};

static struct gpio_keys_button ta361_gpio_keys[] __initdata = {
	{
		.desc		= "WPS button",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = TA361_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TA361_GPIO_BTN_WPS,
		.active_low	= 1,
	},
};

static void __init ta361_gpio_led_setup(void)
{
	ath79_gpio_direction_select(TA361_GPIO_LED_WAN, true);
	ath79_gpio_direction_select(TA361_GPIO_LED_LAN1, true);
	ath79_gpio_direction_select(TA361_GPIO_LED_LAN2, true);
	ath79_gpio_direction_select(TA361_GPIO_LED_LAN3, true);
	ath79_gpio_direction_select(TA361_GPIO_LED_LAN4, true);

	ath79_gpio_output_select(TA361_GPIO_LED_WAN,
			QCA953X_GPIO_OUT_MUX_LED_LINK5);
	ath79_gpio_output_select(TA361_GPIO_LED_LAN1,
			QCA953X_GPIO_OUT_MUX_LED_LINK1);
	ath79_gpio_output_select(TA361_GPIO_LED_LAN2,
			QCA953X_GPIO_OUT_MUX_LED_LINK2);
	ath79_gpio_output_select(TA361_GPIO_LED_LAN3,
			QCA953X_GPIO_OUT_MUX_LED_LINK3);
	ath79_gpio_output_select(TA361_GPIO_LED_LAN4,
			QCA953X_GPIO_OUT_MUX_LED_LINK4);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(ta361_leds_gpio),
			ta361_leds_gpio);
	ath79_register_gpio_keys_polled(-1, TA361_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(ta361_gpio_keys),
			ta361_gpio_keys);
}
#endif

static void __init ta361_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	
	/* disable jtag gpio2 */
	ath79_gpio_function_enable(AR934X_GPIO_FUNC_JTAG_DISABLE);

    /* enable gpio expander spi cs */
	ath79_gpio_direction_select(TA361_GPIO_EXPANDER_GPIO_SPI_CS, true);
	ath79_gpio_output_select(TA361_GPIO_EXPANDER_GPIO_SPI_CS,
			QCA953X_GPIO_OUT_MUX_SPI_CS1);
			
	ath79_gpio_direction_select(TA361_GPIO_4P2V_EN,true);
    gpio_set_value(TA361_GPIO_4P2V_EN,TA361_GPIO_4P2V_EN_VAL);
			
	ath79_register_m25p80(NULL);

    /* There is not led on TA361-00 cpu gpio, all of led is on expander gpio */
	//ta361_gpio_led_setup();

	ath79_register_usb();
	ath79_register_pci();

	ath79_register_wmac(art + TA361_WMAC_CALDATA_OFFSET, NULL);

	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + TA361_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + TA361_MAC1_OFFSET, 0);

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

MIPS_MACHINE(ATH79_MACH_CUSTOM_TA361, "ta361", "Custom TA361 board",
	     ta361_setup);
