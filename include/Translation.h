#pragma once

const char FILEBROWSER_PAGE_BASE_URL[] PROGMEM = "/sdcard";
const char FILEBROWSER_PAGE_TITLE[] PROGMEM = "File browser";
const char FILEBROWSER_PAGE_404_ERROR[] PROGMEM = "SD Card is unavailable or path does not exist";
const char FILEBROWSER_PAGE_TABLE_HEADER_TEMPLATE[] PROGMEM = R"raw(
    <table class="pure-table pure-table-horizontal pure-table-stretch">
        <thead>
            <tr>
                <th></th>
                <th>Filename</th>
                <th>Size</th>
                <th>Actions</th>
            </tr>
        </thead>
)raw";
const char FILEBROWSER_PAGE_DIRECTORY_EMPTY_ROW[] PROGMEM = R"raw(
    <tr>
        <td class="text-center" colspan="100">
            Directory is empty
        </td>
    </tr>
)raw";

const char FIRMWARE_PAGE_BASE_URL[] PROGMEM = "/sdcard";
const char FIRMWARE_PAGE_TITLE[] PROGMEM = "Firmware update";
const char FIRMWARE_PAGE_SUBTITLE[] PROGMEM = "Current version: " FIRMWARE;
const char FIRMWARE_PAGE_SUBMIT[] PROGMEM = "Update to latest version";
const char FIRMWARE_PAGE_NEWEST_VERSION[] PROGMEM = "You have newest firmware version";
const char FIRMWARE_PAGE_UPDATE_SUCCESS[] PROGMEM = "Update was successful!";