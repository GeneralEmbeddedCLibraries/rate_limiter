# Rate limiter
Rate limiter implementation in C for general DSP purposes. Module works on floating point numbers and support configuration of rise and fall rate of signals. 

Filter memory space is dynamically allocated and success of allocation is taken into consideration before using that instance.

#### Dependencies
Filter module needs ring buffer in order to store data. Ring buffer sources can be found under this [link](https://github.com/Misc-library-for-DSP/ring_buffer). 

Definition of flaot32_t must be provided by user. In current implementation it is defined in "*project_config.h*". Just add following statement to your code where it suits the best.

```C
// Define float
typedef float float32_t;
```

 #### API

 - rate_limiter_status_t **rate_limiter_init**(p_rate_limiter * p_rl_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt);
 - float32_t **rate_limiter_update**(p_rate_limiter rl_inst, const float32_t x);
 - bool **rate_limiter_is_init**(p_rate_limiter rl_inst);
 - rate_limiter_status_t **rate_limiter_change_rate**(p_rate_limiter rl_inst, const float32_t rise_rate, const float32_t fall_rate);


##### Example of usage

```C

// 1. Declare rate limiter instance
p_rate_limiter my_rate_limiter_inst = NULL;

/* 
*   2. Init rate limiter with following parameters:
*   - rise = 1.0 unit/sec
*   - fall = 0.5 unit/sec
*   - dt = 0.01 sec --> SAMPLE PERIOD
*/ 
if ( eRATE_LIMITER_OK != rate_limiter_init( &my_rate_limiter_inst, 1.0f, 0.5f, 0.01f ))
{
    // Rate limiter init failed
    // Further actions here...
}

// 3. Apply rate limiter in period of 10ms
loop @SAMPLE_TIME
{
    // Update rate limiter
    slew_rate_limited_signal = rate_limiter_update( my_rate_limiter_inst, raw_signal );
}

```