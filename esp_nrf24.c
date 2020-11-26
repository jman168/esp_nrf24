#include <stdio.h>
#include "esp_nrf24.h"

esp_err_t init_nrf24(nrf24_t *dev, spi_host_device_t host_id, int mosi_io_num, int miso_io_num, int sclk_io_num, int ce_io_num, int csn_io_num) {
    esp_err_t ret;
    
    gpio_pad_select_gpio( ce_io_num );
	gpio_set_direction( ce_io_num, GPIO_MODE_OUTPUT );
	gpio_set_level( ce_io_num, 0 );

    const spi_bus_config_t config = {
        mosi_io_num: mosi_io_num,
        miso_io_num: miso_io_num,
        sclk_io_num: sclk_io_num,
        quadwp_io_num: -1,
        quadhd_io_num: -1,
    };
    ret = spi_bus_initialize(host_id, &config, 1);
    ESP_LOGI(TAG, "Initalized SPI bus, status: %d", ret);
    if(ret != ESP_OK) return ret;
    
    const spi_device_interface_config_t devcfg = {
		.clock_speed_hz = SPI_FREQUENCY,
		.queue_size = 7,
		.mode = 0,
		.flags = SPI_DEVICE_NO_DUMMY,
        .spics_io_num = csn_io_num,
        .command_bits = 8
	};
    spi_device_handle_t handle;
    ret = spi_bus_add_device(host_id, &devcfg, &handle);
    ESP_LOGI(TAG, "Added SPI bus device, status: %d", ret);
    if(ret != ESP_OK) return ret;

    dev->host_id = host_id;
    dev->spi_handle = handle;
    dev->ce_io_num = ce_io_num;
    dev->csn_io_num = csn_io_num;

    return ESP_OK;
}

esp_err_t free_nrf24(nrf24_t *dev) {
    esp_err_t ret;

    ret = spi_bus_remove_device(dev->spi_handle);
    ESP_LOGI(TAG, "Removed SPI device, status: %d", ret);
    if(ret != ESP_OK) return ret;

    ret = spi_bus_free(dev->host_id);
    ESP_LOGI(TAG, "Freed SPI bus, status: %d", ret);
    if(ret != ESP_OK) return ret;

    return ESP_OK;
}


esp_err_t nrf24_get_register(nrf24_t *dev, uint8_t reg, uint8_t *data, uint8_t len) {
    spi_transaction_t transaction;
    spi_transaction_ext_t ext_transaction; // Se we can have an address phase to fix bit misalignment in response
    memset(&transaction, 0, sizeof(spi_transaction_t));
    memset(&ext_transaction, 0, sizeof(spi_transaction_ext_t));
    transaction.flags = SPI_TRANS_VARIABLE_ADDR,
    transaction.cmd = R_REGISTER | (REGISTER_MASK & reg);
    transaction.length = len * 8;
    transaction.tx_buffer = data;
    transaction.rx_buffer = data;

    ext_transaction.base = transaction;
    ext_transaction.address_bits = 1; // Add one address bit to fix bit misalignment in response

    return spi_device_transmit(dev->spi_handle, (spi_transaction_t *)&ext_transaction);
}

esp_err_t nrf24_set_register(nrf24_t *dev, uint8_t reg, uint8_t *data, uint8_t len) {
    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(spi_transaction_t));
    transaction.cmd = W_REGISTER | (REGISTER_MASK & reg);
    transaction.length = len * 8;
    transaction.tx_buffer = data;
    transaction.rx_buffer = NULL;

    return spi_device_transmit(dev->spi_handle, &transaction);
}