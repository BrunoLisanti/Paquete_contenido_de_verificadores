window.onload = function() {

    // *** VARIABLES
    // ***

    var s_saver; // Guarda el timeout del screensaver, se declara acá para que clearTimeout() no de error por referencia a variable no declarada
    var PESABLES = false;
    var FLAG_PESABLE = "";
    var PRODUCTO_DEFAULT = "Codigo=0|Descripcion=Artículo no encontrado|PrecioUnit=$0.00|Oferta=" // Lo que se muestra si no se encontró el producto

    var counter = -1;
    var texts = [""];
    var logoFooter = document.getElementById("logopie");
    var divOferta = document.getElementById("Oferta");
    var campoDeTexto = document.getElementById("plu");
    var contadorMostrado = document.getElementById('contador');

    // *** FUNCIONES
    // ***
    
    function changeOffer(touchIsOnRight){
        if(touchIsOnRight){
            counter++;
            if (counter >= texts.length){
                counter = 0;
            }
        }
        else{
            counter--;
            if (counter < 0){
                counter = texts.length -1;
            }
        }

        divOferta.innerHTML = texts[counter];
        if (texts.length > 1)
            contadorMostrado.innerHTML = counter + 1 + '/' + texts.length;
        else
            contadorMostrado.innerHTML = "";
    }
    
    
    function ponerSalvaPantallas(){
        $('#salvapantallas').fadeIn(900);
        texts = [""];
    }

    function conDecimales(numero) {
        /* Devuelve STRING del número tomando sus últimos 2 dígitos como decimales.
        */
        return (numero/100).toFixed(2);
    }

    function parsearProductoPluAsc(lineaPluAsc) {
        /* Devolver entrada de plu.asc como un array campo-valor 
        */
        lineaPluAsc = lineaPluAsc.replace(/(\r\n|\n|\r)/gm, "");
        var camposValorProducto = lineaPluAsc.split('|');
        for (i = 0; i < camposValorProducto.length; i++) {
            camposValorProducto[i] = camposValorProducto[i].split('=');
        }
        return camposValorProducto;
    }

    function dividirPlu(plu, campo1, campo2, campo3) {
        /* 	Dividir la PLU en 3 campos
            En cada parámetro se especifica la longitud del campo
        */
        if (campo1 + campo2 + campo3 > plu.length) return [plu];

        campo2 = campo1+campo2;
        campo3 = campo2+campo3;
        return [plu.substring(0, campo1), plu.substring(campo1, campo2), plu.substring(campo2, campo3)];
    }

    //document.getElementsByTagName('video')[0].onended = function () { //Posible función para reproducir un video en el salvapantallas
    //  this.load();
    //  this.play();
    //};

    // *** LÓGICA DEL VERIFICADOR
    // ***

    // document.addEventListener("click", function() { //Llamar a server_conn al tocar la pantalla, no funciona
    //     campoDeTexto.focus(); // Asegurar que el campo de texto siempre esté enfocado
    //     changeOffer();
    //     // var xml = new XMLHttpRequest();
    //     // xml.open("POST", "/conn", false);
    //     // xml.send(null);
    // });

    $(document).on('click', function(e){
        campoDeTexto.focus();

        var center = $(this).width() / 2;
        if (e.offsetX > center)
        {
            changeOffer(true);
        }
        else
        {
            changeOffer(false);
        }
        
    })

    // Detectar tecla Enter (enviar PLU)
    document.addEventListener("keypress", function(tecla) {
        var key_code = (tecla.keyCode ? tecla.keyCode : tecla.which);
        if (key_code != 13) 
            return;        
        document.getElementById("Oferta").textContent = "";

        var texto_escaneado = campoDeTexto.value; 
        var plu = texto_escaneado;
        var talle = texto_escaneado.substring(7, 10);
        // var precio_cod = "$" + Number(texto_escaneado.substring(11)) + ".00";
        var precio_cod = texto_escaneado.substring(11);
        if (precio_cod.length > 5)
            precio_cod = texto_escaneado.substring(13);
        precio_cod = "$" + Number(precio_cod) + ".00";

        var flag = plu.substring(0, 2);

        campoDeTexto.value = "";

        // if (flag == FLAG_PESABLE) { // Procesamiento del código PLU pesable
        //     plu = dividirPlu(plu, 2, 5, 6);
        //     var peso = conDecimales(Number(plu[2].slice(0, -1))); // Con slice() se elimina el digito verificador

        //     plu[2] = "000000"; // Llenar último espacio con ceros
        //     plu = plu.join('');
        // }

        var request = "barcode=" + plu;
        var xhr 	= new XMLHttpRequest();
        xhr.withCredentials = true;
        xhr.addEventListener("readystatechange", function () {
            if (this.readyState === 4){ //DONE
                var producto = PRODUCTO_DEFAULT;
                if (this.responseText.split('|').length != 1) { // Encontró producto
                    producto = this.responseText;
                }
                camposValorProducto = parsearProductoPluAsc(producto);

                camposValorProducto.forEach(function(campoValor) {
                    var campo = campoValor[0];
                    var valor = campoValor[1];
                    
                    iconoTouch = document.getElementsByClassName('iconoTouch')[0];
                    contadorMostrado.display = 'none';
                    iconoTouch.style.opacity = '0';
                    logoFooter.style.display = "block";

                    switch(campo){
                        case "PrecioUnit":
                            if (valor !== '$0.00')
                                valor = precio_cod;
                        break;
                        case "Oferta":
                            if (valor !== ""){
                                logoFooter.style.display = "None";
                                texts = valor.split("_");
                                if (texts.length > 1)
                                {
                                    contadorMostrado.display = 'inline';
                                    iconoTouch.style.opacity = "0.5";
                                }
                                else
                                {
                                    contadorMostrado.display = 'none';
                                    iconoTouch.style.opacity = "0";
                                }

                                counter = -1;
                                changeOffer(true)
                            }
                            else{
                                texts = [""];
                                iconoTouch.style.opacity = "0";
                                logoFooter.style.display = "block";
                            }
                    }

                    // Escribir campos
                    var elementoDOM = document.getElementById(campo);
                    if (elementoDOM && valor.trim() !== "None") {
                        if (campo !== "Oferta")
                            elementoDOM.textContent = valor;
                    }
                });

                if (producto != PRODUCTO_DEFAULT){
                    document.getElementById('PrecioUnit').textContent = precio_cod;
                }

                // Sacar salvapantallas, empezar timer
                $('#salvapantallas').fadeOut(100);
                clearTimeout(s_saver);
                s_saver = setTimeout(ponerSalvaPantallas, texts.length * 3 * 3000);

            }
        });
        xhr.open("POST", "/");
        xhr.setRequestHeader("Accept", "text/plain"); // Tipo de MIME aceptado (*/* --> cualquiera)
        xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        xhr.send(request);
    });

}
