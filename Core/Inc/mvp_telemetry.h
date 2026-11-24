#ifndef MVP_TELEMETRY_H
#define MVP_TELEMETRY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Prepare PA8 for telemetry after CubeMX has initialized GPIO and TIM1. */
void MvpTelemetry_Init(void);

/** @brief Service the PD2 edge latch or the bench clock once per millisecond. */
void MvpTelemetry_Poll(void);

#ifdef __cplusplus
}
#endif
#endif
