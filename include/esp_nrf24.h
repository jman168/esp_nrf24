#include "hal/spi_types.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define SPI_FREQUENCY 40000000
#define TAG "NRF24"

typedef struct {
    spi_host_device_t host_id;
    spi_device_handle_t spi_handle;
    int ce_io_num;
    int csn_io_num;
} nrf24_t;

esp_err_t init_nrf24(nrf24_t *dev, spi_host_device_t host_id, int mosi_io_num, int miso_io_num, int sclk_io_num, int ce_io_num, int csn_io_num);
esp_err_t free_nrf24(nrf24_t *dev);
