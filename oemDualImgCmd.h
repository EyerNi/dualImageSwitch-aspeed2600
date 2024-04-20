/*odic module head file
EyerNi 23/11/16*/

#ifdef __GNUC__

#ifndef __OEMDUALIMGCMD_H__
#define __OEMDUALIMGCMD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define _NEAR_
#define _FAR_

// #define odic_debug_enable
#ifdef odic_debug_enable
#define odic_alert_deg_log(...) printf(__VA_ARGS__)
#else
#define odic_alert_deg_log(...)
#endif

#define ODIC_GET_NEXT_BOOT_IMG odic_get_boot_indicator
#define CMD_ODIC_GET_NEXT_BOOT_IMG 0x05

#define ODIC_SET_NEXT_BOOT_IMG odic_set_boot_indicator
#define CMD_ODIC_SET_NEXT_BOOT_IMG 0x06

#define MEM_DEV "/dev/mem"
#define MAP_SIZE 0x1FF
#if defined(SOC_AST2600)
#define AST_FMC_BASE (0x1e620000)
#define FMC_WDT2_CRAB_IO_ADDR_OFFSET (0x64)
#define FMC_WDT2_TRVR_IO_ADDR_OFFSET (0x68)
#define FMC_WDT2_TRR_IO_ADDR_OFFSET (0x6c)
#define FMC_WDT2_TRR_KEY (0x4755)
#define FMC_WDT2_COUNTDOWN (0x32)
#define FMC_WDT2_BIT0_ENABLE (1 << 0)
#define FMC_WDT2_BIT4_BOOT_INDICATOR (0x10)
#define FMC_WDT2_CMD_CLEAR_BOOT_INDICATOR (0xEA0000)
#else
#pragma erro("platform none 2600")
#endif

#define IPMICMD_DEF(netfun, cmd, specInfo, implementer...) ipmi_odic_res

#define OPEN_FMC                     \
  if (ODIC_SUCC != open_fmc_stdin()) \
    return ODIC_MEM_MAP_FAIL;

#define CLOSE_FMC          \
  (void)close_fmc_stdin(); \
  return ODIC_SUCC;

#define read_and_write_fmc_stdin(offset) (*(volatile u32 *)(fmc_stdin + offset))

/*oem dual image cmd*/
#define ODIC static odic_res_e

  typedef int ipmi_odic_res;
  typedef unsigned char u8;
  typedef unsigned int u32;

  typedef enum
  {
    ODIC_FILE_ACCESS_FILE = -1,
    ODIC_MEM_MAP_FAIL = -2,
    ODIC_NOT_DUAL_MOD = -3,
    ODIC_SUCC = 0
  } odic_res_e;

  ipmi_odic_res odic_get_boot_indicator(_NEAR_ u8 *pReq, u32 ReqLen, _NEAR_ u8 *pRes, _NEAR_ int __attribute__((unused)) BMCInst);
  ipmi_odic_res odic_set_boot_indicator(_NEAR_ u8 *pReq, u32 ReqLen, _NEAR_ u8 *pRes, _NEAR_ int __attribute__((unused)) BMCInst);
#ifdef __cplusplus
}
#endif

#endif
#endif