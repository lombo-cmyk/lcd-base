//
// Created by Lukasz on 06.03.2022.
//

#include <cmath>
#include <stdexcept>
#include <string>

#include "I2CWrapper.hpp"
#include "LcdBase.hpp"

void LcdBase::Init(std::uint8_t numberOfColumns, std::uint8_t address) {
    numberOfColumns_ = numberOfColumns;
    /* I2CWrapper should be initialized in main program, as many devices might
     * need the bus  and the initialization is generic*/
    auto& I2C = I2CWrapper::getInstance();
    I2C.AddDevice(address, LcdTag_);
    I2C.CheckCommunicationWithDevices();
    i2c_lcd1602_init(&LcdInfo_,
                     I2C.GetInfo(LcdTag_),
                     true,
                     2,
                     32,
                     numberOfColumns_);
    i2c_lcd1602_reset(&LcdInfo_);
    i2c_lcd1602_set_backlight(&LcdInfo_, isBacklight_);
}

void LcdBase::AdjustLine(std::string& line) const {
    std::string buf;
    buf.reserve(numberOfColumns_);
    std::size_t charsToInsert = numberOfColumns_ - line.length();
    std::size_t startingChars = std::floor(charsToInsert / 2);
    std::size_t endingChars = charsToInsert - startingChars;
    buf.insert(0, startingChars, ' ');
    buf.insert(startingChars, line);
    buf.insert(std::end(buf), endingChars, ' ');
    line = buf;
}
void LcdBase::DisplayLine(std::string& line, std::uint8_t row) const {
    std::uint8_t pos = 0;
    if (line.length() > numberOfColumns_) {
        std::string lcd_err = "Line too long!";
        i2c_lcd1602_clear(&LcdInfo_);
        DisplayLine(lcd_err, 0);
        throw std::invalid_argument("Line too long!");
    }
    AdjustLine(line);
    for (const char& letter : line) {
        i2c_lcd1602_move_cursor(&LcdInfo_, pos++, row);
        i2c_lcd1602_write_char(&LcdInfo_, letter);
    }
}

void LcdBase::DisplayTwoLines(std::string& line_1, std::string& line_2) const {
    DisplayLine(line_1, 0);
    DisplayLine(line_2, 1);
}
