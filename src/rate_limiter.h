// Copyright (c) 2021 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/**
*@file      rate_limiter.h
*@brief     Rate limiter for general use
*@author    Ziga Miklosic
*@date      19.02.2021
*@version   V1.0.1
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

#include "project_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * 	Module version
 */
#define RATE_LIMITER_VER_MAJOR			( 1 )
#define RATE_LIMITER_VER_MINOR			( 0 )
#define RATE_LIMITER_VER_DEVELOP		( 1 )

/**
 * 	Status
 */
typedef enum
{
	eRATE_LIMITER_OK = 0, 	/**<Normal operation */
	eRATE_LIMITER_ERROR,	/**<General error */
} rate_limiter_status_t;

/**
 * 	Pointer to slew rate limiter instance
 */
typedef struct rate_limiter_s * p_rate_limiter_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t 	rate_limiter_init			(p_rate_limiter_t * p_rl_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt);
float32_t				rate_limiter_update			(p_rate_limiter_t rl_inst, const float32_t x);
bool					rate_limiter_is_init		(p_rate_limiter_t rl_inst);
rate_limiter_status_t	rate_limiter_change_rate	(p_rate_limiter_t rl_inst, const float32_t rise_rate, const float32_t fall_rate);

#endif // __RATE_LIMITER_H

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
