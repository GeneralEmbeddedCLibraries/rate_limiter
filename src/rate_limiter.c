////////////////////////////////////////////////////////////////////////////////
/**
*@file      rate_limiter.c
*@brief     Rate limiter for general use
*@author    Ziga Miklosic
*@date      19.02.2021
*@version   V1.0.0
*
*@section Description
*

*
*@section Code_example
*@code
*

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

/**
 * 	Slew rate limiter
 */
typedef struct rate_limiter_s
{
	float32_t 	x_prev;		/**<Previous value of input */
	float32_t 	k_rise;		/**<Rising slew rate factor*/
	float32_t 	k_fall;		/**<Falling slew rate factor*/
	float32_t 	dt;			/**<Period of update */
	bool		is_init;	/**<Rate limiter initialization success flag */
} rate_limiter_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static float32_t rate_limiter_calc_rate_factor(p_rate_limiter rl_inst, const float32_t slew_rate);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////


// This function will calculate slew rate factor based on dt
static float32_t rate_limiter_calc_rate_factor(p_rate_limiter rl_inst, const float32_t slew_rate)
{
	float32_t k_rate = NAN;

	if ( rl_inst-> dt != 0.0f )
	{
		k_rate = ( slew_rate / rl_inst->dt );
	}

	return k_rate;
}


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
* @brief    Initialize ring buffer instance
*
* @param[out]  	p_ring_buffer	- Pointer to ring buffer instance
* @param[in]  	size			- Size of ring buffer
* @return       status			- Either OK or Error
*/
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t rate_limiter_init(p_rate_limiter * p_rl_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt)
{
	rate_limiter_status_t status = eRATE_LIMITER_OK;

	if 	(	( NULL != *p_rl_inst )
		&& 	( dt > 0.0f ))
	{
		// Allocate space
		*p_rl_inst = malloc( sizeof( rate_limiter_t ));

		if ( NULL != *p_rl_inst )
		{
			// Calculate rise/fall factors
			(*p_rl_inst)->k_rise = rate_limiter_calc_rate_factor( *p_rl_inst, rise_rate );
			(*p_rl_inst)->k_fall = rate_limiter_calc_rate_factor( *p_rl_inst, fall_rate );

			// Init previous value & period
			(*p_rl_inst)->x_prev = 0.0f;
			(*p_rl_inst)->dt = dt;

			// Init success
			(*p_rl_inst)->is_init = true;
		}
	}
	else
	{
		status = eRATE_LIMITER_ERROR;
	}

	return status;
}


float32_t rate_limiter_update(p_rate_limiter rl_inst, const float32_t x)
{
	float32_t y = 0.0f;
	float32_t dx = 0.0f;

	// Check for instance and initialization
	if ( NULL != rl_inst )
	{
		if ( true == rl_inst->is_init )
		{
			// Calculate change
			dx = x - rl_inst->x_prev;

			// Rising limit
			if ( dx >= rl_inst->k_rise )
			{
				y = rl_inst->x_prev + rl_inst->k_rise;
			}

			// Falling limit
			else if ( dx <= -( rl_inst->k_fall ))
			{
				y = rl_inst->x_prev - rl_inst->k_fall;
			}

			else
			{
				// No limitations...
			}

			// Store current value
			rl_inst->x_prev = y;
		}
	}

	return y;
}


bool rate_limiter_is_init(p_rate_limiter rl_inst)
{
	bool is_init = false;

	if ( NULL != rl_inst )
	{
		is_init = rl_inst->is_init;
	}

	return is_init;
}


rate_limiter_status_t rate_limiter_change_rate(p_rate_limiter rl_inst, const float32_t rise_rate, const float32_t fall_rate)
{
	rate_limiter_status_t status = eRATE_LIMITER_ERROR;

	// Check for instance and initialization
	if ( NULL != rl_inst )
	{
		if ( true == rl_inst->is_init )
		{
			rl_inst->k_rise = rate_limiter_calc_rate_factor( rl_inst, rise_rate );
			rl_inst->k_fall = rate_limiter_calc_rate_factor( rl_inst, fall_rate );

			status = eRATE_LIMITER_OK;
		}
	}

	return status;
}




////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
