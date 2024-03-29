#include "sntp_sync.h"

static const char *TAG = "SNTP_SYNC";

uint32_t sntp_getTime(void)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    // Set timezone to Viet Nam Standard Time
    setenv("TZ", "GMT-07", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    return now;
}

void sntp_getDate(char *date_time)
{
    char strftime_buf[64];
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    // Set timezone to Viet Nam Standard Time
    setenv("TZ", "GMT-07", 1);
    tzset();
    localtime_r(&now, &timeinfo);

    strftime(strftime_buf, sizeof(strftime_buf), "%X", &timeinfo);
    // ESP_LOGI(TAG, "The current date/time in Viet Nam is: %s", strftime_buf);
    strcpy(date_time, strftime_buf);
}

void sntp_init_func(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    // sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

bool obtainTime(void)
{
    sntp_init_func();
    // // wait for time to be set
    // time_t now = 0;
    // struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    if (retry == retry_count)
    {
        return false;
    }
    else
    {
        ESP_LOGI(TAG, "Time is set.");
        return true;
    }
    // time(&now);
    // localtime_r(&now, &timeinfo);
}

void Set_SystemTime_SNTP(void)
{

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2023 - 1900))
    {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtainTime();
        // update 'now' variable with current time
        time(&now);
    }
}
