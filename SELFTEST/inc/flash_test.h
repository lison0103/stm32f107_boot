/**
 * Company	: ThyssenKrupp PDC
 * @file	: flash_test.h
 * @purpose     :
 * @brief	: header for c based test for internal FLASH 
 *                  POST and BIST internal FLASH test for IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 23-April-2014
 * @author	: Paul
*/

#ifndef __IEC61508_FLASH_TEST_H__
#define __IEC61508_FLASH_TEST_H__

/*! @addtogroup IEC61508_FLASH_TEST
 * @{
 * @image html Flash-Test.jpg
 */

#include "ram_test.h"
   
#ifdef __cplusplus
extern "C" {
#endif
/*! @addtogroup IEC61508_CONFIG
 * @{
 */

#define dynamic_CRC_table    0
/*!
 * @}
 */

#define SIZE32K              0x00007FFF
#define SIZE64K              0x0000FFFF
#define SIZE128K             0x0001FFFF

#define FLASHSIZE            SIZE128K

#define FLASH_SIGN1          1
#define FLASH_SIGN2          2

#define CRC_block_size       1024

#define FLASH_CRC_Done       1          /*!< status information of this struct */
#define FLASH_CRC_InProg     2
#define FLASH_CRC_Missing    3
#define FLASH_CRC_Valid      4
#define FLASH_CRC_Restart    5

/*! @addtogroup IEC61508_CONFIG
 * @{
 */
/*!
 * @brief     struct defining the FLASH content with CRC, size etc.
 * @attention this struct must be filled correctly with values before using in the Flash tests.
 * CRC32Val: this can be calculated by firmware (function IEC61508_FLASHtest_POST) or external tools
 * Status  : set to valid to run the test
 */
/*  predefined               CRC32Val  , MemLenght, NextAddress, BlockSize,  Status; */
#define ENTRY_FLASH_CRC    { 0x0A59B834, FLASHSIZE, (uint32_t) __STEXT, FLASHSIZE, FLASH_CRC_Valid }
/*!
 * @}
 */

/*!
 * @brief   private function calculating CRC32 table based checksum by POST and BIST tests
 * @param   crc         start CRC32 value if partial usage
 * @param   src         pointer to start of the memory to check
 * @param   len         length of processed memory in byte
 * @return  uint32_t    CRC32 calculated checksum
 * @attention  the routine cahnges the content of the src pointer to the next address in the 
 *             runtime. As a result the next valid address to check can be read out of  the scr
 *             pointers content
 */
uint32_t crc32(uint32_t crc,uint8_t ** const pp_src, uint32_t len);

/*!
 * @brief   public function to check the Flash content POST only
 *          by recalculation and comparison with Flash stored CRC32 value
 * @return passed or failed. See \link #testResult_enum testResult_t \endlink.
 * @attention FLASHSIZE must be configured in the file ram_test.h and
 *            linker label _stext must be available
 */
type_testResult_t IEC61508_FLASHtest_POST(void);

type_testResult_t IEC61508_FLASHtest_POST2(void);

/*!
 * @brief   public function (competitive) to check a part of the Flash content
 * @param   StartMode   flag to restart the BIST test
 *                      available values are : FLASH_CRC_Restart restarts the calculation
 *                      all other will continue to test next addresses
 * @return  passed or failed. See \link #testResult_enum testResult_t \endlink.
 * @note    the memory section which is processed is defined in the struct bistFlashCRC
 */
type_testResult_t IEC61508_FLASHtest_BIST(uint8_t StartMode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*!
 * @}
 */

#endif /* __IEC61508_FLASH_TEST_H__ */

/************************************** EOF *********************************/
