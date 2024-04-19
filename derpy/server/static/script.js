var socket = io();
var textfield = document.getElementById("textfield");
var block = document.getElementById("block");


function myFunction() {
    if(textfield.value == ""){
        block.innerHTML = "";
    }else{
        socket.emit("FindLibs", textfield.value)
    }
}

socket.on("getlib", function(data) {
    var content = "";
    for (var package of data) {
        content += '<div class="card mb-3"><div class="card-body d-flex justify-content-between align-items-center"><h5 class="card-title">' + package[0] + '</h5><p class="card-text">Version: ' + package[1] + '</p><button type="button" class="btn btn-primary" onclick="copyText(\'' + package[0] + '\')">Download</button></div></div>';
    }
    block.innerHTML = content;
});

function copyText(text) {
    var tempTextArea = document.createElement("textarea");
    tempTextArea.value = "derpy install " + text;
    document.body.appendChild(tempTextArea);
    tempTextArea.select();
    document.execCommand("copy");
    document.body.removeChild(tempTextArea);
    alert("Command copied to clipboard");
}


