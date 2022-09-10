function call(color){
    fetch(`/${color}`)
}

function callInput(){
    let value = document.querySelector("#value")
   if(value.value>=0 && value.value<256)
    call(value.value)
}