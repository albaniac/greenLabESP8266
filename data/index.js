function main() {
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

function onSaveButton (){

}
function onPauseButton (){
    
}
function onStopButton (){
    
}
