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
static float32_t rate_limiter_calc_rate_factor(const float32_t dt, const float32_t slew_rate);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Calculate slew rate factor base on update time.
*
* @param[in]  	dt			- Update (period) time
* @param[in]	slew_rate	- Wanted slew rate
* @return       k_rate		- Slew rate factor
*/
////////////////////////////////////////////////////////////////////////////////
static float32_t rate_limiter_calc_rate_factor(const float32_t dt, const float32_t slew_rate)
{
	float32_t k_rate = 0.0f;

	k_rate = ( slew_rate * dt );

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
* @brief    Initialize rate limiter
*
* @note Rising/Falling slew rate is references to change on seconds.
*
* 		E.g.:
* 			- for 1V/s -> put rise/fall rate = 1.0
* 			- for 0.5V/s -> put rise/fall rate = 0.5
*
* @param[out]  	p_rl_inst	- Pointer to rate limiter instance
* @param[in]  	rise_rate	- Rising slew rate
* @param[in]  	fall_rate	- Falling slew rate
* @param[in]  	dt			- Update (period) time
* @return       status		- Either OK or Error
*/
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t rate_limiter_init(p_rate_limiter * p_rl_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt)
{
	rate_limiter_status_t status = eRATE_LIMITER_OK;

	if 	(	( NULL != p_rl_inst )
		&& 	( dt > 0.0f ))
	{
		// Allocate space
		*p_rl_inst = malloc( sizeof( rate_limiter_t ));

		if ( NULL != *p_rl_inst )
		{
			// Init previous value & period
			(*p_rl_inst)->x_prev = 0.0f;
			(*p_rl_inst)->dt = dt;

			// Calculate rise/fall factors
			(*p_rl_inst)->k_rise = rate_limiter_calc_rate_factor( dt, rise_rate );
			(*p_rl_inst)->k_fall = rate_limiter_calc_rate_factor( dt, fall_rate );

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

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Update rate limiter
*
* @note User shall provide cyclic call of that function with a value of dt
* 		given at initialization phase.
*
* @param[out]  	p_rl_inst	- Pointer to rate limiter instance
* @param[in]  	x			- Input signal
* @return       y			- Output (slew limited) signal
*/
////////////////////////////////////////////////////////////////////////////////
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

			// No limitations...
			else
			{
				y = x;
			}

			// Store current value
			rl_inst->x_prev = y;
		}
	}

	return y;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Get success initialization flag
*
* @param[out]  	p_rl_inst	- Pointer to rate limiter instance
* @return       is_init		- Success initialization flag
*/
////////////////////////////////////////////////////////////////////////////////
bool rate_limiter_is_init(p_rate_limiter rl_inst)
{
	bool is_init = false;

	if ( NULL != rl_inst )
	{
		is_init = rl_inst->is_init;
	}

	return is_init;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Change slew rate
*
* @note Slew rate limit has same logic as with initialization function.
*
* @param[out]  	p_rl_inst	- Pointer to rate limiter instance
* @param[in]  	rise_rate	- Rising slew rate
* @param[in]  	fall_rate	- Falling slew rate
* @return       status		- Either OK or Error
*/
////////////////////////////////////////////////////////////////////////////////
rate_limiter_status_t rate_limiter_change_rate(p_rate_limiter rl_inst, const float32_t rise_rate, const float32_t fall_rate)
{
	rate_limiter_status_t status = eRATE_LIMITER_ERROR;

	// Check for instance and initialization
	if ( NULL != rl_inst )
	{
		if ( true == rl_inst->is_init )
		{
			rl_inst->k_rise = rate_limiter_calc_rate_factor( rl_inst->dt, rise_rate );
			rl_inst->k_fall = rate_limiter_calc_rate_factor( rl_inst->dt, fall_rate );

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
