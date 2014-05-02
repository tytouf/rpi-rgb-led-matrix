
#include "bbb_gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/*static*/ const uint32_t GPIO::kValidBits 
= ((1 << 2) | (1 << 3) | (1 << 4) | // R1, G1, B1
   (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | // ROW
   (1 << 10) | (1 << 11) | (1 << 12) | // OE, CLK, STB/LAT
   (1 << 13) | (1 << 14) | (1 << 15));  // R2, G2, B2

uint32_t GPIO::InitOutputs(uint32_t outputs) {
  if (gpio_port_ == NULL) {
    fprintf(stderr, "Attempt to init outputs but initialized.\n");
    return 0;
  }
  outputs &= kValidBits;   // Sanitize input.
  output_bits_ = outputs;

  // Configure pins as output
  volatile uint32_t *gpio_oe_addr = gpio_port_ + GPIO_OE;
  uint32_t reg = *gpio_oe_addr;
  reg = reg & (0xffffffff - kValidBits);
  *gpio_oe_addr = reg;

  SetBits(outputs);
  return output_bits_;
}

GPIO::GPIO() : output_bits_(0), gpio_port_(NULL) {
}

// Based on code example found in http://elinux.org/RPi_Low-level_peripherals
bool GPIO::Init() {

  int mem_fd;
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
    perror("can't open /dev/mem: ");
    return false;
  }

  char *gpio_map =
    (char*) mmap(NULL,             //Any adddress in our space will do
         BLOCK_SIZE,       //Map length
         PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
         MAP_SHARED,       //Shared with other processes
         mem_fd,           //File to map
         GPIO_BASE         //Offset to GPIO peripheral
         );

  //close(mem_fd); //No need to keep mem_fd open after mmap

  if (gpio_map == MAP_FAILED) {
    fprintf(stderr, "mmap error %ld\n", (long)gpio_map);
    return false;
  }

  gpio_port_ = (volatile uint32_t *)gpio_map;
  return true;
}
