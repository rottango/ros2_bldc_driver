#ifndef BLDC_ENCODER_HPP
#define BLDC_ENCODER_HPP

#include <cstdint>
#include "stm32f7xx_hal.h"

class BLDCEncoder {
public:
    BLDCEncoder(
        GPIO_TypeDef * hu_register, uint16_t hu_pin,
        GPIO_TypeDef * hv_register, uint16_t hv_pin,
        GPIO_TypeDef * hw_register, uint16_t hw_pin
    );

    ~BLDCEncoder() = default;

    BLDCEncoder(const BLDCEncoder&) = delete; // disable copy constructor
    BLDCEncoder& operator=(const BLDCEncoder&) = delete; // disable copy assignment

    /// Returns the rotor state based on the Hall sensor signals.
    ///
    /// The return value encodes the three Hall sensor inputs as a 3-bit number:
    ///     - Bit 2 (MSB) : HU pin state (0 = low, 1 = high)
    ///     - Bit 1       : HV pin state (0 = low, 1 = high)
    ///     - Bit 0 (LSB) : HW pin state (0 = low, 1 = high)
    ///
    ///     HU  HV  HW
    ///     1   1   0 -> 0b110 -> 330° to 30°  rotation
    ///     0   1   0 -> 0b010 -> 30°  to 90°  rotation   
    ///     0   1   1 -> 0b011 -> 90°  to 150° rotation
    ///     0   0   1 -> 0b001 -> 150° to 210° rotation
    ///     1   0   1 -> 0b101 -> 210° to 270° rotation
    ///     1   0   0 -> 0b100 -> 270° to 330° rotation
    ///     
    /// Values 0b000 and 0b111 are invalid states.
    /// This can be used to determine the current rotor sector for commutation.
    uint8_t get_state() const noexcept;

    /// Returns max rotor angle corresponding to the current Hall state.
    /// For invalid rotation state returns INVALID_DEG.
    uint16_t get_degree_rotation() const noexcept;

    /// Returns max rotor angle corresponding to the current Hall state in radians.
    /// For invalid rotation state returns INVALID_RAD.
    float  get_radian_rotation() const noexcept;

    static constexpr uint16_t INVALID_DEG = 0xFFFF;
    static constexpr float    INVALID_RAD = -1.0f;

private:
    GPIO_TypeDef * const hu_port_;
    GPIO_TypeDef * const hv_port_;
    GPIO_TypeDef * const hw_port_;

    const uint16_t hu_pin_;
    const uint16_t hv_pin_;
    const uint16_t hw_pin_;
};

#endif // BLDC_ENCODER_HPP