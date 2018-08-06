#ifndef MEMORY_H
#define MEMORY_H

/**************************************************************************
*  DEBUG CONTROL
**************************************************************************/
#define MEM_TEST                (1)

// do not change the test size !!!!
#define MEM_TEST_SIZE           (0x2000)

/**************************************************************************
*  DRAM SIZE 
**************************************************************************/
//#define E1_DRAM_SIZE            (0x10000000)
//#define E2_DRAM_SIZE            (0x08000000)

/**************************************************************************
*  EXPOSED API
**************************************************************************/
//extern u32 mt6516_get_hardware_ver (void);
//extern void mt6516_mem_init (void);

#if MEM_TEST
extern int complex_mem_test (unsigned int start, unsigned int len);
#endif

#define UINT32P unsigned int *
#define EMI_base 0x10004000
#define EMI_IDLA                 ((volatile UINT32P)(EMI_base+0x0200))
#define EMI_IDLB                 ((volatile UINT32P)(EMI_base+0x0208))

#endif
