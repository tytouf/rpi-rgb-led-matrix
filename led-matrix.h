// -*- c++ -*-
// Controlling a 32x32 RGB matrix via GPIO.

#ifndef RPI_RGBMATRIX_H
#define RPI_RGBMATRIX_H

#include <stdint.h>
#include "gpio.h"

class RGBMatrix {
 public:
  RGBMatrix(GPIO *io);
  void ClearScreen();
  void FillScreen(uint8_t red, uint8_t green, uint8_t blue);

  // Here the set-up  [>] [>]
  //                         v
  //                  [<] [<]   ... so column 65..127 are backwards.
  int width() const { return 64; }
  int height() const { return 64; }
  void SetPixel(uint8_t x, uint8_t y,
                uint8_t red, uint8_t green, uint8_t blue);

  // Updates the screen once. Call this in a continous loop in some realtime
  // thread.
  void UpdateScreen();


private:
  GPIO *const io_;

  enum {
    kDoubleRows = 16,     // Physical constant of the used board.
    kChainedBoards = 4,   // Number of boards that are daisy-chained.
    kColumns = kChainedBoards * 32,
    kPWMBits = 4          // maximum PWM resolution.
  };

  union IoBits {
    GPIO_BITS_MAPPING(bits);
    uint32_t raw;
    IoBits() : raw(0) {}
  };

  // A double row represents row n and n+16. The physical layout of the
  // 32x32 RGB is two sub-panels with 32 columns and 16 rows.
  struct DoubleRow {
    IoBits column[kColumns];  // only color bits are set
  };
  struct Screen {
    DoubleRow row[kDoubleRows];
  };

  Screen bitplane_[kPWMBits];
};

#endif  // RPI_RGBMATRIX_H
