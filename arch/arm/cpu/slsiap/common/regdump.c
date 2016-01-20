#include <common.h>

#if (0)
#define	NX_DEBUG_MSG(args...) 	printf(args)
#else
#define	NX_DEBUG_MSG(args...) 	do{}while(0)
#endif

unsigned int GetADDR(const char *a) 
{
	char ch = *a; 
	unsigned int cnt = 0, hex = 0;

	while(((ch != '\r') && (ch != '\n')) || (cnt == 0)) {
		ch  = *a; 

		if (! ch) 
			break;

		if (cnt != 8) {
			if (ch >= '0' && ch <= '9') {
				hex = (hex<<4) + (ch - '0');
				cnt++;
			} else if (ch >= 'a' && ch <= 'f') {
				hex = (hex<<4) + (ch-'a' + 0x0a);
				cnt++;
			} else if (ch >= 'A' && ch <= 'F') {
				hex = (hex<<4) + (ch-'A' + 0x0A);
				cnt++;
			}
		}
		a++;
	}   
	return hex;
}

void GetREG(uint32_t addr, uint32_t *buf)
{
	ulong bytes = 4;
	const void *read_buf = map_sysmem(addr, bytes);

	NX_DEBUG_MSG("read data %x\n", *(volatile uint32_t *)read_buf);

	*buf = *(volatile uint32_t *)read_buf;
	NX_DEBUG_MSG("copy data %x\n", *buf);
}

