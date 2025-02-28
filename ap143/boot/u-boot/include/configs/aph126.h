/*
 * This file contains the configuration parameters for the db120 board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/ar7240.h>

#if !defined(CONFIG_ATH_NAND_BR)
#define FLASH_SIZE 16
#endif
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS     1	    /* max number of memory banks */
#if (FLASH_SIZE == 16)
#define CFG_MAX_FLASH_SECT      256    /* max number of sectors on one chip */
#elif (FLASH_SIZE == 8)
#define CFG_MAX_FLASH_SECT      128    /* max number of sectors on one chip */
#else
#define CFG_MAX_FLASH_SECT      64    /* max number of sectors on one chip */
#endif

#define CFG_FLASH_SECTOR_SIZE   (64*1024)
#if (FLASH_SIZE == 16)
#define CFG_FLASH_SIZE          0x01000000 /* Total flash size */
#elif (FLASH_SIZE == 8)
#define CFG_FLASH_SIZE          0x00800000    /* max number of sectors on one chip */
#else
#define CFG_FLASH_SIZE          0x00400000 /* Total flash size */
#endif

#ifndef COMPRESSED_UBOOT
#define ENABLE_DYNAMIC_CONF 1
#endif

#define CONFIG_WASP_SUPPORT 1
#undef CFG_ATHRS26_PHY

#if (CFG_MAX_FLASH_SECT * CFG_FLASH_SECTOR_SIZE) != CFG_FLASH_SIZE
#	error "Invalid flash configuration"
#endif

#define CFG_FLASH_WORD_SIZE     unsigned short 

// Set this to zero, for 16bit ddr2
#define DDR2_32BIT_SUPPORT 	1

/* S16 specific defines */

#define CONFIG_AG7240_GE0_IS_CONNECTED 1

/* Assuming nand flash wouldn't use compressed u-boot */
#ifdef CONFIG_ATH_NAND_BR
#	define CFG_FLASH_BASE			0
#else
	/*
	 * We boot from this flash
	 */
#	define CFG_FLASH_BASE			0x9f000000
#endif

#ifdef COMPRESSED_UBOOT
#define BOOTSTRAP_TEXT_BASE         CFG_FLASH_BASE
#define BOOTSTRAP_CFG_MONITOR_BASE  BOOTSTRAP_TEXT_BASE
#endif

/*
 * Defines to change flash size on reboot
 */
#ifdef ENABLE_DYNAMIC_CONF
#define UBOOT_FLASH_SIZE          (256 * 1024)
#define UBOOT_ENV_SEC_START        (CFG_FLASH_BASE + UBOOT_FLASH_SIZE)

#define CFG_FLASH_MAGIC           0xaabacada  
#define CFG_FLASH_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x20)
#define CFG_FLASH_SECTOR_SIZE_F   *(volatile int *)(CFG_FLASH_MAGIC_F + 0x4)
#define CFG_FLASH_SIZE_F          *(volatile int *)(CFG_FLASH_MAGIC_F + 0x8) /* Total flash size */
#define CFG_MAX_FLASH_SECT_F      (CFG_FLASH_SIZE / CFG_FLASH_SECTOR_SIZE) /* max number of sectors on one chip */
#else
#define CFG_FLASH_SIZE_F          CFG_FLASH_SIZE
#define CFG_FLASH_SECTOR_SIZE_F   CFG_FLASH_SECTOR_SIZE
#endif


/* 
 * The following #defines are needed to get flash environment right 
 */
#define	CFG_MONITOR_BASE	TEXT_BASE
#define	CFG_MONITOR_LEN		(192 << 10)

/* Address and size of Primary Environment Sector   */
#define CFG_ENV_ADDR        (CFG_FLASH_BASE+UBOOT_FLASH_SIZE)
#define CFG_ENV_SIZE        CFG_FLASH_SECTOR_SIZE


#undef CONFIG_BOOTARGS

#if (FLASH_SIZE == 16)
#define CONFIG_BOOTARGS     "console=ttyS0,115200 root=31:02 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),6336k(rootfs),1408k(uImage),8192k(reserve),64k(mib0),64k(ART)"
/* default mtd partition table */
#define MTDPARTS_DEFAULT    "mtdparts=ath-nor0:256k(u-boot),256k(u-boot-env),5120k(rootfs),1024k(uImage)"
#else /* FLASH_SIZE=8M */
#ifdef COMPRESSED_UBOOT
#define	CONFIG_BOOTARGS     "console=ttyS0,115200 root=31:01 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:64k(u-boot),6592k(rootfs),1408k(uImage),64k(mib0),64k(ART)"
/* default mtd partition table */
#define MTDPARTS_DEFAULT    "mtdparts=ath-nor0:64k(u-boot),6592k(rootfs),1408k(uImage),64k(mib0),64k(ART)"
#else
#define	CONFIG_BOOTARGS     "console=ttyS0,115200 root=31:02 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),6336k(rootfs),1408k(uImage),64k(mib0),64k(ART)"
/* default mtd partition table */
#define MTDPARTS_DEFAULT    "mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),6336k(rootfs),1408k(uImage),64k(mib0),64k(ART)"
#endif
#endif

#if (FLASH_SIZE == 16)
#define CFG_KERNEL_ADDR 0x9f680000
#else
#define CFG_KERNEL_ADDR 0x9f680000
#endif
#define CFG_MIB0_ADDR		0x9ffe0000

#define CONFIG_BOOTCOMMAND "bootm ${KERNELADDR}"

#ifndef CFG_LOAD_ADDR
#define CFG_LOAD_ADDR		0x80060000
#endif
#ifndef COMPRESSED_UBOOT
#define CFG_FS_ADDR			0x9f050000//(CFG_ENV_ADDR+CFG_ENV_SIZE)
#else
#define CFG_FS_ADDR			0x9f010000//(CFG_FLASH_BASE+UBOOT_FLASH_SIZE)
#endif /* COMPRESSED_UBOOT */
#define ENV_FS_ADDR			"FSADDR=" MK_STR(CFG_FS_ADDR) "\0"

#define ENV_UBOOT_ADDR		"UBOOTADDR=" MK_STR(CFG_FLASH_BASE) "\0"
#define ENV_KERNEL_ADDR		"KERNELADDR=" MK_STR(CFG_KERNEL_ADDR) "\0"
#define ENV_MIB0_ADDR		"MIB0ADDR=" MK_STR(CFG_MIB0_ADDR) "\0"

#define ENV_UPDATE \
    "update=tftpboot " MK_STR(CFG_LOAD_ADDR) " ${filename} && "	\
            "erase ${loadaddr} +${filesize} && "				\
            "cp.b ${fileaddr} ${loadaddr} ${filesize}\0"

#define ENV_UPDATE_KERNEL \
    "updk=loadaddr=${KERNELADDR} && filename=zImage.uImage && " \
    "run update\0"

#define ENV_UPDATE_FS \
    "updfs=loadaddr=${FSADDR} && filename=rootfs-jffs2 && " \
    "run update\0"

#ifdef COMPRESSED_UBOOT
#define UBOOT_FILENAME "tuboot.bin"
#else
#define UBOOT_FILENAME "u-boot.bin"
#endif

#define ENV_UPDATE_UBOOT \
    "updub=loadaddr=${UBOOTADDR} && filename=" UBOOT_FILENAME " && " \
    "run update\0"

#define ENV_UPDATE_ALL \
	"updall=run updub && run updfs && run updk\0"

#define ENV_ERASE_MIB0 \
	"erasemib0=erase ${MIB0ADDR} +0x10000"

#define CONFIG_EXTRA_ENV_SETTINGS \
		ENV_UBOOT_ADDR \
		ENV_FS_ADDR \
		ENV_KERNEL_ADDR \
		ENV_MIB0_ADDR \
		ENV_UPDATE \
		ENV_UPDATE_KERNEL \
		ENV_UPDATE_FS \
		ENV_UPDATE_UBOOT \
		ENV_UPDATE_ALL \
		ENV_ERASE_MIB0 \
		""

#undef CFG_PLL_FREQ

//#define CFG_PLL_FREQ	CFG_PLL_600_266_133
//#define CFG_PLL_FREQ	CFG_PLL_600_525_262
//#define CFG_PLL_FREQ	CFG_PLL_566_550_275
//#define CFG_PLL_FREQ	CFG_PLL_566_525_262
//#define CFG_PLL_FREQ	CFG_PLL_600_332_166
//#define CFG_PLL_FREQ	CFG_PLL_566_475_237
//#define CFG_PLL_FREQ	CFG_PLL_600_575_287
//#define CFG_PLL_FREQ	CFG_PLL_400_400_200
//#define CFG_PLL_FREQ	CFG_PLL_533_400_200
//#define CFG_PLL_FREQ	CFG_PLL_600_450_200
//#define CFG_PLL_FREQ	CFG_PLL_600_500_1G_250
//#define CFG_PLL_FREQ	CFG_PLL_600_550_1_1G_275
//#define CFG_PLL_FREQ	CFG_PLL_600_350_175
//#define CFG_PLL_FREQ	CFG_PLL_600_300_150
//#define CFG_PLL_FREQ	CFG_PLL_600_1_2G_400_200
//#define CFG_PLL_FREQ	CFG_PLL_300_300_150
//#define CFG_PLL_FREQ	CFG_PLL_500_400_200
//#define CFG_PLL_FREQ	CFG_PLL_400_200_200
#if !defined(CONFIG_AP123) && !defined(CONFIG_MI124)
#define CFG_PLL_FREQ    CFG_PLL_560_480_240
#else
#define CFG_PLL_FREQ    CFG_PLL_533_400_200
#endif

#undef CFG_HZ
/*
 * MIPS32 24K Processor Core Family Software User's Manual
 *
 * 6.2.9 Count Register (CP0 Register 9, Select 0)
 * The Count register acts as a timer, incrementing at a constant
 * rate, whether or not an instruction is executed, retired, or
 * any forward progress is made through the pipeline.  The counter
 * increments every other clock, if the DC bit in the Cause register
 * is 0.
 */
/* Since the count is incremented every other tick, divide by 2 */
/* XXX derive this from CFG_PLL_FREQ */
#if (CFG_PLL_FREQ == CFG_PLL_200_200_100)
#   define CFG_HZ          (200000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_300_300_150)
#   define CFG_HZ          (300000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_350_350_175)
#   define CFG_HZ          (350000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_333_333_166)
#   define CFG_HZ          (333000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_133)
#   define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_66)
#   define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_400_400_200) || (CFG_PLL_FREQ == CFG_PLL_400_400_100)
#   define CFG_HZ          (400000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_320_320_80) || (CFG_PLL_FREQ == CFG_PLL_320_320_160)
#   define CFG_HZ          (320000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_410_400_200)
#   define CFG_HZ          (410000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_420_400_200)
#   define CFG_HZ          (420000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_240_240_120)
#   define CFG_HZ          (240000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_160_160_80)
#   define CFG_HZ          (160000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_400_200_200)
#   define CFG_HZ          (400000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_500_400_200)
#   define CFG_HZ          (500000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_400_200) || (CFG_PLL_FREQ == CFG_PLL_600_450_200) || \
      (CFG_PLL_FREQ == CFG_PLL_600_600_300) || (CFG_PLL_FREQ == CFG_PLL_600_550_275) || \
      (CFG_PLL_FREQ == CFG_PLL_600_332_166) || (CFG_PLL_FREQ == CFG_PLL_600_575_287) || \
      (CFG_PLL_FREQ == CFG_PLL_600_525_262) || (CFG_PLL_FREQ == CFG_PLL_600_332_200) || \
      (CFG_PLL_FREQ == CFG_PLL_600_266_133) || (CFG_PLL_FREQ == CFG_PLL_600_266_200) || \
      (CFG_PLL_FREQ == CFG_PLL_600_650_325) ||  (CFG_PLL_FREQ == CFG_PLL_600_1_2G_400_200)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_1_2G_400_200)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_500_1G_250)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_550_1_1G_275)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_500_250)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_350_175)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_300_150)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_533_400_200) || (CFG_PLL_FREQ == CFG_PLL_533_500_250)
#   define CFG_HZ          (533000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_700_400_200)
#   define CFG_HZ          (700000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_650_600_300)
#   define CFG_HZ          (650000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_560_480_240)
#   define CFG_HZ          (560000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_566_475_237) || (CFG_PLL_FREQ == CFG_PLL_566_450_225) || \
      (CFG_PLL_FREQ == CFG_PLL_566_550_275) || (CFG_PLL_FREQ == CFG_PLL_566_525_262) || \
      (CFG_PLL_FREQ == CFG_PLL_566_400_200) || (CFG_PLL_FREQ == CFG_PLL_566_500_250)
#   define CFG_HZ          (566000000/2)
#endif

/*
 * timeout values are in ticks
 */
#define CFG_FLASH_ERASE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

/*
 * Cache lock for stack
 */
#define CFG_INIT_SP_OFFSET	0x1000
#define CFG_INIT_SRAM_SP_OFFSET 0xbd007000

#ifndef COMPRESSED_UBOOT
#define	CFG_ENV_IS_IN_FLASH    1
#undef CFG_ENV_IS_NOWHERE
#else
#undef  CFG_ENV_IS_IN_FLASH
#define CFG_ENV_IS_NOWHERE  1
#endif

#if 0
#ifdef CONFIG_ATH_NAND_BR
#define CONFIG_BOOTCOMMAND "bootm 0x80000"
#define CFG_ENV_ADDR		0x00040000
#else  /* CONFIG_ATH_NAND_BR */
#define CFG_ENV_ADDR		0x9f040000
#if (FLASH_SIZE == 16)
#define CONFIG_BOOTCOMMAND "bootm 0x9f550000"
#else  /* FLASH_SIZE == 16 */
#ifdef COMPRESSED_UBOOT
#if (FLASH_SIZE == 4)
#define CONFIG_BOOTCOMMAND "bootm 0x9f300000"
#else  /* FLASH_SIZE == 4 */
#define CONFIG_BOOTCOMMAND "bootm 0x9f320000"
#endif  /* FLASH_SIZE == 4 */
#else /* COMPRESSED_UBOOT */
#if defined(CONFIG_MI124) || defined(CONFIG_AP123)
#define CONFIG_BOOTCOMMAND "bootm 0x9f300000"
#else
#define CONFIG_BOOTCOMMAND "bootm 0x9f680000"
#endif
#endif  /* COMPRESSED_UBOOT */
#endif  /* FLASH_SIZE == 16 */
#endif  /* CONFIG_ATH_NAND_BR */
#endif



//#define CONFIG_FLASH_16BIT

/* DDR init values */

#define CONFIG_NR_DRAM_BANKS	2

/* DDR settings for WASP */

#define CFG_DDR_REFRESH_VAL     0x4270
#define CFG_DDR_CONFIG_VAL      0xc7bc8cd0
#define CFG_DDR_MODE_VAL_INIT   0x133
#define CFG_DDR_EXT_MODE_VAL    0x0
#define CFG_DDR_MODE_VAL        0x33

#define CFG_DDR_TRTW_VAL        0x1f
#define CFG_DDR_TWTR_VAL        0x1e

#define CFG_DDR_CONFIG2_VAL     0x9dd0e6a8



#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32	0xff
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16	0xffff

#if DDR2_32BIT_SUPPORT
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL		CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32
#else
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL		CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16
#endif

#define CFG_DDR1_RD_DATA_THIS_CYCLE_VAL		0xffff
#define CFG_SDRAM_RD_DATA_THIS_CYCLE_VAL	0xffffffff

/* DDR2 Init values */
#define CFG_DDR2_EXT_MODE_VAL    0x402


#ifdef ENABLE_DYNAMIC_CONF
#define CFG_DDR_MAGIC           0xaabacada
#define CFG_DDR_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x30)
#define CFG_DDR_CONFIG_VAL_F    *(volatile int *)(CFG_DDR_MAGIC_F + 4)
#define CFG_DDR_CONFIG2_VAL_F	*(volatile int *)(CFG_DDR_MAGIC_F + 8)
#define CFG_DDR_EXT_MODE_VAL_F  *(volatile int *)(CFG_DDR_MAGIC_F + 12)
#endif

#define CONFIG_NET_MULTI
#define CONFIG_MEMSIZE_IN_BYTES
#define CONFIG_PCI 1

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#ifndef COMPRESSED_UBOOT
#define CONFIG_COMMANDS	(( CONFIG_CMD_DFL | CFG_CMD_DHCP | CFG_CMD_ELF | CFG_CMD_PCI | CFG_CMD_FLS |\
	CFG_CMD_MII | CFG_CMD_PING | CFG_CMD_NET | CFG_CMD_ENV | CFG_CMD_PLL| \
	CFG_CMD_FLASH | CFG_CMD_LOADS | CFG_CMD_RUN | CFG_CMD_LOADB | CFG_CMD_ELF | CFG_CMD_DDR | CFG_CMD_ETHREG))
#else
#define CONFIG_COMMANDS (( CONFIG_CMD_DFL | CFG_CMD_PING | CFG_CMD_NET))
#endif /* #ifndef COMPRESSED_UBOOT */

#define CONFIG_IPADDR   192.168.1.1
#define CONFIG_SERVERIP 192.168.1.10
#define CONFIG_ETHADDR 0x00:0xaa:0xbb:0xcc:0xdd:0xee
#define CFG_FAULT_ECHO_LINK_DOWN    1


#define CFG_PHY_ADDR 0 
#define CFG_GMII     0
#define CFG_MII0_RMII             1
#define CFG_AG7100_GE0_RMII             1

#define CFG_BOOTM_LEN	(16 << 20) /* 16 MB */
#define DEBUG
#define CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2 "hush>"

/*
** Parameters defining the location of the calibration/initialization
** information for the two Merlin devices.
** NOTE: **This will change with different flash configurations**
*/

#define WLANCAL                         0x9fff1000
#define BOARDCAL                        0x9fff0000
#define ATHEROS_PRODUCT_ID              137
#define CAL_SECTOR                      (CFG_MAX_FLASH_SECT - 1)

/* For Merlin, both PCI, PCI-E interfaces are valid */
#define AR7240_ART_PCICFG_OFFSET        12

#include <cmd_confdefs.h>

#endif	/* __CONFIG_H */
