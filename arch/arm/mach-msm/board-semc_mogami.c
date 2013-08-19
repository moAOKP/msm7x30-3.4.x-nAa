/* Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Adapted for SEMC 2011 devices by Vassilis Tsogkas (tsogkas@ceid.upatras.gr)
 */

#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/gpio_event.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/bootmem.h>
#include <linux/io.h>
#ifdef CONFIG_SPI_QSD
#include <linux/spi/spi.h>
#endif
#include <linux/msm_ssbi.h>
#include <linux/mfd/pmic8058.h>
#include <linux/mfd/marimba.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/ofn_atlab.h>
#include <linux/power_supply.h>
#include <linux/msm_adc.h>
#include <linux/dma-mapping.h>
#include <linux/regulator/consumer.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/setup.h>

#include <mach/mpp.h>
#include <mach/board.h>
#include <mach/camera.h>
#include <mach/memory.h>
#include <mach/msm_iomap.h>
#include <mach/msm_hsusb.h>
#include <mach/rpc_hsusb.h>
#include <mach/msm_spi.h>
#include <mach/qdsp5v2/msm_lpa.h>
#include <mach/dma.h>
#include <linux/android_pmem.h>
#include <mach/pmic.h>
#include <mach/rpc_pmapp.h>
#include <mach/qdsp5v2/aux_pcm.h>
#include <mach/qdsp5v2/mi2s.h>
#include <mach/qdsp5v2/audio_dev_ctl.h>
#include <mach/rpc_server_handset.h>
#include <mach/socinfo.h>
#include <mach/msm_memtypes.h>
#include <linux/cyttsp-qc.h>

#include <asm/mach/mmc.h>
#include <asm/mach/flash.h>
#include <mach/vreg.h>
#include <linux/platform_data/qcom_crypto_device.h>

#include "devices.h"
#include "timer.h"
#ifdef CONFIG_USB_G_ANDROID
#include <linux/usb/android.h>
#include <mach/usbdiag.h>
#endif
#include "pm.h"
#include "pm-boot.h"
#include "spm.h"
#include "acpuclock.h"
#include "clock.h"
#include <mach/dal_axi.h>
#include <mach/msm_serial_hs.h>
#include <mach/qdsp5v2/mi2s.h>
#include <mach/qdsp5v2/audio_dev_ctl.h>
#include <mach/sdio_al.h>
#include "smd_private.h"

#include "board-msm7x30-regulator.h"
#include "pm.h"

#include "board-semc_mogami-gpio.h"
#include "board-semc_mogami-keypad.h"
#include "board-semc_mogami-touch.h"

#ifdef CONFIG_SIMPLE_REMOTE_PLATFORM
#include <mach/simple_remote_msm7x30_pf.h>
#endif
#include <mach/semc_rpc_server_handset.h>

#ifdef CONFIG_CHARGER_BQ24185
#include <linux/i2c/bq24185_charger.h>
#endif
#include <linux/i2c/bq27520_battery.h>
#include <linux/battery_chargalg.h>
#include <mach/semc_battery_data.h>
#ifdef CONFIG_USB_MSM_OTG_72K
#include <mach/msm72k_otg.h>
#endif
#ifdef CONFIG_SEMC_CHARGER_USB_ARCH
#include <mach/semc_charger_usb.h>
#endif

#ifdef CONFIG_SENSORS_AKM8975
#include <linux/i2c/akm8975.h>
#endif
#ifdef CONFIG_INPUT_BMA150_NG
#include <linux/bma150_ng.h>
#endif
#ifdef CONFIG_INPUT_BMA250
#include <linux/bma250.h>
#endif
#ifdef CONFIG_INPUT_APDS9702
#include <linux/apds9702.h>
#endif

#if defined(CONFIG_LM3560) || defined(CONFIG_LM3561)
#include <linux/lm356x.h>
#endif
#ifdef CONFIG_LEDS_AS3676
#include <linux/leds-as3676.h>
#include "board-semc_mogami-leds.h"
#endif

#ifdef CONFIG_FB_MSM_MDDI_NOVATEK_FWVGA
#include <mach/mddi_novatek_fwvga.h>
#endif
#if defined(CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD)
#include <linux/mddi_sony_s6d05a1_hvga.h>
#endif
#if defined(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD)
#include <linux/mddi_hitachi_r61529_hvga.h>
#endif
#if defined(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD)
#include <linux/mddi_sii_r61529_hvga.h>
#endif
#if defined(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)
#include <linux/mddi_auo_s6d05a1_hvga.h>
#endif
#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
#include <linux/uio_driver.h>
#include <linux/i2c/sii9024.h>
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */

#ifdef CONFIG_TOUCHSCREEN_CY8CTMA300_SPI
#include <linux/spi/cy8ctma300_touch.h>
#endif
#ifdef CONFIG_TOUCHSCREEN_CLEARPAD
#include <linux/clearpad.h>
#endif

#ifdef CONFIG_CHARGER_BQ24185
#define BQ24185_GPIO_IRQ		(31)
#endif

#ifdef CONFIG_SENSORS_AKM8975
#define AKM8975_GPIO			(92)
#endif
#ifdef CONFIG_INPUT_BMA150_NG
#define BMA150_GPIO			(51)
#endif
#ifdef CONFIG_INPUT_BMA250
#define BMA250_GPIO			(51)
#endif

#if defined(CONFIG_LM3560) || defined(CONFIG_LM3561)
#define LM356X_HW_RESET_GPIO 2
#endif

#ifdef CONFIG_FB_MSM_MDDI_NOVATEK_FWVGA
#define NOVATEK_GPIO_RESET              (157)
#endif

#if defined(CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)
#define GPIO_MSM_MDDI_XRES		(157)
#endif

#define CYPRESS_TOUCH_GPIO_RESET	(40)
#define CYPRESS_TOUCH_GPIO_IRQ		(42)
#ifdef CONFIG_TOUCHSCREEN_CLEARPAD
#define SYNAPTICS_TOUCH_GPIO_IRQ	(42)
#endif
#define CYPRESS_TOUCH_GPIO_SPI_CS	(46)

#ifdef CONFIG_FB_MSM_HDPI
#define MSM_PMEM_SF_SIZE  0x2500000
#else
#define MSM_PMEM_SF_SIZE  0x1600000
#endif
#ifdef CONFIG_FB_MSM_TRIPLE_BUFFER
#define MSM_FB_PRIM_BUF_SIZE   (864 * 480 * 4 * 3) /* 4bpp * 3 Pages */
#else
#define MSM_FB_PRIM_BUF_SIZE   (864 * 480 * 4 * 2) /* 4bpp * 2 Pages */
#endif

#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
#define MSM_FB_EXT_BUF_SIZE    0x30000
#else
#define MSM_FB_EXT_BUF_SIZE    0
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */

#define MSM_FB_SIZE roundup(MSM_FB_PRIM_BUF_SIZE + MSM_FB_EXT_BUF_SIZE, 4096)

#ifdef CONFIG_SEMC_CAMERA_8MP
#define MSM_PMEM_ADSP_SIZE      0x2500000
#else
#define MSM_PMEM_ADSP_SIZE      0x1C00000
#endif
#define PMEM_KERNEL_EBI0_SIZE   0x600000

#define PMIC_GPIO_INT		27
#define PMIC_GPIO_SD_DET	22

#define PMIC_GPIO_SDC4_PWR_EN_N 24  /* PMIC GPIO Number 25 */

/* Macros assume PMIC GPIOs start at 0 */
#define PM8058_GPIO_PM_TO_SYS(pm_gpio)     (pm_gpio + NR_GPIO_IRQS)
#define PM8058_GPIO_SYS_TO_PM(sys_gpio)    (sys_gpio - NR_GPIO_IRQS)
#define PM8058_MPP_BASE			   PM8058_GPIO_PM_TO_SYS(PM8058_GPIOS)
#define PM8058_MPP_PM_TO_SYS(pm_gpio)	   (pm_gpio + PM8058_MPP_BASE)

#define PMIC_GPIO_QUICKVX_CLK 37 /* PMIC GPIO 38 */

#define DDR0_BANK_BASE PHYS_OFFSET
#define DDR0_BANK_SIZE 0X03C00000
#define DDR1_BANK_BASE 0x07000000
#define DDR1_BANK_SIZE 0x09000000
#define DDR2_BANK_BASE 0X40000000
#define DDR2_BANK_SIZE 0X10000000

/* Platform-specific regulator name mappings according to conf. spec. */
#define VREG_L8	"gp7"	/* BMA150, AK8975B, LCD, Touch, HDMI */
#define VREG_L10	"gp4"	/* BMA150, AK8975B */
#define VREG_L15	"gp6"	/* LCD */
#define VREG_L20	"gp13"	/* Touch */


/* Platform specific HW-ID GPIO mask */
static const u8 hw_id_gpios[] = {150, 149, 148, 43};

static unsigned int phys_add = DDR2_BANK_BASE;
unsigned long ebi1_phys_offset = DDR2_BANK_BASE;
EXPORT_SYMBOL(ebi1_phys_offset);

extern void msm_init_pmic_vibrator(void);

static int vreg_helper_on(const char *pzName, unsigned mv)
{
	struct vreg *reg = NULL;
	int rc = 0;

	reg = vreg_get(NULL, pzName);
	if (IS_ERR(reg)) {
		printk(KERN_ERR "Unable to resolve VREG name \"%s\"\n", pzName);
		return rc;
	}

	if (mv != (unsigned int)-1)
		rc = vreg_set_level(reg, mv);

	if (rc) {
		printk(KERN_ERR "Unable to set vreg \"%s\" level\n", pzName);
		return rc;
	}

	rc = vreg_enable(reg);
	if (rc) {
		printk(KERN_ERR "Unable to enable vreg \"%s\" level\n", pzName);
		return rc;
	}

	printk(KERN_INFO "Enabled VREG \"%s\" at %u mV\n", pzName, mv);
	return rc;
}

static void vreg_helper_off(const char *pzName)
{
	struct vreg *reg = NULL;
	int rc;

	reg = vreg_get(NULL, pzName);
	if (IS_ERR(reg)) {
		printk(KERN_ERR "Unable to resolve VREG name \"%s\"\n", pzName);
		return;
	}

	rc = vreg_disable(reg);
	if (rc) {
		printk(KERN_ERR "Unable to disable vreg \"%s\" level\n",
			pzName);
		return;
	}

	printk(KERN_INFO "Disabled VREG \"%s\"\n", pzName);
}


static ssize_t hw_id_get_mask(struct class *class, struct class_attribute *attr, char *buf)
{

	char hwid;
	unsigned int i;
	unsigned cfg;
	int rc;
	for (hwid = i = 0; i < ARRAY_SIZE(hw_id_gpios); i++) {
		cfg = GPIO_CFG(hw_id_gpios[i], 0, GPIO_CFG_INPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA);
		rc = gpio_tlmm_config(cfg, GPIO_CFG_ENABLE);
		if (rc) {
			printk(KERN_ERR
				"%s: Enabling of GPIO failed. "
				"gpio_tlmm_config(%#x, enable)=%d\n",
				__func__, cfg, rc);
			return rc;
		}
		hwid |= (gpio_get_value(hw_id_gpios[i]) & 1) << i;
		rc = gpio_tlmm_config(cfg, GPIO_CFG_DISABLE);
		if (rc) {
			printk(KERN_INFO
				"%s: Disabling of GPIO failed. "
				"The got GPIO value is valid. "
				"gpio_tlmm_config(%#x, disable)=%d\n",
				__func__, cfg, rc);
		}
	}
	printk(KERN_INFO "Board Mogami HW ID: 0x%02x\n", hwid);
	return sprintf(buf, "0x%02x\n", hwid);
}

static CLASS_ATTR(hwid, 0444, hw_id_get_mask, NULL);
static struct class hwid_class = {.name	= "hwid",};
static void __init hw_id_class_init(void)
{
	int error;
	error = class_register(&hwid_class);
	if (error) {
		printk(KERN_ERR "%s: class_register failed\n", __func__);
		return;
	}
	error = class_create_file(&hwid_class, &class_attr_hwid);
	if (error) {
		printk(KERN_ERR "%s: class_create_file failed\n",
		__func__);
		class_unregister(&hwid_class);
	}
}

#ifdef CONFIG_MOGAMI_SLIDER

static const struct gpio_event_direct_entry slider_mogami_gpio_map[] = {
	{180, SW_LID},
};

static struct gpio_event_input_info slider_gpio_info = {
	.info.func = gpio_event_input_func,
	.flags = 0, /* GPIO event active low*/
	.type = EV_SW,
	.keymap = slider_mogami_gpio_map,
	.keymap_size = ARRAY_SIZE(slider_mogami_gpio_map),
};

static struct gpio_event_info *slider_info[] = {
	&slider_gpio_info.info,
};

static struct gpio_event_platform_data slider_data = {
	.name		= "slider-mogami",
	.info		= slider_info,
	.info_count	= ARRAY_SIZE(slider_info),
};

struct platform_device slider_device_mogami = {
	.name	= GPIO_EVENT_DEV_NAME,
	.id	= -1,
	.dev	= {
		.platform_data	= &slider_data,
	},
};

#endif /* CONFIG_MOGAMI_SLIDER */

static struct input_dev *input_dev_pwr_key = NULL;
static void msm_pmic_pwr_key_rpc_callback(uint32_t key, uint32_t event)
{
	if (!input_dev_pwr_key)
		return;
	switch (key) {
	case HS_PWR_K:
		key = KEY_POWER;
		break;
	case HS_END_K:
		key = KEY_END;
		break;
	default:
		return;
	}
	input_report_key(input_dev_pwr_key, key, event != HS_REL_K);
	input_sync(input_dev_pwr_key);
}

static int __init msm_pmic_pwr_key_init(void)
{
	input_dev_pwr_key = input_allocate_device();
	if (!input_dev_pwr_key) {
		printk(KERN_ERR "%s: Error, unable to alloc pwr key device\n",
			__func__);
		return -1;
	}
	input_dev_pwr_key->name = "msm_pmic_pwr_key";
	input_dev_pwr_key->phys = "semc_rpc_server_handset";
	input_set_capability(input_dev_pwr_key, EV_KEY, KEY_POWER);
	input_set_capability(input_dev_pwr_key, EV_KEY, KEY_END);
	if (input_register_device(input_dev_pwr_key)) {
		printk(KERN_ERR "%s: Error, unable to reg pwr key device\n",
			__func__);
		input_free_device(input_dev_pwr_key);
		return -1;
	}
	return 0;
}
module_init(msm_pmic_pwr_key_init);

/*
 * Add callbacks here. Every defined callback will receive
 * all events. The types are defined in the file
 * semc_rpc_server_handset.h
 */

static handset_cb_array_t semc_rpc_hs_callbacks = {
	&msm_pmic_pwr_key_rpc_callback,
#ifdef CONFIG_SIMPLE_REMOTE_PLATFORM
	&simple_remote_pf_button_handler,
#endif
};

static struct semc_handset_data semc_rpc_hs_data = {
	.callbacks = semc_rpc_hs_callbacks,
	.num_callbacks = ARRAY_SIZE(semc_rpc_hs_callbacks),
};

static struct platform_device semc_rpc_handset_device = {
	.name = SEMC_HANDSET_DRIVER_NAME,
	.id = -1,
	.dev = {
		.platform_data = &semc_rpc_hs_data,
	},
};

struct pm8xxx_gpio_init_info {
	unsigned			gpio;
	struct pm_gpio			config;
};

static int pm8058_gpios_init(void)
{
	int rc;

#ifdef CONFIG_CHARGER_BQ24185
	struct pm8xxx_gpio_init_info bq24185_irq = {
		PM8058_GPIO_PM_TO_SYS(BQ24185_GPIO_IRQ - 1),
		{
			.direction      = PM_GPIO_DIR_IN,
			.pull           = PM_GPIO_PULL_NO,
			.vin_sel        = PM8058_GPIO_VIN_S3,
			.function       = PM_GPIO_FUNC_NORMAL,
			.inv_int_pol    = 0,
		},
	};
#endif

	struct pm8xxx_gpio_init_info sdc4_pwr_en = {
		PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SDC4_PWR_EN_N),
		{
			.direction      = PM_GPIO_DIR_OUT,
			.pull           = PM_GPIO_PULL_NO,
			.vin_sel        = PM8058_GPIO_VIN_L5,
			.function       = PM_GPIO_FUNC_NORMAL,
			.inv_int_pol    = 0,
			.out_strength   = PM_GPIO_STRENGTH_LOW,
			.output_value   = 0,
		},
	};

	struct pm8xxx_gpio_init_info sdcc_det = {
		PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SD_DET - 1),
		{
			.direction      = PM_GPIO_DIR_IN,
			.pull           = PM_GPIO_PULL_UP_1P5,
			.vin_sel        = 2,
			.function       = PM_GPIO_FUNC_NORMAL,
			.inv_int_pol    = 0,
		},
	};

	rc = pm8xxx_gpio_config(sdcc_det.gpio, &sdcc_det.config);
	if (rc) {
		pr_err("%s PMIC_GPIO_SD_DET config failed\n", __func__);
		return rc;
	}

#ifdef CONFIG_CHARGER_BQ24185
	rc = pm8xxx_gpio_config(bq24185_irq.gpio, &bq24185_irq.config);
	if (rc) {
		pr_err("%s BQ24185_GPIO_IRQ config failed with %d\n", __func__, rc);
		return rc;
	}
#endif

	/* SDC4 gpio_25 */
	rc = pm8xxx_gpio_config(sdc4_pwr_en.gpio, &sdc4_pwr_en.config);
	if (rc) {
		pr_err("%s PMIC_GPIO_SDC4_PWR_EN_N config failed: %d\n",
		       __func__, rc);
		return rc;
	}

	rc = gpio_request(sdc4_pwr_en.gpio, "sdc4_pwr_en");
	if (rc) {
		pr_err("PMIC_GPIO_SDC4_PWR_EN_N gpio_req failed: %d\n",
		       rc);
		return rc;
	}

	return 0;
}

/* Regulator API support */

#ifdef CONFIG_MSM_PROC_COMM_REGULATOR
static struct platform_device msm_proccomm_regulator_dev = {
	.name = PROCCOMM_REGULATOR_DEV_NAME,
	.id   = -1,
	.dev  = {
		.platform_data = &msm7x30_proccomm_regulator_data
	}
};
#endif

static int pm8058_pwm_config(struct pwm_device *pwm, int ch, int on)
{
	struct pm_gpio pwm_gpio_config = {
		.direction      = PM_GPIO_DIR_OUT,
		.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
		.output_value   = 0,
		.pull           = PM_GPIO_PULL_NO,
		.vin_sel        = PM8058_GPIO_VIN_S3,
		.out_strength   = PM_GPIO_STRENGTH_HIGH,
		.function       = PM_GPIO_FUNC_2,
	};
	int	rc = -EINVAL;
	int	id, mode, max_mA;

	id = mode = max_mA = 0;
	switch (ch) {
	case 0:
	case 1:
	case 2:
		if (on) {
			id = 24 + ch;
			rc = pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(id - 1),
							&pwm_gpio_config);
			if (rc)
				pr_err("%s: pm8xxx_gpio_config(%d): rc=%d\n",
				       __func__, id, rc);
		}
		break;

	case 3:
		id = PM_PWM_LED_KPD;
		mode = PM_PWM_CONF_DTEST3;
		max_mA = 200;
		break;

	case 4:
		id = PM_PWM_LED_0;
		mode = PM_PWM_CONF_PWM1;
		max_mA = 40;
		break;

	case 5:
		id = PM_PWM_LED_2;
		mode = PM_PWM_CONF_PWM2;
		max_mA = 40;
		break;

	case 6:
		id = PM_PWM_LED_FLASH;
		mode = PM_PWM_CONF_DTEST3;
		max_mA = 200;
		break;

	default:
		break;
	}

	if (ch >= 3 && ch <= 6) {
		if (!on) {
			mode = PM_PWM_CONF_NONE;
			max_mA = 0;
		}
		rc = pm8058_pwm_config_led(pwm, id, mode, max_mA);
		if (rc)
			pr_err("%s: pm8058_pwm_config_led(ch=%d): rc=%d\n",
			       __func__, ch, rc);
	}

	return rc;
}

static int pm8058_pwm_enable(struct pwm_device *pwm, int ch, int on)
{
	int	rc;

	switch (ch) {
	case 7:
		rc = pm8058_pwm_set_dtest(pwm, on);
		if (rc)
			pr_err("%s: pwm_set_dtest(%d): rc=%d\n",
			       __func__, on, rc);
		break;
	default:
		rc = -EINVAL;
		break;
	}
	return rc;
}

static struct pm8058_pwm_pdata pm8058_pwm_data = {
	.config         = pm8058_pwm_config,
	.enable         = pm8058_pwm_enable,
};

static struct pm8xxx_irq_platform_data pm8xxx_irq_pdata = {
	.irq_base		= PMIC8058_IRQ_BASE,
	.devirq			= MSM_GPIO_TO_INT(PMIC_GPIO_INT),
	.irq_trigger_flag       = IRQF_TRIGGER_LOW,
};

static struct pm8xxx_gpio_platform_data pm8xxx_gpio_pdata = {
	.gpio_base		= PM8058_GPIO_PM_TO_SYS(0),
};

static struct pm8xxx_mpp_platform_data pm8xxx_mpp_pdata = {
	.mpp_base	= PM8058_MPP_PM_TO_SYS(0),
};

static struct pm8058_platform_data pm8058_7x30_data = {
	.irq_pdata		= &pm8xxx_irq_pdata,
	.gpio_pdata		= &pm8xxx_gpio_pdata,
	.mpp_pdata		= &pm8xxx_mpp_pdata,
	.pwm_pdata		= &pm8058_pwm_data,
};

#ifdef CONFIG_MSM_SSBI
static struct msm_ssbi_platform_data msm7x30_ssbi_pm8058_pdata = {
	.rsl_id = "D:PMIC_SSBI",
	.controller_type = MSM_SBI_CTRL_SSBI2,
	.slave	= {
		.name			= "pm8058-core",
		.platform_data		= &pm8058_7x30_data,
	},
};
#endif

#ifdef CONFIG_FB_MSM_MDDI_NOVATEK_FWVGA
static const struct panel_id *novatek_panels[] = {
#ifdef CONFIG_MDDI_NOVATEK_PANEL_SHARP_LS040T8LX01
	&novatek_panel_id_sharp_ls040t8lx01_rev_c,
	&novatek_panel_id_sharp_ls040t8lx01_rev_d,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_SHARP_LS042T3LX
	&novatek_panel_id_sharp_ls042t3lx_type1,
	&novatek_panel_id_sharp_ls042t3lx,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_SONY_ACX424AKM
	&novatek_panel_id_sony_acx424akm_type1,
	&novatek_panel_id_sony_acx424akm,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_SONY_ACX427AK
	&novatek_panel_id_sony_acx427ak,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_SONY_ACX424AK
	&novatek_panel_id_sony_acx424ak,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_HITACHI_DX09D09VM
	&novatek_panel_id_hitachi_dx09d09vm_type1,
	&novatek_panel_id_hitachi_dx09d09vm,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_SHARP_LS033T3LX01
	&novatek_panel_id_sharp_ls033t3lx01,
#endif
#ifdef CONFIG_MDDI_NOVATEK_PANEL_TMD_LT033MDV1000
	&novatek_panel_id_tmd_lt033mdv1000,
#endif
	NULL,
};

struct novatek_i2c_pdata novatek_i2c_pdata = {
	.panels = novatek_panels,
};
#endif

#ifdef CONFIG_INPUT_BMA150_NG
static int bma150_gpio_setup(bool request)
{
	if (request)
		return gpio_request(BMA150_GPIO, "bma150_irq");
	else
		gpio_free(BMA150_GPIO);
	return 0;
}

struct bma150_platform_data bma150_ng_platform_data = {
	.gpio_setup = bma150_gpio_setup,
};
#endif

static struct i2c_board_info msm_camera_boardinfo[] __initdata = {
#ifdef CONFIG_FB_MSM_MDDI_NOVATEK_FWVGA
	{
		I2C_BOARD_INFO(MDDI_NOVATEK_I2C_NAME, 0x98 >> 1),
		.type = MDDI_NOVATEK_I2C_NAME,
		.platform_data = &novatek_i2c_pdata,
	},
#endif
#ifdef CONFIG_INPUT_BMA150_NG
	{
		I2C_BOARD_INFO("bma150", 0x70 >> 1),
		.irq = MSM_GPIO_TO_INT(BMA150_GPIO),
		.platform_data = &bma150_ng_platform_data,
		.type = "bma150"
	},
#endif
#ifdef CONFIG_SEMC_CAMERA_MODULE
	{
		I2C_BOARD_INFO("semc_camera", 0x1A),
		.type = "semc_camera"
	},
#endif
#ifdef CONFIG_SEMC_SUB_CAMERA_MODULE
	{
		I2C_BOARD_INFO("semc_sub_camera", 0x3D),
		.type = "semc_sub_camera"
	},
#endif
};

#ifdef CONFIG_MSM_CAMERA
#define	CAM_STNDBY	143
static uint32_t camera_off_vcm_gpio_table[] = {
GPIO_CFG(1, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* VCM */
};

static uint32_t camera_off_gpio_table[] = {
	/* parallel CAMERA interfaces */
	/* RST */
	GPIO_CFG(0,  0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#if !defined(CONFIG_SEMC_CAMERA_MODULE)
#if !defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	/* DAT2 */
	GPIO_CFG(2,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT3 */
	GPIO_CFG(3,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
#endif
#endif
#if defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	/* DAT4 */
	GPIO_CFG(4,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT5 */
	GPIO_CFG(5,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT6 */
	GPIO_CFG(6,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT7 */
	GPIO_CFG(7,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT8 */
	GPIO_CFG(8,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT9 */
	GPIO_CFG(9,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT10 */
	GPIO_CFG(10, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT11 */
	GPIO_CFG(11, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* PCLK */
	GPIO_CFG(12, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* HSYNC_IN */
	GPIO_CFG(13, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* VSYNC_IN */
	GPIO_CFG(14, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
#endif
	/* MCLK */
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#if defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	/* CAM_VGA_RST_N */
	GPIO_CFG(31, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#endif
};

static uint32_t camera_on_vcm_gpio_table[] = {
GPIO_CFG(1, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), /* VCM */
};

static uint32_t camera_on_gpio_table[] = {
	/* parallel CAMERA interfaces */
	/* RST */
	GPIO_CFG(0,  0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#if !defined(CONFIG_SEMC_CAMERA_MODULE)
#if !defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	/* DAT2 */
	GPIO_CFG(2,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT3 */
	GPIO_CFG(3,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
#endif
#endif
#if defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	/* DAT4 */
	GPIO_CFG(4,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT5 */
	GPIO_CFG(5,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT6 */
	GPIO_CFG(6,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT7 */
	GPIO_CFG(7,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT8 */
	GPIO_CFG(8,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT9 */
	GPIO_CFG(9,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT10 */
	GPIO_CFG(10, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* DAT11 */
	GPIO_CFG(11, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* PCLK */
	GPIO_CFG(12, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* HSYNC_IN */
	GPIO_CFG(13, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/* VSYNC_IN */
	GPIO_CFG(14, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
#endif
	/* MCLK */
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_6MA),
#if defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	/* CAM_VGA_RST_N */
	GPIO_CFG(31, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#endif
};

static void config_gpio_table(uint32_t *table, int len)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n], GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}
static int config_camera_on_gpios(void)
{
	config_gpio_table(camera_on_gpio_table,
		ARRAY_SIZE(camera_on_gpio_table));

	config_gpio_table(camera_on_vcm_gpio_table,
		ARRAY_SIZE(camera_on_vcm_gpio_table));

	return 0;
}

static void config_camera_off_gpios(void)
{
	config_gpio_table(camera_off_gpio_table,
		ARRAY_SIZE(camera_off_gpio_table));

	config_gpio_table(camera_off_vcm_gpio_table,
		ARRAY_SIZE(camera_off_vcm_gpio_table));
}

struct resource msm_camera_resources[] = {
	{
		.start	= 0xA6000000,
		.end	= 0xA6000000 + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_VFE,
		.end	= INT_VFE,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.flags  = IORESOURCE_DMA,
	}
};

struct msm_camera_device_platform_data msm_camera_device_data = {
	.camera_gpio_on  = config_camera_on_gpios,
	.camera_gpio_off = config_camera_off_gpios,
	.ioext.camifpadphy = 0xAB000000,
	.ioext.camifpadsz  = 0x00000400,
	.ioext.csiphy = 0xA6100000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = INT_CSI,
#if defined(CONFIG_SEMC_CAMERA_MODULE) || defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	.ioclk.mclk_clk_rate = 8000000,
	.ioclk.vfe_clk_rate  = 192000000,
#else
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 147456000,
#endif
};

#if defined(CONFIG_SEMC_CAMERA_MODULE) || defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
static struct msm_camera_sensor_flash_data flash_none = {
	.flash_type = MSM_CAMERA_FLASH_NONE,
	.flash_src  = NULL
 };
#endif

#ifdef CONFIG_SEMC_CAMERA_MODULE
static struct msm_camera_sensor_info msm_camera_sensor_semc_camera_data = {
	.sensor_name      = "semc_camera",
	.sensor_reset     = 0,
	.sub_sensor_reset = 31,
	.sensor_pwd       = 0,
	.vcm_pwd          = 0,
	.vcm_enable       = 0,
	.mclk             = 15,
	.flash_type       = MSM_CAMERA_FLASH_NONE,
	.pdata            = &msm_camera_device_data,
	.resource         = msm_camera_resources,
	.num_resources    = ARRAY_SIZE(msm_camera_resources),
	.flash_data       = &flash_none,
	.csi_if           = 1, /* mipi interface direct */
	.csi_params       = {
		.data_format    = CSI_10BIT,
		.lane_cnt       = 2,
		.lane_assign    = 0xe4,
		.settle_cnt     = 25,
		.dpcm_scheme    = 0
	},
	.vcam_io       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "lvsw1"
	},
	.vcam_sd       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "gp15"
	},
	.vcam_sa       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "gp2"
	},
	.vcam_af       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "gp10"
	},
};

static struct platform_device msm_camera_sensor_semc_camera = {
	.name = "msm_camera_semc_camera",
	.dev  = {
		.platform_data = &msm_camera_sensor_semc_camera_data,
	},
};
#endif

#ifdef CONFIG_SEMC_SUB_CAMERA_MODULE
static struct msm_camera_sensor_info msm_camera_sensor_semc_sub_camera_data = {
	.sensor_name      = "semc_sub_camera",
	.sensor_reset     = 0,
	.sub_sensor_reset = 31,
	.sensor_pwd       = 0,
	.vcm_pwd          = 0,
	.vcm_enable       = 0,
	.mclk             = 15,
	.flash_type       = MSM_CAMERA_FLASH_NONE,
	.pdata            = &msm_camera_device_data,
	.resource         = msm_camera_resources,
	.num_resources    = ARRAY_SIZE(msm_camera_resources),
	.flash_data       = &flash_none,
	.csi_if           = 0, /* parallel interface */
	.csi_params       = {
		.data_format    = 0,
		.lane_cnt       = 0,
		.lane_assign    = 0,
		.settle_cnt     = 0,
		.dpcm_scheme    = 0
	},
	.vcam_io       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "lvsw1"
	},
	.vcam_sd       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "gp15"
	},
	.vcam_sa       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "gp2"
	},
	.vcam_af       = {
		.type = MSM_CAMERA_SENSOR_PWR_VREG,
		.resource.name = "gp10"
	},
};

static struct platform_device msm_camera_sensor_semc_sub_camera = {
	.name = "msm_camera_semc_sub_camera",
	.dev  = {
		.platform_data = &msm_camera_sensor_semc_sub_camera_data,
	},
};
#endif

#if defined (CONFIG_MSM_VPE) || defined(CONFIG_MSM_VPE_STANDALONE)
static struct resource msm_vpe_resources[] = {
	{
		.start	= 0xAD200000,
		.end	= 0xAD200000 + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_VPE,
		.end	= INT_VPE,
		.flags	= IORESOURCE_IRQ,
	},
};
#endif
#ifdef CONFIG_MSM_VPE
static struct platform_device msm_vpe_device = {
       .name = "msm_vpe",
       .id   = 0,
       .num_resources = ARRAY_SIZE(msm_vpe_resources),
       .resource = msm_vpe_resources,
};
#endif
#ifdef CONFIG_MSM_VPE_STANDALONE
static struct platform_device msm_vpe_standalone_device = {
       .name = "msm_vpe_standalone",
       .id   = 0,
       .num_resources = ARRAY_SIZE(msm_vpe_resources),
       .resource = msm_vpe_resources,
};
#endif

#endif /*CONFIG_MSM_CAMERA*/

#ifdef CONFIG_MSM_GEMINI
static struct resource msm_gemini_resources[] = {
	{
		.start  = 0xA3A00000,
		.end    = 0xA3A00000 + 0x0150 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = INT_JPEG,
		.end    = INT_JPEG,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device msm_gemini_device = {
	.name           = "msm_gemini",
	.resource       = msm_gemini_resources,
	.num_resources  = ARRAY_SIZE(msm_gemini_resources),
};
#endif

#ifdef CONFIG_MSM7KV2_AUDIO
static uint32_t audio_pamp_gpio_config =
   GPIO_CFG(82, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA);

static uint32_t HAC_amp_gpio_config =
   GPIO_CFG(109, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA);

static int __init snddev_poweramp_gpio_init(void)
{
	int rc;

	pr_info("snddev_poweramp_gpio_init \n");
	rc = gpio_tlmm_config(audio_pamp_gpio_config, GPIO_CFG_ENABLE);
	if (rc) {
		printk(KERN_ERR
			"%s: gpio_tlmm_config(%#x)=%d\n",
			__func__, audio_pamp_gpio_config, rc);
	}

	/* Enabling HAC amplifier */
	rc = gpio_tlmm_config(HAC_amp_gpio_config, GPIO_CFG_ENABLE);
	if (rc) {
		printk(KERN_ERR
			"%s: gpio_tlmm_config(%#x)=%d\n",
			__func__, HAC_amp_gpio_config, rc);
	}

	return rc;
}

void msm_snddev_tx_route_config(void)
{
	pr_debug("%s()\n", __func__);
}

void msm_snddev_tx_route_deconfig(void)
{
	pr_debug("%s()\n", __func__);
}

void msm_hac_amp_on(void)
{
}

void msm_hac_amp_off(void)
{
}

void msm_snddev_poweramp_on(void)
{
	gpio_set_value(82, 1);	/* enable spkr poweramp */
	pr_info("%s: power on amplifier\n", __func__);
}

void msm_snddev_poweramp_off(void)
{
	gpio_set_value(82, 0);	/* disable spkr poweramp */
	pr_info("%s: power off amplifier\n", __func__);
}

static struct regulator_bulk_data snddev_regs[] = {
	{ .supply = "gp4", .min_uV = 2600000, .max_uV = 2600000 },
	{ .supply = "ncp", .min_uV = 1800000, .max_uV = 1800000 },
};

static int __init snddev_hsed_voltage_init(void)
{
	int rc;

	rc = regulator_bulk_get(NULL, ARRAY_SIZE(snddev_regs), snddev_regs);

	if (rc) {
		pr_err("%s: could not get regulators: %d\n", __func__, rc);
		goto out;
	}

	rc = regulator_bulk_set_voltage(ARRAY_SIZE(snddev_regs), snddev_regs);

	if (rc) {
		pr_err("%s: could not set regulator voltages: %d\n",
				__func__, rc);
		goto regs_free;
	}

	return 0;

regs_free:
	regulator_bulk_free(ARRAY_SIZE(snddev_regs), snddev_regs);
out:
	return rc;
}


void msm_snddev_hsed_voltage_on(void)
{
	int rc = regulator_bulk_enable(ARRAY_SIZE(snddev_regs), snddev_regs);

	if (rc)
		pr_err("%s: could not enable regulators: %d\n", __func__, rc);
}

void msm_snddev_hsed_voltage_off(void)
{
	int rc = regulator_bulk_disable(ARRAY_SIZE(snddev_regs), snddev_regs);

	if (rc) {
		pr_err("%s: could not disable regulators: %d\n", __func__, rc);
	}
}

static unsigned aux_pcm_gpio_on[] = {
	GPIO_CFG(138, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),   /* PCM_DOUT */
	GPIO_CFG(139, 1, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),   /* PCM_DIN  */
	GPIO_CFG(140, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),   /* PCM_SYNC */
	GPIO_CFG(141, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),   /* PCM_CLK  */
};

static int __init aux_pcm_gpio_init(void)
{
	int pin, rc;

	pr_info("aux_pcm_gpio_init \n");
	for (pin = 0; pin < ARRAY_SIZE(aux_pcm_gpio_on); pin++) {
		rc = gpio_tlmm_config(aux_pcm_gpio_on[pin],
					GPIO_CFG_ENABLE);
		if (rc) {
			printk(KERN_ERR
				"%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, aux_pcm_gpio_on[pin], rc);
		}
	}
	return rc;
}

static struct msm_gpio mi2s_clk_gpios[] = {
	{ GPIO_CFG(145, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_SCLK"},
	{ GPIO_CFG(144, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_WS"},
	{ GPIO_CFG(120, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_MCLK_A"},
};

static struct msm_gpio mi2s_rx_data_lines_gpios[] = {
	{ GPIO_CFG(121, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_DATA_SD0_A"},
	{ GPIO_CFG(122, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_DATA_SD1_A"},
	{ GPIO_CFG(123, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_DATA_SD2_A"},
	{ GPIO_CFG(146, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_DATA_SD3"},
};

static struct msm_gpio mi2s_tx_data_lines_gpios[] = {
	{ GPIO_CFG(146, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	    "MI2S_DATA_SD3"},
};

int mi2s_config_clk_gpio(void)
{
	int rc = 0;

	rc = msm_gpios_request_enable(mi2s_clk_gpios,
			ARRAY_SIZE(mi2s_clk_gpios));
	if (rc) {
		pr_err("%s: enable mi2s clk gpios  failed\n",
					__func__);
		return rc;
	}
	return 0;
}

int  mi2s_unconfig_data_gpio(u32 direction, u8 sd_line_mask)
{
	int i, rc = 0;
	sd_line_mask &= MI2S_SD_LINE_MASK;

	switch (direction) {
	case DIR_TX:
		msm_gpios_disable_free(mi2s_tx_data_lines_gpios, 1);
		break;
	case DIR_RX:
		i = 0;
		while (sd_line_mask) {
			if (sd_line_mask & 0x1)
				msm_gpios_disable_free(
					mi2s_rx_data_lines_gpios + i , 1);
			sd_line_mask = sd_line_mask >> 1;
			i++;
		}
		break;
	default:
		pr_err("%s: Invaild direction  direction = %u\n",
						__func__, direction);
		rc = -EINVAL;
		break;
	}
	return rc;
}

int mi2s_config_data_gpio(u32 direction, u8 sd_line_mask)
{
	int i , rc = 0;
	u8 sd_config_done_mask = 0;

	sd_line_mask &= MI2S_SD_LINE_MASK;

	switch (direction) {
	case DIR_TX:
		if ((sd_line_mask & MI2S_SD_0) || (sd_line_mask & MI2S_SD_1) ||
		   (sd_line_mask & MI2S_SD_2) || !(sd_line_mask & MI2S_SD_3)) {
			pr_err("%s: can not use SD0 or SD1 or SD2 for TX"
				".only can use SD3. sd_line_mask = 0x%x\n",
				__func__ , sd_line_mask);
			rc = -EINVAL;
		} else {
			rc = msm_gpios_request_enable(mi2s_tx_data_lines_gpios,
							 1);
			if (rc)
				pr_err("%s: enable mi2s gpios for TX failed\n",
					   __func__);
		}
		break;
	case DIR_RX:
		i = 0;
		while (sd_line_mask && (rc == 0)) {
			if (sd_line_mask & 0x1) {
				rc = msm_gpios_request_enable(
					mi2s_rx_data_lines_gpios + i , 1);
				if (rc) {
					pr_err("%s: enable mi2s gpios for"
					 "RX failed.  SD line = %s\n",
					 __func__,
					 (mi2s_rx_data_lines_gpios + i)->label);
					mi2s_unconfig_data_gpio(DIR_RX,
						sd_config_done_mask);
				} else
					sd_config_done_mask |= (1 << i);
			}
			sd_line_mask = sd_line_mask >> 1;
			i++;
		}
		break;
	default:
		pr_err("%s: Invaild direction  direction = %u\n",
			__func__, direction);
		rc = -EINVAL;
		break;
	}
	return rc;
}

int mi2s_unconfig_clk_gpio(void)
{
	msm_gpios_disable_free(mi2s_clk_gpios, ARRAY_SIZE(mi2s_clk_gpios));
	return 0;
}

#endif /* CONFIG_MSM7KV2_AUDIO */

static int __init buses_init(void)
{
	if (gpio_tlmm_config(GPIO_CFG(PMIC_GPIO_INT, 1, GPIO_CFG_INPUT,
				  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE))
		pr_err("%s: gpio_tlmm_config (gpio=%d) failed\n",
		       __func__, PMIC_GPIO_INT);

	pm8058_7x30_data.keypad_pdata = mogami_keypad_data();

	return 0;
}


static struct regulator *vreg_marimba_1;
static struct regulator *vreg_marimba_2;

static unsigned int msm_marimba_setup_power(void)
{
	int rc;

	rc = regulator_enable(vreg_marimba_1);
	if (rc) {
		pr_err("%s: regulator_enable failed (%d)\n", __func__, rc);
		goto out;
	}

	rc = regulator_enable(vreg_marimba_2);
	if (rc) {
		pr_err("%s: regulator_enable failed (%d)\n", __func__, rc);
		goto disable_marimba_1;
	}

	return 0;

	regulator_disable(vreg_marimba_2);
disable_marimba_1:
	regulator_disable(vreg_marimba_1);
out:
	return rc;
};

static void msm_marimba_shutdown_power(void)
{
	int rc;

	rc = regulator_disable(vreg_marimba_2);
	if (rc)
		pr_err("%s: regulator_disable failed (%d)\n", __func__, rc);

	rc = regulator_disable(vreg_marimba_1);
	if (rc)
		pr_err("%s: regulator_disable failed (%d)\n", __func__, rc);
};

/* Slave id address for FM/CDC/QMEMBIST
 * Values can be programmed using Marimba slave id 0
 * should there be a conflict with other I2C devices
 * */
#define MARIMBA_SLAVE_ID_FM_ADDR	0x2A
#define MARIMBA_SLAVE_ID_CDC_ADDR	0x77
#define MARIMBA_SLAVE_ID_QMEMBIST_ADDR	0X66

static struct regulator_bulk_data codec_regs[] = {
	{ .supply = "s4", .min_uV = 2200000, .max_uV = 2200000 },
};

static int __init msm_marimba_codec_init(void)
{
	int rc = regulator_bulk_get(NULL, ARRAY_SIZE(codec_regs), codec_regs);

	if (rc) {
		pr_err("%s: could not get regulators: %d\n", __func__, rc);
		goto out;
	}

	rc = regulator_bulk_set_voltage(ARRAY_SIZE(codec_regs), codec_regs);
	if (rc) {
		pr_err("%s: could not set regulator voltages: %d\n",
				__func__, rc);
		goto reg_free;
	}

	return rc;

reg_free:
	regulator_bulk_free(ARRAY_SIZE(codec_regs), codec_regs);
out:
	return rc;
}

static int msm_marimba_codec_power(int vreg_on)
{
	int rc = vreg_on ?
		regulator_bulk_enable(ARRAY_SIZE(codec_regs), codec_regs) :
		regulator_bulk_disable(ARRAY_SIZE(codec_regs), codec_regs);

	if (rc) {
		pr_err("%s: could not %sable regulators: %d",
				__func__, vreg_on ? "en" : "dis", rc);
		return rc;
	}

	return 0;
}

static struct marimba_codec_platform_data mariba_codec_pdata = {
	.marimba_codec_power =  msm_marimba_codec_power,
#ifdef CONFIG_MARIMBA_CODEC
	.snddev_profile_init = msm_snddev_init,
#endif
};

static struct marimba_platform_data marimba_pdata = {
	.slave_id[MARIMBA_SLAVE_ID_FM]       = MARIMBA_SLAVE_ID_FM_ADDR,
	.slave_id[MARIMBA_SLAVE_ID_CDC]	     = MARIMBA_SLAVE_ID_CDC_ADDR,
	.slave_id[MARIMBA_SLAVE_ID_QMEMBIST] = MARIMBA_SLAVE_ID_QMEMBIST_ADDR,
	.marimba_setup = msm_marimba_setup_power,
	.marimba_shutdown = msm_marimba_shutdown_power,
	.codec = &mariba_codec_pdata,
	.tsadc_ssbi_adap = MARIMBA_SSBI_ADAP,
};

static void __init msm7x30_init_marimba(void)
{
	int rc;

	struct regulator_bulk_data regs[] = {
		{ .supply = "s3",   .min_uV = 1800000, .max_uV = 1800000 },
		{ .supply = "gp16", .min_uV = 1200000, .max_uV = 1200000 },
		{ .supply = "usb2", .min_uV = 1800000, .max_uV = 1800000 },
	};

	rc = msm_marimba_codec_init();

	if (rc) {
		pr_err("%s: msm_marimba_codec_init failed (%d)\n",
				__func__, rc);
		return;
	}

	rc = regulator_bulk_get(NULL, ARRAY_SIZE(regs), regs);

	if (rc) {
		pr_err("%s: could not get regulators: %d\n", __func__, rc);
		return;
	}

	rc = regulator_bulk_set_voltage(ARRAY_SIZE(regs), regs);

	if (rc) {
		pr_err("%s: could not set voltages: %d\n", __func__, rc);
		regulator_bulk_free(ARRAY_SIZE(regs), regs);
		return;
	}

	vreg_marimba_1 = regs[0].consumer;
	vreg_marimba_2 = regs[1].consumer;
}

#ifdef CONFIG_MSM7KV2_AUDIO
static struct resource msm_aictl_resources[] = {
	{
		.name = "aictl",
		.start = 0xa5000100,
		.end = 0xa5000100,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource msm_mi2s_resources[] = {
	{
		.name = "hdmi",
		.start = 0xac900000,
		.end = 0xac900038,
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "codec_rx",
		.start = 0xac940040,
		.end = 0xac940078,
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "codec_tx",
		.start = 0xac980080,
		.end = 0xac9800B8,
		.flags = IORESOURCE_MEM,
	}

};

static struct msm_lpa_platform_data lpa_pdata = {
	.obuf_hlb_size = 0x2BFF8,
	.dsp_proc_id = 0,
	.app_proc_id = 2,
	.nosb_config = {
		.llb_min_addr = 0,
		.llb_max_addr = 0x3ff8,
		.sb_min_addr = 0,
		.sb_max_addr = 0,
	},
	.sb_config = {
		.llb_min_addr = 0,
		.llb_max_addr = 0x37f8,
		.sb_min_addr = 0x3800,
		.sb_max_addr = 0x3ff8,
	}
};

static struct resource msm_lpa_resources[] = {
	{
		.name = "lpa",
		.start = 0xa5000000,
		.end = 0xa50000a0,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource msm_aux_pcm_resources[] = {

	{
		.name = "aux_codec_reg_addr",
		.start = 0xac9c00c0,
		.end = 0xac9c00c8,
		.flags = IORESOURCE_MEM,
	},
	{
		.name   = "aux_pcm_dout",
		.start  = 138,
		.end    = 138,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "aux_pcm_din",
		.start  = 139,
		.end    = 139,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "aux_pcm_syncout",
		.start  = 140,
		.end    = 140,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "aux_pcm_clkin_a",
		.start  = 141,
		.end    = 141,
		.flags  = IORESOURCE_IO,
	},
};

static struct platform_device msm_aux_pcm_device = {
	.name   = "msm_aux_pcm",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_aux_pcm_resources),
	.resource       = msm_aux_pcm_resources,
};

struct platform_device msm_aictl_device = {
	.name = "audio_interct",
	.id   = 0,
	.num_resources = ARRAY_SIZE(msm_aictl_resources),
	.resource = msm_aictl_resources,
};

struct platform_device msm_mi2s_device = {
	.name = "mi2s",
	.id   = 0,
	.num_resources = ARRAY_SIZE(msm_mi2s_resources),
	.resource = msm_mi2s_resources,
};

struct platform_device msm_lpa_device = {
	.name = "lpa",
	.id   = 0,
	.num_resources = ARRAY_SIZE(msm_lpa_resources),
	.resource = msm_lpa_resources,
	.dev		= {
		.platform_data = &lpa_pdata,
	},
};
#endif /* CONFIG_MSM7KV2_AUDIO */

#define DEC0_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC1_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
 #define DEC2_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
 #define DEC3_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC4_FORMAT (1<<MSM_ADSP_CODEC_MIDI)

static unsigned int dec_concurrency_table[] = {
	/* Audio LP */
	0,
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_MODE_LP)|
	(1<<MSM_ADSP_OP_DM)),

	/* Concurrency 1 */
	(DEC4_FORMAT),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),

	 /* Concurrency 2 */
	(DEC4_FORMAT),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),

	/* Concurrency 3 */
	(DEC4_FORMAT),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),

	/* Concurrency 4 */
	(DEC4_FORMAT),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),

	/* Concurrency 5 */
	(DEC4_FORMAT),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),

	/* Concurrency 6 */
	(DEC4_FORMAT),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
};

#define DEC_INFO(name, queueid, decid, nr_codec) { .module_name = name, \
	.module_queueid = queueid, .module_decid = decid, \
	.nr_codec_support = nr_codec}

#define DEC_INSTANCE(max_instance_same, max_instance_diff) { \
	.max_instances_same_dec = max_instance_same, \
	.max_instances_diff_dec = max_instance_diff}

static struct msm_adspdec_info dec_info_list[] = {
	DEC_INFO("AUDPLAY4TASK", 17, 4, 1),  /* AudPlay4BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY3TASK", 16, 3, 11),  /* AudPlay3BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY2TASK", 15, 2, 11),  /* AudPlay2BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY1TASK", 14, 1, 11),  /* AudPlay1BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY0TASK", 13, 0, 11), /* AudPlay0BitStreamCtrlQueue */
};

static struct dec_instance_table dec_instance_list[][MSM_MAX_DEC_CNT] = {
	/* Non Turbo Mode */
	{
		DEC_INSTANCE(4, 3), /* WAV */
		DEC_INSTANCE(4, 3), /* ADPCM */
		DEC_INSTANCE(4, 2), /* MP3 */
		DEC_INSTANCE(0, 0), /* Real Audio */
		DEC_INSTANCE(4, 2), /* WMA */
		DEC_INSTANCE(3, 2), /* AAC */
		DEC_INSTANCE(0, 0), /* Reserved */
		DEC_INSTANCE(0, 0), /* MIDI */
		DEC_INSTANCE(4, 3), /* YADPCM */
		DEC_INSTANCE(4, 3), /* QCELP */
		DEC_INSTANCE(4, 3), /* AMRNB */
		DEC_INSTANCE(1, 1), /* AMRWB/WB+ */
		DEC_INSTANCE(4, 3), /* EVRC */
		DEC_INSTANCE(1, 1), /* WMAPRO */
	},
	/* Turbo Mode */
	{
		DEC_INSTANCE(4, 3), /* WAV */
		DEC_INSTANCE(4, 3), /* ADPCM */
		DEC_INSTANCE(4, 3), /* MP3 */
		DEC_INSTANCE(0, 0), /* Real Audio */
		DEC_INSTANCE(4, 3), /* WMA */
		DEC_INSTANCE(4, 3), /* AAC */
		DEC_INSTANCE(0, 0), /* Reserved */
		DEC_INSTANCE(0, 0), /* MIDI */
		DEC_INSTANCE(4, 3), /* YADPCM */
		DEC_INSTANCE(4, 3), /* QCELP */
		DEC_INSTANCE(4, 3), /* AMRNB */
		DEC_INSTANCE(2, 3), /* AMRWB/WB+ */
		DEC_INSTANCE(4, 3), /* EVRC */
		DEC_INSTANCE(1, 2), /* WMAPRO */
	},
};

static struct msm_adspdec_database msm_device_adspdec_database = {
	.num_dec = ARRAY_SIZE(dec_info_list),
	.num_concurrency_support = (ARRAY_SIZE(dec_concurrency_table) / \
					ARRAY_SIZE(dec_info_list)),
	.dec_concurrency_table = dec_concurrency_table,
	.dec_info_list = dec_info_list,
	.dec_instance_list = &dec_instance_list[0][0],
};

static struct platform_device msm_device_adspdec = {
	.name = "msm_adspdec",
	.id = -1,
	.dev    = {
		.platform_data = &msm_device_adspdec_database
	},
};


#ifdef CONFIG_USB_G_ANDROID
static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id		= -1,
	.dev		= {
		.platform_data = &android_usb_pdata,
	},
};
#endif

#ifdef CONFIG_FB_MSM_MDDI_NOVATEK_FWVGA
static int novatek_reset(void)
{
	msleep(10);
	gpio_set_value(NOVATEK_GPIO_RESET, 1);
	msleep(5); /* hw spec says: 2 ms */
	gpio_set_value(NOVATEK_GPIO_RESET, 0);
	msleep(5); /* hw spec says: 2 ms */
	gpio_set_value(NOVATEK_GPIO_RESET, 1);
	msleep(30); /* hw spec says: 20 ms */
	return 0;
}

static struct novatek_fwvga_platform_data novatek_platform_data = {
	.power = NULL,
	.reset = novatek_reset,
};

static struct platform_device novatek_device = {
	.name	= MDDI_NOVATEK_FWVGA_NAME,
	.id	= -1,
	.dev	= {
		.platform_data = &novatek_platform_data,
	}
};
#endif

#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
static struct msm_gpio sii9024_gpio_config_data_enable[] = {
	{ GPIO_CFG(90, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
		"HDMI_INT" },
	{ GPIO_CFG(93, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"HDMI_5V_EN" },
	{ GPIO_CFG(102, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"HDMI_PWR_EN" },
	{ GPIO_CFG(105, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"HDMI_RESET_N" },

	{ GPIO_CFG(124, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_6MA),
		"DTV_PCLK" },
	{ GPIO_CFG(125, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_EN" },
	{ GPIO_CFG(126, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_VSYNC" },
	{ GPIO_CFG(127, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_HSYNC" },

	{ GPIO_CFG(128, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA0" },
	{ GPIO_CFG(129, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA1" },
	{ GPIO_CFG(130, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA2" },
	{ GPIO_CFG(131, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA3" },
	{ GPIO_CFG(132, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA4" },
	{ GPIO_CFG(160, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA5" },
	{ GPIO_CFG(161, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA6" },
	{ GPIO_CFG(162, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA7" },
	{ GPIO_CFG(163, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA8" },
	{ GPIO_CFG(164, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA9" },
	{ GPIO_CFG(165, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA10" },
	{ GPIO_CFG(166, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA11" },
	{ GPIO_CFG(167, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA12" },
	{ GPIO_CFG(168, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA13" },
	{ GPIO_CFG(169, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA14" },
	{ GPIO_CFG(170, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA15" },
	{ GPIO_CFG(171, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA16" },
	{ GPIO_CFG(172, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA17" },
	{ GPIO_CFG(173, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA18" },
	{ GPIO_CFG(174, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA19" },
	{ GPIO_CFG(175, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA20" },
	{ GPIO_CFG(176, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA21" },
	{ GPIO_CFG(177, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA22" },
	{ GPIO_CFG(178, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
		"DTV_DATA23" },
};

static struct msm_gpio sii9024_gpio_config_data_disable[] = {
	{ GPIO_CFG(90, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"HDMI_INT" },
	{ GPIO_CFG(93, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"HDMI_5V_EN" },
	{ GPIO_CFG(102, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"HDMI_PWR_EN" },
	{ GPIO_CFG(105, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"HDMI_RESET_N" },

	{ GPIO_CFG(124, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_PCLK" },
	{ GPIO_CFG(125, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_EN" },
	{ GPIO_CFG(126, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_VSYNC" },
	{ GPIO_CFG(127, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_HSYNC" },

	{ GPIO_CFG(128, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA0" },
	{ GPIO_CFG(129, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA1" },
	{ GPIO_CFG(130, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA2" },
	{ GPIO_CFG(131, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA3" },
	{ GPIO_CFG(132, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA4" },
	{ GPIO_CFG(160, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA5" },
	{ GPIO_CFG(161, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA6" },
	{ GPIO_CFG(162, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA7" },
	{ GPIO_CFG(163, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA8" },
	{ GPIO_CFG(164, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA9" },
	{ GPIO_CFG(165, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA10" },
	{ GPIO_CFG(166, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA11" },
	{ GPIO_CFG(167, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA12" },
	{ GPIO_CFG(168, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA13" },
	{ GPIO_CFG(169, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA14" },
	{ GPIO_CFG(170, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA15" },
	{ GPIO_CFG(171, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA16" },
	{ GPIO_CFG(172, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA17" },
	{ GPIO_CFG(173, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA18" },
	{ GPIO_CFG(174, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA19" },
	{ GPIO_CFG(175, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA20" },
	{ GPIO_CFG(176, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA21" },
	{ GPIO_CFG(177, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA22" },
	{ GPIO_CFG(178, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		"DTV_DATA23" },
};

static int hdmi_sii_panel_power(int on)
{
	int flag_on = !!on;
	static int dtv_power_save_on;
	struct vreg *vreg_ldo23;
	int rc;
	if (dtv_power_save_on == flag_on)
		return 0;

	dtv_power_save_on = flag_on;

	if (on) {
		rc = msm_gpios_request_enable(sii9024_gpio_config_data_enable,
				ARRAY_SIZE(sii9024_gpio_config_data_enable));
		if (rc < 0) {
			printk(KERN_ERR "%s: gpio config failed: %d\n",
				__func__, rc);
			return rc;
		}
	} else {
		rc = msm_gpios_enable(sii9024_gpio_config_data_disable,
				ARRAY_SIZE(sii9024_gpio_config_data_disable));
		msm_gpios_free(sii9024_gpio_config_data_disable,
				ARRAY_SIZE(sii9024_gpio_config_data_disable));
	}

	/*  -- LDO23 for HDMI */
	vreg_ldo23 = vreg_get(NULL, "gp5");

	if (IS_ERR(vreg_ldo23)) {
		printk(KERN_ERR "%s:  vreg23 get failed (%ld)\n",
			__func__, PTR_ERR(vreg_ldo23));
		return rc;
	}

	rc = vreg_set_level(vreg_ldo23, 1200);
	if (rc) {
		printk(KERN_ERR "%s: vreg LDO23 set level failed (%d)\n",
			__func__, rc);
		return rc;
	}

	if (on)
		rc = vreg_enable(vreg_ldo23);
	else
		rc = vreg_disable(vreg_ldo23);

	if (rc) {
		printk(KERN_ERR "%s: LDO23 vreg enable failed (%d)\n",
			__func__, rc);
		return rc;
	}

	mdelay(5);		/* ensure power is stable */

	return rc;
}

static struct platform_device hdmi_sii9024a_panel_device = {
	.name   = "sii9024a",
	.id     = 2,
};

static struct sii9024_platform_data sii9024_platform_data = {
	.setchippower        = hdmi_sii_panel_power,
	/* set panel_info */
	.xres               = 1280,
	.yres               = 720,
	.type               = 7, /* DTV_PANEL */
	.pdest              = 1, /* DISPLAY_2 */
	.wait_cycle         = 0,
	.bpp                = 24,
	.fb_num             = 1,
	.clk_rate           = 74250000,
	.lcdc_h_back_porch  = 220,
	.lcdc_h_front_porch = 110,
	.lcdc_h_pulse_width = 40,
	.lcdc_v_back_porch  = 20,
	.lcdc_v_front_porch = 5,
	.lcdc_v_pulse_width = 5,
	.lcdc_border_clr    = 0, /* blk */
	.lcdc_underflow_clr = 0xff, /* blue */
	.lcdc_hsync_skew    = 0,
};
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */

#if defined(CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)
/*  Generic LCD Regulators On function for SEMC mogami displays */
static void semc_mogami_lcd_regulators_on(void)
{
	vreg_helper_on("gp7",1800);  /* L8 */
#ifdef CONFIG_MSM_UNDERVOLT_LCD
	vreg_helper_on("gp6",2300);  /* L15 */
#else
	vreg_helper_on("gp6",2850);  /* L15 */
#endif
}

/* Generic Power On function for SEMC mogami displays */
static void semc_mogami_lcd_power_on(u8 delay1, u8 delay2, u8 delay3)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_MDDI_XRES,
			0,
			GPIO_CFG_OUTPUT,
			GPIO_CFG_NO_PULL,
			GPIO_CFG_2MA),
			GPIO_CFG_ENABLE );
	gpio_set_value(GPIO_MSM_MDDI_XRES,0);
	semc_mogami_lcd_regulators_on();
	mdelay(delay1);
	gpio_set_value(GPIO_MSM_MDDI_XRES,0);
	mdelay(delay2);
	gpio_set_value(GPIO_MSM_MDDI_XRES,1);
	mdelay(delay3);
}
#endif  /* (CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD) ||
	(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD) ||
	(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD) ||
	(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)*/

#if defined(CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD)
/* Display resolutin */
#define SONY_HVGA_PANEL_XRES 320
#define SONY_HVGA_PANEL_YRES 480

static void sony_hvga_lcd_power_on(void)
{
	semc_mogami_lcd_regulators_on();
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(11);           /* spec > 10 ms*/
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(2);           /* spec > 1 ms*/
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(21); /* spec > 20 ms */
}

static void sony_hvga_lcd_power_off(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(121); /* spec > 120ms */
	vreg_helper_off("gp7");  /* L8 */
	vreg_helper_off("gp6");  /* L15 */
}

static void sony_hvga_lcd_exit_deep_standby(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(4);   /* spec: > 3 ms */
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(11);  /* spec: > 10 ms */
}

static struct sony_hvga_platform_data sony_hvga_panel_ext = {
	.power_on = sony_hvga_lcd_power_on,
	.power_off = sony_hvga_lcd_power_off,
	.exit_deep_standby = sony_hvga_lcd_exit_deep_standby,
};

static struct platform_device mddi_sony_hvga_display_device = {
	.name = "mddi_sony_s6d05a1_hvga",
	.id = -1,
	.dev = {
		.platform_data = &sony_hvga_panel_ext,
	}
};
#endif  /* (CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD) */

#if defined(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD)
/* Display resolution */
#define HITACHI_HVGA_PANEL_XRES 320
#define HITACHI_HVGA_PANEL_YRES 480

static void hitachi_hvga_lcd_power_on(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	semc_mogami_lcd_regulators_on();
	msleep(1);           /* spec > 310us*/
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(11); /* spec > 10 */
}

static void hitachi_hvga_lcd_power_off(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(121); /* spec > 120ms */
	vreg_helper_off("gp7");  /* L8 */
	vreg_helper_off("gp6");  /* L15 */
}

static void hitachi_hvga_lcd_exit_deep_standby(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(2);   /* spec: > 1ms */
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(6);  /* spec: > 5 ms */
}

static struct msm_fb_panel_data hitachi_hvga_panel_data = {
	.panel_info = {
		.xres = HITACHI_HVGA_PANEL_XRES,
		.yres = HITACHI_HVGA_PANEL_YRES,
		.pdest = DISPLAY_1,
		.type = MDDI_PANEL,
		.wait_cycle = 0,
		.bpp = 24,
		.clk_rate = 192000000,
		.clk_min =  190000000,
		.clk_max =  200000000,
		.fb_num = 2,
		.bl_max = 4,
		.bl_min = 1,
		.width = 42,
		.height = 63,
	},
};

static struct hitachi_hvga_platform_data hitachi_hvga_panel_ext = {
	.power_on = hitachi_hvga_lcd_power_on,
	.power_off = hitachi_hvga_lcd_power_off,
	.exit_deep_standby = hitachi_hvga_lcd_exit_deep_standby,
	.dbc_on = 1,
	.dbc_mode = DBC_MODE_VIDEO,
	.panel_data = &hitachi_hvga_panel_data,
};

static struct platform_device mddi_hitachi_hvga_display_device = {
	.name = MDDI_HITACH_R61529_HVGA_NAME,
	.id = -1,
	.dev = {
		.platform_data = &hitachi_hvga_panel_ext,
	}
};
#endif   /* CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD  */

#if defined(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD)
/* Display resolution */
#define SII_HVGA_PANEL_XRES 320
#define SII_HVGA_PANEL_YRES 480

static void sii_hvga_lcd_power_on(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	semc_mogami_lcd_regulators_on();
	msleep(1);           /* spec > 310us*/
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(11); /* spec > 10 */
}

static void sii_hvga_lcd_power_off(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(121); /* spec > 120ms */
	vreg_helper_off("gp7");  /* L8 */
	vreg_helper_off("gp6");  /* L15 */
}

static void sii_hvga_lcd_exit_deep_standby(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(2);   /* spec: > 1ms */
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(6);  /* spec: > 5 ms */
}

static struct msm_fb_panel_data sii_hvga_panel_data = {
	.panel_info = {
		.xres = SII_HVGA_PANEL_XRES,
		.yres = SII_HVGA_PANEL_YRES,
		.pdest = DISPLAY_1,
		.type = MDDI_PANEL,
		.wait_cycle = 0,
		.bpp = 24,
		.clk_rate = 192000000,
		.clk_min =  190000000,
		.clk_max =  200000000,
		.fb_num = 2,
		.bl_max = 4,
		.bl_min = 1,
		.width = 42,
		.height = 63,
	},
};

static struct sii_hvga_platform_data sii_hvga_panel_ext = {
	.power_on = sii_hvga_lcd_power_on,
	.power_off = sii_hvga_lcd_power_off,
	.exit_deep_standby = sii_hvga_lcd_exit_deep_standby,
	.dbc_on = 1,
	.dbc_mode = DBC_MODE_VIDEO,
	.panel_data = &sii_hvga_panel_data,
};

static struct platform_device mddi_sii_hvga_display_device = {
	.name = MDDI_SII_R61529_HVGA_NAME,
	.id = -1,
	.dev = {
		.platform_data = &sii_hvga_panel_ext,
	}
};
#endif   /* CONFIG_FB_MSM_MDDI_SII_HVGA_LCD  */

#if defined(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)
/* Display resolution */
#define AUO_HVGA_PANEL_XRES 320
#define AUO_HVGA_PANEL_YRES 480

static void auo_hvga_lcd_power_on(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	semc_mogami_lcd_regulators_on();
	msleep(2);           /* spec > 1 ms*/
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(51); /* spec > 50 ms */
}

static void auo_hvga_lcd_power_off(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(121); /* spec > 120ms */
	vreg_helper_off("gp7");  /* L8 */
	vreg_helper_off("gp6");  /* L15 */
}

static void auo_hvga_lcd_exit_deep_standby(void)
{
	gpio_set_value(GPIO_MSM_MDDI_XRES, 0);
	msleep(2);   /* spec: > 1ms */
	gpio_set_value(GPIO_MSM_MDDI_XRES, 1);
	msleep(51);  /* spec: > 50 ms */
}

static struct msm_fb_panel_data auo_hvga_panel_data = {
	.panel_info = {
		.xres = AUO_HVGA_PANEL_XRES,
		.yres = AUO_HVGA_PANEL_YRES,
		.pdest = DISPLAY_1,
		.type = MDDI_PANEL,
		.wait_cycle = 0,
		.bpp = 24,
		.clk_rate = 192000000,
		.clk_min =  190000000,
		.clk_max =  200000000,
		.fb_num = 2,
		.bl_max = 4,
		.bl_min = 1,
		.width = 42,
		.height = 63,
	},
};

static struct auo_hvga_platform_data auo_hvga_panel_ext = {
	.power_on = auo_hvga_lcd_power_on,
	.power_off = auo_hvga_lcd_power_off,
	.exit_deep_standby = auo_hvga_lcd_exit_deep_standby,
	.dbc_on = 1,
	.dbc_mode = DBC_MODE_VIDEO,
	.panel_data = &auo_hvga_panel_data,
};

static struct platform_device mddi_auo_hvga_display_device = {
	.name = MDDI_AUO_S6D05A1_HVGA_NAME,
	.id = -1,
	.dev = {
		.platform_data = &auo_hvga_panel_ext,
	}
};
#endif   /* CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD  */

#if defined(CONFIG_TOUCHSCREEN_CY8CTMA300_SPI) || \
	defined(CONFIG_TOUCHSCREEN_CYTTSP_SPI)
struct msm_gpio ttsp_gpio_cfg_data[] = {
	{ GPIO_CFG(42, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
		"ttsp_irq" },
};
#endif

#ifdef CONFIG_TOUCHSCREEN_CY8CTMA300_SPI
static int cypress_touch_gpio_init(void);
static int cypress_touch_spi_cs_set(bool val);

static struct cypress_touch_platform_data cypress_touch_data = {
	.x_max = CONFIG_CY8CTMA300_SPI_MAX_X,
	.y_max = CONFIG_CY8CTMA300_SPI_MAX_Y,
	.z_max = CONFIG_CY8CTMA300_SPI_MAX_Z,
	.width_major = CONFIG_CY8CTMA300_SPI_WIDTH_MAJOR,
	.gpio_init = cypress_touch_gpio_init,
	.gpio_irq_pin = CYPRESS_TOUCH_GPIO_IRQ,
	.gpio_reset_pin = CYPRESS_TOUCH_GPIO_RESET,
	.spi_cs_set = cypress_touch_spi_cs_set,
};

static int cypress_touch_gpio_init(void)
{
	int rc;

	msleep(10);

	rc = msm_gpios_enable(ttsp_gpio_cfg_data,
				ARRAY_SIZE(ttsp_gpio_cfg_data));
	if (rc)
		return rc;

	gpio_set_value(CYPRESS_TOUCH_GPIO_RESET, 1);
	return 0;
}

static int cypress_touch_spi_cs_set(bool val)
{
	int rc = 0;
	int cfg;

	if (val) {
		gpio_set_value(CYPRESS_TOUCH_GPIO_SPI_CS, 1);
		cfg = GPIO_CFG(CYPRESS_TOUCH_GPIO_SPI_CS, 1, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA);
		rc = gpio_tlmm_config(cfg, GPIO_CFG_ENABLE);
		if (rc)
			pr_err("%s: Enabling of GPIO failed. "
				"gpio_tlmm_config(%#x, enable)=%d\n",
				__func__, cfg, rc);
	} else {
		cfg = GPIO_CFG(CYPRESS_TOUCH_GPIO_SPI_CS, 0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA);
		rc = gpio_tlmm_config(cfg, GPIO_CFG_ENABLE);
		if (rc)
			pr_err("%s: Enabling of GPIO failed. "
				"gpio_tlmm_config(%#x, enable)=%d\n",
				__func__, cfg, rc);
		gpio_set_value(CYPRESS_TOUCH_GPIO_SPI_CS, 0);
	}
	return rc;
}
#endif /* CONFIG_TOUCHSCREEN_CY8CTMA300_SPI */

#ifdef CONFIG_TOUCHSCREEN_CYTTSP_SPI
int cyttsp_xres(void)
{
	int polarity;
	int rc;
	rc = gpio_direction_input(CYPRESS_TOUCH_GPIO_RESET);
	if (rc) {
		printk(KERN_ERR "%s: failed to set direction input, %d\n",
		       __func__, rc);
		return -EIO;
	}
	polarity = gpio_get_value(CYPRESS_TOUCH_GPIO_RESET) & 0x01;
	printk(KERN_INFO "%s: %d\n", __func__, polarity);
	rc = gpio_direction_output(CYPRESS_TOUCH_GPIO_RESET, polarity ^ 1);
	if (rc) {
		printk(KERN_ERR "%s: failed to set direction output, %d\n",
		       __func__, rc);
		return -EIO;
	}
	msleep(1);
	gpio_set_value(CYPRESS_TOUCH_GPIO_RESET, polarity);
	return 0;
}

int cyttsp_init(int on)
{
	int rc = -1;
	if (on) {
		if (gpio_request(CYPRESS_TOUCH_GPIO_IRQ, "ttsp_irq"))
			goto ttsp_irq_err;
		if (gpio_request(CYPRESS_TOUCH_GPIO_RESET, "ttsp_reset"))
			goto ttsp_reset_err;

		rc = msm_gpios_enable(ttsp_gpio_cfg_data,
					ARRAY_SIZE(ttsp_gpio_cfg_data));
		if (rc)
			goto ttsp_gpio_cfg_err;
		return 0;
	} else {
		rc = 0;
	}
ttsp_gpio_cfg_err:
	gpio_free(CYPRESS_TOUCH_GPIO_RESET);
ttsp_reset_err:
	gpio_free(CYPRESS_TOUCH_GPIO_IRQ);
ttsp_irq_err:
	return rc;
}

int cyttsp_wakeup(void)
{
	int ret;

	ret = gpio_direction_output(CYPRESS_TOUCH_GPIO_IRQ, 0);
	if (ret) {
		printk(KERN_ERR "%s: Failed to request gpio_direction_output\n",
		__func__);
                return ret;
	}
	msleep(50);
	gpio_set_value(CYPRESS_TOUCH_GPIO_IRQ, 0);
	msleep(1);
	gpio_set_value(CYPRESS_TOUCH_GPIO_IRQ, 1);
	udelay(100);
	gpio_set_value(CYPRESS_TOUCH_GPIO_IRQ, 0);
	msleep(1);
	gpio_set_value(CYPRESS_TOUCH_GPIO_IRQ, 1);
	printk(KERN_INFO "%s: wakeup\n", __func__);
	ret = gpio_direction_input(CYPRESS_TOUCH_GPIO_IRQ);
	if (ret) {
		printk(KERN_ERR "%s: Failed to request gpio_direction_input\n",
		__func__);
		return ret;
	}
	msleep(50);
	return 0;
}

#ifdef CONFIG_TOUCHSCREEN_CYTTSP_KEY
#define TT_KEY_BACK_FLAG	0x01
#define TT_KEY_MENU_FLAG	0x02
#define TT_KEY_HOME_FLAG	0x04

static struct input_dev *input_dev_cyttsp_key;

static int __init cyttsp_key_init(void)
{
	input_dev_cyttsp_key = input_allocate_device();
	if (!input_dev_cyttsp_key) {
		pr_err("%s: Error, unable to alloc cyttsp key device\n", __func__);
		return -ENOMEM;
	}
	input_dev_cyttsp_key->name = "cyttsp_key";
	input_dev_cyttsp_key->phys = "/sys/bus/spi/devices/spi0.0/";
	input_set_capability(input_dev_cyttsp_key, EV_KEY, KEY_MENU);
	input_set_capability(input_dev_cyttsp_key, EV_KEY, KEY_BACK);
	input_set_capability(input_dev_cyttsp_key, EV_KEY, KEY_HOME);
	if (input_register_device(input_dev_cyttsp_key)) {
		pr_err("%s: Error, unable to reg cyttsp key device\n", __func__);
		input_free_device(input_dev_cyttsp_key);
		return -ENODEV;
	}
	return 0;
}
module_init(cyttsp_key_init);

int cyttsp_key_rpc_callback(u8 data[], int size)
{
	static u8 last;
	u8 toggled = last ^ data[0];

	if (toggled & TT_KEY_MENU_FLAG)
		input_report_key(input_dev_cyttsp_key, KEY_MENU,
			!!(*data & TT_KEY_MENU_FLAG));

	if (toggled & TT_KEY_BACK_FLAG)
		input_report_key(input_dev_cyttsp_key, KEY_BACK,
			!!(*data & TT_KEY_BACK_FLAG));

	if (toggled & TT_KEY_HOME_FLAG)
		input_report_key(input_dev_cyttsp_key, KEY_HOME,
			!!(*data & TT_KEY_HOME_FLAG));

	input_sync(input_dev_cyttsp_key);
	last = data[0];
	return 0;
}
#endif /* CONFIG_TOUCHSCREEN_CYTTSP_KEY */

#endif /* CONFIG_TOUCHSCREEN_CYTTSP_SPI */

#ifdef CONFIG_TOUCHSCREEN_CLEARPAD
static struct msm_gpio clearpad_gpio_config_data[] = {
	{ GPIO_CFG(SYNAPTICS_TOUCH_GPIO_IRQ, 0, GPIO_CFG_INPUT,
		   GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "clearpad3000_irq" },
};

static int clearpad_gpio_configure(int enable)
{
	int rc = 0;

	if (enable)
		rc = msm_gpios_request_enable(clearpad_gpio_config_data,
				ARRAY_SIZE(clearpad_gpio_config_data));
	else
		msm_gpios_free(clearpad_gpio_config_data,
				ARRAY_SIZE(clearpad_gpio_config_data));
	return rc;
}

static struct synaptics_button synaptics_menu_key = {
	.type = EV_KEY,
	.code = KEY_MENU,
};

static struct synaptics_button synaptics_back_key = {
	.type = EV_KEY,
	.code = KEY_BACK,
};

static struct synaptics_funcarea clearpad_funcarea_array[] = {
	{ 0, 0, 479, 853, SYN_FUNCAREA_POINTER, NULL },
	{ 0, 854, 479, 863, SYN_FUNCAREA_BOTTOM_EDGE, NULL},
	{ 0, 884, 159, 921, SYN_FUNCAREA_BUTTON, &synaptics_back_key },
	{ 0, 864, 179, 921, SYN_FUNCAREA_BTN_INBOUND, &synaptics_back_key },
	{ 320, 884, 479, 921, SYN_FUNCAREA_BUTTON, &synaptics_menu_key },
	{ 300, 864, 479, 921, SYN_FUNCAREA_BTN_INBOUND, &synaptics_menu_key },
	{ .func = SYN_FUNCAREA_END }
};

static void clearpad_vreg_off(void)
{
	int i;

	vreg_helper_off(VREG_L20);
	for (i = 0; i < 500; i++)
		udelay(1000);
}

static struct clearpad_platform_data clearpad_platform_data = {
	.irq = MSM_GPIO_TO_INT(SYNAPTICS_TOUCH_GPIO_IRQ),
	.funcarea = clearpad_funcarea_array,
	.gpio_configure = clearpad_gpio_configure,
	.vreg_off = clearpad_vreg_off,
};
#endif

#ifdef CONFIG_SEMC_CHARGER_USB_ARCH
static char *semc_chg_usb_supplied_to[] = {
	BATTERY_CHARGALG_NAME,
	BQ27520_NAME,
};
#endif

static char *semc_bdata_supplied_to[] = {
	BQ27520_NAME,
	BATTERY_CHARGALG_NAME,
};

static struct semc_battery_platform_data semc_battery_platform_data = {
	.supplied_to = semc_bdata_supplied_to,
	.num_supplicants = ARRAY_SIZE(semc_bdata_supplied_to),
};

static struct platform_device bdata_driver = {
	.name = SEMC_BDATA_NAME,
	.id = -1,
	.dev = {
		.platform_data = &semc_battery_platform_data,
	},
};

#define GPIO_BQ27520_SOC_INT 20
#define LIPO_BAT_MAX_VOLTAGE 4200
#define LIPO_BAT_MIN_VOLTAGE 3000
#define FULLY_CHARGED_AND_RECHARGE_CAP 95

static char *bq27520_supplied_to[] = {
	BATTERY_CHARGALG_NAME,
};

static struct bq27520_block_table bq27520_block_table[BQ27520_BTBL_MAX] = {
	{0x61, 0x00}, {0x3E, 0x24}, {0x3F, 0x00}, {0x42, 0x00},
	{0x43, 0x46}, {0x44, 0x00}, {0x45, 0x19}, {0x46, 0x00},
	{0x47, 0x64}, {0x48, 0x28}, {0x4B, 0xFF}, {0x4C, 0x5F},
	{0x60, 0xF4}
};

static int bq27520_gpio_configure(int enable)
{
	int rc = 0;

	if (!!enable) {
		rc = gpio_request(GPIO_BQ27520_SOC_INT, "bq27520");
		if (rc)
			pr_err("%s: gpio_requeset failed, "
					"rc=%d\n", __func__, rc);
	} else {
		gpio_free(GPIO_BQ27520_SOC_INT);
	}
	return rc;
}

struct bq27520_platform_data bq27520_platform_data = {
	.name = BQ27520_NAME,
	.supplied_to = bq27520_supplied_to,
	.num_supplicants = ARRAY_SIZE(bq27520_supplied_to),
	.lipo_bat_max_volt = LIPO_BAT_MAX_VOLTAGE,
	.lipo_bat_min_volt = LIPO_BAT_MIN_VOLTAGE,
	.battery_dev_name = SEMC_BDATA_NAME,
	.gpio_configure = bq27520_gpio_configure,
	.polling_lower_capacity = FULLY_CHARGED_AND_RECHARGE_CAP,
	.polling_upper_capacity = 100,
	.udatap = bq27520_block_table,
#ifdef CONFIG_BATTERY_CHARGALG
	.disable_algorithm = battery_chargalg_disable,
#endif
};

#ifdef CONFIG_CHARGER_BQ24185
static char *bq24185_supplied_to[] = {
	BATTERY_CHARGALG_NAME,
	SEMC_BDATA_NAME,
};

struct bq24185_platform_data bq24185_platform_data = {
	.name = BQ24185_NAME,
	.supplied_to = bq24185_supplied_to,
	.num_supplicants = ARRAY_SIZE(bq24185_supplied_to),
	.support_boot_charging = 1,
	.rsens = BQ24185_RSENS_REF,
	/* Maximum battery regulation voltage = 4200mV */
	.mbrv = BQ24185_MBRV_MV_4200,
	/* Maximum charger current sense voltage = 71.4mV */
	.mccsv = BQ24185_MCCSV_MV_6p8 | BQ24185_MCCSV_MV_27p2 |
		BQ24185_MCCSV_MV_37p4,
#ifdef CONFIG_USB_MSM_OTG_72K
	.notify_vbus_drop = msm_otg_notify_vbus_drop,
#endif
	.vindpm_usb_compliant = VINDPM_4550MV,
	.vindpm_non_compliant = VINDPM_4390MV,
};
#endif

static char *battery_chargalg_supplied_to[] = {
	SEMC_BDATA_NAME,
};

static struct battery_chargalg_platform_data battery_chargalg_platform_data = {
	.name = BATTERY_CHARGALG_NAME,
	.supplied_to = battery_chargalg_supplied_to,
	.num_supplicants = ARRAY_SIZE(battery_chargalg_supplied_to),
	.ext_eoc_recharge_enable = 1,
	.temp_hysteresis_design = 3,
	.ddata = &device_data,
	.batt_volt_psy_name = BQ27520_NAME,
	.batt_curr_psy_name = BQ27520_NAME,
#ifdef CONFIG_CHARGER_BQ24185
	.turn_on_charger = bq24185_turn_on_charger,
	.turn_off_charger = bq24185_turn_off_charger,
	.set_charger_voltage = bq24185_set_charger_voltage,
	.set_charger_current = bq24185_set_charger_current,
	.set_input_current_limit = bq24185_set_input_current_limit,
	.set_charging_status = bq24185_set_ext_charging_status,
	.get_supply_current_limit = NULL,
	.get_restrict_ctl = NULL,
	.get_restricted_setting = NULL,
	.setup_exchanged_power_supply = NULL,
	.charge_set_current_1 = 350,
	.charge_set_current_2 = 550,
	.charge_set_current_3 = 750,
	.overvoltage_max_design = 4225,
#endif
#ifdef CONFIG_SEMC_CHARGER_USB_ARCH
	.get_supply_current_limit = semc_charger_usb_current_ma,
#endif
	.allow_dynamic_charge_current_ctrl = 1,
	.average_current_min_limit = -1,
	.average_current_max_limit = 250,
};

static struct platform_device battery_chargalg_platform_device = {
	.name = BATTERY_CHARGALG_NAME,
	.id = -1,
	.dev = {
		.platform_data = &battery_chargalg_platform_data,
	},
};

#if defined(CONFIG_LM3560) || defined(CONFIG_LM3561)
int lm356x_request_gpio_pins(void)
{
	int result;

	result = gpio_request(LM356X_HW_RESET_GPIO, "LM356X hw reset");
	if (result)
		return result;

	gpio_set_value(LM356X_HW_RESET_GPIO, 1);

	udelay(20);
	return result;
}

int lm356x_release_gpio_pins(void)
{

	gpio_set_value(LM356X_HW_RESET_GPIO, 0);
	gpio_free(LM356X_HW_RESET_GPIO);

	return 0;
}
#endif

#ifdef CONFIG_LM3560
static struct lm356x_platform_data lm3560_platform_data = {
	.hw_enable              = lm356x_request_gpio_pins,
	.hw_disable             = lm356x_release_gpio_pins,
	.led_nums		= 2,
	.strobe_trigger		= LM356X_STROBE_TRIGGER_EDGE,
	.privacy_terminate	= LM356X_PRIVACY_MODE_TURN_BACK,
	.privacy_led_nums	= 1,
	.privacy_blink_period	= 0, /* No bliking */
	.current_limit		= 2300000, /* uA */
	.flash_sync		= LM356X_SYNC_OFF,
	.strobe_polarity	= LM356X_STROBE_POLARITY_HIGH,
	.ledintc_pin_setting	= LM356X_LEDINTC_NTC_THERMISTOR_INPUT,
	.tx1_polarity		= LM356X_TX1_POLARITY_HIGH,
	.tx2_polarity		= LM356X_TX2_POLARITY_HIGH,
	.hw_torch_mode		= LM356X_HW_TORCH_MODE_DISABLE,
};
#endif
#ifdef CONFIG_LM3561
static struct lm356x_platform_data lm3561_platform_data = {
	.hw_enable              = lm356x_request_gpio_pins,
	.hw_disable             = lm356x_release_gpio_pins,
	.led_nums		= 1,
	.strobe_trigger		= LM356X_STROBE_TRIGGER_EDGE,
	.privacy_terminate	= LM356X_PRIVACY_MODE_TURN_BACK,
	.privacy_led_nums	= 0,
	.privacy_blink_period	= 0, /* No bliking */
	.current_limit		= 1000000, /* uA
				   selectable value are 1500mA or 1000mA.
				   if set other value,
				   it assume current limit is 1000mA.
				*/
	.flash_sync		= LM356X_SYNC_OFF,
	.strobe_polarity	= LM356X_STROBE_POLARITY_HIGH,
	.ledintc_pin_setting	= LM356X_LEDINTC_NTC_THERMISTOR_INPUT,
	.tx1_polarity		= LM356X_TX1_POLARITY_HIGH,
	.tx2_polarity		= LM356X_TX2_POLARITY_HIGH,
	.hw_torch_mode		= LM356X_HW_TORCH_MODE_DISABLE,
};
#endif

#ifdef CONFIG_INPUT_BMA250
static int bma250_gpio_setup(struct device *dev)
{
	return gpio_request(BMA250_GPIO, "bma250_irq");
}

static void bma250_gpio_teardown(struct device *dev)
{
	gpio_free(BMA250_GPIO);
}

static struct registers bma250_reg_setup = {
	.range                = BMA250_RANGE_4G,
	.bw_sel               = BMA250_BW_250HZ,
	.int_mode_ctrl        = BMA250_MODE_SLEEP_50MS,
	.int_enable1          = BMA250_INT_SLOPE_Z |
				BMA250_INT_SLOPE_Y |
				BMA250_INT_SLOPE_X |
				BMA250_INT_ORIENT,
	.int_enable2          = BMA250_INT_NEW_DATA,
	.int_pin1             = BMA250_INT_PIN1_SLOPE |
				BMA250_INT_PIN1_ORIENT,
	.int_new_data         = BMA250_INT_PIN1,
	.int_pin2             = -1,
};

static struct bma250_platform_data bma250_platform_data = {
	.setup                = bma250_gpio_setup,
	.teardown             = bma250_gpio_teardown,
	.reg                  = &bma250_reg_setup,
};
#endif

#ifdef CONFIG_INPUT_APDS9702
#define APDS9702_DOUT_GPIO   88
#ifdef CONFIG_MSM_UNDERVOLT_PROXIMITY
#define APDS9702_VDD_VOLTAGE 2400
#else
#define APDS9702_VDD_VOLTAGE 2900
#endif
#define APDS9702_WAIT_TIME   5000

static int apds9702_gpio_setup(struct device *dev, int request)
{
	if (request) {
		return gpio_request(APDS9702_DOUT_GPIO, "apds9702_dout");
	} else {
		gpio_free(APDS9702_DOUT_GPIO);
		return 0;
	}
}


static void apds9702_power_mode(struct device *dev, int enable)
{
	enable = !!enable;
	if (enable)
		vreg_helper_on("wlan", APDS9702_VDD_VOLTAGE);
	else
		vreg_helper_off("wlan");
	usleep(APDS9702_WAIT_TIME);
}

static struct apds9702_platform_data apds9702_pdata = {
	.gpio_dout      = APDS9702_DOUT_GPIO,
	.is_irq_wakeup  = 1,
	.hw_config      = apds9702_power_mode,
	.gpio_setup     = apds9702_gpio_setup,
	.ctl_reg = {
		.trg   = 1,
		.pwr   = 1,
		.burst = 7,
		.frq   = 3,
		.dur   = 2,
		.th    = 15,
		.rfilt = 0,
	},
	.phys_dev_path = "/sys/devices/i2c-12/12-0054"
};
#endif

#ifdef CONFIG_SENSORS_AKM8975
static struct msm_gpio akm8975_gpio_config_data[] = {
	{ GPIO_CFG(AKM8975_GPIO, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN,
		GPIO_CFG_2MA), "akm8975_drdy_irq" },
};

static int akm8975_gpio_setup(void)
{
	return msm_gpios_request_enable(akm8975_gpio_config_data,
			ARRAY_SIZE(akm8975_gpio_config_data));
}

static void akm8975_gpio_shutdown(void)
{
	msm_gpios_disable_free(akm8975_gpio_config_data,
		ARRAY_SIZE(akm8975_gpio_config_data));

}

static struct akm8975_platform_data akm8975_platform_data = {
	.setup = akm8975_gpio_setup,
	.shutdown = akm8975_gpio_shutdown,
};
#endif

static struct i2c_board_info msm_i2c_board_info[] = {
#ifdef CONFIG_TOUCHSCREEN_CLEARPAD_I2C
	{
		I2C_BOARD_INFO(CLEARPADI2C_NAME, 0x58 >> 1),
		.platform_data = &clearpad_platform_data,
	},
#endif
#ifdef CONFIG_LEDS_AS3676
	{
		/* Config-spec is 8-bit = 0x80, src-code need 7-bit => 0x40 */
		I2C_BOARD_INFO("as3676", 0x80 >> 1),
		.platform_data = &as3676_platform_data,
	},
#endif
	{
		I2C_BOARD_INFO(BQ27520_NAME, 0xAA >> 1),
		.irq = MSM_GPIO_TO_INT(GPIO_BQ27520_SOC_INT),
		.platform_data = &bq27520_platform_data,
		.type = BQ27520_NAME,
	},
#ifdef CONFIG_CHARGER_BQ24185
	{
		I2C_BOARD_INFO(BQ24185_NAME, 0xD6 >> 1),
		.irq = PM8058_GPIO_IRQ(PMIC8058_IRQ_BASE, BQ24185_GPIO_IRQ - 1),
		.platform_data = &bq24185_platform_data,
		.type = BQ24185_NAME,
	},
#endif
#ifdef CONFIG_INPUT_BMA150_NG
	{
		I2C_BOARD_INFO("bma150", 0x70 >> 1),
		.irq = MSM_GPIO_TO_INT(BMA150_GPIO),
		.platform_data = &bma150_ng_platform_data,
		.type = "bma150"
	},
#endif
#ifdef CONFIG_INPUT_BMA250
	{
		I2C_BOARD_INFO("bma250", 0x18),
		.irq = MSM_GPIO_TO_INT(BMA250_GPIO),
		.platform_data = &bma250_platform_data,
	},
#endif
#ifdef CONFIG_INPUT_BMP180
	{
		I2C_BOARD_INFO("bmp180", 0x77)
	},
#endif
#ifdef CONFIG_INPUT_APDS9702
	{
		/* Config-spec is 8-bit = 0xA8, src-code need 7-bit => 0x54 */
		I2C_BOARD_INFO(APDS9702_NAME, 0xA8 >> 1),
		.platform_data = &apds9702_pdata,
	},
#endif
#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
	{
		I2C_BOARD_INFO("sii9024a", 0x76 >> 1),
		.platform_data = &sii9024_platform_data,
		.type = "sii9024a"
	},
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */
#ifdef CONFIG_SENSORS_AKM8975
	{
		I2C_BOARD_INFO(AKM8975_I2C_NAME, 0x18 >> 1),
		.irq = MSM_GPIO_TO_INT(AKM8975_GPIO),
		.platform_data = &akm8975_platform_data,
	},
#endif
#ifdef CONFIG_LM3560
	{
		I2C_BOARD_INFO("lm3560", 0xA6 >> 1),
		.platform_data = &lm3560_platform_data,
	},
#endif
#ifdef CONFIG_LM3561
	{
		I2C_BOARD_INFO("lm3561", 0xA6 >> 1),
		.platform_data = &lm3561_platform_data,
	},
#endif
};

static struct spi_board_info spi_board_info[] __initdata = {
#ifdef CONFIG_TOUCHSCREEN_CY8CTMA300_SPI
	{
		.modalias       = "cypress_touchscreen",
		.mode           = SPI_MODE_0,
		.platform_data  = &cypress_touch_data,
		.bus_num        = 0,
		.chip_select    = 0,
		.max_speed_hz   = 1 * 1000 * 1000,
		.irq		= MSM_GPIO_TO_INT(CYPRESS_TOUCH_GPIO_IRQ),
	},
#endif
#ifdef CONFIG_TOUCHSCREEN_CYTTSP_SPI
        {
                .modalias       = CY_SPI_NAME,
                .mode           = SPI_MODE_0,
                .irq            = MSM_GPIO_TO_INT(CYPRESS_TOUCH_GPIO_IRQ),
                .platform_data  = &cyttsp_data,
                .bus_num        = 0,
                .chip_select    = 0,
                .max_speed_hz   = 1 * 1000 * 1000,
        },
#endif /* CONFIG_TOUCHSCREEN_CYTTSP_SPI */
};

static struct i2c_board_info msm_marimba_board_info[] = {
	{
		I2C_BOARD_INFO("marimba", 0xc),
		.platform_data = &marimba_pdata,
	}
};


static struct msm_handset_platform_data hs_platform_data = {
	.hs_name = "7k_handset",
	.pwr_key_delay_ms = 500, /* 0 will disable end key */
};

static struct platform_device hs_device = {
	.name   = "msm-handset",
	.id     = -1,
	.dev    = {
		.platform_data = &hs_platform_data,
	},
};

static struct msm_pm_platform_data msm_pm_data[MSM_PM_SLEEP_MODE_NR] = {
	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_POWER_COLLAPSE)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 1,
		.suspend_enabled = 1,
		.latency = 8594,
		.residency = 23740,
	},
	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 1,
		.suspend_enabled = 1,
		.latency = 4594,
		.residency = 23740,
	},
	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE)] = {
#ifdef CONFIG_MSM_STANDALONE_POWER_COLLAPSE
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 1,
		.suspend_enabled = 0,
#else /*CONFIG_MSM_STANDALONE_POWER_COLLAPSE*/
		.idle_supported = 0,
		.suspend_supported = 0,
		.idle_enabled = 0,
		.suspend_enabled = 0,
#endif /*CONFIG_MSM_STANDALONE_POWER_COLLAPSE*/
		.latency = 500,
		.residency = 6000,
	},
	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 0,
		.suspend_enabled = 1,
		.latency = 443,
		.residency = 1098,
	},
	[MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT)] = {
		.idle_supported = 1,
		.suspend_supported = 1,
		.idle_enabled = 1,
		.suspend_enabled = 1,
		.latency = 2,
		.residency = 0,
	},
};

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_RESET_VECTOR_VIRT,
	.v_addr = (uint32_t *)PAGE_OFFSET,
};

static struct resource qsd_spi_resources[] = {
	{
		.name   = "spi_irq_in",
		.start	= INT_SPI_INPUT,
		.end	= INT_SPI_INPUT,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name   = "spi_irq_out",
		.start	= INT_SPI_OUTPUT,
		.end	= INT_SPI_OUTPUT,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name   = "spi_irq_err",
		.start	= INT_SPI_ERROR,
		.end	= INT_SPI_ERROR,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name   = "spi_base",
		.start	= 0xA8000000,
		.end	= 0xA8000000 + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "spidm_channels",
		.flags  = IORESOURCE_DMA,
	},
	{
		.name   = "spidm_crci",
		.flags  = IORESOURCE_DMA,
	},
};

#define AMDH0_BASE_PHYS		0xAC200000
#define ADMH0_GP_CTL		(ct_adm_base + 0x3D8)
static int msm_qsd_spi_dma_config(void)
{
	void __iomem *ct_adm_base = 0;
	u32 spi_mux = 0;
	int ret = 0;

	ct_adm_base = ioremap(AMDH0_BASE_PHYS, PAGE_SIZE);
	if (!ct_adm_base) {
		pr_err("%s: Could not remap %x\n", __func__, AMDH0_BASE_PHYS);
		return -ENOMEM;
	}

	spi_mux = (ioread32(ADMH0_GP_CTL) & (0x3 << 12)) >> 12;

	qsd_spi_resources[4].start  = DMOV_USB_CHAN;
	qsd_spi_resources[4].end    = DMOV_TSIF_CHAN;

	switch (spi_mux) {
	case (1):
		qsd_spi_resources[5].start  = DMOV_HSUART1_RX_CRCI;
		qsd_spi_resources[5].end    = DMOV_HSUART1_TX_CRCI;
		break;
	case (2):
		qsd_spi_resources[5].start  = DMOV_HSUART2_RX_CRCI;
		qsd_spi_resources[5].end    = DMOV_HSUART2_TX_CRCI;
		break;
	case (3):
		qsd_spi_resources[5].start  = DMOV_CE_OUT_CRCI;
		qsd_spi_resources[5].end    = DMOV_CE_IN_CRCI;
		break;
	default:
		ret = -ENOENT;
	}

	iounmap(ct_adm_base);

	return ret;
}

static struct platform_device qsd_device_spi = {
	.name		= "spi_qsd",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qsd_spi_resources),
	.resource	= qsd_spi_resources,
};

static int msm_qsd_spi_gpio_config(void)
{
	return msm_gpios_request_enable(qsd_spi_gpio_config_data,
					qsd_spi_gpio_config_data_size);
}

static void msm_qsd_spi_gpio_release(void)
{
	msm_gpios_disable_free(qsd_spi_gpio_config_data,
			       qsd_spi_gpio_config_data_size);
}

static struct msm_spi_platform_data qsd_spi_pdata = {
	.max_clock_speed = 26331429,
	.gpio_config  = msm_qsd_spi_gpio_config,
	.gpio_release = msm_qsd_spi_gpio_release,
	.dma_config = msm_qsd_spi_dma_config,
};

static void __init msm_qsd_spi_init(void)
{
	qsd_device_spi.dev.platform_data = &qsd_spi_pdata;
}

#ifdef CONFIG_USB_EHCI_MSM_72K
static void msm_hsusb_vbus_power(unsigned phy_info, int on)
{
#ifdef CONFIG_CHARGER_BQ24185
	if (on)
		(void)bq24185_set_opa_mode(CHARGER_BOOST_MODE);
	else
		(void)bq24185_set_opa_mode(CHARGER_CHARGER_MODE);
#else
        int rc;
        static int vbus_is_on;
	struct pm8xxx_gpio_init_info usb_vbus = {
		PM8058_GPIO_PM_TO_SYS(36),
		{
			.direction      = PM_GPIO_DIR_OUT,
			.pull           = PM_GPIO_PULL_NO,
			.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
			.output_value   = 1,
			.vin_sel        = 2,
			.out_strength   = PM_GPIO_STRENGTH_MED,
			.function       = PM_GPIO_FUNC_NORMAL,
			.inv_int_pol    = 0,
		},
	};

        /* If VBUS is already on (or off), do nothing. */
        if (unlikely(on == vbus_is_on))
                return;

        if (on) {
		rc = pm8xxx_gpio_config(usb_vbus.gpio, &usb_vbus.config);
		if (rc) {
                        pr_err("%s PMIC GPIO 36 write failed\n", __func__);
                        return;
                }
	} else {
		gpio_set_value_cansleep(PM8058_GPIO_PM_TO_SYS(36), 0);
	}

        vbus_is_on = on;
#endif
}

static struct msm_usb_host_platform_data msm_usb_host_pdata = {
        .phy_info   = (USB_PHY_INTEGRATED | USB_PHY_MODEL_45NM),
        .vbus_power = msm_hsusb_vbus_power,
        .power_budget   = 300,
};
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
static int hsusb_rpc_connect(int connect)
{
	if (connect)
		return msm_hsusb_rpc_connect();
	else
		return msm_hsusb_rpc_close();
}
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
static struct regulator *vreg_3p3;
static int msm_hsusb_ldo_init(int init)
{
	uint32_t version = 0;
	int def_vol = 3400000;

	version = socinfo_get_version();

	if (SOCINFO_VERSION_MAJOR(version) >= 2 &&
			SOCINFO_VERSION_MINOR(version) >= 1) {
		def_vol = 3075000;
		pr_debug("%s: default voltage:%d\n", __func__, def_vol);
	}

	if (init) {
		vreg_3p3 = regulator_get(NULL, "usb");
		if (IS_ERR(vreg_3p3))
			return PTR_ERR(vreg_3p3);
		regulator_set_voltage(vreg_3p3, def_vol, def_vol);
	} else
		regulator_put(vreg_3p3);

	return 0;
}

static int msm_hsusb_ldo_enable(int enable)
{
	static int ldo_status;

	if (!vreg_3p3 || IS_ERR(vreg_3p3))
		return -ENODEV;

	if (ldo_status == enable)
		return 0;

	ldo_status = enable;

	if (enable)
		return regulator_enable(vreg_3p3);
	else
		return regulator_disable(vreg_3p3);
}

static int msm_hsusb_ldo_set_voltage(int mV)
{
	static int cur_voltage;

	if (!vreg_3p3 || IS_ERR(vreg_3p3))
		return -ENODEV;

	if (cur_voltage == mV)
		return 0;

	cur_voltage = mV;

	pr_debug("%s: (%d)\n", __func__, mV);

	return regulator_set_voltage(vreg_3p3, mV*1000, mV*1000);
}
#endif

#ifndef CONFIG_USB_EHCI_MSM_72K
static int msm_hsusb_pmic_notif_init(void (*callback)(int online), int init);
#endif
static struct msm_otg_platform_data msm_otg_pdata = {
	.rpc_connect	= hsusb_rpc_connect,

#ifndef CONFIG_USB_EHCI_MSM_72K
	.pmic_vbus_notif_init         = msm_hsusb_pmic_notif_init,
#else
	.vbus_power = msm_hsusb_vbus_power,
#endif
	.pemp_level		 = PRE_EMPHASIS_WITH_20_PERCENT,
	.cdr_autoreset		 = CDR_AUTO_RESET_DISABLE,
	.drv_ampl		 = HS_DRV_AMPLITUDE_DEFAULT,
	.se1_gating		 = SE1_GATING_DISABLE,
	.ldo_enable		 = msm_hsusb_ldo_enable,
	.ldo_init		 = msm_hsusb_ldo_init,
	.ldo_set_voltage	 = msm_hsusb_ldo_set_voltage,
#ifdef CONFIG_SEMC_CHARGER_USB_ARCH
	.chg_vbus_draw		 = semc_charger_usb_vbus_draw,
	.chg_connected		 = semc_charger_usb_connected,
	.chg_init		 = semc_charger_usb_init,
#endif
#ifdef CONFIG_CHARGER_BQ24185
	.chg_is_initialized	 = bq24185_charger_initialized,
#endif
	.phy_can_powercollapse	 = 1,
	.chg_drawable_ida	 = USB_IDCHG_MAX,
};

#ifdef CONFIG_USB_GADGET
static struct msm_hsusb_gadget_platform_data msm_gadget_pdata = {
	.is_phy_status_timer_on = 1,
};
#endif
#ifndef CONFIG_USB_EHCI_MSM_72K
typedef void (*notify_vbus_state) (int);
notify_vbus_state notify_vbus_state_func_ptr;
int vbus_on_irq;
static irqreturn_t pmic_vbus_on_irq(int irq, void *data)
{
	pr_info("%s: vbus notification from pmic\n", __func__);

	(*notify_vbus_state_func_ptr) (1);

	return IRQ_HANDLED;
}
static int msm_hsusb_pmic_notif_init(void (*callback)(int online), int init)
{
	int ret;

	if (init) {
		if (!callback)
			return -ENODEV;

		notify_vbus_state_func_ptr = callback;
		vbus_on_irq = platform_get_irq_byname(&msm_device_otg,
			"vbus_on");
		if (vbus_on_irq <= 0) {
			pr_err("%s: unable to get vbus on irq\n", __func__);
			return -ENODEV;
		}

		ret = request_any_context_irq(vbus_on_irq, pmic_vbus_on_irq,
			IRQF_TRIGGER_RISING, "msm_otg_vbus_on", NULL);
		if (ret < 0) {
			pr_info("%s: request_irq for vbus_on"
				"interrupt failed\n", __func__);
			return ret;
		}
		msm_otg_pdata.pmic_vbus_irq = vbus_on_irq;
		return 0;
	} else {
		free_irq(vbus_on_irq, 0);
		notify_vbus_state_func_ptr = NULL;
		return 0;
	}
}
#endif

static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_ALLORNOTHING,
	.cached = 1,
	.memory_type = MEMTYPE_EBI0,
};

static struct platform_device android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = { .platform_data = &android_pmem_pdata },
};

#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
static struct resource sii_uio_resources[] = {
	[0] = {
		.start  = MSM_GPIO_TO_INT(90),
		.end    = MSM_GPIO_TO_INT(90),
		.flags  = IORESOURCE_IRQ,
	},
};

static struct uio_info sii_uio_pdata = {
	.name	=  "sii9024a_uio",
	.version = "0.001",
	.mem = {
		{
			.memtype = UIO_MEM_NONE,
			.size    = 0
		}
	},
	.irq       = MSM_GPIO_TO_INT(90),
	.irq_flags = IRQF_TRIGGER_LOW,
};

static struct platform_device sii_uio_dev = {
	.name           = "uio_pdrv_genirq",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(sii_uio_resources),
	.resource       = sii_uio_resources,
	.dev            = {
		.platform_data = &sii_uio_pdata,
	},
};

static struct lcdc_platform_data dtv_pdata = {
};
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */

static struct msm_serial_hs_platform_data msm_uart_dm1_pdata = {
       .inject_rx_on_wakeup = 1,
       .rx_to_inject = 0xFD,
};

static struct resource msm_fb_resources[] = {
	{
		.flags  = IORESOURCE_DMA,
	}
};

#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
static int msm_fb_detect_panel(const char *name)
{
	if (!strcmp(name, "sii9024a")) {
		printk(KERN_ERR
			"[HDMI] msm_fb_detect_panel() : name(%s)\n", name);
		return 0;
	}
	return -ENODEV;
}
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */

static struct msm_fb_platform_data msm_fb_pdata = {
#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
	.detect_client = msm_fb_detect_panel,
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */
	.mddi_prescan = 1,
};

static struct platform_device msm_fb_device = {
	.name   = "msm_fb",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_fb_resources),
	.resource       = msm_fb_resources,
	.dev    = {
		.platform_data = &msm_fb_pdata,
	}
};

static struct platform_device msm_migrate_pages_device = {
	.name   = "msm_migrate_pages",
	.id     = -1,
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
       .name = "pmem_adsp",
       .allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
       .cached = 0,
	.memory_type = MEMTYPE_EBI0,
};

static struct platform_device android_pmem_adsp_device = {
       .name = "android_pmem",
       .id = 2,
       .dev = { .platform_data = &android_pmem_adsp_pdata },
};

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

#define QCE_SIZE		0x10000
#define QCE_0_BASE		0xA8400000

#define QCE_HW_KEY_SUPPORT	1
#define QCE_SHA_HMAC_SUPPORT	0
#define QCE_SHARE_CE_RESOURCE	0
#define QCE_CE_SHARED		0

static struct resource qcrypto_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[4] = {
		.name = "crypto_crci_hash",
		.start = DMOV_CE_HASH_CRCI,
		.end = DMOV_CE_HASH_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource qcedev_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[4] = {
		.name = "crypto_crci_hash",
		.start = DMOV_CE_HASH_CRCI,
		.end = DMOV_CE_HASH_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)

static struct msm_ce_hw_support qcrypto_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	/* Bus Scaling declaration*/
	.bus_scale_table = NULL,
};

static struct platform_device qcrypto_device = {
	.name		= "qcrypto",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcrypto_resources),
	.resource	= qcrypto_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcrypto_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

static struct msm_ce_hw_support qcedev_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	/* Bus Scaling declaration*/
	.bus_scale_table = NULL,
};
static struct platform_device qcedev_device = {
	.name		= "qce",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcedev_resources),
	.resource	= qcedev_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcedev_ce_hw_suppport,
	},
};
#endif

static unsigned char quickvx_mddi_client = 1, other_mddi_client = 1;
static struct regulator *mddi_ldo16;

static int msm_fb_mddi_sel_clk(u32 *clk_rate)
{
	*clk_rate *= 2;
	return 0;
}

static int msm_fb_mddi_client_power(u32 client_id)
{
	int rc;
	printk(KERN_NOTICE "\n client_id = 0x%x", client_id);
	/* Check if it is Quicklogic client */
	if (client_id == 0xc5835800) {
		printk(KERN_NOTICE "\n Quicklogic MDDI client");
		other_mddi_client = 0;
		if (IS_ERR(mddi_ldo16)) {
			rc = PTR_ERR(mddi_ldo16);
			pr_err("%s: gp10 vreg get failed (%d)\n", __func__, rc);
			return rc;
		}
		rc = regulator_disable(mddi_ldo16);
		if (rc) {
			pr_err("%s: LDO16 vreg enable failed (%d)\n",
							__func__, rc);
			return rc;
		}

	} else {
		printk(KERN_NOTICE "\n Non-Quicklogic MDDI client");
		quickvx_mddi_client = 0;
		gpio_set_value(97, 0);
		gpio_set_value_cansleep(PM8058_GPIO_PM_TO_SYS(
			PMIC_GPIO_QUICKVX_CLK), 0);
	}

	return 0;
}

static struct mddi_platform_data mddi_pdata = {
	//.mddi_power_save = display_common_power,
	.mddi_sel_clk = msm_fb_mddi_sel_clk,
	.mddi_client_power = msm_fb_mddi_client_power,
};

static struct msm_panel_common_pdata mdp_pdata = {
	.hw_revision_addr = 0xac001270,
	.gpio = 30,
	.mdp_max_clk = 192000000,
	.mdp_rev = MDP_REV_40,
	.mem_hid = MEMTYPE_EBI0,
};

#ifdef CONFIG_BT
static uint32_t bt_config_on_gpios[] = {
	GPIO_CFG(134, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
	GPIO_CFG(135, 1, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(136, 1, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(137, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
	GPIO_CFG(103, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static uint32_t bt_config_off_gpios[] = {
	GPIO_CFG(134, 0, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(135, 0, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(136, 0, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(137, 0, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(103, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static int bluetooth_power(int on)
{
	if (on) {
		config_gpio_table(bt_config_on_gpios,
				  ARRAY_SIZE(bt_config_on_gpios));
		gpio_set_value(103, 1);
	} else {
		gpio_set_value(103, 0);
		config_gpio_table(bt_config_off_gpios,
				  ARRAY_SIZE(bt_config_off_gpios));
	}
	return 0;
}

static struct platform_device mogami_device_rfkill = {
	.name = "mogami-rfkill",
	.dev.platform_data = &bluetooth_power,
};
#endif

static struct regulator *atv_s4, *atv_ldo9;

static int __init atv_dac_power_init(void)
{
	int rc;
	struct regulator_bulk_data regs[] = {
		{ .supply = "smps4", .min_uV = 2200000, .max_uV = 2200000 },
		{ .supply = "ldo9",  .min_uV = 2050000, .max_uV = 2050000 },
	};

	rc = regulator_bulk_get(NULL, ARRAY_SIZE(regs), regs);

	if (rc) {
		pr_err("%s: could not get regulators: %d\n", __func__, rc);
		goto bail;
	}

	rc = regulator_bulk_set_voltage(ARRAY_SIZE(regs), regs);

	if (rc) {
		pr_err("%s: could not set voltages: %d\n", __func__, rc);
		goto reg_free;
	}

	atv_s4   = regs[0].consumer;
	atv_ldo9 = regs[1].consumer;

reg_free:
	regulator_bulk_free(ARRAY_SIZE(regs), regs);
bail:
	return rc;
}

#ifdef CONFIG_SIMPLE_REMOTE_PLATFORM
#define PLUG_DET_ENA_PIN 80
#define PLUG_DET_READ_PIN 26
#define MODE_SWITCH_PIN -1

int simple_remote_pf_initialize_gpio(struct simple_remote_platform_data *data)
{
	int err = 0;
	int i;

	if (!data || -1 == data->headset_detect_enable_pin) {
		printk(KERN_ERR
		       "*** %s - Error: Invalid inparameter (GPIO Pins)."
		       " Aborting!\n", __func__);
		return -EIO;
	}

	err = gpio_request(data->headset_detect_enable_pin,
			   "Simple_remote_plug_detect_enable");
	if (err) {
		printk(KERN_CRIT "%s: Error %d - Request hs_detect_enable pin",
		       __func__, err);
		goto out;
	}

	err = gpio_direction_output(data->headset_detect_enable_pin, 1);
	if (err) {
		printk(KERN_CRIT "%s: Error %d - Set hs_detect_enable pin"
		       " as output high\n", __func__, err);
		goto out_hs_det_enable;
	}

	err = gpio_request(data->headset_detect_read_pin,
			   "Simple_remote_plug_detect_read");
	if (err) {
		printk(KERN_CRIT "%s - Error %d - Request hs-detect_read pin",
		       __func__, err);
		goto out_hs_det_enable;
	}

	err = gpio_direction_input(data->headset_detect_read_pin);
	if (err) {
		printk(KERN_CRIT "%s - Error %d - Set hs-detect pin as input\n",
		       __func__, err);
		goto out_hs_det_read;
	}

	if (0 < data->headset_mode_switch_pin) {
		err = gpio_request(data->headset_mode_switch_pin,
				   "Simple_remote_headset_mode_switch");
		if (err) {
			printk(KERN_CRIT
			       "%s - Error %d - Request hs-mode_switch pin",
			       __func__, err);
			goto out_hs_det_read;
		}

		err = gpio_direction_output(data->headset_mode_switch_pin, 0);
		if (err) {
			printk(KERN_CRIT
			       "%s - Error %d - Set hs-mode_switch pin as "
			       "input\n", __func__, err);
			goto out_hs_mode_switch;
		}
	}

	for (i = 0; i < data->num_regs; i++) {
		data->regs[i].reg = vreg_get(NULL, data->regs[i].name);
		if (IS_ERR(data->regs[i].reg)) {
			printk(KERN_ERR "%s - Failed to find regulator %s\n",
			       __func__, data->regs[i].name);
			err = PTR_ERR(data->regs[i].reg);
			if (0 <= data->headset_mode_switch_pin)
				goto out_hs_mode_switch;
			else
				goto out_hs_det_read;
		}
	}

	return err;

out_hs_mode_switch:
	gpio_free(data->headset_mode_switch_pin);

out_hs_det_read:
	gpio_free(data->headset_detect_read_pin);

out_hs_det_enable:
	gpio_free(data->headset_detect_enable_pin);
out:
	return err;
}

void simple_remote_pf_deinitialize_gpio(
	struct simple_remote_platform_data *data)
{
	gpio_free(data->headset_detect_read_pin);
	gpio_free(data->headset_detect_enable_pin);
}

static struct simple_remote_platform_regulators regs[] =  {
	{
		.name = "ncp",
	},
	{
		.name = "s3",
	},
	{
		.name = "s2",
	},

};

static struct simple_remote_platform_data simple_remote_pf_data = {
	.headset_detect_enable_pin = PLUG_DET_ENA_PIN,
	.headset_detect_read_pin = PLUG_DET_READ_PIN,
	.headset_mode_switch_pin = MODE_SWITCH_PIN,
	.initialize = &simple_remote_pf_initialize_gpio,
	.deinitialize = &simple_remote_pf_deinitialize_gpio,

	.regs = regs,
	.num_regs = ARRAY_SIZE(regs),

	.controller = PM_HSED_CONTROLLER_1,

#ifdef CONFIG_SIMPLE_REMOTE_INVERT_PLUG_DETECTION_STATE
	.invert_plug_det = 1,
#else
	.invert_plug_det = 0,
#endif
};

static struct platform_device simple_remote_pf_device = {
	.name = SIMPLE_REMOTE_PF_NAME,
	.dev = {
		.platform_data = &simple_remote_pf_data,
	},
};
#endif

static void __init msm_fb_add_devices(void)
{
	msm_fb_register_device("mdp", &mdp_pdata);
	msm_fb_register_device("pmdh", &mddi_pdata);
#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
	msm_fb_register_device("dtv", &dtv_pdata);
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */
}

static char *msm_adc_surf_device_names[] = {
	"XO_ADC",
};

static struct msm_adc_platform_data msm_adc_pdata;

static struct platform_device msm_adc_device = {
	.name   = "msm_adc",
	.id = -1,
	.dev = {
		.platform_data = &msm_adc_pdata,
	},
};

#ifdef CONFIG_MSM_SDIO_AL
static struct msm_gpio mdm2ap_status = {
	GPIO_CFG(77, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	"mdm2ap_status"
};


static int configure_mdm2ap_status(int on)
{
	if (on)
		return msm_gpios_request_enable(&mdm2ap_status, 1);
	else {
		msm_gpios_disable_free(&mdm2ap_status, 1);
		return 0;
	}
}

static int get_mdm2ap_status(void)
{
	return gpio_get_value(GPIO_PIN(mdm2ap_status.gpio_cfg));
}

static struct sdio_al_platform_data sdio_al_pdata = {
	.config_mdm2ap_status = configure_mdm2ap_status,
	.get_mdm2ap_status = get_mdm2ap_status,
	.allow_sdioc_version_major_2 = 1,
	.peer_sdioc_version_minor = 0x0001,
	.peer_sdioc_version_major = 0x0003,
	.peer_sdioc_boot_version_minor = 0x0001,
	.peer_sdioc_boot_version_major = 0x0003,
};

struct platform_device msm_device_sdio_al = {
	.name = "msm_sdio_al",
	.id = -1,
	.dev		= {
		.platform_data	= &sdio_al_pdata,
	},
};

#endif /* CONFIG_MSM_SDIO_AL */

static struct platform_device *devices[] __initdata = {
#if defined(CONFIG_SERIAL_MSM) || defined(CONFIG_MSM_SERIAL_DEBUGGER)
	&msm_device_uart3,
#endif
#ifdef CONFIG_ANDROID_RAM_CONSOLE
	&ram_console_device,
#endif
#ifdef CONFIG_MSM_PROC_COMM_REGULATOR
	&msm_proccomm_regulator_dev,
#endif
	&asoc_msm_pcm,
	&asoc_msm_dai0,
	&asoc_msm_dai1,
#if defined (CONFIG_SND_MSM_MVS_DAI_SOC)
	&asoc_msm_mvs,
	&asoc_mvs_dai0,
	&asoc_mvs_dai1,
#endif
	&msm_device_smd,
	&msm_device_dmov,
	&msm_device_nand,
#ifdef CONFIG_USB_MSM_OTG_72K
	&msm_device_otg,
#ifdef CONFIG_USB_GADGET
	&msm_device_gadget_peripheral,
#endif
#endif
#ifdef CONFIG_USB_G_ANDROID
	&android_usb_device,
#endif
	&qsd_device_spi,

#ifdef CONFIG_MSM_SSBI
	&msm_device_ssbi_pmic1,
#endif
#ifdef CONFIG_I2C_SSBI
	&msm_device_ssbi7,
#endif
	&android_pmem_device,
	&msm_fb_device,
	&msm_migrate_pages_device,
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
#ifdef CONFIG_FB_MSM_HDMI_SII9024A_PANEL
	&hdmi_sii9024a_panel_device,
	&sii_uio_dev,
#endif /* CONFIG_FB_MSM_HDMI_SII9024A_PANEL */
	&android_pmem_adsp_device,
	&msm_device_i2c,
	&msm_device_i2c_2,
	&msm_device_uart_dm1,
	&semc_rpc_handset_device,
	&hs_device,
#ifdef CONFIG_MSM7KV2_AUDIO
	&msm_aictl_device,
	&msm_mi2s_device,
	&msm_lpa_device,
	&msm_aux_pcm_device,
#endif
	&msm_device_adspdec,
	&qup_device_i2c,
	&msm_kgsl_3d0,
	&msm_kgsl_2d0,
#ifdef CONFIG_SEMC_CAMERA_MODULE
	&msm_camera_sensor_semc_camera,
#endif
#ifdef CONFIG_SEMC_SUB_CAMERA_MODULE
	&msm_camera_sensor_semc_sub_camera,
#endif
	&msm_device_vidc_720p,
#ifdef CONFIG_MSM_GEMINI
	&msm_gemini_device,
#endif
#ifdef CONFIG_MSM_VPE
	&msm_vpe_device,
#endif
#ifdef CONFIG_MSM_VPE_STANDALONE
	&msm_vpe_standalone_device,
#endif
	&bdata_driver,
	&battery_chargalg_platform_device,
#ifdef CONFIG_SIMPLE_REMOTE_PLATFORM
	&simple_remote_pf_device,
#endif
#ifdef CONFIG_FB_MSM_MDDI_NOVATEK_FWVGA
	&novatek_device,
#endif
#if defined(CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD)
	&mddi_sony_hvga_display_device,
#endif
#if defined(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD)
	&mddi_hitachi_hvga_display_device,
#endif
#if defined(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD)
	&mddi_sii_hvga_display_device,
#endif
#if defined(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)
	&mddi_auo_hvga_display_device,
#endif
#ifdef CONFIG_MOGAMI_SLIDER
	&slider_device_mogami,
#endif
#ifdef CONFIG_BT
	&mogami_device_rfkill,
#endif
#ifdef CONFIG_MSM_SDIO_AL
	&msm_device_sdio_al,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)
	&qcrypto_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)
	&qcedev_device,
#endif

	&msm_adc_device,
	&msm_ebi0_thermal,
	&msm_ebi1_thermal,
	&msm_adsp_device,
};

static void
msm_i2c_gpio_config(int adap_id, int config_type)
{
	struct msm_gpio *msm_i2c_table;

	/* Each adapter gets 2 lines from the table */
	if (adap_id > 0)
		return;
	if (config_type)
		msm_i2c_table = &msm_i2c_gpios_hw[adap_id*2];
	else
		msm_i2c_table = &msm_i2c_gpios_io[adap_id*2];
	msm_gpios_enable(msm_i2c_table, 2);
}
/*This needs to be enabled only for OEMS*/
#ifndef CONFIG_QUP_EXCLUSIVE_TO_CAMERA
static struct regulator *qup_vreg;
#endif
static void
qup_i2c_gpio_config(int adap_id, int config_type)
{
	int rc = 0;
	struct msm_gpio *qup_i2c_table;
	/* Each adapter gets 2 lines from the table */
	if (adap_id != 4)
		return;
	if (config_type)
		qup_i2c_table = qup_i2c_gpios_hw;
	else
		qup_i2c_table = qup_i2c_gpios_io;
	rc = msm_gpios_enable(qup_i2c_table, 2);
	if (rc < 0)
		printk(KERN_ERR "QUP GPIO enable failed: %d\n", rc);
#if !defined(CONFIG_SEMC_CAMERA_MODULE) && \
	!defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	if (qup_vreg) {
		int rc = vreg_set_level(qup_vreg, 1800);
		if (rc) {
			pr_err("%s: vreg LVS1 set level failed (%d)\n",
			       __func__, rc);
		}
		rc = vreg_enable(qup_vreg);
		if (rc) {
			pr_err("%s: vreg_enable() = %d \n",
				__func__, rc);
		}
	}
#endif
}

static struct msm_i2c_platform_data msm_i2c_pdata = {
	.clk_freq = 100000,
	.pri_clk = 70,
	.pri_dat = 71,
	.rmutex  = 1,
	.rsl_id = "D:I2C02000021",
	.msm_i2c_config_gpio = msm_i2c_gpio_config,
};

static void __init msm_device_i2c_init(void)
{
	if (msm_gpios_request(msm_i2c_gpios_hw, msm_i2c_gpios_hw_size))
		pr_err("failed to request I2C gpios\n");

	msm_device_i2c.dev.platform_data = &msm_i2c_pdata;
}

static struct msm_i2c_platform_data msm_i2c_2_pdata = {
	.clk_freq = 100000,
	.rmutex  = 0, // = 1,
	//.rsl_id = "D:I2C02000022",
	.msm_i2c_config_gpio = msm_i2c_gpio_config,
};

static void __init msm_device_i2c_2_init(void)
{
	msm_device_i2c_2.dev.platform_data = &msm_i2c_2_pdata;
}

static struct msm_i2c_platform_data qup_i2c_pdata = {
#if defined(CONFIG_SEMC_CAMERA_MODULE) || defined(CONFIG_SEMC_SUB_CAMERA_MODULE)
	.clk_freq = 100000,
#else
	.clk_freq = 384000,
#endif
	.msm_i2c_config_gpio = qup_i2c_gpio_config,
};

static void __init qup_device_i2c_init(void)
{
	if (msm_gpios_request(qup_i2c_gpios_hw, qup_i2c_gpios_hw_size))
		pr_err("failed to request I2C gpios\n");

	qup_device_i2c.dev.platform_data = &qup_i2c_pdata;
	/*This needs to be enabled only for OEMS*/
#ifndef CONFIG_QUP_EXCLUSIVE_TO_CAMERA
	qup_vreg = regulator_get(&qup_device_i2c.dev, "lvsw1");
	if (IS_ERR(qup_vreg)) {
		dev_err(&qup_device_i2c.dev,
			"%s: regulator_get failed: %ld\n",
			__func__, PTR_ERR(qup_vreg));
	}
#endif
}

#ifdef CONFIG_I2C_SSBI
static struct msm_i2c_ssbi_platform_data msm_i2c_ssbi7_pdata = {
	.rsl_id = "D:CODEC_SSBI",
	.controller_type = MSM_SBI_CTRL_SSBI,
};
#endif

static void __init msm7x30_init_irq(void)
{
	msm_init_irq();
}

static struct msm_gpio msm_nand_ebi2_cfg_data[] = {
	{GPIO_CFG(86, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "ebi2_cs1"},
	{GPIO_CFG(115, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "ebi2_busy1"},
};

#if (defined(CONFIG_MMC_MSM_SDC1_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC2_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC3_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC4_SUPPORT))

struct sdcc_gpio {
	struct msm_gpio *cfg_data;
	uint32_t size;
	struct msm_gpio *sleep_cfg_data;
};

static struct msm_gpio sdc3_cfg_data[] = {
	{GPIO_CFG(110, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
								"sdc3_clk"},
	{GPIO_CFG(111, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),
								"sdc3_cmd"},
	{GPIO_CFG(116, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),
								"sdc3_dat_3"},
	{GPIO_CFG(117, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),
								"sdc3_dat_2"},
	{GPIO_CFG(118, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),
								"sdc3_dat_1"},
	{GPIO_CFG(119, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_4MA),
								"sdc3_dat_0"},
};

static struct msm_gpio sdc3_sleep_cfg_data[] = {
	{GPIO_CFG(110, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
								"sdc3_clk"},
	{GPIO_CFG(111, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc3_cmd"},
	{GPIO_CFG(116, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc3_dat_3"},
	{GPIO_CFG(117, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc3_dat_2"},
	{GPIO_CFG(118, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc3_dat_1"},
	{GPIO_CFG(119, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc3_dat_0"},
};

#ifdef CONFIG_MMC_MSM_SDC4_LOW_DRIVE_STRENGTH
static struct msm_gpio sdc4_cfg_data[] = {
	{GPIO_CFG(58, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_clk"},
	{GPIO_CFG(59, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
								"sdc4_cmd"},
	{GPIO_CFG(60, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
								"sdc4_dat_3"},
	{GPIO_CFG(61, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
								"sdc4_dat_2"},
	{GPIO_CFG(62, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
								"sdc4_dat_1"},
	{GPIO_CFG(63, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA),
								"sdc4_dat_0"},
};
#else
static struct msm_gpio sdc4_cfg_data[] = {
	{GPIO_CFG(58, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_clk"},
	{GPIO_CFG(59, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_cmd"},
	{GPIO_CFG(60, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_dat_3"},
	{GPIO_CFG(61, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_dat_2"},
	{GPIO_CFG(62, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_dat_1"},
	{GPIO_CFG(63, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_dat_0"},
};
#endif

static struct msm_gpio sdc4_sleep_cfg_data[] = {
	{GPIO_CFG(58, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
								"sdc4_clk"},
	{GPIO_CFG(59, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
								"sdc4_cmd"},
	{GPIO_CFG(60, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
								"sdc4_dat_3"},
	{GPIO_CFG(61, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
								"sdc4_dat_2"},
	{GPIO_CFG(62, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
								"sdc4_dat_1"},
	{GPIO_CFG(63, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
								"sdc4_dat_0"},
};

static struct sdcc_gpio sdcc_cfg_data[] = {
	{
		.cfg_data = NULL,
		.size = 0,
		.sleep_cfg_data = NULL,
	},
	{
		.cfg_data = NULL,
		.size = 0,
		.sleep_cfg_data = NULL,
	},
	{
		.cfg_data = sdc3_cfg_data,
		.size = ARRAY_SIZE(sdc3_cfg_data),
		.sleep_cfg_data = sdc3_sleep_cfg_data,
	},
	{
		.cfg_data = sdc4_cfg_data,
		.size = ARRAY_SIZE(sdc4_cfg_data),
		.sleep_cfg_data = sdc4_sleep_cfg_data,
	},
};

static struct regulator *sdcc_vreg_data[ARRAY_SIZE(sdcc_cfg_data)];

static unsigned long vreg_sts, gpio_sts;

static uint32_t msm_sdcc_setup_gpio(int dev_id, unsigned int enable)
{
	int rc = 0;
	struct sdcc_gpio *curr;

	curr = &sdcc_cfg_data[dev_id - 1];

	if (!(test_bit(dev_id, &gpio_sts)^enable))
		return rc;

	if (enable) {
		set_bit(dev_id, &gpio_sts);
		rc = msm_gpios_request_enable(curr->cfg_data, curr->size);
		if (rc)
			printk(KERN_ERR "%s: Failed to turn on GPIOs for slot %d\n",
				__func__,  dev_id);
	} else {
		clear_bit(dev_id, &gpio_sts);
		if (curr->sleep_cfg_data) {
			msm_gpios_enable(curr->sleep_cfg_data, curr->size);
			msm_gpios_free(curr->sleep_cfg_data, curr->size);
			if (dev_id == 4) {
				/*
				 * 200 milliseconds delay should be sufficient to allow
				 * microSD reaches zero voltage when uSD is power off.
				 */
				msleep(200);
			}
		} else {
			msm_gpios_disable_free(curr->cfg_data, curr->size);
		}
	}

	return rc;
}

static uint32_t msm_sdcc_setup_vreg(int dev_id, unsigned int enable)
{
	int rc = 0;
	struct regulator *curr = sdcc_vreg_data[dev_id - 1];
	static int enabled_once[] = {0, 0, 0, 0};

	if (test_bit(dev_id, &vreg_sts) == enable)
		return rc;

	if (dev_id == 4) {
		if (enable) {
			pr_debug("Enable Vdd dev_%d\n", dev_id);
			gpio_set_value_cansleep(
				PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SDC4_PWR_EN_N),
						0);
			set_bit(dev_id, &vreg_sts);
		} else {
			pr_debug("Disable Vdd dev_%d\n", dev_id);
			gpio_set_value_cansleep(
				PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SDC4_PWR_EN_N),
				1);
			clear_bit(dev_id, &vreg_sts);
		}
	}

	if (!enable || enabled_once[dev_id - 1])
			return 0;
	if (!curr)
		return -ENODEV;

	if (IS_ERR(curr))
		return PTR_ERR(curr);

	if (enable) {
		set_bit(dev_id, &vreg_sts);

		rc = regulator_enable(curr);
		if (rc)
			pr_err("%s: could not enable regulator: %d\n",
					__func__, rc);
		enabled_once[dev_id - 1] = 1;
	} else {
		clear_bit(dev_id, &vreg_sts);

		rc = regulator_disable(curr);
		if (rc)
			pr_err("%s: could not disable regulator: %d\n",
					__func__, rc);
	}
	return rc;
}

static uint32_t msm_sdcc_setup_power(struct device *dv, unsigned int vdd)
{
	int rc = 0;
	struct platform_device *pdev;

	pdev = container_of(dv, struct platform_device, dev);
	rc = msm_sdcc_setup_gpio(pdev->id, (vdd ? 1 : 0));
	if (rc)
		goto out;

	if (pdev->id == 4) /* S3 is always ON and cannot be disabled */
		rc = msm_sdcc_setup_vreg(pdev->id, (vdd ? 1 : 0));
out:
	return rc;
}


#endif

#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
static unsigned int msm7x30_sdcc_slot_status(struct device *dev)
{
	return (unsigned int)
		!gpio_get_value_cansleep(
			PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SD_DET - 1));
}
#endif

#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
/* Wifi chip power control */
extern int mogami_wifi_power(int on);
static uint32_t wifi_setup_power(struct device *dv, unsigned int vdd)
{
	uint32_t ret = msm_sdcc_setup_power(dv, vdd);
	if (vdd)
		mogami_wifi_power(1);
	else
		mogami_wifi_power(0);
	return ret;
}

static struct mmc_platform_data msm7x30_sdc3_data = {
#ifdef CONFIG_MSM_UNDERVOLT_WIFI
	.ocr_mask	= MMC_VDD_20_21 | MMC_VDD_21_22,
#else
	.ocr_mask	= MMC_VDD_27_28 | MMC_VDD_28_29,
#endif
	.translate_vdd	= wifi_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA | MMC_CAP_POWER_OFF_CARD,
	.sdiowakeup_irq = MSM_GPIO_TO_INT(118),
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 1,
};
#endif

#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
static struct mmc_platform_data msm7x30_sdc4_data = {
	.ocr_mask	= MMC_VDD_27_28 | MMC_VDD_28_29,
	.translate_vdd	= msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.status      = msm7x30_sdcc_slot_status,
	.status_irq  = PM8058_GPIO_IRQ(PMIC8058_IRQ_BASE, PMIC_GPIO_SD_DET - 1),
	.irq_flags   = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 0,
};
#endif

static int mmc_regulator_init(int sdcc_no, const char *supply, int uV)
{
	int rc;

	BUG_ON(sdcc_no < 1 || sdcc_no > 4);

	sdcc_no--;

	sdcc_vreg_data[sdcc_no] = regulator_get(NULL, supply);

	if (IS_ERR(sdcc_vreg_data[sdcc_no])) {
		rc = PTR_ERR(sdcc_vreg_data[sdcc_no]);
		pr_err("%s: could not get regulator \"%s\": %d\n",
				__func__, supply, rc);
		goto out;
	}

	rc = regulator_set_voltage(sdcc_vreg_data[sdcc_no], uV, uV);

	if (rc) {
		pr_err("%s: could not set voltage for \"%s\" to %d uV: %d\n",
				__func__, supply, uV, rc);
		goto reg_free;
	}

	return rc;

reg_free:
	regulator_put(sdcc_vreg_data[sdcc_no]);
out:
	sdcc_vreg_data[sdcc_no] = NULL;
	return rc;
}

static void __init msm7x30_init_mmc(void)
{
#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
	if (mmc_regulator_init(3, "s3", 1800000))
		goto out3;

	msm_sdcc_setup_gpio(3, 1);
	msm_add_sdcc(3, &msm7x30_sdc3_data);
out3:
#endif
#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
	if (mmc_regulator_init(4, "mmc", 2850000))
		return;

	msm_add_sdcc(4, &msm7x30_sdc4_data);
#endif

}

/*
 * Temporary place for hardware initialization until the devices in question
 * gets proper drivers
 */
static void __init mogami_temp_fixups(void)
{
	vreg_helper_off("gp3");	/* L0 */
	vreg_helper_off("gp5");	/* L23 */
	gpio_set_value(46, 1);	/* SPI_CS0_N */
	gpio_set_value(134, 1);	/* UART1DM_RFR_N */
	gpio_set_value(137, 1);	/* UART1DM_TXD */
}

static void __init shared_vreg_on(void)
{
#ifdef CONFIG_MSM_UNDERVOLT_TOUCH
	vreg_helper_on(VREG_L20, 2800);
#else
	vreg_helper_on(VREG_L20, 3050);
#endif
	vreg_helper_on(VREG_L10, 2600);
#ifdef CONFIG_MSM_UNDERVOLT_LCD
	vreg_helper_on(VREG_L15, 2300);
#else
	vreg_helper_on(VREG_L15, 2900);
#endif
	vreg_helper_on(VREG_L8, 1800);
}

static void __init msm7x30_init_nand(void)
{
	char *build_id;
	struct flash_platform_data *plat_data;

	build_id = socinfo_get_build_id();
	if (build_id == NULL) {
		pr_err("%s: Build ID not available from socinfo\n", __func__);
		return;
	}

	if (build_id[8] == 'C' &&
			!msm_gpios_request_enable(msm_nand_ebi2_cfg_data,
			ARRAY_SIZE(msm_nand_ebi2_cfg_data))) {
		plat_data = msm_device_nand.dev.platform_data;
		plat_data->interleave = 1;
		printk(KERN_INFO "%s: Interleave mode Build ID found\n",
			__func__);
	}
}

#ifdef CONFIG_SERIAL_MSM_CONSOLE
static struct msm_gpio uart3_config_data[] = {
//	{ GPIO_CFG(49, 2, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "UART2_RFR"},
//	{ GPIO_CFG(50, 2, GPIO_CFG_INPUT,   GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "UART2_CTS"},
	{ GPIO_CFG(53, 1, GPIO_CFG_INPUT,   GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "UART3_Rx"},
	{ GPIO_CFG(54, 1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_4MA), "UART3_Tx"},
};

static void msm7x30_init_uart3(void)
{
	msm_gpios_request_enable(uart3_config_data,
			ARRAY_SIZE(uart3_config_data));

}
#endif

static struct msm_spm_platform_data msm_spm_data __initdata = {
	.reg_base_addr = MSM_SAW0_BASE,

	.reg_init_values[MSM_SPM_REG_SAW_CFG] = 0x05,
	.reg_init_values[MSM_SPM_REG_SAW_SPM_CTL] = 0x18,
	.reg_init_values[MSM_SPM_REG_SAW_SPM_SLP_TMR_DLY] = 0x00006666,
	.reg_init_values[MSM_SPM_REG_SAW_SPM_WAKE_TMR_DLY] = 0xFF000666,

	.reg_init_values[MSM_SPM_REG_SAW_SLP_CLK_EN] = 0x01,
	.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_PRECLMP_EN] = 0x03,
	.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_POSTCLMP_EN] = 0x00,

	.reg_init_values[MSM_SPM_REG_SAW_SLP_CLMP_EN] = 0x01,
	.reg_init_values[MSM_SPM_REG_SAW_SLP_RST_EN] = 0x00,
	.reg_init_values[MSM_SPM_REG_SAW_SPM_MPM_CFG] = 0x00,

	.awake_vlevel = 0xF2,
	.retention_vlevel = 0xE0,
	.collapse_vlevel = 0x72,
	.retention_mid_vlevel = 0xE0,
	.collapse_mid_vlevel = 0xE0,

	.vctl_timeout_us = 50,
};

#ifdef CONFIG_INPUT_KEYRESET
#include <linux/keyreset.h>
/* keyreset platform device */
static int mogami_reset_keys_up[] = {
	KEY_VOLUMEDOWN,
	0
};

static struct keyreset_platform_data mogami_reset_keys_pdata = {
	.keys_up = mogami_reset_keys_up,
	.keys_down = {
		KEY_POWER,
		KEY_HOME,
		0
	},
};

struct platform_device mogami_reset_keys_device = {
	.name = KEYRESET_NAME,
	.dev    = {
		.platform_data = &mogami_reset_keys_pdata,
	},
};
#endif

static void __init msm7x30_init(void)
{
	unsigned smem_size;
	uint32_t soc_version = 0;

	soc_version = socinfo_get_version();

	msm_clock_init(&msm7x30_clock_init_data);
#ifdef CONFIG_SERIAL_MSM_CONSOLE
	msm7x30_init_uart3();
#endif
	msm_spm_init(&msm_spm_data, 1);
	platform_device_register(&msm7x30_device_acpuclk);
#ifdef CONFIG_USB_MSM_OTG_72K
	if (SOCINFO_VERSION_MAJOR(soc_version) >= 2 &&
			SOCINFO_VERSION_MINOR(soc_version) >= 1) {
		pr_debug("%s: SOC Version:2.(1 or more)\n", __func__);
		msm_otg_pdata.ldo_set_voltage = 0;
	}
	msm_device_otg.dev.platform_data = &msm_otg_pdata;
#ifdef CONFIG_USB_GADGET
	msm_otg_pdata.swfi_latency =
 	msm_pm_data
 	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency;
	msm_device_gadget_peripheral.dev.platform_data = &msm_gadget_pdata;
#endif
#endif
	msm_uart_dm1_pdata.wakeup_irq = gpio_to_irq(136);
	msm_device_uart_dm1.dev.platform_data = &msm_uart_dm1_pdata;
	msm_adc_pdata.dev_names = msm_adc_surf_device_names;
	msm_adc_pdata.num_adc = ARRAY_SIZE(msm_adc_surf_device_names);
	buses_init();
#ifdef CONFIG_MSM_SSBI
	msm_device_ssbi_pmic1.dev.platform_data =
				&msm7x30_ssbi_pm8058_pdata;
#endif
	platform_add_devices(msm_footswitch_devices,
			     msm_num_footswitch_devices);
	platform_add_devices(devices, ARRAY_SIZE(devices));
	mogami_temp_fixups();
#ifdef CONFIG_SEMC_CHARGER_USB_ARCH
	semc_chg_usb_set_supplicants(semc_chg_usb_supplied_to,
				  ARRAY_SIZE(semc_chg_usb_supplied_to));
#endif
#ifdef CONFIG_USB_EHCI_MSM_72K
	msm_add_host(0, &msm_usb_host_pdata);
#endif
	msm7x30_init_mmc();
	msm_qsd_spi_init();
	msm7x30_init_nand();
#ifdef CONFIG_BT
	bluetooth_power(0);
#endif
	atv_dac_power_init();
#ifdef CONFIG_INPUT_KEYRESET
	platform_device_register(&mogami_reset_keys_device);
#endif
	msm_fb_add_devices();
	msm_pm_set_platform_data(msm_pm_data, ARRAY_SIZE(msm_pm_data));
	BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));
	msm_pm_register_irqs();
	msm_device_i2c_init();
	msm_device_i2c_2_init();
	qup_device_i2c_init();
	msm7x30_init_marimba();
#ifdef CONFIG_MSM7KV2_AUDIO
	snddev_poweramp_gpio_init();
	snddev_hsed_voltage_init();
	aux_pcm_gpio_init();
#endif
	hw_id_class_init();
	shared_vreg_on();
#ifdef CONFIG_TOUCHSCREEN_CY8CTMA300_SPI
	cypress_touch_gpio_init();
#endif /* CONFIG_TOUCHSCREEN_CY8CTMA300_SPI */
	msm_init_pmic_vibrator();

	i2c_register_board_info(0, msm_i2c_board_info,
			ARRAY_SIZE(msm_i2c_board_info));

	i2c_register_board_info(2, msm_marimba_board_info,
			ARRAY_SIZE(msm_marimba_board_info));

	i2c_register_board_info(4 /* QUP ID */, msm_camera_boardinfo,
				ARRAY_SIZE(msm_camera_boardinfo));
	spi_register_board_info(spi_board_info,
		ARRAY_SIZE(spi_board_info));

#ifdef CONFIG_I2C_SSBI
	msm_device_ssbi7.dev.platform_data = &msm_i2c_ssbi7_pdata;
#endif

	pm8058_gpios_init();

#if defined(CONFIG_FB_MSM_MDDI_SONY_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_HITACHI_HVGA_LCD) || \
	defined(CONFIG_FB_MSM_MDDI_SII_HVGA_LCD)
	semc_mogami_lcd_power_on(11, 2, 21);
#endif
#if defined(CONFIG_FB_MSM_MDDI_AUO_HVGA_LCD)
	semc_mogami_lcd_power_on(2, 21, 51);
#endif

	boot_reason = *(unsigned int *)
		(smem_get_entry(SMEM_POWER_ON_STATUS_INFO, &smem_size));
	printk(KERN_NOTICE "Boot Reason = 0x%02x\n", boot_reason);
}

static unsigned pmem_sf_size = MSM_PMEM_SF_SIZE;
static int __init pmem_sf_size_setup(char *p)
{
	pmem_sf_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_sf_size", pmem_sf_size_setup);

static unsigned fb_size;
static int __init fb_size_setup(char *p)
{
	fb_size = memparse(p, NULL);
	return 0;
}
early_param("fb_size", fb_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;
static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_kernel_ebi0_size = PMEM_KERNEL_EBI0_SIZE;
static int __init pmem_kernel_ebi0_size_setup(char *p)
{
	pmem_kernel_ebi0_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_kernel_ebi0_size", pmem_kernel_ebi0_size_setup);

static struct memtype_reserve msm7x30_reserve_table[] __initdata = {
	[MEMTYPE_SMI] = {
	},
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};

static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_pdata.size = pmem_sf_size;
#endif
}

static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	msm7x30_reserve_table[p->memory_type].size += p->size;
}

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_pdata);
	msm7x30_reserve_table[MEMTYPE_EBI0].size += pmem_kernel_ebi0_size;
#endif
}

static void __init msm7x30_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
}

static int msm7x30_paddr_to_memtype(unsigned int paddr)
{
	if (paddr < phys_add)
		return MEMTYPE_EBI0;
	if (paddr >= phys_add && paddr < 0x80000000)
		return MEMTYPE_EBI1;
	return MEMTYPE_NONE;
}

static struct reserve_info msm7x30_reserve_info __initdata = {
	.memtype_reserve_table = msm7x30_reserve_table,
	.calculate_reserve_sizes = msm7x30_calculate_reserve_sizes,
	.paddr_to_memtype = msm7x30_paddr_to_memtype,
};

static void __init msm7x30_reserve(void)
{
	reserve_info = &msm7x30_reserve_info;
	msm_reserve();
#ifdef CONFIG_ANDROID_PERSISTENT_RAM
	add_persistent_ram();
#endif
}

static void __init msm7x30_allocate_memory_regions(void)
{
	void *addr;
	unsigned long size;

	size = fb_size ? : MSM_FB_SIZE;
	addr = alloc_bootmem_align(size, 0x1000);
	msm_fb_resources[0].start = __pa(addr);
	msm_fb_resources[0].end = msm_fb_resources[0].start + size - 1;
	pr_info("allocating %lu bytes at %p (%lx physical) for fb\n",
		size, addr, __pa(addr));
}

static void __init msm7x30_map_io(void)
{
	msm_shared_ram_phys = 0x00100000;
	msm_map_msm7x30_io();
	if (socinfo_init() < 0)
		printk(KERN_ERR "%s: socinfo_init() failed!\n",
		       __func__);
}

static void __init msm7x30_init_early(void)
{
	msm7x30_allocate_memory_regions();
}

static void __init msm7x30_fixup(struct tag *tags, char **cmdline,
				 struct meminfo *mi)
{
	mi->nr_banks = 3;
	mi->bank[0].start = DDR0_BANK_BASE;
	mi->bank[0].size = DDR0_BANK_SIZE;
	mi->bank[1].start = DDR1_BANK_BASE;
	mi->bank[1].size = DDR1_BANK_SIZE;
	mi->bank[2].start = DDR2_BANK_BASE;
	mi->bank[2].size = DDR2_BANK_SIZE;
}

MACHINE_START(SEMC_MOGAMI, "mogami")
	.atag_offset = 0x100,
	.map_io = msm7x30_map_io,
	.reserve = msm7x30_reserve,
	.init_irq = msm7x30_init_irq,
	.init_machine = msm7x30_init,
	.timer = &msm_timer,
	.init_early = msm7x30_init_early,
	.handle_irq = vic_handle_irq,
	.fixup = msm7x30_fixup,
MACHINE_END
