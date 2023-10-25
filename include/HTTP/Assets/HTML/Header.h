#pragma once

const char HTML_HEADER[] PROGMEM = R"raw(<!DOCTYPE html><html lang="en"><head><title>EnduroGPS</title><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1"><link rel="stylesheet" href="/css/style.css"></head><body><div class="container-fluid"><div class="row"><div class="col-sm-3 col-lg-2"><nav class="navbar navbar-inverse navbar-fixed-side"><div class="container"><div class="navbar-header"><button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar"><span class="sr-only">Toggle navigation</span><span class="icon-bar"></span><span class="icon-bar"></span><span class="icon-bar"></span></button><a class="navbar-brand" href="#">EnduroGPS</a></div><div id="navbar" class="collapse navbar-collapse"><ul class="nav navbar-nav"><li><a href="#">Status</a></li><li><a href="/sdcard">File browser</a></li><li class="dropdown"><a class="dropdown-toggle" href="#">Recording</a><ul class="dropdown-menu"><li><a href="/device/recording/start">Start</a></li><li><a href="/device/recording/stop">Stop</a></li></ul></li><li class="dropdown"><a class="dropdown-toggle" href="#">Settings</a><ul class="dropdown-menu"><li><a href="/settings/wifi" class="pure-menu-link">Wireless</a></li><li><a href="/settings/gps" class="pure-menu-link">GPS</a></li></ul></li><li class="dropdown"><a class="dropdown-toggle" href="#">Power</a><ul class="dropdown-menu"><li><a href="/device/restart" class="pure-menu-link" onclick="confirm('Are you sure want to restart device?')">Restart</a></li><li><a href="/device/poweroff" class="pure-menu-link" onclick="confirm('Are you sure want to power off device?')">Power off</a></li></ul></li></ul><ul class="nav navbar-nav navbar-right"><p class="navbar-text text-center"><a href="https://github.com/)raw" GIT_REPO R"raw(">)raw" VERSION R"raw(</a></p></ul></div></div></nav></div><div class="col-sm-9 col-lg-10"><div class="container">)raw";