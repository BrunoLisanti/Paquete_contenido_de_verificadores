window.onload = function() {

    var s_saver; //Guarda el timeout del screensaver, se declara acá para que clearTimeout() no de error por referencia a variable no declarada
    var campoDeTexto = document.getElementById("plu");
    var iframe = document.getElementById('iframe');
    var overlay = document.getElementById('overlay');
    // *** FUNCIONES
    // ***

    function ponerSalvaPantallas(){
        $('#salvapantallas').fadeIn(900);
        setTimeout(function(){
            iframe.src = "";
        }, 900);
    }

    document.addEventListener('click', function(e){
        campoDeTexto.focus();
    })

    overlay.addEventListener('click', function(e){
        campoDeTexto.focus();
    })

    // Detectar tecla Enter (enviar PLU)
    document.addEventListener("keypress", function(tecla) {
        var key_code = (tecla.keyCode ? tecla.keyCode : tecla.which);
        if (key_code != 13) 
            return; 
        tecla.preventDefault(); // Evitar el comportamiento predeterminado
        
        iframe.src = campoDeTexto.value;
        campoDeTexto.focus();
        campoDeTexto.value = "";

        $('#salvapantallas').fadeOut(100);
        clearTimeout(s_saver);
        s_saver = setTimeout(ponerSalvaPantallas, 15000);

    });
}
