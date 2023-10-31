#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <FixedString.h>

#include "Global.h"
#include "Params.h"
#include "HTTP/HTTPCodes.h"
#include "HTTP/Assets/HTML/Header.h"
#include "HTTP/Assets/HTML/Footer.h"

const char HTML_PAGE_HEADER[] = R"raw(<div class="page-header"><h1>%s</h1></div>)raw";
const char HTML_FORM_OPEN[] = R"raw(<form method="POST">)raw";
const char HTML_FORM_CLOSE[] = R"raw(<button type="submit" class="btn btn-primary">Save</button></form>)raw";
const char HTML_CHECKBOX[] = R"raw(<div class="checkbox"><label for="ch_%s" class="pure-checkbox"><input type="checkbox" id="ch_%s" name="%s" value="1" %s /> %s</label></div>)raw";
const char HTML_INPUT[] = R"raw(<label for="in_%s">%s</label><input type="%s" maxlength="%i" class="form-control" id="in_%s" name="%s" value="%s" %s />)raw";
const char HTML_SELECT_OPEN[] = R"raw(<label for="se_%s">%s</label><select class="form-control" id="se_%s" name="%s">)raw";
const char HTML_SELECT_CLOSE[] = R"raw(</select>)raw";
const char HTML_OPTION[] = R"raw(<option value="%s"%s>%s</option>)raw";
const char HTML_REDIRECT[] = R"raw(<div class="text-center">Redirecting to <a href="%s">%s</a></div>)raw";
const char HTML_BREADCRUMB_ACTIVE[] = R"raw(<li class="active">%s</li>)raw";
const char HTML_BREADCRUMB_INACTIVE[] = R"raw(<li><a href="%s">%s</a></li>)raw";
const char HTML_SETTINGS_SUCCESS[] = R"raw(<div class="alert alert-success">Settings saved successfully. Settings will apply after device restart.</div>)raw";
const char HTML_JS[] = R"raw(<script>var g_isRecording=%s;var g_sessionName='%s';</script>)raw";
const char HTML_ALERT[] = R"raw(<div class="alert alert-danger">%s</div>)raw";

class HTML
{
public:
    static const char *select(const char *label, const char *name)
    {
        FixedString256 select;
        select.appendFormat(HTML_SELECT_OPEN, name, label, name, name);

        return select.c_str();
    }

    static const char *option(int value, const char *label, bool selected = false)
    {
        char c[16] = "";
        sprintf(c, "%d", value);
        return option(c, label, selected);
    }

    static const char *option(const char *value, const char *label, bool selected = false)
    {
        FixedString128 option;
        option.appendFormat(HTML_OPTION, value, selected ? "selected" : "", label);

        return option.c_str();
    }

    static const char *checkbox(const char *label, const char *name, bool checked)
    {
        FixedString512 checkbox;
        checkbox.appendFormat(HTML_CHECKBOX, name, name, name, checked ? "checked" : "", label);

        return checkbox.c_str();
    }

    static const char *input(const char *label, const char *name, const char *value = "", const char *type = "text", uint maxlength = 255, const char *attributes = "")
    {
        FixedString512 input;
        input.appendFormat(HTML_INPUT, name, label, type, maxlength, name, name, value, attributes);

        return input.c_str();
    }

    static const char *breadcrumb(const char *url, const char *label, bool active = false)
    {
        FixedString128 breadcrumb;
        if (active)
        {
            breadcrumb.appendFormat(HTML_BREADCRUMB_ACTIVE, label);
        }
        else
        {
            breadcrumb.appendFormat(HTML_BREADCRUMB_INACTIVE, url, label, active);
        }

        return breadcrumb.c_str();
    }

    static const char *js()
    {
        Params params;
        params.load();

        FixedString256 js;
        js.appendFormat(HTML_JS,
                        g_isRecording ? "true" : "false",
                        params.storage.gpsSessionName);

        return js.c_str();
    }
};
