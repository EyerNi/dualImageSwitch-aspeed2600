/*follow pure ast2600 soc chip fmc register feature design
************************************************************
odic moduel programing:
  1.open_fmc_stdin
    for opening register mapping
  2.share_fmc_stdin
    for writing and reading register
  3.close_fmc_stdin
    for closing register mapping
************************************************************
function impl:
  1.odic_get_boot_indicator
    get now active img index
      00 -> master spi
      01 -> backup spi
  2.odic_set_boot_indicator
    set next boot image index and reset bmc
      if setted next boot img index == now active img index
        do nothing
      else
        switch to another image __NOW__
************************************************************

rm NDA info

************************************************************
EyerNi 23/11/16*/

#include <dlfcn.h>
#include <string.h>
#include <sys/mman.h>

/*rm NDA info*/

#include "oemDualImgCmd.h"

/* ami spx framework ipmi cmd config */
/*rm NDA info*/

/*rm NDA info*/

/* memery maping head address */
u8 *fmc_stdin = NULL;

/* memery device */
signed memdev = -1;

/*pre define*/
ODIC static odic_res_e open_fmc_stdin(void);
ODIC static odic_res_e close_fmc_stdin(void);
ODIC static odic_res_e reverse_img(void);
ODIC static odic_res_e clear_boot_indicator_to_0(void);
ODIC static odic_res_e get_boot_indicator(u8 *next_boot_spi_index);
ODIC ipmi_odic_res odic_get_boot_indicator(_NEAR_ u8 *pReq, u32 ReqLen, _NEAR_ u8 *pRes, _NEAR_ int __attribute__((unused)) BMCInst);
ODIC ipmi_odic_res odic_set_boot_indicator(_NEAR_ u8 *pReq, u32 ReqLen, _NEAR_ u8 *pRes, _NEAR_ int __attribute__((unused)) BMCInst);

/*odic moduel std input open*/
ODIC static odic_res_e
open_fmc_stdin(void)
{
  memdev = open("/dev/mem", O_RDWR | O_NDELAY);
  if (memdev < 0)
    return ODIC_FILE_ACCESS_FILE;

  fmc_stdin = (unsigned char *)mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memdev, AST_FMC_BASE);
  if (fmc_stdin == NULL)
  {
    close(memdev);
    return ODIC_MEM_MAP_FAIL;
  }
  return ODIC_SUCC;
}

/*odic moduel std input close*/
ODIC static odic_res_e
close_fmc_stdin(void)
{
  close(memdev);
  memdev = -1;
  munmap(fmc_stdin, MAP_SIZE);
  fmc_stdin = NULL;
  return ODIC_SUCC;
}

/* get next boot spi index */
ODIC static odic_res_e
get_boot_indicator(u8 *next_boot_spi_index)
{
  if (ODIC_SUCC != open_fmc_stdin())
    return ODIC_MEM_MAP_FAIL;

  *next_boot_spi_index = (share_fmc_stdin(FMC_WDT2_CRAB_IO_ADDR_OFFSET) & FMC_WDT2_BIT4_BOOT_INDICATOR) >> 4;
  (void)close_fmc_stdin();
  return ODIC_SUCC;
}

/* clear boot indicator to 0 */
ODIC static odic_res_e
clear_boot_indicator_to_0(void)
{
  if (ODIC_SUCC != open_fmc_stdin())
    return ODIC_MEM_MAP_FAIL;

  share_fmc_stdin(FMC_WDT2_CRAB_IO_ADDR_OFFSET) |= FMC_WDT2_CMD_CLEAR_BOOT_INDICATOR;
  (void)close_fmc_stdin();
  return ODIC_SUCC;
}

/* goto spix = (nowspi == spi0 ? spi1 : spi0) */
ODIC static odic_res_e
reverse_img(void)
{
  if (ODIC_SUCC != open_fmc_stdin())
    return ODIC_MEM_MAP_FAIL;

  share_fmc_stdin(FMC_WDT2_CRAB_IO_ADDR_OFFSET) |= FMC_WDT2_BIT0_ENABLE;
  share_fmc_stdin(FMC_WDT2_TRVR_IO_ADDR_OFFSET) = FMC_WDT2_COUNTDOWN;
  share_fmc_stdin(FMC_WDT2_TRR_IO_ADDR_OFFSET) = FMC_WDT2_TRR_KEY;
  (void)close_fmc_stdin();
  return ODIC_SUCC;
}

/*odic moduel ipmi interface get active image index*/
ODIC ipmi_odic_res
odic_get_boot_indicator(_NEAR_ u8 *pReq, u32 ReqLen, _NEAR_ u8 *pRes, _NEAR_ int __attribute__((unused)) BMCInst)
{
  if (0)
  {
    pReq = pReq;
    ReqLen = ReqLen;
  }

  u8 next_boot_spi_index = 0xff;

  odic_res_e cmd_status = get_boot_indicator(&next_boot_spi_index);

  if (cmd_status == ODIC_FILE_ACCESS_FILE || cmd_status == ODIC_MEM_MAP_FAIL)
  {
    *pRes = -1 /*rm NDA info*/;
    return sizeof(u8);
  }
  else if (cmd_status == ODIC_SUCC)
  {
    *pRes = CC_NORMAL;
    *(pRes + sizeof(u8)) = next_boot_spi_index;
    return sizeof(u8) * 2;
  }

  *pRes = /* ? */ -1 /*rm NDA info*/;
  return sizeof(u8);
}

/*odic moduel ipmi interface set active image index*/
ODIC ipmi_odic_res
odic_set_boot_indicator(_NEAR_ u8 *pReq, u32 ReqLen, _NEAR_ u8 *pRes, _NEAR_ int __attribute__((unused)) BMCInst)
{
  if (0)
  {
    ReqLen = ReqLen;
  }

  /* feature reqs more than just 1 spi */
  /*rm NDA info*/

  /* input need <= max count of spi , for now it is (0) or (1) <= (2-1) */
  /*rm NDA info*/

  u8 next_boot_spi_index = 0xff;
  odic_res_e cmd_status = get_boot_indicator(&next_boot_spi_index);
  if (cmd_status == ODIC_FILE_ACCESS_FILE || cmd_status == ODIC_MEM_MAP_FAIL)
  {
    *pRes = /*rm NDA info*/ -1;
    return sizeof(u8);
  }

  if (*pReq == next_boot_spi_index)
  {
    *pRes = /*rm NDA info*/ 0;
    return sizeof(u8);
  }

  cmd_status = reverse_img();

  if (cmd_status == ODIC_FILE_ACCESS_FILE || cmd_status == ODIC_MEM_MAP_FAIL)
    *pRes = /*rm NDA info*/ -1;
  else if (cmd_status == ODIC_SUCC)
    *pRes = /*rm NDA info*/ 0;

  if (*pRes == /*rm NDA info*/ 0)
    BMC_TAUDIT(LOG_INFO, "Reserved", "Flash%d manually switch to flash%d", next_boot_spi_index, next_boot_spi_index == 0 ? 1 : 0);

  return sizeof(u8);
}

/*useless*/
ODIC odic_res_e
ODIC_UNUSE(void)
{
  if (0)
  {
    clear_boot_indicator_to_0();
  }
  return ODIC_SUCC;
}
