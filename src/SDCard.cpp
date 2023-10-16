#include "SDCard.h"

SDCard::SDCard() 
{
    if (!SD.begin()) {
        log_e("SD Card mount failed");

        return;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        log_e("No SD card attached");

        return;
    }


    switch (cardType) {
        case CARD_MMC:
            log_i("Found MMC card");
            break;
        case CARD_SD:
            log_i("Found SDSC card");
            break;
        case CARD_SDHC:
            log_i("Found SDHC card");
            break;
        default:
            log_i("Found unknown card");
            break;
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
