#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include <stdint.h>

#define GPIO_BITS_MAPPING(name) \
    struct { \
      unsigned int unused1 : 2;  /* 0..1 */ \
      unsigned int output_enable : 1;  /* 2 */ \
      unsigned int clock  : 1;   /* 3 */ \
      unsigned int strobe : 1;   /* 4 */ \
      unsigned int unused2 : 2;  /* 5..6 */ \
      unsigned int row : 4;  /* 7..10 */ \
      unsigned int unused3 : 6;  /* 11..16 */ \
      unsigned int r1 : 1;   /* 17 */ \
      unsigned int g1 : 1;   /* 18 */ \
      unsigned int unused4 : 3; \
      unsigned int b1 : 1;   /* 22 */ \
      unsigned int r2 : 1;   /* 23 */ \
      unsigned int g2 : 1;   /* 24 */ \
      unsigned int b2 : 1;   /* 25 */ \
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
    *(gpio_port_ + 7) = value;
  }

  // Clear the bits that are '1' in the output. Leave the rest untouched.
  inline void ClearBits(uint32_t value) {
    *(gpio_port_ + 10) = value;
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

#endif  // RPI_GPIO_H
