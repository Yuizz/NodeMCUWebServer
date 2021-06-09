let isLogging = false;

const buttonToggle = document.getElementById("toggle")

const numberControlPlus = document.getElementById("increase-sample-rate")
const numberControlMinus = document.getElementById("decrease-sample-rate")
const numberControlInput = document.getElementById("sample-rate")

//The events to change the number input with the buttons
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

//The event on the toggle button for change the state
buttonToggle.addEventListener("click", (event) => {
    event.preventDefault()

    let xhttp = new XMLHttpRequest()
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log("GET:'/toggle':DONE")
        }
    };
    xhttp.open("GET", "/toggle", true)
    xhttp.send()
    
    const sps = numberControlInput.value
    if (!isLogging) {
        let xhr = new XMLHttpRequest()
        xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 204) {
                console.log("POST:'/sample-rate':DONE")
            }
        }
        xhr.open('POST', "/sample-rate", true)
        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
        const params = "SPS=" + sps
        xhr.send(params)
    }
})

//Interval to consult the state
setInterval(function ( ) {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            isLogging = this.responseText === "true" ? true : false
            buttonToggle.checked = isLogging
        }
    };
    xhttp.open("GET", "/state", true)
    xhttp.send()
    }, 75) 