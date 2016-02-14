/**
* @file          cpu_define.h
* @brief         CPU configuration
* @author        lijiale
* @version       1.00
* @date          2010/08/20
*                  Copyright 2010 FUJITSU ELECTRONICS INC.  LIMITED
**/
#ifndef __CPU_CONFIG_INC__
#define __CPU_CONFIG_INC__

/*----------------------------------------------------------------------------*/
/* Headers                                                                    */
/*----------------------------------------------------------------------------*/
/*  includes cpu individual header file                                       */

/* 16LX family -------------------------------------------------------------- */

/* MB90330 series */
#if   defined(__CPU_MB90V330__)  || defined(__CPU_MB90F334__) || \
      defined(__CPU_MB90333__)   || \
      defined(__CPU_MB90V330A__) || defined(__CPU_MB90F334A__) || \
      defined(__CPU_MB90333A__)  || defined(__CPU_MB90334A__)
  #include "MB90330/cpu_map_MB90330.h"

/* FR80 family -------------------------------------------------------------- */
/* MB91660 series */
#elif defined(__CPU_MB91F662__) || defined(__CPU_MB91661__)  || defined(__CPU_MB91664__)
  #include "MB91660/cpu_map_MB91660.h"

/* MB91665 series */
#elif defined(__CPU_MB91F668__) || defined(__CPU_MB91669__)
  #include "MB91665/cpu_map_MB91665.h"

/* MB9B500 series */
#elif defined(__CPU_MB9B500__) || defined(__CPU_MB9B618__)
  #include "cpu_map_MB9B500.h"

/* unknown or not supported MPUs */
#else
  #error "Configuration file of the specified CPU type could not be found."

#endif

#endif   /* __CPU_CONFIG_INC__ */
