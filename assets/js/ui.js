var startRec = document.getElementById('start-rec');
var stopRec = document.getElementById('stop-rec');

stopRec.style.display = g_isRecording ? 'list-item' : 'none';
startRec.style.display = g_isRecording ? 'none' : 'list-item';

startRec.onclick = function (e) {
    e.preventDefault();
    var sessionName = prompt('Enter session name', g_sessionName);
    if (sessionName !== null) {
        var a = startRec.querySelector('a');
        window.location.href = a.href + '?sessionName=' + sessionName;
    }
};