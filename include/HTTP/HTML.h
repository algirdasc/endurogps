#pragma once

#include <Arduino.h>

const char HTML_SELECT_TAG_OPEN[] PROGMEM = R"raw(<label for="se_%s">%s</label><select class="pure-input-1" id="se_%s" name="%s">)raw";
const char HTML_SELECT_OPTION_TAG[] PROGMEM = R"raw(<option value="%s"%s>%s</option>)raw";
const char HTML_SELECT_TAG_CLOSE[] PROGMEM = "</select>";
const char HTML_BUTTON_TAG[] PROGMEM = R"raw(<a href="%s" class="pure-button %s">%s</a>)raw";
const char HTML_FORM_OPEN_TAG[] PROGMEM = R"raw(<form class="pure-form pure-form-stacked" method="POST">)raw";
const char HTML_FORM_CLOSE_TAG[] PROGMEM = R"raw(<button type="submit" class="pure-button pure-button-primary">%s</button></form>)raw";
const char HTML_CHECKBOX_TAG[] PROGMEM = R"raw(<label for="ch_%s" class="pure-checkbox"><input type="checkbox" id="ch_%s" name="%s" value="1" %s /> %s</label>)raw";
const char HTML_INPUT_TAG[] PROGMEM = R"raw(<label for="in_%s">%s</label><input type="%s" maxlength="255" class="pure-input-1" id="in_%s" name="%s" value="%s" />)raw";

class HTML
{
    public:
        static String formStart()
        {
            return String(HTML_FORM_OPEN_TAG);
        }

        static String formEnd(const char *submitText = "Submit")
        {
            char buff[128];
            sprintf(buff, HTML_FORM_CLOSE_TAG, submitText);

            return String(buff);
        }

        static String checkbox(const char *label, const char *name, bool checked)
        {
            char buff[256];
            sprintf(buff, HTML_CHECKBOX_TAG, name, name, name, (checked ? "checked" : ""), label); 

            return String(buff);
        }

        static String input(String label, String name, String value = "", String type = "text")
        {
            char buff[512];
            sprintf(buff, HTML_INPUT_TAG, name, label, type, name, name, value);

            return String(buff);
        }

        static String button(String label, String url, String cssClass = "")
        {
            char buff[128];
            sprintf(buff, HTML_BUTTON_TAG, url, cssClass, label);

            // TODO: use buff?
            return String(buff);
        }

        static String select(const char *label, const char *name, const char *values[], const char *labels[], int options = 0, String selected = "")
        {
            char buff[256];
            sprintf(buff, HTML_SELECT_TAG_OPEN, name, label, name, name);

            String select = buff;

            for (int i = 0; i < options; i++) {
                char option[128];
                sprintf(option, HTML_SELECT_OPTION_TAG, values[i], (String(values[i]).equalsIgnoreCase(selected) ? "selected" : ""), labels[i]);       
                select += String(option);
            }

            select += HTML_SELECT_TAG_CLOSE;

            return select;
        }
};
