#include "bldc_encoder.hpp"

BLDCEncoder::BLDCEncoder(
    GPIO_TypeDef * hu_port, uint16_t hu_pin,
    GPIO_TypeDef * hv_port, uint16_t hv_pin,
    GPIO_TypeDef * hw_port, uint16_t hw_pin
)
    :   hu_port_(hu_port),
        hv_port_(hv_port),
        hw_port_(hw_port),
        hu_pin_(hu_pin),
        hv_pin_(hv_pin),
        hw_pin_(hw_pin)
{}

uint8_t BLDCEncoder::get_state() const noexcept {
    uint8_t state{};

    state |= HAL_GPIO_ReadPin(hu_port_, hu_pin_) << 2;
    state |= HAL_GPIO_ReadPin(hv_port_, hv_pin_) << 1;
    state |= HAL_GPIO_ReadPin(hw_port_, hw_pin_);

    return state;
}

uint16_t BLDCEncoder::get_degree_rotation() const noexcept {
    static constexpr uint16_t degrees[8] = {
        INVALID_DEG, // 0b000 invalid
        210,         // 0b001
        90,          // 0b010
        150,         // 0b011
        330,         // 0b100
        270,         // 0b101
        30,          // 0b110
        INVALID_DEG  // 0b111 invalid
    };

    return degrees[get_state()];
}

float BLDCEncoder::get_radian_rotation() const noexcept {
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float radians[8] = {
        INVALID_RAD,       // 0b000 invalid
        7.0f  * PI / 6.0f, // 0b001
        1.0f  * PI / 2.0f, // 0b010
        5.0f  * PI / 6.0f, // 0b011
        11.0f * PI / 6.0f, // 0b100
        3.0f  * PI / 2.0f, // 0b101
        1.0f  * PI / 6.0f, // 0b110
        INVALID_RAD        // 0b111 invalid    
    };

    return radians[get_state()];
}