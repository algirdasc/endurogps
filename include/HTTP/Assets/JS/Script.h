#pragma once

const char JS_SCRIPT[] = R"raw(var recStatus=document.getElementById('rec-status');var startRec=document.getElementById('start-rec');var stopRec=document.getElementById('stop-rec');var navbar=document.getElementById('navbar');var navbarToggle=document.getElementById('navbar-toggle');stopRec.style.display=g_isRecording?'list-item':'none';startRec.style.display=g_isRecording?'none':'list-item';recStatus.innerHTML=g_isRecording?'⏺':'⏹';recStatus.className=g_isRecording&&g_isLocationValid?'text-danger text-blink':'text-info';startRec.onclick=function(a){a.preventDefault();var b=prompt('Enter session name',g_sessionName);if(b!==null){var c=startRec.querySelector('a');window.location.href=c.href+'?sessionName='+b;}};navbarToggle.onclick=function(a){a.preventDefault();navbar.style.display=navbar.style.display==='block'?'none':'block';};)raw";