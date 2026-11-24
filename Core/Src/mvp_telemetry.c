#include "mvp_telemetry.h"
#include "tim.h"

#ifndef MT3_MVP_STANDALONE
#define MT3_MVP_STANDALONE 0
#endif

#define PAIR_HOLD_MS       5000U
#define BENCH_SLOT_MS       125U
#define CAPTURE_PERIODS     501U
#define SYNC_HZ            7992U
#define EMPTY_CATCH_HZ     8192U

typedef struct {
    uint16_t depth_hz;
    uint16_t temperature_hz;
} TestTones;

/* Fixed setpoints from the AVR's V3_3/Out_New.c transfer functions.
   Each slot uses one timer period for its entire duration. */
static const TestTones test_pairs[] = {
    { .depth_hz = 9156U, .temperature_hz = 12036U }, /* P1 = 30 m,  T1 = 10 C */
    { .depth_hz = 9722U, .temperature_hz = 13536U }, /* P2 = 150 m, T2 = 25 C */
};

typedef enum {
    SLOT_SYNC, SLOT_CATCH, SLOT_DEPTH, SLOT_TEMPERATURE
} TelemetrySlot;

static TelemetrySlot next_slot;
static const TestTones *frame_pair;
static uint32_t cycle_start_ms;
static uint32_t last_slot_ms;
static uint32_t slot_hold_ms; /* Zero until the first slot starts. */
static uint32_t counter_hz;

static uint32_t next_frequency_hz(uint32_t now_ms)
{
    switch (next_slot) {
    case SLOT_CATCH:
        next_slot = SLOT_DEPTH;
        return EMPTY_CATCH_HZ;

    case SLOT_DEPTH: {
        /* One selection serves both data slots, even across a 5 s boundary.
           Keep the cycle origin recent for unsigned tick subtraction at rollover. */
        const uint32_t phase_ms = (now_ms - cycle_start_ms) % (2U * PAIR_HOLD_MS);
        cycle_start_ms = now_ms - phase_ms;
        frame_pair = &test_pairs[phase_ms / PAIR_HOLD_MS];
        next_slot = SLOT_TEMPERATURE;
        return frame_pair->depth_hz;
    }
    case SLOT_TEMPERATURE:
        next_slot = SLOT_SYNC;
        return frame_pair->temperature_hz;

    case SLOT_SYNC:
    default:
        next_slot = SLOT_CATCH;
        return SYNC_HZ;
    }
}

static uint32_t period_ticks(uint32_t frequency_hz)
{
    return (counter_hz + frequency_hz / 2U) / frequency_hz;
}

void MvpTelemetry_Init(void)
{
#if !MT3_MVP_STANDALONE
    /* The AVR advances on the falling edge. EXTI records even short pulses
       between polls. CubeMX leaves the EXTI2 NVIC interrupt disabled. */
    GPIO_InitTypeDef sync = {
        .Pin = SYNC_IN_Pin, .Mode = GPIO_MODE_IT_FALLING, .Pull = GPIO_NOPULL
    };
    HAL_GPIO_Init(SYNC_IN_GPIO_Port, &sync);
    __HAL_GPIO_EXTI_CLEAR_IT(SYNC_IN_Pin);
#endif

    next_slot = SLOT_SYNC;
    frame_pair = &test_pairs[0];
    last_slot_ms = 0U;
    slot_hold_ms = 0U;

    /* The configured APB2 divider is 1, so PCLK2 is the TIM1 input clock. */
    counter_hz = HAL_RCC_GetPCLK2Freq() / (htim1.Init.Prescaler + 1U);
    SET_BIT(htim1.Instance->CR1, TIM_CR1_ARPE);
    __HAL_TIM_SET_AUTORELOAD(&htim1, period_ticks(SYNC_HZ) - 1U);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U); /* Idle low before the first slot. */
    htim1.Instance->EGR = TIM_EGR_UG;
    if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
}

void MvpTelemetry_Poll(void)
{
#if !MT3_MVP_STANDALONE
    if (__HAL_GPIO_EXTI_GET_IT(SYNC_IN_Pin) == 0U) {
        return;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(SYNC_IN_Pin);
#endif

    const uint32_t now_ms = HAL_GetTick();
    if (now_ms - last_slot_ms < slot_hold_ms) {
        return;
    }
    if (slot_hold_ms == 0U) {
        cycle_start_ms = now_ms;
    }

    const uint32_t frequency_hz = next_frequency_hz(now_ms);
    const uint32_t ticks = period_ticks(frequency_hz);
    /* Latch period and duty on the same rollover. HAL PWM enables CCR1 preload. */
    SET_BIT(htim1.Instance->CR1, TIM_CR1_UDIS);
    __HAL_TIM_SET_AUTORELOAD(&htim1, ticks - 1U);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ticks / 2U);
    CLEAR_BIT(htim1.Instance->CR1, TIM_CR1_UDIS);
    last_slot_ms = now_ms;

#if MT3_MVP_STANDALONE
    slot_hold_ms = BENCH_SLOT_MS;
#else
    /* Allow the receiver to measure 500 periods. The AVR rearms after 501.
       Round up and allow one millisecond for the HAL tick resolution. */
    slot_hold_ms = 1U + (CAPTURE_PERIODS * 1000U + frequency_hz - 1U) / frequency_hz;
#endif
}
