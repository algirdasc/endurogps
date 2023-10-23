import htmlmin
import csscompressor

assets_dir = "include/HTTP/Assets/"

html_files = {
    "assets/html/header.html": "HTML/Header.h",
}

css_files = {
    "assets/css/style.css": "CSS/Style.h",
    "assets/css/pure-css.css": "CSS/PureCSS.h"
}

def variable_name(filename):
    var = filename.upper()
    var = var.replace(".H", "").replace("/", "_")
    return var

for html_file, header_file in html_files.items():
    with open(html_file, "r") as html:
        minified = htmlmin.minify(html.read(), remove_empty_space=True)
        with open("{}/{}".format(assets_dir, header_file), "w") as header:
            header.write("#pragma once\n\n")
            header.write("const char {}[] PROGMEM = R\"raw({})raw\";".format(variable_name(header_file), minified))

for css_file, header_file in css_files.items():
    with open(css_file, "r") as css:
        minified = csscompressor.compress(css.read())
        with open("{}/{}".format(assets_dir, header_file), "w") as header:
            header.write("#pragma once\n\n")
            header.write("const char {}[] PROGMEM = R\"raw({})raw\";".format(variable_name(header_file), minified))

