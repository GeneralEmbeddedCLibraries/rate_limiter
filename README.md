# Rate limiter
This repository provides a flexible and efficient way to limit the rate of change of a signal, a process known as slew rate limiting. This is a critical function in embedded systems to prevent sudden, large changes in a control signal that could cause damage to hardware or introduce instability. The code works by creating a software "filter" for an input signal. Instead of the output instantly following the input, it can only change at a defined maximum rate.

## **Dependencies**

### **1. Utils Module**
Utils module must take following path:
```
"root/common/utils/src/utils.h"
```

## **General Embedded C Libraries Ecosystem**
In order to be part of *General Embedded C Libraries Ecosystem* this module must be placed in following path: 
```
root/middleware/rate_limiter/"module_space"
```

 ## **API**

| API Functions | Description | Prototype |
| --- | ----------- | ----- |
| **rate_limiter_init**         | Initialization of rate limiter            | rate_limiter_status_t rate_limiter_init(p_rate_limiter_t * p_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt) |
| **rate_limiter_init_static**  | Static initialization of rate limiter     | rate_limiter_status_t rate_limiter_init_static(p_rate_limiter_t p_inst, const float32_t rise_rate, const float32_t fall_rate, const float32_t dt) |
| **rate_limiter_is_init**      | Is rate limiter initialized               | bool rate_limiter_is_init(const p_rate_limiter_t inst) |
| **rate_limiter_hndl**         | Handle rate limiter                       | float32_t rate_limiter_hndl(const p_rate_limiter_t inst, const float32_t x) |
| **rate_limiter_change_rate**  | Change rate limiter slew                  | rate_limiter_status_t rate_limiter_change_rate(const p_rate_limiter_t inst, const float32_t rise_rate, const float32_t fall_rate) |


## **Usage**

### Dynamic instances allocation 
```C
// 1. Declare rate limiter instance pointer
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
    slew_rate_limited_signal = rate_limiter_hndl( my_rate_limiter_inst, signal );
}
```

### Static instances allocation 
```C
// 1. Declare rate limiter instance
rate_limiter my_rate_limiter_inst;

/* 
*   2. Init rate limiter with following parameters:
*   - rise = 1.0 unit/sec
*   - fall = 0.5 unit/sec
*   - dt = 0.01 sec --> SAMPLE PERIOD
*/ 
if ( eRATE_LIMITER_OK != rate_limiter_init_static( &my_rate_limiter_inst, 1.0f, 0.5f, 0.01f ))
{
    // Rate limiter init failed
    // Further actions here...
}

// 3. Apply rate limiter in period of 10ms
loop @SAMPLE_TIME
{
    // Update rate limiter
    slew_rate_limited_signal = rate_limiter_hndl( &my_rate_limiter_inst, signal );
}
```
