let isLogging = false;

const buttonToggle = document.getElementById("toggle")

const numberControlPlus = document.getElementById("increase-sample-rate")
const numberControlMinus = document.getElementById("decrease-sample-rate")
const numberControlInput = document.getElementById("sample-rate")

numberControlPlus.addEventListener("click", (event) => {
    event.preventDefault();
    if (isLogging) return;
    const step = parseInt(numberControlInput.step)
    const currentValue = parseInt(numberControlInput.value)
    numberControlInput.value = currentValue + step > 260 ? 260 :currentValue + step;
})
numberControlMinus.addEventListener("click", (event) => {
    event.preventDefault();
    if(isLogging) return;
    const step = parseInt(numberControlInput.step)
    const currentValue = parseInt(numberControlInput.value)
    numberControlInput.value = currentValue - step < 10 ? 10 :currentValue - step;
})

buttonToggle.addEventListener("click", (event) => {
    event.preventDefault()

    let xhttp = new XMLHttpRequest()
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // document.getElementById("temperature").innerHTML = this.responseText;
            console.log(this)
        }
    };
    xhttp.open("GET", "/toggle", true)
    xhttp.send()

    const sps = numberControlInput.value
    if (!isLogging) {
        $.post("/sample-rate", {
            sps: sps,
        })
    }
})

setInterval(function ( ) {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            isLogging = this.responseText === "true" ? true : false;
            buttonToggle.checked = isLogging;
        }
    };
    xhttp.open("GET", "/state", true);
    xhttp.send();
    }, 75) ;