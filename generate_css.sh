#!/bin/bash

echo "/* Auto generated with generate_css.sh, do not modify */" > include/HTTP/Style.h
printf "const char STYLE_CSS[] PROGMEM = R\"raw(%s)raw\";" \
"`python3 -m csscompressor  assets/css/style.css`" \
 >> include/HTTP/Style.h

# echo "/* Auto generated with generate_css.sh, do not modify */" > include/bonogps_css_base_battery.h
# printf "const char WEBPORTAL_CSS[] PROGMEM = \"%s%s\";" \
# "`python3 -m csscompressor  include/style_base.css`" \
# "`python3 -m csscompressor  include/style_base_battery.css`" \
# >> include/bonogps_css_base_battery.h

# echo "include/bonogps_css_base_battery.h generated"
