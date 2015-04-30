#ifndef __DEVICE_DNA_INFO_HEADER__
#define __DEVICE_DNA_INFO_HEADER__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define NAND_PAGE_SIZE                  512 // bytes
#define RESERVED_SIZE                   NAND_PAGE_SIZE // bytes

#define ONE_KBYTES_SIZE                 1024

#define FINE_CPU_SIZE                   ONE_KBYTES_SIZE // 1KBytes
#define FINE_VERSIONS_SIZE              ONE_KBYTES_SIZE // 1KBytes
#define FINE_PERM_SIZE                  ONE_KBYTES_SIZE // 1KBytes
#define FINE_TPEG_SIZE                  ONE_KBYTES_SIZE // 1KBytes
#define FINE_REALCAM_SIZE               ONE_KBYTES_SIZE // 1KBytes
#define FINE_MISC_SIZE                  ONE_KBYTES_SIZE // 1KBytes
#define FINE_MODE_SIZE                  ONE_KBYTES_SIZE // 1KBytes

#define RESERVED_SIZE                   NAND_PAGE_SIZE // bytes

#define DATE_SIZE                       14
#define SYSTEMTIME_SIZE                 16
#define UUID_DATE_SIZE                  128
#define TPEG_KEY_SIZE                   256
#define UPDATE_SIZE                     4096
#define VERSION_SIZE                    18
#define RECOVERY_SIZE                   17 // IQ9000VYYYYMMDDHH

typedef enum _FINE_DNA_INDEX
{
    CPU,
    FRP1,
    FRP2,
    PERM1,
    PERM2,
    TPEG,
    REARCAM,
    MISC,
    CALDATA,
    CALDATA_INC,
    MODE,
    IMAGE
} FINE_DNA_INDEX;

typedef enum _PROCESSOR_ARCHTECTURE
{
    PROCESSOR_ARCHTECTURE_UNKNOWN,
    PROCESSOR_ARCHTECTURE_MIPS,
    PROCESSOR_ARCHTECTURE_SHX,
    PROCESSOR_ARCHTECTURE_ARM,
    PROCESSOR_ARCHTECTURE_CORTEX,
} PROCESSOR_ARCHTECTURE;

typedef enum _PROCESSOR_TYPE
{
    PROCESSOR_TYPE_UNKNOWN,
    PROCESSOR_CORTEX_A5,
    PROCESSOR_CORTEX_A8,
    PROCESSOR_CORTEX_A9,
} PROCESSOR_TYPE;

typedef enum _PROCESSOR_REVISION
{
    PROCESS_VER_UNKNOWN,
    PROCESS_VER_ARMv4,
    PROCESS_VER_ARMv4I,
    PROCESS_VER_ARMv7,
} PROCESSOR_REVISION;

typedef enum _FINE_PLATFORM_ID
{
    PLATCFG_PLATFORM_ID_UNKNOWN,    // - unknown
    PLATCFG_PLATFORM_ID_WS,         // - WS
    PLATCFG_PLATFORM_ID_ES,         // - ES
    PLATCFG_PLATFORM_ID_PP,         // - PP
    PLATCFG_PLATFORM_ID_MP,         // - MP
} FINE_PLATFORM_ID;

typedef enum _ANTI_THIEF_ID
{
    ANTI_THIEF_UNKNOWN,
    ANTI_THIEF_UNLOCK,
    ANTI_THIEF_LOCK,
} ANTI_THIEF_ID;

//========================================
#define FINE_VERSION_SIG    "FINE"
#define FINE_OS_SIG         "OSIM"
#define FINE_BOOT_SIG       "BOOT"
#define FINE_MISC_SIG       "MISC"
#define FINE_UUID_SIG       "UUID"
#define FINE_ANTI_THIEF_SIG "ANTF"
#define FINE_UTIL_SIG       "UTIL"
#define FINE_PERMENENT_SIG  "PERM"
#define FINE_TPEG_SIG       "TPEG"
#define FINE_FLAG_SIG       "FLAG"
//========================================

//==================== CPU ====================

typedef struct _FINE_CPU_INFO
{
    unsigned char wArchitecture;
    unsigned char wProcessorType;
    unsigned char wProcessorCompiler;
    unsigned char wReserved[(NAND_PAGE_SIZE*FINE_CPU_SIZE)-(sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned char))];
} FINE_CPU_INFO;

#define FINE_CPU_ADDR          (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * CPU)

//==================== FRP1 ====================

typedef struct _SYSTEMTIME {
   unsigned short wYear;
   unsigned short wMonth;
   unsigned short wDayOfWeek;
   unsigned short wDay;
   unsigned short wHour;
   unsigned short wMinute;
   unsigned short wSecond;
   unsigned short wMilliseconds;
} SYSTEMTIME;

typedef struct _FINE_BOOT_VERSION
{
    unsigned long dwSig;
    unsigned char wBuildDate[DATE_SIZE];
    unsigned char dwReserved[(RESERVED_SIZE)-(sizeof(unsigned long)+(sizeof(unsigned char) * DATE_SIZE))];
} FINE_BOOT_VERSION;

typedef struct _FINE_OS_VERSION
{
    unsigned long dwSig;
    unsigned char wBuildDate[DATE_SIZE];
    unsigned char dwReserved[(RESERVED_SIZE)-(sizeof(unsigned long)+(sizeof(unsigned char) * DATE_SIZE))];
} FINE_OS_VERSION;

typedef struct _FINE_PCB_VERSION {
    unsigned long dwModelId;
    unsigned int dwPlatformId;
    unsigned int dwPCBVersion;
    unsigned char dwReserved[(RESERVED_SIZE)-(sizeof(unsigned long)+sizeof(unsigned int)+sizeof(unsigned int))];
} FINE_PCB_VERSION;

typedef struct _FINE_ANTI_THIEF
{
    unsigned long dwSig;
    unsigned long dwLocked;
    unsigned long dwNumLocked;
    SYSTEMTIME time;
    unsigned char dwReserved[(RESERVED_SIZE)-(sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(SYSTEMTIME))];
} FINE_ANTI_THIEF;

typedef struct _FINE_UTIL
{
    unsigned long dwSig;
    unsigned long dwBootCount;
    unsigned long AutoRecoveryScratch1;
    unsigned long AutoRecoveryScratch2;
    unsigned long AuthSDCheckScratch1;
    unsigned long AuthSDCheckScratch2;
    unsigned long Reserve1;
    unsigned long Reserve2;
    unsigned long Reserve3;
    unsigned char dwReserved[(RESERVED_SIZE)-(sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long))];
} FINE_UTIL;

typedef struct _FINE_VERSIONS
{
    unsigned long dwSig;
    FINE_OS_VERSION osVer;
    FINE_BOOT_VERSION bootVer;
    FINE_PCB_VERSION pcbVer;
    unsigned char Reserved[(NAND_PAGE_SIZE*FINE_VERSIONS_SIZE)-(sizeof(unsigned long)+sizeof(FINE_OS_VERSION)+sizeof(FINE_BOOT_VERSION)+sizeof(FINE_PCB_VERSION)+sizeof(FINE_ANTI_THIEF)+sizeof(FINE_UTIL))];
} FINE_VERSIONS;

#define FINE_VERSIONS_ADDR1          (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * FRP1)
#define FINE_ANTI_THIEF_ADDR1        FINE_VERSIONS_ADDR1 + sizeof(FINE_VERSIONS)
#define FINE_FINE_UTIL_ADDR1         FINE_ANTI_THIEF_ADDR1 + sizeof(FINE_ANTI_THIEF)

//==================== FRP2 ====================

#define FINE_VERSIONS_ADDR2          (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * FRP2)
#define FINE_ANTI_THIEF_ADDR2        FINE_VERSIONS_ADDR2 + sizeof(FINE_VERSIONS)
#define FINE_FINE_UTIL_ADDR2         FINE_ANTI_THIEF_ADDR2 + sizeof(FINE_ANTI_THIEF)

//==================== PERM1 ====================

typedef struct _FINE_UUID
{
    unsigned long dwSig;
    unsigned char arrUUID[UUID_DATE_SIZE];
    unsigned char dwReserved[(RESERVED_SIZE)-(sizeof(unsigned long)+(sizeof(unsigned char) * UUID_DATE_SIZE))];
} FINE_UUID;

typedef struct _FINE_PERMENENT_INFO
{
    unsigned long dwSig;
    FINE_UUID uuid;
    unsigned char Reserved[(NAND_PAGE_SIZE*FINE_PERM_SIZE)-(sizeof(unsigned long)+sizeof(FINE_UUID))];
} FINE_PERMENENT_INFO;

#define FINE_PERMENENT_ADDR1          (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * PERM1)

//==================== PERM1 ====================

#define FINE_PERMENENT_ADDR2          (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * PERM2)

//==================== TPEG ====================

typedef struct _FINE_TPEG
{
    unsigned long dwSig;
    unsigned char arrTPEG_KEY[TPEG_KEY_SIZE];
    unsigned char Reserved[(NAND_PAGE_SIZE*FINE_TPEG_SIZE)-(sizeof(unsigned long)+(sizeof(unsigned char) * TPEG_KEY_SIZE))];
} FINE_TPEG;

#define FINE_TPEG_ADDR                (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * TPEG)

//==================== REARCAM ====================

typedef struct _FINE_REARCAM
{
    unsigned long dwRearcamX;
    unsigned long dwRearcamY;
    unsigned long dwWheelAngle;
    unsigned long dwCamAngle;
    unsigned long dwCamHeight;
    unsigned long dwParklineDisable;
    unsigned long dwWheelParklineDisable;
    unsigned long dwFrontCameraOnOff;
    unsigned long dwCameraAutoControlOnOff;
    unsigned long dwRearcamDisable;
    unsigned char Reserved[(NAND_PAGE_SIZE*FINE_REALCAM_SIZE)-(sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long))];
} FINE_REARCAM;

#define FINE_REARCAM_ADDR                (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * REARCAM)

//==================== MISC ====================

typedef struct _FINE_MISE
{
    unsigned long dwBlackBoxOnlyMode;
    int nDRAngle;
    unsigned long dwCalibrationCheck;
    unsigned long dwTvoutEnable;
    unsigned long dwDrFault;
    // TODO : OnAir 관련 추가 필요
    unsigned char Reserved[(NAND_PAGE_SIZE*FINE_MISC_SIZE)-(sizeof(unsigned long)+sizeof(int)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long))];
} FINE_MISE;

#define FINE_MISC_ADDR                (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * MISC)

//==================== 4DCALDATA ====================

#define FINE_4DCALDATA_ADDR           (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * CALDATA)

//==================== MODE ====================

typedef struct _FINE_MODE
{
    unsigned int dwMode;
    unsigned int dwCurrent;
    unsigned int dwPercentage;
    unsigned int dwFDTHistory;
    unsigned int dwBootCount;
    unsigned int dwOSCurrent;
    unsigned char dwUpdate[UPDATE_SIZE];
    unsigned char dwVersion[VERSION_SIZE];
    unsigned char dwRecovery[RECOVERY_SIZE];
    unsigned int dwModeUPDATEUSB;
    unsigned int dwUpdateCase;
    unsigned int dwWaitProgress;
    unsigned int dwTime1;
    unsigned int dwTime2;
    unsigned int dwTime3;
    unsigned int dwTime4;
    unsigned int dwTime5;
    unsigned char Reserved[(NAND_PAGE_SIZE*FINE_MODE_SIZE)-(sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+(sizeof(unsigned char) * UPDATE_SIZE)+(sizeof(unsigned char) * VERSION_SIZE)+(sizeof(unsigned char) * RECOVERY_SIZE)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int))];
} FINE_MODE;

#define FINE_MODE_ADDR                (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * MODE)

#define IMAGE_OFFSET                  800 * 480 * 4
#define FINE_IMAGE_ADDR               (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * IMAGE)
#define FINE_FORCE_IMAGE_ADDR         FINE_IMAGE_ADDR                      // 580000 ~ 6F6FFF
#define FINE_THIEF_IMAGE_ADDR         FINE_IMAGE_ADDR + (IMAGE_OFFSET * 1) // 6F7000 ~ 86DFFF
#define FINE_ERROR_IMAGE_ADDR         FINE_IMAGE_ADDR + (IMAGE_OFFSET * 2) // 86E000 ~ 9E4FFF
#define FINE_ERROR2_IMAGE_ADDR        FINE_IMAGE_ADDR + (IMAGE_OFFSET * 3) // 9E5000 ~ B4BFFF
#define FINE_ERROR3_IMAGE_ADDR        FINE_IMAGE_ADDR + (IMAGE_OFFSET * 4) // B5C000 ~ CD2FFF
#define FINE_ERROR4_IMAGE_ADDR        FINE_IMAGE_ADDR + (IMAGE_OFFSET * 5) // CD3000 ~ E49FFF
                                                                           // E4A000 ~ FC0FFF
                                                                           // FC1000 ~ 1137FFF
                                                                           // 1138000 ~ 12AEFFF

//==================== FLAG ====================

typedef struct _FINE_FLAG
{
    unsigned long dwSig;
    unsigned char wReserved[(NAND_PAGE_SIZE*FINE_CPU_SIZE*2)-(sizeof(unsigned long))];
} FINE_FLAG;

#define FINE_FLAG_ADDR          (ONE_KBYTES_SIZE * NAND_PAGE_SIZE * 19)

//========================================
#define FINE_VERSION_SUCCESS                    0
#define FINE_VERSION_DATA_NOT_FOUND             1
#define FINE_VERSION_OS_SIGNATURE_MISMATCH      2
#define FINE_VERSION_BOOT_SIGNATURE_MISMATCH    3
#define FINE_VERSION_UUID_SIGNATURE_MISMATCH    4
#define FINE_VERSION_DATA_WRITE_FAIL            5
//========================================

#define FINE_ANTI_THIEF_LOCK_SIG    "LOCK"
#define FINE_ANTI_THIEF_UNLOCK_SIG  "UNLK"

#define DNA_DEVICE_NAME        "/dev/block/mmcblk0p3"
#define PARK_DEVICE_NAME       "/dev/block/mmcblk0p2"
//#define SPLASH_DEVICE_NAME     "/dev/block/mmcblk0p8"

struct dna_info
{
    unsigned int     SeekAddress1;
    unsigned int     SeekAddress2;
    unsigned char    *DataBuffer;
    unsigned int     DataBufferSize;
};

#define PERCENTAGE_CASE1_START 0
#define PERCENTAGE_CASE1_END   25
#define PERCENTAGE_CASE2_START 26
#define PERCENTAGE_CASE2_END   50
#define PERCENTAGE_CASE3_START 51
#define PERCENTAGE_CASE3_END   75
#define PERCENTAGE_CASE4_START 76
#define PERCENTAGE_CASE4_END   100
#define PERCENTAGE_CASE5_START 200
#define PERCENTAGE_CASE5_END   250

#define PERCENTAGE_CASE3_OFFSET 18
#define PERCENTAGE_CASE4_OFFSET 6
#define PERCENTAGE_WAIT_OFFSET 5
#define PERCENTAGE_CASE1_WAIT_OFFSET 20
#define PERCENTAGE_CASE2_WAIT_OFFSET 20
#define PERCENTAGE_CASE3_WAIT_OFFSET 10
#define PERCENTAGE_CASE4_WAIT_OFFSET 10

#define DEFAULT_WAIT_TIME1 1200
#define DEFAULT_WAIT_TIME2 900
#define DEFAULT_WAIT_TIME3 1800
#define DEFAULT_WAIT_TIME4 2000
#define DEFAULT_WAIT_TIME5 500

#define ERROR_CASE_101 101
#define ERROR_CASE_102 102
#define ERROR_CASE_103 103
#define ERROR_CASE_104 104
#define ERROR_CASE_105 105
#define ERROR_CASE_106 106
#define ERROR_CASE_107 107
#define ERROR_CASE_108 108
#define ERROR_CASE_109 109
#define ERROR_CASE_110 110
#define ERROR_CASE_111 111
#define ERROR_CASE_112 112

#define ERROR_CASE_301 301
#define ERROR_CASE_302 302
#define ERROR_CASE_303 303
#define ERROR_CASE_304 304
#define ERROR_CASE_305 305
#define ERROR_CASE_306 306
#define ERROR_CASE_307 307
#define ERROR_CASE_308 308
#define ERROR_CASE_309 309
#define ERROR_CASE_310 310
#define ERROR_CASE_311 311
#define ERROR_CASE_312 312
#define ERROR_CASE_313 313
#define ERROR_CASE_314 314
#define ERROR_CASE_315 315
#define ERROR_CASE_316 316
#define ERROR_CASE_317 317
#define ERROR_CASE_318 318

#define ERROR_CASE_401 401
#define ERROR_CASE_402 402
#define ERROR_CASE_403 403
#define ERROR_CASE_404 404
#define ERROR_CASE_405 405
#define ERROR_CASE_406 406
#define ERROR_CASE_407 407
#define ERROR_CASE_408 408
#define ERROR_CASE_409 409
#define ERROR_CASE_410 410
#define ERROR_CASE_411 411
#define ERROR_CASE_412 412
#define ERROR_CASE_413 413
#define ERROR_CASE_414 414
#define ERROR_CASE_415 415
#define ERROR_CASE_416 416
#define ERROR_CASE_417 417
#define ERROR_CASE_418 418

#define ERROR_CASE_501 501
#define ERROR_CASE_502 502
#define ERROR_CASE_503 503
#define ERROR_CASE_504 504
#define ERROR_CASE_505 505
#define ERROR_CASE_506 506
#define ERROR_CASE_507 507
#define ERROR_CASE_508 508
#define ERROR_CASE_509 509
#define ERROR_CASE_510 510
#define ERROR_CASE_511 511
#define ERROR_CASE_512 512
#define ERROR_CASE_513 513
#define ERROR_CASE_514 514
#define ERROR_CASE_515 515
#define ERROR_CASE_516 516
#define ERROR_CASE_517 517
#define ERROR_CASE_518 518

#define CURRENT_NORMAL 0
#define CURRENT_FORCE 1
#define CURRENT_AUTO 2
#define CURRENT_UPDATE 3
#define CURRENT_MASS 4
#define PERCENTAGE_OS 1
#define PERCENTAGE_APP 2
#define PERCENTAGE_NORMAL 3
#define PERCENTAGE_QB 4
#define PERCENTAGE_ONLYAPP 5

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#define dgbprintf fprintf
#define dgbprintk printk
#else /* no debug */
#define dgbprintf(x...) do {} while(0)
#define dgbprintk(x...) do {} while(0)
#endif

#ifdef __cplusplus
    }
#endif //__cplusplus

#endif //__DEVICE_DNA_INFO_HEADER__

