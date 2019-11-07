#include <stdint.h>

union Flags {
  uint16_t full;
  struct {
    union {
      uint8_t raw;
      struct {
        unsigned int otc : 1;
        unsigned int otd : 1;
        unsigned int bathi : 1;
        unsigned int batlow : 1;
        unsigned int chginh : 1;
        unsigned int xchg : 1;
        unsigned int fc : 1;
        unsigned int chg : 1;
      } fields;
    } high;
    union {
      uint8_t raw;
      struct {
        unsigned int ocvtaken : 1;
        unsigned int rsvd0 : 1;
        unsigned int rsvd1 : 1;
        unsigned int cf : 1;
        unsigned int rsvd2 : 1;
        unsigned int soc1 : 1;
        unsigned int socf : 1;
        unsigned int dsg : 1;
      } fields;
    } low;
  } components;
  struct {
    unsigned int otc : 1;
    unsigned int otd : 1;
    unsigned int bathi : 1;
    unsigned int batlow : 1;
    unsigned int chginh : 1;
    unsigned int xchg : 1;
    unsigned int fc : 1;
    unsigned int chg : 1;
    unsigned int ocvtaken : 1;
    unsigned int rsvd0 : 1;
    unsigned int rsvd1 : 1;
    unsigned int cf : 1;
    unsigned int rsvd2 : 1;
    unsigned int soc1 : 1;
    unsigned int socf : 1;
    unsigned int dsg : 1;
  } fields;
};

union FlagsB {
  uint16_t full;
  struct {
    union {
      uint8_t raw;
      struct {
        unsigned int soh : 1;
        unsigned int life : 1;
        unsigned int firstdod : 1;
        unsigned int rsvd0 : 1;
        unsigned int rsvd1 : 1;
        unsigned int dodeoc : 1;
        unsigned int dtrc : 1;
        unsigned int rsvd2 : 1;
      } fields;
    } high;
    union {
      uint8_t raw;
      struct {
        unsigned int rsvd3 : 1;
        unsigned int rsvd4 : 1;
        unsigned int rsvd5 : 1;
        unsigned int rsvd6 : 1;
        unsigned int rsvd7 : 1;
        unsigned int rsvd8 : 1;
        unsigned int rsvd9 : 1;
        unsigned int rsvda : 1;
      } fields;
    } low;
  } components;
  struct {
    unsigned int soh : 1;
    unsigned int life : 1;
    unsigned int firstdod : 1;
    unsigned int rsvd0 : 1;
    unsigned int rsvd1 : 1;
    unsigned int dodeoc : 1;
    unsigned int dtrc : 1;
    unsigned int rsvd2 : 1;
    unsigned int rsvd3 : 1;
    unsigned int rsvd4 : 1;
    unsigned int rsvd5 : 1;
    unsigned int rsvd6 : 1;
    unsigned int rsvd7 : 1;
    unsigned int rsvd8 : 1;
    unsigned int rsvd9 : 1;
    unsigned int rsvda : 1;
  } fields;
};
