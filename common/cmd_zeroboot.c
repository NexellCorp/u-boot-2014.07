#include <common.h>
#include <command.h>
#include <nand.h>
#include <mmc.h>
#include "zbi.h"

#define	get_storage_priv_offset()	(((zbi_t *)ZBI_PADDR)->storage_priv)

#define DW_MMC_DMA_COUNT_MAX	2048
u32 dw_mmc_start_block_index = 0;
u32 dw_mmc_dma_addr_count = 0;
u32 dw_mmc_dma_addr_list[DW_MMC_DMA_COUNT_MAX];
static u32  mmc_block_size =  512;  
#define MMC_START_BLOCK			(ZB_STORAGE_OFFSET/mmc_block_size)

struct zb_priv_mmc *zb_mmc;
struct zb_priv_mmc zero_mmc;


#define ZB_STORAGE_OFFSET 0x280000 //0x0 //0x3100000 //0x5b00000 //0x10000000
#define NAND_PAGE_SIZE 4096
#define MMC_PAGE_SIZE 4096
//#define ARG_NAL_PHYS_OFFSET 0x40000000
#define ARG_NAL_PAGE_OFFSET 0x80000000



#define DWMCI_CTRL		0x000
#define	DWMCI_PWREN		0x004
#define DWMCI_CLKDIV		0x008
#define DWMCI_CLKSRC		0x00C
#define DWMCI_CLKENA		0x010
#define DWMCI_TMOUT		0x014
#define DWMCI_CTYPE		0x018
#define DWMCI_BLKSIZ		0x01C
#define DWMCI_BYTCNT		0x020
#define DWMCI_INTMASK		0x024
#define DWMCI_CMDARG		0x028
#define DWMCI_CMD		0x02C
#define DWMCI_RESP0		0x030
#define DWMCI_RESP1		0x034
#define DWMCI_RESP2		0x038
#define DWMCI_RESP3		0x03C
#define DWMCI_MINTSTS		0x040
#define DWMCI_RINTSTS		0x044
#define DWMCI_STATUS		0x048
#define DWMCI_FIFOTH		0x04C
#define DWMCI_CDETECT		0x050
#define DWMCI_WRTPRT		0x054
#define DWMCI_GPIO		0x058
#define DWMCI_TCMCNT		0x05C
#define DWMCI_TBBCNT		0x060
#define DWMCI_DEBNCE		0x064
#define DWMCI_USRID		0x068
#define DWMCI_VERID		0x06C
#define DWMCI_HCON		0x070
#define DWMCI_UHS_REG		0x074
#define DWMCI_BMOD		0x080
#define DWMCI_PLDMND		0x084
#define DWMCI_DBADDR		0x088
#define DWMCI_IDSTS		0x08C
#define DWMCI_IDINTEN		0x090
#define DWMCI_DSCADDR		0x094
#define DWMCI_BUFADDR		0x098
#define DWMCI_DATA		0x200
#define DWMCI_CLKCTRL		0x114 //JHYOON  /*add by YoungbokPark */

/* Internal DMAC interrupt defines */  //JHYOON
#define SDMMC_IDMAC_INT_AI		(1 << 9)//BIT(9)
#define SDMMC_IDMAC_INT_NI		(1 << 8)//BIT(8)
#define SDMMC_IDMAC_INT_CES		(1 << 5)//BIT(5)
#define SDMMC_IDMAC_INT_DU		(1 << 4)//BIT(4)
#define SDMMC_IDMAC_INT_FBE		(1 << 2)//BIT(2)
#define SDMMC_IDMAC_INT_RI		(1 << 1)//BIT(1)
#define SDMMC_IDMAC_INT_TI		(1 << 0)//BIT(0)

/* Interrupt Mask register */
#define DWMCI_INTMSK_ALL	0xffffffff
#define DWMCI_INTMSK_RE		(1 << 1)
#define DWMCI_INTMSK_CDONE	(1 << 2)
#define DWMCI_INTMSK_DTO	(1 << 3)
#define DWMCI_INTMSK_TXDR	(1 << 4)
#define DWMCI_INTMSK_RXDR	(1 << 5)
#define DWMCI_INTMSK_DCRC	(1 << 7)
#define DWMCI_INTMSK_RTO	(1 << 8)
#define DWMCI_INTMSK_DRTO	(1 << 9)
#define DWMCI_INTMSK_HTO	(1 << 10)
#define DWMCI_INTMSK_FRUN	(1 << 11)
#define DWMCI_INTMSK_HLE	(1 << 12)
#define DWMCI_INTMSK_SBE	(1 << 13)
#define DWMCI_INTMSK_ACD	(1 << 14)
#define DWMCI_INTMSK_EBE	(1 << 15)

#define DWMCI_INTMSK_RCRC	(1 << 6)  //JHYOON

#define DWMCI_INTMSK_ERROR_FLAGS ( DWMCI_INTMSK_DRTO|DWMCI_INTMSK_DCRC|DWMCI_INTMSK_HTO| \
										DWMCI_INTMSK_SBE | DWMCI_INTMSK_EBE|DWMCI_INTMSK_RTO| \
										DWMCI_INTMSK_RCRC |DWMCI_INTMSK_RE|DWMCI_INTMSK_HLE  )

/* Raw interrupt Regsiter */
#define DWMCI_DATA_ERR	(DWMCI_INTMSK_EBE | DWMCI_INTMSK_SBE | DWMCI_INTMSK_HLE |\
			DWMCI_INTMSK_FRUN | DWMCI_INTMSK_EBE | DWMCI_INTMSK_DCRC)
#define DWMCI_DATA_TOUT	(DWMCI_INTMSK_HTO | DWMCI_INTMSK_DRTO)
/* CTRL register */
#define DWMCI_CTRL_RESET	(1 << 0)
#define DWMCI_CTRL_FIFO_RESET	(1 << 1)
#define DWMCI_CTRL_DMA_RESET	(1 << 2)
#define DWMCI_DMA_EN		(1 << 5)
#define DWMCI_CTRL_SEND_AS_CCSD	(1 << 10)
#define DWMCI_IDMAC_EN		(1 << 25)
#define DWMCI_RESET_ALL		(DWMCI_CTRL_RESET | DWMCI_CTRL_FIFO_RESET |\
				DWMCI_CTRL_DMA_RESET)
#define DWMCI_INT_ENABLE		(1 <<4)  //JHYOON

/* CMD register */
#define DWMCI_CMD_RESP_EXP	(1 << 6)
#define DWMCI_CMD_RESP_LENGTH	(1 << 7)
#define DWMCI_CMD_CHECK_CRC	(1 << 8)
#define DWMCI_CMD_DATA_EXP	(1 << 9)
#define DWMCI_CMD_RW		(1 << 10)
#define DWMCI_CMD_SEND_STOP	(1 << 12)
#define DWMCI_CMD_ABORT_STOP	(1 << 14)
#define DWMCI_CMD_PRV_DAT_WAIT	(1 << 13)
#define DWMCI_CMD_UPD_CLK	(1 << 21)
#define DWMCI_CMD_USE_HOLD_REG	(1 << 29)
#define DWMCI_CMD_START		(1 << 31)
#define DWMCI_CMD_INIT		(1 << 15) //JHYOON

/* CLKENA register */
#define DWMCI_CLKEN_ENABLE	(1 << 0)
#define DWMCI_CLKEN_LOW_PWR	(1 << 16)

/* Card-type registe */
#define DWMCI_CTYPE_1BIT	0
#define DWMCI_CTYPE_4BIT	(1 << 0)
#define DWMCI_CTYPE_8BIT	(1 << 16)

/* Status Register */
#define DWMCI_BUSY		(1 << 9)

/* FIFOTH Register */
#define MSIZE(x)		((x) << 28)
#define RX_WMARK(x)		((x) << 16)
#define TX_WMARK(x)		(x)
#define RX_WMARK_SHIFT		16
#define RX_WMARK_MASK		(0xfff << RX_WMARK_SHIFT)

#define DWMCI_IDMAC_OWN		(1 << 31)
#define DWMCI_IDMAC_CH		(1 << 4)
#define DWMCI_IDMAC_FS		(1 << 3)
#define DWMCI_IDMAC_LD		(1 << 2)

/*  Bus Mode Register */
#define DWMCI_BMOD_IDMAC_RESET	(1 << 0)
#define DWMCI_BMOD_IDMAC_FB	(1 << 1)
#define DWMCI_BMOD_IDMAC_EN	(1 << 7)

/* UHS register */
#define DWMCI_DDR_MODE	(1 << 16)

/* quirks */
#define DWMCI_QUIRK_DISABLE_SMU		(1 << 0)


struct dwmci_idmac {
	u32 flags;
	u32 cnt;
	u32 addr;
	u32 next_addr;
};


struct zb_priv_mmc {
	union {
		struct {
			unsigned int ioaddr;
			unsigned int ioaddr_phys;
			unsigned int data_buf;
			unsigned int data_buf_phys;
			unsigned int idmac_buf;
			unsigned int idmac_buf_phys;	
			unsigned int read_bl_len;
			unsigned int write_bl_len;
			unsigned int erase_grp_size;
			unsigned int high_capacity;
			short	    rca;
			unsigned int card_caps;
			unsigned int bus_width;
			unsigned int clock;
			unsigned int ocr;
			unsigned int version;
			unsigned int host_sg_dma_addr;
		};
	};

	u32 storage_priv[128];	// 최대사이즈.
};

static inline u32 readl(const volatile u32 *addr)
{
  return *(const volatile u32  *) addr;
}

static inline void writel(u32 b, volatile u32  *addr)
{
       *(volatile u32  *) addr = b;
}

static inline void dwmci_writel(struct zb_priv_mmc *mmc, int reg, u32 val)
{
	writel(val, mmc->ioaddr + reg);
}

static inline u32 dwmci_readl(struct zb_priv_mmc *mmc, int reg)
{
	return readl(mmc->ioaddr + reg);
}


////////////////////// LOW LEVEL //////////////////////////////////////////////////////////////////////////
static int dwmci_wait_reset(struct zb_priv_mmc *mmc, u32 value)
{
	unsigned long timeout = 1000;
	u32 ctrl;

	dwmci_writel(mmc, DWMCI_CTRL, value);

	while (timeout--) {
		ctrl = dwmci_readl(mmc, DWMCI_CTRL);
		if (!(ctrl & DWMCI_RESET_ALL))
			return 1;
		udelay(10);
	}
	 serial_printf("Timeout on reset fifo\n");
	return 0;
}

static void dwmci_set_idma_desc(struct dwmci_idmac *idmac,
		u32 desc0, u32 desc1, u32 desc2)
{
	struct dwmci_idmac *desc = idmac;

	desc->flags = desc0;
	desc->cnt = desc1;
	desc->addr = desc2;
	desc->next_addr = (unsigned int)desc + sizeof(struct dwmci_idmac);
}

static void dwmci_prepare_data(struct zb_priv_mmc *mmc,
			       struct mmc_data *data,
			       struct dwmci_idmac *cur_idmac,
			       void *bounce_buffer)
{
	unsigned long ctrl;
	unsigned int i = 0, flags, cnt, blk_cnt;
	ulong data_start, data_end;


	blk_cnt = data->blocks;

	dwmci_wait_reset(mmc, DWMCI_CTRL_FIFO_RESET);

	data_start = (ulong)cur_idmac;
	dwmci_writel(mmc, DWMCI_DBADDR, (unsigned int)mmc->idmac_buf_phys);

	do {
		flags = DWMCI_IDMAC_OWN | DWMCI_IDMAC_CH ;
		flags |= (i == 0) ? DWMCI_IDMAC_FS : 0;
		if (blk_cnt <= 8) {
			flags |= DWMCI_IDMAC_LD;
			cnt = data->blocksize * blk_cnt;
		} else
			cnt = data->blocksize * 8;
		dwmci_set_idma_desc(cur_idmac, flags, cnt,
				    (u32)dw_mmc_dma_addr_list[i]);


		if (blk_cnt <= 8)
			break;
		blk_cnt -= 8;
		cur_idmac++;
		i++;
	} while(1);

	data_end = (ulong)cur_idmac;

	flush_dcache_all();
	//flush_dcache_range( (unsigned int)mmc->idmac_buf_phys, (unsigned int)mmc->idmac_buf_phys + PAGE_SIZE);

	ctrl = dwmci_readl(mmc, DWMCI_CTRL);
	ctrl |= DWMCI_IDMAC_EN | DWMCI_DMA_EN;
	dwmci_writel(mmc, DWMCI_CTRL, ctrl);

	ctrl = dwmci_readl(mmc, DWMCI_BMOD);
	ctrl |= DWMCI_BMOD_IDMAC_FB | DWMCI_BMOD_IDMAC_EN;
	dwmci_writel(mmc, DWMCI_BMOD, ctrl);

	dwmci_writel(mmc, DWMCI_BLKSIZ, data->blocksize);
	dwmci_writel(mmc, DWMCI_BYTCNT, data->blocksize * data->blocks);
}

static int dwmci_set_transfer_mode(struct zb_priv_mmc *mmc,
		struct mmc_data *data)
{
	unsigned long mode;

	mode = DWMCI_CMD_DATA_EXP;
	if (data->flags & MMC_DATA_WRITE)
		mode |= DWMCI_CMD_RW;

	return mode;
}

static int zb_mmc_send_cmd(struct zb_priv_mmc *mmc,struct mmc_cmd *cmd,
			struct mmc_data *data)
{
	//struct mmc_data backup;

	int flags = 0, i;

	u32 retry = 10000;
	u32 mask, ctrl;
	u32 count=0;


//// START
	while (dwmci_readl(mmc, DWMCI_STATUS) & DWMCI_BUSY) {
		if (  count > retry )
		{
		
			//FIXME
			serial_printf("Timeout on data busy\n");
			return -1;
		}
		count++;
		udelay(1);
	}

	
	dwmci_writel(mmc, DWMCI_RINTSTS, DWMCI_INTMSK_ALL);
	
	//dwmci_writel(mmc, DWMCI_CMD, 0);


	if (data) {
		
		
		dwmci_writel(mmc, DWMCI_DBADDR, (unsigned int)mmc->idmac_buf_phys);

	
		dwmci_prepare_data(mmc, data, mmc->idmac_buf, //JHYOON.. PHYS or VIRT???
				   mmc->data_buf);


	}

	dwmci_writel(mmc, DWMCI_CMDARG, cmd->cmdarg);

	if (data)
		flags = dwmci_set_transfer_mode(mmc, data);

	if ((cmd->resp_type & MMC_RSP_136) && (cmd->resp_type & MMC_RSP_BUSY))
		return -1;

	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION)
		flags |= DWMCI_CMD_ABORT_STOP;
	else
		flags |= DWMCI_CMD_PRV_DAT_WAIT;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		flags |= DWMCI_CMD_RESP_EXP;
		if (cmd->resp_type & MMC_RSP_136)
			flags |= DWMCI_CMD_RESP_LENGTH;
	}

	if (cmd->resp_type & MMC_RSP_CRC)
		flags |= DWMCI_CMD_CHECK_CRC;

	flags |= (cmd->cmdidx | DWMCI_CMD_START | DWMCI_CMD_USE_HOLD_REG);



	dwmci_writel(mmc, DWMCI_CMD, flags);

	
	for (i = 0; i < retry; i++) {
		mask = dwmci_readl(mmc, DWMCI_RINTSTS);
		if (mask & DWMCI_INTMSK_CDONE) {
			if (!data)
				dwmci_writel(mmc, DWMCI_RINTSTS, mask);
			break;
		}
		udelay(1);
	}

	if (i == retry) {
		serial_printf("DWMCI_INTMSK_CDONE TIME OUT...!\n");
		return -1;
	}
	if (mask & DWMCI_INTMSK_RTO) {
		serial_printf("Response Timeout..\n");
		return -1;
	} else if (mask & DWMCI_INTMSK_RE) {
		serial_printf("Response Error..\n");
		return -1;
	}


	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			cmd->response[0] = dwmci_readl(mmc, DWMCI_RESP3);
			cmd->response[1] = dwmci_readl(mmc, DWMCI_RESP2);
			cmd->response[2] = dwmci_readl(mmc, DWMCI_RESP1);
			cmd->response[3] = dwmci_readl(mmc, DWMCI_RESP0);
		} else {
			cmd->response[0] = dwmci_readl(mmc, DWMCI_RESP0);
		}
	}

	if (data) {
		do {
			retry = 10000 * 100;
			mask = dwmci_readl(mmc, DWMCI_RINTSTS);
			if (mask & (DWMCI_DATA_ERR | DWMCI_DATA_TOUT)) {
				serial_printf("DATA ERROR!\n");
				return -1;
			}
			udelay(1);
		} while (!(mask & DWMCI_INTMSK_DTO));
		if (retry ==0) {
			return -1;
		}
		
		dwmci_writel(mmc, DWMCI_RINTSTS, mask);

		ctrl = dwmci_readl(mmc, DWMCI_CTRL);
		ctrl &= ~(DWMCI_DMA_EN);
		dwmci_writel(mmc, DWMCI_CTRL, ctrl);
	
	}

	udelay(10);

	return 0; 
}


static int zb_mmc_read_blocks(struct zb_priv_mmc *mmc, void *dst, lbaint_t start,
			   lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;

	data.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	if (zb_mmc_send_cmd(mmc, &cmd, &data))
		return 0;

	if (blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		if (zb_mmc_send_cmd(mmc, &cmd, NULL)) {

			serial_printf("mmc fail to send stop cmd\n");

			return 0;
		}
	}

	return blkcnt;
}

static unsigned int mem_index_to_mmc_phys(unsigned int mem_page)
{
	unsigned int offset_mmc_block;

	offset_mmc_block = mem_page * (PAGE_SIZE/mmc_block_size);
//printk( "-----%s(%d) offset_nand_page=%d  NAND_START_PAGE=%d\n", __func__,__LINE__, offset_nand_page, NAND_START_PAGE );	
	return MMC_START_BLOCK + offset_mmc_block;
}




/* function : zb_init_dma_list
 * desc		: init dma count
 * return	: none
 */
void zb_init_dma_list(void)
{
	serial_printf("zb_init_dma_list Called="); 

	dw_mmc_start_block_index = 0;
	dw_mmc_dma_addr_count = 0;

}
/* function : zb_add_dma_list
 * desc		: add one dma list
 * blkinx	: block index
 * phys		: physical memory address
 * return	: dma count reach max 1
 * 			  ohterwise 0
 */
void zb_add_dma_list(unsigned int blkidx, unsigned int phys)
{
	if ( dw_mmc_dma_addr_count == 0 ) {
		int block_index;
		block_index 	= mem_index_to_mmc_phys(blkidx);
		dw_mmc_start_block_index = block_index;
	}
#if 1 // block continues check..
	if ( dw_mmc_dma_addr_count >=  DW_MMC_DMA_COUNT_MAX ) serial_printf("HANG ADDRESS!--- OVER MAX!!!!!!!!!!!!!!!\n");
#endif
	dw_mmc_dma_addr_list[dw_mmc_dma_addr_count] = phys;
	dw_mmc_dma_addr_count++;
	
}
/* function : zb_request_dma_transfer
 * desc		: request transfer all dma list
 * return	: none
 */
void zb_request_dma_transfer(void)
{
	if ( dw_mmc_dma_addr_count == 0  )  {
		serial_printf("dma count is 0 ================================> Error");
	}  else {
	  	serial_printf("dma count =%d\n"); 
	  }
	if ( zb_mmc_read_blocks(&zero_mmc,(void *)NULL, dw_mmc_start_block_index, dw_mmc_dma_addr_count * 8) != dw_mmc_dma_addr_count* 8) {
		serial_printf("DMA READ ERROR !!!! \n");
	}
	serial_printf("zb_request_dma_transfer end.\n");
	
	//// CALL EXTERNAL  BOOTLOADER FUNCTION
	//// CALL EXTERNAL  BOOTLOADER FUNCTION
	//// CALL EXTERNAL  BOOTLOADER FUNCTION


}

void get_zb_mmc_info() {
	
	zero_mmc.ioaddr_phys = (unsigned int) zb_mmc->ioaddr_phys;
	zero_mmc.data_buf_phys = (unsigned int) zb_mmc->data_buf_phys;
	zero_mmc.idmac_buf_phys = (unsigned int) zb_mmc->idmac_buf_phys;
	zero_mmc.read_bl_len = (unsigned int) zb_mmc->read_bl_len;
	zero_mmc.write_bl_len = (unsigned int) zb_mmc->write_bl_len;
	zero_mmc.high_capacity = (unsigned int) zb_mmc->high_capacity;
	zero_mmc.rca = (unsigned int) zb_mmc->rca;
	zero_mmc.card_caps = (unsigned int) zb_mmc->card_caps;
	zero_mmc.host_sg_dma_addr = (unsigned int) zb_mmc->host_sg_dma_addr;
	
	zero_mmc.ioaddr = (unsigned int) zb_mmc->ioaddr_phys;
	zero_mmc.data_buf =  (unsigned int) zb_mmc->data_buf_phys;
	zero_mmc.idmac_buf=  (unsigned int) zb_mmc->idmac_buf_phys;

}


void zb_storage_first_call(void)
{
 	zb_mmc = (struct zb_priv_mmc *)get_storage_priv_offset();
	
	get_zb_mmc_info();

	serial_printf( "> mmc_io_addr			=  %08x\n", zero_mmc.ioaddr);
	serial_printf( "> mmc_io_addr_phys			=  %08x\n",zero_mmc.ioaddr_phys);
	serial_printf( "> mmc_data_buf_phys			=  %08x\n", zero_mmc.data_buf_phys);
	serial_printf( "> mmc_idmac_phys			=  %08x\n", zero_mmc.idmac_buf_phys);
	serial_printf( "> mmc_read_bl_len			=  %08x\n", zero_mmc.read_bl_len);
	serial_printf( "> mmc_write_bl_len			=  %08x\n",zero_mmc.write_bl_len);
	serial_printf( "> mmc_erase_grp_size			=  %08x\n", zero_mmc.write_bl_len);
	serial_printf( "> mmc_high_capacity			=  %08x\n",zero_mmc.high_capacity);
	serial_printf( "> mmc_rca			=  %08x\n",zero_mmc.rca);
	serial_printf( "> mmc_card_caps			= %08x\n ",zero_mmc.card_caps);
	serial_printf( "> mmc_host_sg_dma_addr			= %08x\n ",zero_mmc.host_sg_dma_addr);
	

}

zbi_t	*zbi = (zbi_t *) ZBI_PADDR;

void zb_read_zbi_header( void )
{
	int	lp;
	u32   nand;
	unsigned char  *data;
	u32	zbi_used;
	
	nand = 0x0;
	data = (unsigned char *) zbi;
	
	// Read zbi first page
//	put_raw_str( "> storage start page = "); put_raw_hex32( ZB_STORAGE_OFFSET/2048 );	//put_raw_str( "\n");
	serial_printf( "> storage start page = %08x\n", zbi->storage_priv[0]>>11 );	//put_raw_str( "\n");
	zb_init_dma_list();
	//zeroboot_read_4k_page( nand, data );
	zb_add_dma_list(nand, (unsigned int)data);
	zb_request_dma_transfer();

	nand++;
	data += PAGE_SIZE;

	// calculate how page used
	zbi_used = (u32)(&zbi->copy_data[zbi->copy_count+1]) - (u32)zbi;
	zbi_used = (zbi_used+(PAGE_SIZE-1))/PAGE_SIZE;
	if(zbi_used>0) zbi_used--;

	serial_printf( "> zbi_used           = %08x\n",zbi_used   );	//put_raw_str( "\n");
	zb_init_dma_list();
	for( lp = 0; lp < zbi_used; lp++ )
	{
		//zeroboot_read_4k_page( nand, data );
		zb_add_dma_list(nand, (unsigned int)data);
		nand++;
		data += PAGE_SIZE;
	}	
	zb_request_dma_transfer();
	
	serial_printf( "> magic              = %08x\n",zbi->magic           );		//put_raw_str( "\n");
	serial_printf( "> version            = %08x\n",zbi->version         );		//put_raw_str( "\n");
	serial_printf( "> copy_count         =%08x\n ",zbi->copy_count      );		//put_raw_str( "\n");
	serial_printf( "> copy_checksum      = %08x\n", zbi->copy_checksum   );		//put_raw_str( "\n");
	serial_printf( "> jump               = %08x\n",zbi->jump_vaddr      );		//put_raw_str( "\n");
	serial_printf( "> bootup_address     =%08x\n ", zbi->bootup_address  );		//put_raw_str( "\n");
///	serial_printf( "> phys_cpu_ptr   	   =%08x\n ",zbi->phys_cpu_ptr  );		//put_raw_str( "\n");  
///////////zbi->phys_cpu_pt : Please CHeck
///	serial_printf( "> idmap_pgd     	   = %08x\n", zbi->cpu_idmap  );		//put_raw_str( "\n");

}



void zb_copy_data( void )
{
	zbi_copy_info_t	*copy_data;
	u32 			index;
	u32 			nand_page;
	volatile u8 	*page_buf;
	u32				last_page;
	u32				inx_count;

#ifdef MAKE_SUM
	u32				checksum_lp;
	u32 			check_sum;
	
	check_sum = 0;
#endif

    copy_data = zbi->copy_data;
    
	zb_init_dma_list();
	last_page = 0;
	inx_count = 0;
    for( index = 0; index < zbi->copy_count; index++ )
    {
   	nand_page = copy_data->page_offset;	
		page_buf  = (u8 *) copy_data->dest_paddr;

		// skip self area
		//if( (u32)page_buf != (u32)(PHYS_OFFSET+0x0008D000) ) {
		if( ((u32)page_buf < (u32)zbi->bootup_paddress) || ((u32)page_buf >= ((u32)zbi->bootup_paddress + (u32)zbi->bootup_size + (8 * 1024)))) {
//			zeroboot_read_4k_page( nand_page, (unsigned char * ) page_buf );

#define	MAX_DMA_COUNT	256
		// continue check
			if (last_page && (last_page != nand_page) ) {
//				put_raw_str( "last_page = "); put_raw_hex32( last_page );
//				put_raw_str( "nand_page = "); put_raw_hex32( nand_page );
//				put_raw_str( "\n" );
				zb_request_dma_transfer();
				zb_init_dma_list();
				inx_count = 0;
			}

			zb_add_dma_list(nand_page, (unsigned int)page_buf);

		// check max count
			if (inx_count < MAX_DMA_COUNT)
				inx_count++;

			if (inx_count >= MAX_DMA_COUNT) {
				zb_request_dma_transfer();
				zb_init_dma_list();
				inx_count = 0;
				last_page = 0;
			} else
				last_page = nand_page + 1;

#ifdef MAKE_SUM
			for( checksum_lp = 0; checksum_lp < PAGE_SIZE; checksum_lp++ )
			{
				check_sum += page_buf[checksum_lp];
			}
#endif
		} 
		copy_data++;
    }
	if (inx_count)
		zb_request_dma_transfer();
    
#ifdef MAKE_SUM
    serial_printf( "> CHECKSUM :%08x ",check_sum ); 
#endif
    
}

u32 force_mmu_off(u32 dummy);
asm("   	            					\n\
											\n\
.align  5									\n\
.text                                       \n\
.global force_mmu_off                       \n\
force_mmu_off:                              \n\
				mrs r0, cpsr				\n\
				orr r0, r0, #1<<6			\n\
				orr r0, r0, #1<<7			\n\
				msr cpsr, r0				\n\
				mrs r0, cpsr				\n\
				mrc p15, 0, r0, c1, c0, 0	\n\
				bic r0, r0, #1				\n\
				mcr p15, 0, r0, c1, c0, 0	\n\
				isb							\n\
				dsb							\n\
				mov	pc,lr                   \n\
");



void zeroboot_mmc(int mode) // 0:NORMAL 1:FULL LOAD
{
	zbi_t *zbi;
	u32 (*bootup)(int val);
	//int curr_device = 1; // Micro SD at sabrelite
	//int curr_device = 0; // Micro SD at sabrelite
	int curr_device = 2; // Micro SD at sabrelite
	//int curr_device = 3; // Micro SD at sabrelite
	int n;
	int offset; 
	int cnt;
	int page_index, block_index;

	struct mmc *mmc;

	u32 *ptr;
	u32 idmap_pgd, sp;


	
	serial_printf("find...\n");
	mmc = find_mmc_device(curr_device);
	
	if (!mmc) {
			serial_printf("no mmc device at slot %x\n", curr_device);
			return 1;
	}
	mmc_init(mmc);
	zbi = (zbi_t *)ZBI_PADDR;
	offset = ZB_STORAGE_OFFSET;
	cnt = MMC_PAGE_SIZE / 512;
	serial_printf("mmc read start at %x\n", curr_device);
	serial_printf("ZBI HEADER LOCATED : %8X!\n", zbi);
	
	n = mmc->block_dev.block_read(curr_device, offset, cnt, (u_char *)zbi);	
	
	serial_printf(" \n");
	
	if(0xFA0000B2 != zbi->magic){
		serial_printf("different magic read value : 0x%x != 0xFA0000B2\n", zbi->magic);
		return;
	}
	serial_printf("ZBI HEADER FOUND!\n");
	
//	bootup = (u32 (*)(void))(PHYS_OFFSET + 0X00090000); //0X0008d000);
	/*for (i=0; i< zbi->bootup_size; i+= MMC_PAGE_SIZE) {
		n = mmc->block_dev.block_read(curr_device, offset+zbi->bootup_address*MMC_PAGE_SIZE + i, cnt, (u_char *)bootup + i);
	}*/
	serial_printf("Bootup code start mmc block offset - %8x\n", offset+zbi->bootup_address*cnt);
	for (page_index=0; page_index*MMC_PAGE_SIZE < zbi->bootup_size; page_index++) {
		block_index = page_index * cnt;
		//n = mmc->block_dev.block_read(curr_device, offset*cnt + zbi->bootup_address*cnt + block_index, cnt, (u_char *)bootup + page_index*MMC_PAGE_SIZE);
		n = mmc->block_dev.block_read(curr_device, offset + zbi->bootup_address*cnt + block_index, cnt, (u_char *)bootup + page_index*MMC_PAGE_SIZE);		
		serial_printf("read block = 0x%x\n",offset + zbi->bootup_address*cnt + block_index);	
	}
	//serial_printf(">jump zero bootup = 0x%x\n",(u32)bootup);
	
	//bootup(mode);

	zb_storage_first_call();
	zb_read_zbi_header();
	zb_copy_data();

	void (*jumpkernel)(u32, u32) = 0;

	force_mmu_off(0);

	ptr = (u32*)zbi->phys_cpu_ptr;
	
	idmap_pgd = *ptr++;
	sp		  = *ptr++;
	jumpkernel = (void (*)(void))(zbi->cpu_resume);

/*	put_raw_str( "ptr " );
	put_raw_hex32(ptr);
	put_raw_str( "\n" );

	put_raw_str( "idmap_pgd " );
	put_raw_hex32(idmap_pgd);
	put_raw_str( "\n" );

	put_raw_str( "jumpto kernel " );
	put_raw_hex32(jumpkernel);
	put_raw_str( "\n" );
*/
	jumpkernel(ptr, idmap_pgd);

	serial_printf( "Bug\n" );
	while(1);
	
}
int do_zeroboot(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc < 2)
		return CMD_RET_USAGE;
	if (strcmp(argv[1], "0") == 0) {
		printf("> zeroboot NORMAL BOOT..\n");
		zeroboot_mmc(0);
	} else if ( strcmp(argv[1], "1") == 0 ) {
		printf("> zeroboot FULL LOAD BOOT\n");
		zeroboot_mmc(1);
		
	} else
		return CMD_RET_USAGE;
	
	return 0;
}

U_BOOT_CMD(zeroboot, 2, 1, do_zeroboot, "NORMAL BOOT : zeroboot 0\n", "FULL LOAD BOOT : zeroboot 1\n"
														"run zeroboot mmc\n");
		
