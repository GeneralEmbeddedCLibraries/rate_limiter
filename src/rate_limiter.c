// Copyright (c) 2025 Ziga Miklosic
// All Rights Reserved
// This software is under MIT licence (https://opensource.org/licenses/MIT)
////////////////////////////////////////////////////////////////////////////////
/**
*@file      rate_limiter.c
*@brief     Rate limiter for general use
*@author    Ziga Miklosic
*@mail      ziga.miklosic@gmail.com
*@date      16.08.2025
*@version   V2.0.0
*
*@section Description
*
*	This code contains slew rate limiter implementation. Desing to be
*	used as general purpose module, where individual instances of rate
*	limiter are independent from each other.
*
*	Each instance is defined by rising & falling slew rate, as well
*	as period time of update.
*
*@section Code_example
*@code
*
*	// Declare rate limiter instance pointer
*	static p_rate_limiter_t my_rate_limter = NULL;
*
*	// Initialize
*	if ( eRATE_LIMITER != rate_limiter_init( &my_rate_limiter, rise_time, fall_time, period_time ))
*	{
*		// Init failed...
*		// Furhter actions here...
*	}
*
*
*	// Update (slew limit wanted signal)
*	@period_time
*	{
*		slew_rated_signal = rate_limiter_update( my_rate_limiter, raw_signal );
*	}
*
*@endcode
*
*/
////////////////////////////////////////////////////////////////////////////////
/*!
* @addtogroup RATE_LIMITER
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "rate_limiter.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup RATE_LIMITER_API
* @{ <!-- BEGIN GROUP -->
*
* 	Following function are part or rate limiter API.
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Initialize rate limiter
*
* @note Rising/Falling slew rate is references to change on seconds.
*
* 		E.g.:
* 			- for 1V/s -> put rise/fall rate = 1.0
* 			- for 0.5V/s -> put rise/fall rate = 0.5
*
* @param[out]  	p_inst      - Pointer to rate limiter instance
* @param[in]  	rise_rate   - Rising slew rate
* @param[in]  	fall_rate   - Falling slew rate
* @param[in]  	dt          - Update (period) time in seconds
* @return       status      - Either OK or Error
*/
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t rate_limiter_init(p_rate_limiter_t * p_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt)
{
	rate_limiter_status_t status = eRATE_LIMITER_OK;

	if 	(	( NULL != p_inst )
		&& 	( dt > 0.0f ))
	{
		// Allocate space
		*p_inst = malloc( sizeof( rate_limiter_t ));

		if ( NULL != *p_inst )
		{
			// Init previous value & period
			(*p_inst)->x_prev = 0.0f;
			(*p_inst)->dt = dt;

			// Calculate rise/fall factors
			(*p_inst)->k_rise = rise_rate * dt;
			(*p_inst)->k_fall = fall_rate * dt;

			// Init success
			(*p_inst)->is_init = true;
		}
	}
	else
	{
		status = eRATE_LIMITER_ERROR;
	}

	return status;
}

rate_limiter_status_t rate_limiter_init_static(p_rate_limiter_t inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt)
{
    rate_limiter_status_t status = eRATE_LIMITER_OK;

    if ( dt > 0.0f )
    {
        // Init previous value & period
        inst->x_prev = 0.0f;
        inst->dt = dt;

        // Calculate rise/fall factors
        inst->k_rise = rise_rate * dt;
        inst->k_fall = fall_rate * dt;

        // Init success
        inst->is_init = true;
    }
    else
    {
        status = eRATE_LIMITER_ERROR;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Check if rate limiter instance is initialized
*
* @param[out]   inst    - Pointer to rate limiter instance
* @return       is_init - Success initialization flag
*/
////////////////////////////////////////////////////////////////////////////////
bool rate_limiter_is_init(const p_rate_limiter_t inst)
{
    bool is_init = false;

    if ( NULL != inst )
    {
        is_init = inst->is_init;
    }

    return is_init;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Update the rate limiter with a new input value
**
* @param[out]  	inst - Pointer to rate limiter instance
* @param[in]  	x    - Input signal
* @return       y    - Output (slew limited) signal
*/
////////////////////////////////////////////////////////////////////////////////
float32_t rate_limiter_hndl(const p_rate_limiter_t inst, const float32_t x)
{
	float32_t y = 0.0f;
	float32_t dx = 0.0f;

	// Check for instance and initialization
	if  (   ( NULL != inst )
	    &&  ( true == inst->is_init  ))
	{
        // Calculate change
        dx = x - inst->x_prev;

        // Rising limit
        if ( dx >= inst->k_rise )
        {
            y = inst->x_prev + inst->k_rise;
        }

        // Falling limit
        else if ( dx <= -( inst->k_fall ))
        {
            y = inst->x_prev - inst->k_fall;
        }

        // No limitations...
        else
        {
            y = x;
        }

        // Store current value
        inst->x_prev = y;
	}

	return y;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Change the rise and fall rates of a rate limiter instance
*
* @param[out]  	inst        - Pointer to rate limiter instance
* @param[in]  	rise_rate   - Rising slew rate
* @param[in]  	fall_rate   - Falling slew rate
* @return       status      - Either OK or Error
*/
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t rate_limiter_change_rate(const p_rate_limiter_t inst, const float32_t rise_rate, const float32_t fall_rate)
{
	rate_limiter_status_t status = eRATE_LIMITER_ERROR;

	// Check for instance and initialization
	if  (   ( NULL != inst )
	    &&  ( true == inst->is_init ))
    {
        inst->k_rise = rise_rate * inst->dt;
        inst->k_fall = fall_rate * inst->dt;

        status = eRATE_LIMITER_OK;
    }

	return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
