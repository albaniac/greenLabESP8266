var elem;
var instance;
const mr = {"morkov":"Морковь", "petrushka":"Петрушка", "salat":"Салат"};
function main() {
    elem = document.querySelector('select');
    instance = M.FormSelect.init(elem, {});
    var d = getData('/palntName', 'get');
    instance.input.value = mr[d];

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

function getData(url, method) {
    var data;

    var xhr = new XMLHttpRequest();
    xhr.open(method, url, false);

    xhr.send();

    if (xhr.status != 200) {
        return false;
    } else {
        data = xhr.responseText;
        return data;
    }

}

function sendData(url, data, method) {
    var xhr = new XMLHttpRequest();
    xhr.open(method, url, false);

    xhr.send(data);
}
const m = {"Морковь":"morkov", "Петрушка":"petrushka", "Салат":"salat"};
function onSaveButton() {

    console.log(
        m[instance.input.value]
    );

    sendData('/saveData?body=' + String(m[instance.input.value]), '', 'POST')
}
function onPauseButton() {

}
function onStopButton() {

}
