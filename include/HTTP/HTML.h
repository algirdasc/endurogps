#pragma once

#include <Arduino.h>
#include "HTTP/Assets/HTML/Snippets.h"

class HTML
{
    public:
        static String formStart()
        {
            String form = SNIPPET_FORM_START;

            return form;
        }

        static String formEnd(String submitText = "Submit")
        {
            String form = R"raw(<button type="submit" class="pure-button pure-button-primary">)raw" + submitText + "</button></form>";

            return form;
        }

        static String checkbox(String label, String name, bool checked)
        {
            String checkbox = ""
                "<label for=\"chk_" + name + "\" class=\"pure-checkbox\">"
                "<input type=\"checkbox\" id=\"chk_" + name + "\" name=\"" + name + "\" value=\"1\" " + (checked ? "checked" : "") + " /> " + label + "</label>";

            return checkbox;
        }

        static String input(String label, String name, String value = "", String type = "text")
        {
            String input = ""
                "<label for=\"input_" + name + "\">" + label + "</label>"
                "<input type=\"" + type + "\" maxlength=\"255\" class=\"pure-input-1\" id=\"input_" + name + "\" name=\"" + name + "\" value=\"" + value + "\" />";

            return input;
        }

        static String button(String label, String url, String cssClass = "")
        {
            String input = "<a href=\"" + url + "\" class=\"pure-button " + cssClass + "\">" + label + "</a>";

            return input;
        }

        static String select(String label, String name, String values[], String labels[], int options = 0, String selected = "")
        {
            String select = ""
                "<label for=\"select_" + name + "\">" + label + "</label>"
                "<select class=\"pure-input-1\" id=\"select_" + name + "\" name=\"" + name + "\">";

            for (int i = 0; i < options; i++) {                
                select += "<option value=\"" + values[i] + "\"" + (values[i] == selected ? "selected" : "") + ">";
                select += labels[i];
                select += "</option>";
            }

            select += "</select>";

            return select;
        }

        static String select2(String label, String name, String values[], const char *labels[], int options = 0, String selected = "")
        {
            String select = ""
                "<label for=\"select_" + name + "\">" + label + "</label>"
                "<select class=\"pure-input-1\" id=\"select_" + name + "\" name=\"" + name + "\">";

            for (int i = 0; i < options; i++) {                
                select += "<option value=\"" + values[i] + "\"" + (values[i] == selected ? "selected" : "") + ">";
                select += labels[i];
                select += "</option>";
            }

            select += "</select>";

            return select;
        }
};
