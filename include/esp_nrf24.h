#include "hal/spi_types.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include <inttypes.h>
#include <string.h>

#define SPI_FREQUENCY 40000000
#define TAG "NRF24"


#define R_REGISTER 0b00000000
#define W_REGISTER 0b00100000

#define REGISTER_MASK 0b00011111

typedef struct {
    spi_host_device_t host_id;
    spi_device_handle_t spi_handle;
    int ce_io_num;
    int csn_io_num;
} nrf24_t;

esp_err_t init_nrf24(nrf24_t *dev, spi_host_device_t host_id, int mosi_io_num, int miso_io_num, int sclk_io_num, int ce_io_num, int csn_io_num);
esp_err_t free_nrf24(nrf24_t *dev);

esp_err_t nrf24_get_register(nrf24_t *dev, uint8_t reg, uint8_t *data, uint8_t len);
esp_err_t nrf24_set_register(nrf24_t *dev, uint8_t reg, uint8_t *data, uint8_t len);
