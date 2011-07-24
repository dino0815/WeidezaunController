#ifndef ADC_H 
#define ADC_H

  #include <avr/io.h>
  #include <inttypes.h>

  void     adc_init(void);
  void     adc_start1(uint8_t channel);
  uint16_t adc_read(uint8_t channel);
  uint16_t adc_read_avg(uint8_t channel, uint8_t average);

#endif

