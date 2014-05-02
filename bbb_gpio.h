#ifndef BBB_GPIO_H
#define BBB_GPIO_H

#include <stdint.h>

#define GPIO2_START_ADDR 0x481ac000
#define GPIO_BASE  GPIO2_START_ADDR
// Divide by 4 as addition are done on uint32_t*
#define GPIO_OE (0x134 >> 2)
#define GPIO_CLEARDATAOUT (0x190 >> 2)
#define GPIO_SETDATAOUT (0x194 >> 2)
#define BLOCK_SIZE 0x1000

#define GPIO_SET *(gpio_port_ + GPIO_SETDATAOUT)
#define GPIO_CLR *(gpio_port_ + GPIO_CLEARDATAOUT)

#define GPIO_BITS_MAPPING(name) \
    struct { \
      unsigned int unused0 : 2;/* 0..1 */ \
      unsigned int r1 : 1;   /* 2 */ \
      unsigned int g1 : 1;   /* 3 */ \
      unsigned int b1 : 1;   /* 4 */ \
      unsigned int unused1 : 1; /* 5 */ \
      unsigned int row : 4;   /* 6..9 */ \
      unsigned int output_enable : 1; /* 10 */ \
      unsigned int clock : 1; /* 11 */ \
      unsigned int strobe : 1; /* 12 */ \
      unsigned int r2 : 1;    /* 13 */ \
      unsigned int g2 : 1;    /* 14 */ \
      unsigned int b2 : 1;    /* 15 */ \
    } name;

// For now, everything is initialized as output.
class GPIO {
 public:
  // Available bits that actually have pins.
  static const uint32_t kValidBits;

  GPIO();

  // Initialize before use. Returns 'true' if successful, 'false' otherwise
  // (e.g. due to a permission problem).
  bool Init();

  // Initialize outputs.
  // Returns the bits that are actually set.
  uint32_t InitOutputs(uint32_t outputs);

  // Set the bits that are '1' in the output. Leave the rest untouched.
inline void SetBits(uint32_t value) {
    GPIO_SET = value;
}

  // Clear the bits that are '1' in the output. Leave the rest untouched.
inline void ClearBits(uint32_t value) {
    GPIO_CLR = value;
}

inline void Write(uint32_t value) {
  // Writing a word is two operations. The IO is actually pretty slow, so
  // this should probably  be unnoticable.
  SetBits(value & output_bits_);
  ClearBits(~value & output_bits_);
}
 private:
  uint32_t output_bits_;
  volatile uint32_t *gpio_port_;
};

#endif  // BBB_GPIO_H
