#include "SDCardUpdate.h"

void SDCard::start()
{
    if (SD.begin())
    {
        fwUpdateCheck();
    }
}

void SDCard::fwUpdateCheck()
{
    if (!SD.exists(UPDATE_FILE_PATH))
    {
        log_i("No firmware updates found");

        return;
    }

    File updateBin = SD.open(UPDATE_FILE_PATH);
    if (updateBin.isDirectory())
    {
        log_e("Firmware update file is directory");
        updateBin.close();

        return;
    }

    size_t updateSize = updateBin.size();
    if (updateSize == 0)
    {
        updateBin.close();
        log_e("Firmware update file is empty");

        return;
    }

    SDCard::fwUpdate(updateBin, updateSize);
}

void SDCard::fwUpdate(Stream &updateFile, size_t updateSize)
{
    if (!Update.begin(updateSize))
    {
        log_e("Not enough space to begin OTA");

        return;
    }

    size_t writtenSize = Update.writeStream(updateFile);
    log_i("Written %u out of %u", writtenSize, updateSize);

    if (!Update.end())
    {
        log_e("Error #%d", Update.getError());

        return;
    }

    if (!Update.isFinished())
    {
        log_e("Something went wrong!");

        return;
    }

    log_i("Update successfull");

    SD.remove(UPDATE_FILE_PATH);
}
