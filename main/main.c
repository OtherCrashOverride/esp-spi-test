#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#include <string.h>

spi_transaction_t trans;

void app_main(void)
{
    nvs_flash_init();

    // Initialize SPI
    esp_err_t ret;


    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));

    buscfg.miso_io_num=GPIO_NUM_19;
    buscfg.mosi_io_num=GPIO_NUM_23;
    buscfg.sclk_io_num=GPIO_NUM_18;
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;

    //Initialize the SPI bus
    ret=spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    assert(ret==ESP_OK);


    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));

    devcfg.clock_speed_hz = 2500000;
    devcfg.mode = 0;
    devcfg.spics_io_num = 22;
    devcfg.queue_size = 2;
    devcfg.pre_cb = 0;
    devcfg.flags = 0;

    spi_device_handle_t spi;
    ret=spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);


    memset(&trans, 0, sizeof(trans));


    trans.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;

    trans.tx_data[0] = 0x00;
    trans.tx_data[1] = 0x00;
    trans.tx_data[2] = 0x00;

    trans.rx_data[0] = 0x01;
    trans.rx_data[1] = 0x02;
    trans.rx_data[2] = 0x03;

    trans.length = 3;

    ret = spi_device_transmit(spi, &trans);
    assert(ret==ESP_OK);

    printf("main: rx_data=%d, %d, %d\n", trans.rx_data[0], trans.rx_data[1], trans.rx_data[2]);


    while (true) {
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}
