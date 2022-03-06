//
// Created by Lukasz on 06.03.2022.
//

#ifndef AIRCONTROLLER_LCDBASE_HPP
#define AIRCONTROLLER_LCDBASE_HPP

#include <iomanip>
#include <string>

#include "driver/i2c.h"
#include "i2c-lcd1602.h"

class LcdBase {
public:
    virtual void Init(std::uint8_t numberOfColumns, std::uint8_t address);
    virtual void DisplayWelcomeMessage() const = 0;

protected:
    std::string_view LcdTag_ = "LCD";
    i2c_lcd1602_info_t LcdInfo_;
    std::uint8_t numberOfColumns_ = 0;
    std::uint64_t backlightTimer_ = esp_timer_get_time();
    bool isBacklight_ = true;
    template<typename T>
    std::string ConvertNumberToString(T number, std::uint8_t precision) const {
        std::stringstream stringStream;
        stringStream << std::fixed << std::setprecision(precision) << number;
        return stringStream.str();
    }

    void AdjustLine(std::string& line) const;
    void DisplayLine(std::string& line, std::uint8_t row) const;
    void DisplayTwoLines(std::string& line_1, std::string& line_2) const;

    virtual void DisplayCurrentState() = 0;
    virtual void Setbacklight() = 0;
};

#endif // AIRCONTROLLER_LCDBASE_HPP
