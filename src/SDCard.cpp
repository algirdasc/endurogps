#include "SDCard.h"

void SDCard::start() 
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
    log_i("SD Card Size: %llu MB", cardSize);

    fwUpdateCheck();
}

void SDCard::fwUpdateCheck()
{
    if (!SD.exists(UPDATE_FILE_PATH)) {
        log_d("No firmware updates found");

        return;
    }

    File updateBin = SD.open(UPDATE_FILE_PATH);    
    if (updateBin.isDirectory()) {
        log_e("Firmware update file is directory");
        updateBin.close();

        return;
    }

    size_t updateSize = updateBin.size();
    if (updateSize == 0) {
        log_e("Firmware update file is empty");

        return;
    }

    SDCard::fwUpdate(updateBin, updateSize);
}

void SDCard::fwUpdate(Stream &updateFile, size_t updateSize)
{
    if (!Update.begin(updateSize)) {
        log_e("Not enough space to begin OTA");

        return;
    }

    size_t writtenSize = Update.writeStream(updateFile);
    
    log_i("Written %u out of %u", writtenSize, updateSize);

    if (!Update.end()) {
        log_e("Error #%d", Update.getError());

        return;
    }

    if (!Update.isFinished()) {
        log_e("Something went wrong!");
        
        return;
    }

    log_i("Update successfull");

    SD.remove(UPDATE_FILE_PATH);
}
