var elem;
var instance;
const mr = { "morkov": "Морковь", "petrushka": "Петрушка", "salat": "Салат" };
function main() {
    elem = document.querySelector('select');
    instance = M.FormSelect.init(elem, {});
    getData('/plantName', 'GET', function (d) {
        console.log(d);
        if (d == "stop") {
            document.getElementById("main_body").style.backgroundColor = "#ffffff";
            document.getElementById("gotov").textContent = "готов";
        }
        else {
            instance.input.value = mr[d];
            document.getElementById("main_body").style.backgroundColor = "#9CCC65";
            document.getElementById("gotov").textContent = "работает";
        }
    });



    var save_button = document.getElementById("save_button");
    var pause_button = document.getElementById("pause_button");
    var stop_button = document.getElementById("stop_button");

    save_button.addEventListener('click', () => {
        console.log("save_button");
        onSaveButton();
    });

    pause_button.addEventListener('click', () => {
        console.log("pause_button");
        onPauseButton();
    });

    stop_button.addEventListener('click', () => {
        console.log("stop_button");
        onStopButton();
    });
}

function getData(url, method, callback) {
    var data;

    var xhr = new XMLHttpRequest();
    xhr.open(method, url, true);

    xhr.send();
    xhr.onreadystatechange = function () { // (3)
        if (xhr.readyState != 4) return;

        // button.innerHTML = 'Готово!';

        if (xhr.status != 200) {
            alert(xhr.status + ': ' + xhr.statusText);
        } else {
            console.log(xhr.responseText);
            callback(xhr.responseText)
        }

    }

    // data = xhr.responseText;
    // return data;


}

function sendData(url, data, method) {
    var xhr = new XMLHttpRequest();
    xhr.open(method, url, false);

    xhr.send(data);
}
const m = { "Морковь": "morkov", "Петрушка": "petrushka", "Салат": "salat" };
function onSaveButton() {
    document.getElementById("main_body").style.backgroundColor = "#9CCC65";
    document.getElementById("gotov").textContent = "работает";
    console.log(
        m[instance.input.value]
    );

    sendData('/saveData?body=' + String(m[instance.input.value]), '', 'POST')
}
function onPauseButton() {

}
function onStopButton() {
    sendData('/stopPlant', '', 'POST');
    document.getElementById("gotov").textContent = "готов";

    document.getElementById("main_body").style.backgroundColor = "#ffffff";
}
