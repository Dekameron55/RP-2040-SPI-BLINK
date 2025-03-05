/**

 */
/* Example code to talk to a Custom SPI slave Module from fpga4fun.com
  reference link of the SPI module : https://www.fpga4fun.com/SPI2.html

  WokWi simmulation of a Verilog SPI slave communication with Rp2040

  The Slave takes the LSB of the Sent Msg on MOSI to set the LED on or off and returns the number of recieved
  messages on the MISO line.

  the two messages on the MOSI sent by the RP2040 control the LED on or off via the LSB and prints out the
  number of received mesages on the MISO line.

   GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO on SPI Module
   GPIO 17 (pin 22) Chip select -> CSB/!CS on SPI Module
   GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK on SPI Module
   GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI on SPI Module



*/
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

void pico_spi_init(){
    // This will use SPI0 at 0.5MHz.
    spi_init(spi_default, 1000);
    spi_set_format(spi_default, 16, SPI_CPOL_0 , SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));
}

static void spiExchangeByte(uint8_t data_tx, uint8_t* data_rx) {
    cs_select();
    spi_write_read_blocking(spi_default, &data_tx, data_rx, 1);
    cs_deselect();
    sleep_ms(10);
}
static void spiExchangeData(uint8_t* data_tx, uint8_t* data_rx, uint8_t numBytes) {
    cs_select();
    spi_write_read_blocking(spi_default, data_tx, data_rx, numBytes);
    cs_deselect();
    sleep_ms(10);
}
int main() {
    stdio_init_all();


    uint8_t data_rx;

    stdio_init_all();
    pico_spi_init();


    printf("Hello, RP2040 SPI Slave(v) trial !...\n");


    while (1) {
    spiExchangeByte(0x37, &data_rx);
    printf("Setting LED ON : Number of messages sent to SPI Slave %d\n",data_rx);
    sleep_ms(1000);

    spiExchangeByte(0x54, &data_rx);
    printf("Setting LED OFF: Number of messages sent to SPI Slave %d\n",data_rx);
    sleep_ms(1000);

    }

    return 0;
}
