#include "hal/spi_types.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#include "esp_nrf24_map.h"

#include <inttypes.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NRF24_SPI_FREQUENCY (10*1000*1000)
#define NRF24_TAG "NRF24"

#define NRF24_CHECK_OK(ret) if((ret) != ESP_OK) return (ret)

enum nrf24_data_rate_t {
    NRF24_1MBPS = 0,
    NRF24_2MBPS,
    NRF24_250KBPS
};

enum nrf24_crc_t {
    NRF24_CRC_DISABLED = 0,
    NRF24_CRC_1BYTE,
    NRF24_CRC_2BYTES
};

enum nrf24_data_pipe_t {
    NRF24_P0 = 0,
    NRF24_P1,
    NRF24_P2,
    NRF24_P3,
    NRF24_P4,
    NRF24_P5,
    NRF24_ALL_PIPES
};

typedef struct {
    spi_host_device_t host_id;
    spi_device_handle_t spi_handle;
    int ce_io_num;
    int csn_io_num;
} nrf24_t;

esp_err_t nrf24_init(nrf24_t *dev, spi_host_device_t host_id, int mosi_io_num, int miso_io_num, int sclk_io_num, int ce_io_num, int csn_io_num);
esp_err_t nrf24_free(nrf24_t *dev);

esp_err_t nrf24_get_register(nrf24_t *dev, uint8_t reg, uint8_t *data, uint8_t len);
esp_err_t nrf24_set_register(nrf24_t *dev, uint8_t reg, uint8_t *data, uint8_t len);

esp_err_t nrf24_flush_tx(nrf24_t *dev);
esp_err_t nrf24_flush_rx(nrf24_t *dev);

esp_err_t nrf24_power_up_tx(nrf24_t *dev);
esp_err_t nrf24_power_up_rx(nrf24_t *dev);
esp_err_t nrf24_power_down(nrf24_t *dev);

esp_err_t nrf24_set_data_rate(nrf24_t *dev, enum nrf24_data_rate_t rate);
esp_err_t nrf24_set_crc(nrf24_t *dev, enum nrf24_crc_t crc);
esp_err_t nrf24_set_rf_channel(nrf24_t *dev, uint8_t channel);

esp_err_t nrf24_enable_rx_pipe(nrf24_t *dev, enum nrf24_data_pipe_t pipe);
esp_err_t nrf24_disable_rx_pipe(nrf24_t *dev, enum nrf24_data_pipe_t pipe);

void nrf24_flip_bytes(uint8_t *data, size_t len);

esp_err_t nrf24_set_rx_address(nrf24_t *dev, enum nrf24_data_pipe_t pipe, uint8_t *address, uint8_t address_length);
esp_err_t nrf24_set_tx_address(nrf24_t *dev, uint8_t *address, uint8_t address_length);

esp_err_t nrf24_set_payload_length(nrf24_t *dev, uint8_t length);

esp_err_t nrf24_send_data(nrf24_t *dev, uint8_t *data, uint8_t len);
int nrf24_get_data_available(nrf24_t *dev);
esp_err_t nrf24_get_data(nrf24_t *dev, uint8_t *data, uint8_t *len);

#ifdef __cplusplus
}
#endif