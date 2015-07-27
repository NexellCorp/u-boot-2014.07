#include <common.h>
#include <command.h>
#include <nand.h>
#include <mmc.h>
#include "zbi.h"


#define ZB_STORAGE_OFFSET 0x280000 //0x0 //0x3100000 //0x5b00000 //0x10000000
#define NAND_PAGE_SIZE 4096
#define MMC_PAGE_SIZE 4096
#define ARG_NAL_PHYS_OFFSET 0x40000000
#define ARG_NAL_PAGE_OFFSET 0x80000000


void zeroboot_nand(void)
{
	
#if 0
	zbi_t *zbi;
	u32 (*bootup)(void);

	nand_info_t *nand;
	ulong offset;
	size_t size;
	size_t actual_size;
	loff_t lim;
	int i;
	u32 *test_address = PHYS_OFFSET + 0X00090000;

	nand = &nand_info[nand_curr_device];
	
	zbi = (zbi_t *)ZBI_PADDR;
	offset = ZB_STORAGE_OFFSET;
	size = NAND_PAGE_SIZE;
	lim = NAND_PAGE_SIZE;
	actual_size = NAND_PAGE_SIZE;\

	
	//nand_read_skip_bad(nand,offset,&size,(u_char *)zbi);
	nand_read_skip_bad(nand,offset,&size,&actual_size, lim, (u_char *)zbi);
	//int nand_read_skip_bad(nand_info_t *nand, loff_t offset, size_t *length,
	//	size_t *actual, loff_t lim, u_char *buffer)
	
	serial_printf(" \n");
	
	if(0xFA0000B2 != zbi->magic){
		serial_printf("different magic read value : 0x%x != 0xFA0000B2\n", zbi->magic);
		return;
	}
	//serial_printf("different magic read value : 0x%x != 0xFA0000B2\n", zbi->magic);
	
	bootup = (u32 (*)(void))(PHYS_OFFSET + 0X00090000); //0X0008d000);
	size = NAND_PAGE_SIZE;
	for (i=0; i<zbi->bootup_size; i+=NAND_PAGE_SIZE)
	{	nand_read_skip_bad(nand,offset+zbi->bootup_address*NAND_PAGE_SIZE + i,&size,&actual_size,lim,(u_char *)bootup + i);
		//nand_read_skip_bad(nand,offset+zbi->bootup_address*NAND_PAGE_SIZE + i,&size,(u_char *)bootup + i);
		//serial_printf("nand offset : 0x%x \n", offset+zbi->bootup_address*NAND_PAGE_SIZE + i);	
		}
	serial_printf(">jump zero bootup = 0x%x\n",(u32)bootup);
	
	
	//bootup();
	//serial_printf(" NAL RETURN\n");
	//while(1);
#endif
}


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
	int val;
	struct mmc *mmc;
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
	
	bootup = (u32 (*)(void))(PHYS_OFFSET + 0X00090000); //0X0008d000);
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
	serial_printf(">jump zero bootup = 0x%x\n",(u32)bootup);
	
	bootup(mode);
	serial_printf(" NAL RETURN\n");
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
		
