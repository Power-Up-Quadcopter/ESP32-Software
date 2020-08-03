

#include <driver/sdmmc_host.h>
#include <sdmmc_cmd.h>
#include <fstream>
#include <iostream>
#include <esp_vfs_fat.h>
#include "SD.h"


//https://github.com/espressif/esp-idf/blob/cf056a7d0b90261923b8207f21dc270313b67456/examples/storage/sd_card/main/sd_card_example_main.c
namespace SD{
    const std::string mount_point = MOUNT_POINT;
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    sdmmc_slot_config_t slot_config;
    sdmmc_host_t host;
    sdmmc_card_t* card;

    void init(){
        ESP_LOGI("SD", "Starting");

        esp_err_t ret;

        mount_config = {
                .format_if_mount_failed = false,
                .max_files = 5,
                .allocation_unit_size = 16 * 1024
        };
        slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
        host = sdmmc_host_t {
                .flags = SDMMC_HOST_FLAG_8BIT |
                         SDMMC_HOST_FLAG_4BIT |
                         SDMMC_HOST_FLAG_1BIT |
                         SDMMC_HOST_FLAG_DDR,
                .slot = SDMMC_HOST_SLOT_1,
                .max_freq_khz = SDMMC_FREQ_DEFAULT,
                .io_voltage = 3.3f,
                .init = &sdmmc_host_init,
                .set_bus_width = &sdmmc_host_set_bus_width,
                .get_bus_width = &sdmmc_host_get_slot_width,
                .set_bus_ddr_mode = &sdmmc_host_set_bus_ddr_mode,
                .set_card_clk = &sdmmc_host_set_card_clk,
                .do_transaction = &sdmmc_host_do_transaction,
                .deinit = &sdmmc_host_deinit,
                .io_int_enable = sdmmc_host_io_int_enable,
                .io_int_wait = sdmmc_host_io_int_wait,
                .command_timeout_ms = 0,
            };

        gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
        gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
        gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
        gpio_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
        gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

        ret = esp_vfs_fat_sdmmc_mount(mount_point.c_str(), &host, &slot_config, &mount_config, &card);

        if (ret != ESP_OK) {
            if (ret == ESP_FAIL) {
                ESP_LOGE("SD", "Failed to mount filesystem. ");
            } else {
                ESP_LOGE("SD", "Failed to initialize the card (%s). "
                              "Make sure you have an SD card in the slot", esp_err_to_name(ret));
            }
            return;
        }

        ESP_LOGI("SD", "Writing to file");
        std::ofstream myfile1 (mount_point + "/haha.txt");
        if (myfile1.is_open())
        {
            myfile1 << "ESP32 was here\n";
            myfile1.close();
        }
        else{
            ESP_LOGE("SD", "Failed to write to file");
            return;
        }
        ESP_LOGI("SD", "Reading from file");
        std::string output;
        std::string line;
        std::ifstream myfile2 (mount_point + "/haha.txt");
        if (myfile2.is_open())
        {
            while ( getline (myfile2,line) )
            {
               output.append(line + "\n");
            }
            myfile2.close();
            printf("%s",output.c_str());
        }
        else ESP_LOGE("SD", "Failed to read file");;
    }


}
