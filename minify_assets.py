#!/usr/bin/python3

import re
import os
import string
import htmlmin
import random
import csscompressor
from slimit import minify
from html_classes_obfuscator import html_classes_obfuscator

assets_dir = "./include/HTTP/Assets/"

def copy(src_file, dst_file):
    with open(src_file, "r") as src:
        with open(dst_file, "w") as dst:
            dst.write(src.read())
            
def original_file(file_type, file_path):
    return f"./assets/{file_type}/{os.path.basename(file_path)}"

html_files = {
    "./assets/html/header.html": "HTML/Header.h",
    "./assets/html/footer.html": "HTML/Footer.h",
}

css_header_file = "CSS/Style.h"
css_files = [    
    "./assets/css/bootstrap.css",
    "./assets/css/navbar.css",
    "./assets/css/style.css",
]

js_header_file = "JS/Script.h"
js_files = [
    "./assets/js/ui.js",
]

# temporary file path
os.makedirs("./.tmp/assets/css", exist_ok=True)
os.makedirs("./.tmp/assets/html", exist_ok=True)
os.makedirs("./.tmp/assets/js", exist_ok=True)

# obscure file storage
temp_files = {
    "html": ["./.tmp/assets/html/{}".format(os.path.basename(x)) for x in html_files.keys()],
    "css": ["./.tmp/assets/css/{}".format(os.path.basename(x)) for x in css_files],
    "js": ["./.tmp/assets/js/{}".format(os.path.basename(x)) for x in js_files]
}

# copy files
for file_type, file_paths in temp_files.items():
    for file_path in file_paths:
        copy(f"./assets/{file_type}/{os.path.basename(file_path)}", file_path)

def variable_name(filename):
    var = filename.upper()
    var = var.replace(".H", "").replace("/", "_")
    return var

def generate_class(current_classes_list):

    def random_class():
        return ''.join(random.choice(string.ascii_letters) for i in range(6))

    res = random_class()

    while res in current_classes_list.values():
        res = random_class()

    return res


#html_classes_obfuscator.html_classes_obfuscator(temp_files["html"], temp_files["css"], temp_files["js"], generate_class)

for html_file in temp_files["html"]:
    with open(html_file, "r") as html:
        minified = htmlmin.minify(html.read(), remove_empty_space=True, remove_optional_attribute_quotes=False)
        minified = re.sub(r'\$([A-Z0-9_]+)\$', r')raw" \1 R"raw(', minified)
        header_file = html_files[original_file("html", html_file)]        
        with open("{}/{}".format(assets_dir, header_file), "w") as header:
            header.write("#pragma once\n\n")
            header.write("const char {}[] = R\"raw({})raw\";".format(variable_name(header_file), minified))


css_file_content = ""
for css_file in temp_files["css"]:
    with open(css_file, "r") as css:
        css_file_content += csscompressor.compress(css.read())

with open("{}/{}".format(assets_dir, css_header_file), "w") as header:
    header.write("#pragma once\n\n")
    header.write("const char {}[] = R\"raw({})raw\";".format(variable_name(css_header_file), css_file_content))
    
with open("./.tmp/assets/css/_full.css", "w") as full:
    full.write(css_file_content)

js_file_content = ""
for js_file in temp_files["js"]:
    with open(js_file, "r") as js:
        js_file_content += minify(js.read(), mangle=True)

with open("{}/{}".format(assets_dir, js_header_file), "w") as header:
    header.write("#pragma once\n\n")
    header.write("const char {}[] = R\"raw({})raw\";".format(variable_name(js_header_file), js_file_content))
