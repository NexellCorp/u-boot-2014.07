#ifndef _MICREL_H

#define MII_KSZ9021_EXT_COMMON_CTRL			0x100
#define MII_KSZ9021_EXT_STRAP_STATUS		0x101
#define MII_KSZ9021_EXT_OP_STRAP_OVERRIDE	0x102
#define MII_KSZ9021_EXT_OP_STRAP_STATUS		0x103
#define MII_KSZ9021_EXT_RGMII_CLOCK_SKEW	0x104
#define MII_KSZ9021_EXT_RGMII_RX_DATA_SKEW	0x105
#define MII_KSZ9021_EXT_RGMII_TX_DATA_SKEW	0x106
#define MII_KSZ9021_EXT_ANALOG_TEST			0x107
/* Register operations */
#define MII_KSZ9031_MOD_REG					0x0000
/* Data operations */
#define MII_KSZ9031_MOD_DATA_NO_POST_INC	0x4000
#define MII_KSZ9031_MOD_DATA_POST_INC_RW	0x8000
#define MII_KSZ9031_MOD_DATA_POST_INC_W		0xC000

/* MMD Address 2h registers */
#define MII_KSZ9031_EXT_RGMII_CTRL_SIG_SKEW	0x4
#define MII_KSZ9031_EXT_RGMII_RX_DATA_SKEW	0x5
#define MII_KSZ9031_EXT_RGMII_TX_DATA_SKEW	0x6
#define MII_KSZ9031_EXT_RGMII_CLOCK_SKEW	0x8

#define MII_KSZ9031_EXT_RGMII_RX_DV_SHIFT	4
#define MII_KSZ9031_EXT_RGMII_TX_EN_SHIFT	0

#define MII_KSZ9031_EXT_RGMII_RX3_SHIFT		12
#define MII_KSZ9031_EXT_RGMII_RX2_SHIFT		8
#define MII_KSZ9031_EXT_RGMII_RX1_SHIFT		4
#define MII_KSZ9031_EXT_RGMII_RX0_SHIFT		0

#define MII_KSZ9031_EXT_RGMII_TX3_SHIFT		12
#define MII_KSZ9031_EXT_RGMII_TX2_SHIFT		8
#define MII_KSZ9031_EXT_RGMII_TX1_SHIFT		4
#define MII_KSZ9031_EXT_RGMII_TX0_SHIFT		0

#define MII_KSZ9031_EXT_RGMII_GTX_CLK_SHIFT	5
#define MII_KSZ9031_EXT_RGMII_RX_CLK_SHIFT	0

/* MMD Address 7h registers */
#define MII_KSZ9031_EXT_EEE_ADVERTISEMENT	0x3C
#define MII_KSZ9031_EXT_EEE_1000BASE_SHIFT  2
#define MII_KSZ9031_EXT_EEE_100BASE_SHIFT	1


struct phy_device;
int ksz9021_phy_extended_write(struct phy_device *phydev, int regnum, u16 val);
int ksz9021_phy_extended_read(struct phy_device *phydev, int regnum);

int ksz9031_phy_extended_write(struct phy_device *phydev, int devaddr,
						int regnum, u16 mode, u16 val);
int ksz9031_phy_extended_read(struct phy_device *phydev, int devaddr,
						int regnum, u16 mode);

#endif
