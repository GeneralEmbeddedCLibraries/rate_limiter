// Copyright (c) 2025 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/**
*@file      rate_limiter.h
*@brief     Rate limiter for general use
*@author    Ziga Miklosic
*@mail      ziga.miklosic@gmail.com
*@date      16.08.2025
*@version   V2.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup RATE_LIMITER_API
* @{ <!-- BEGIN GROUP -->
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __RATE_LIMITER_H
#define __RATE_LIMITER_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common/utils/src/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * 	Module version
 */
#define RATE_LIMITER_VER_MAJOR			( 2 )
#define RATE_LIMITER_VER_MINOR			( 0 )
#define RATE_LIMITER_VER_DEVELOP		( 0 )

/**
 * 	Status
 */
typedef enum
{
	eRATE_LIMITER_OK = 0, 	/**<Normal operation */
	eRATE_LIMITER_ERROR,	/**<General error */
} rate_limiter_status_t;

/**
 *  Slew rate limiter
 */
typedef struct rate_limiter_s
{
    float32_t   x_prev;     /**<Previous value of input */
    float32_t   k_rise;     /**<Rising slew rate factor*/
    float32_t   k_fall;     /**<Falling slew rate factor*/
    float32_t   dt;         /**<Period of update */
    bool        is_init;    /**<Rate limiter initialization success flag */
} rate_limiter_t;

/**
 *  Pointer to slew rate limiter instance
 */
typedef struct rate_limiter_s * p_rate_limiter_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t rate_limiter_init		    (p_rate_limiter_t * p_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt);
rate_limiter_status_t rate_limiter_init_static  (p_rate_limiter_t inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt);
bool                  rate_limiter_is_init      (const p_rate_limiter_t inst);
float32_t             rate_limiter_hndl		    (const p_rate_limiter_t inst, const float32_t x);
rate_limiter_status_t rate_limiter_change_rate  (const p_rate_limiter_t inst, const float32_t rise_rate, const float32_t fall_rate);
rate_limiter_status_t rate_limiter_reset        (const p_rate_limiter_t inst);

#endif // __RATE_LIMITER_H

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
