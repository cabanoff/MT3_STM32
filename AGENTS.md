# Project rules

## Language and standard

- Use C++20 unless the existing project explicitly requires another standard.
- Prefer modern C++ constructs where they are compatible with embedded targets.
- Do not use exceptions or RTTI unless already enabled in the project.
- Avoid dynamic memory allocation unless explicitly required.

## Architecture

- Follow SOLID principles where practical.
- Use RAII for resource ownership and cleanup.
- Keep hardware-specific code separated from application logic.
- Prefer dependency injection over direct access to global objects.
- Do not introduce unnecessary abstractions for simple MCU functionality.

## STM32 / HAL

- Preserve STM32CubeMX generated code sections.
- Do not modify generated files unless necessary.
- Put application code inside USER CODE sections when editing Cube-generated files.
- Do not change MCU pin assignments, clocks, peripherals, DMA channels, or interrupt priorities unless explicitly requested.
- Check STM32F1 hardware limitations before using peripheral features.

## Integer types

- Use fixed-width integer types:
  - uint8_t
  - uint16_t
  - uint32_t
  - int8_t
  - int16_t
  - int32_t

- Avoid plain `int` for hardware protocols, registers, serialized data, CAN messages, and persistent storage.

## C++ style

- Use `constexpr` for compile-time constants.
- Use `enum class` instead of unscoped enums.
- Prefer `std::array` over raw arrays when practical.
- Prefer references over pointers when null is not a valid state.
- Use `nullptr`, never `NULL`.
- Use explicit casts such as `static_cast<>`.
- Avoid C-style casts.

## Public API documentation

Use Doxygen comments for public APIs.

Example:

```cpp
/**
 * @brief Sets the motor speed.
 * @param speed Requested speed in the range 0..255.
 * @retval true Speed was accepted.
 * @retval false Speed value was invalid.
 * @note This function does not enable the motor automatically.
 */
bool setSpeed(uint8_t speed);