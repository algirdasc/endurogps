#!/usr/bin/python3

import sys
from urllib.request import urlopen
import subprocess

ESP_ADDR = "192.168.8.32"
PAGES = [
    "/settings/wifi",
    "/settings/gps",
    "/sdcard",
    "/",
]
CSS_FILE = "./.tmp/assets/css/_full.css"
HTML_FILENAMES = []

for page in PAGES:
    filename = "./.tmp/assets/html/{}.html".format(page.replace("/", "_"))
    HTML_FILENAMES.append(filename)
    
    if "refresh" in sys.argv:
        html = urlopen(f"http://{ESP_ADDR}{page}").read().decode("utf-8")
        with open(filename, "w") as h:
            h.write(html)    

command = ["cssdeadwood", CSS_FILE] + HTML_FILENAMES

print(" ".join(command))
print()

subprocess.check_output(command)