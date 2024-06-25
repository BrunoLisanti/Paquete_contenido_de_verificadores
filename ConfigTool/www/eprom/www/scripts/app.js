
window.onload = function() 
{
    function isOverflown(element) {
        return element.scrollHeight > element.clientHeight || element.scrollWidth > element.clientWidth;
      }
      
    var parrafo_oferta = document.getElementById("Oferta");
    if (isOverflown(parrafo_oferta))
        alert("Overflow");
    while(isOverflown(parrafo_oferta))
    {
        parrafo_oferta.style.fontSize--;
    }

// *** VARIABLES
// ***

var s_saver; // Guarda el timeout del screensaver, se declara acá para que clearTimeout() no de error por referencia a variable no declarada
var PESABLES = true;
var FLAG_PESABLE = "65";
var PRODUCTO_DEFAULT = "Codigo=0|Descripcion=Gaseosa Light|PrecioUnit=299.99|Oferta=¡Solo por hoy, 30% de descuento en la segunda unidad!"; // Lo que se muestra si no se encontró el producto

var logoFooter = document.getElementById("logopie");
var campoDeTexto = document.getElementById("plu");

// *** FUNCIONES
// ***

function ponerSalvaPantallas(){
    $('#salvapantallas').fadeIn(900);
}


function conDecimales(numero) {
    /* Devuelve STRING del número tomando sus últimos 2 dígitos como decimales.
	*/
    return (numero/100).toFixed(2);
}

function parsearProductoPluAsc(lineaPluAsc) {
    /* Devolver entrada de plu.asc como un array campo-valor 
	*/
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
	if (campo1+campo2+campo3 > plu.length) return [plu];

    campo2 = campo1+campo2;
    campo3 = campo2+campo3;
    return [plu.substring(0, campo1), plu.substring(campo1, campo2), plu.substring(campo2, campo3)];
}

// *** LÓGICA DEL VERIFICADOR
// ***

document.addEventListener("click", function() { campoDeTexto.focus(); }); // Asegurar que el campo donde se escribe el PLU esté siempre enfocado.

// Detectar tecla Enter (enviar PLU)
document.addEventListener("keypress", function(tecla) {
    var key_code = (tecla.keyCode ? tecla.keyCode : tecla.which);
	if (key_code != 13) return;

    document.getElementById("Oferta").textContent = "";
    
    var plu = campoDeTexto.value;
    var flag = plu.substring(0, 2);

    campoDeTexto.value = "";

    if (flag == FLAG_PESABLE) { // Procesamiento del código PLU pesable
        plu = dividirPlu(plu, 2, 5, 6);
        var peso = conDecimales(Number(plu[2].slice(0, -1))); // Con slice() se elimina el digito verificador

        plu[2] = "000000"; // Llenar último espacio con ceros
        plu = plu.join('');
    }

    var request = "barcode=" + plu;
    var xhr 	= new XMLHttpRequest();
    xhr.withCredentials = true;
    xhr.addEventListener("readystatechange", function () {
        if (this.readyState === 4) // DONE
		{

		var producto = PRODUCTO_DEFAULT;
        if (this.responseText.split('|').length != 1) { // Encontró producto
        	producto = this.responseText;
		}
		camposValorProducto = parsearProductoPluAsc(producto);

		camposValorProducto.forEach(function(campoValor) {
			var campo = campoValor[0];
			var valor = campoValor[1];

			switch(campo)
			{
				case "PrecioUnit":
				if (PESABLES && flag == FLAG_PESABLE)
                	valor = Number(valor) * peso; // Calculo de precio en artículos pesables
					valor = '$' + valor;
				break;	
                case "Oferta":
                    if (valor.length > 30)
                        valor = valor.substring(0,40);
                break;
            }

			// Escribir campos
            var elementoDOM = document.getElementById(campo);
            if (elementoDOM && valor.trim() !== "None") {

                elementoDOM.textContent = valor;
            }

            var parrafo_oferta = document.getElementById("Oferta");
            while(isOverflown(parrafo_oferta))
            {
                parrafo_oferta.style.fontSize--;
            }

		});

		// Sacar salvapantallas, empezar timer
		$('#salvapantallas').fadeOut(100);
		clearTimeout(s_saver);
		s_saver = setTimeout(ponerSalvaPantallas,15000);
        }
    });
    xhr.open("POST", "/");
    xhr.setRequestHeader("Accept", "text/plain"); // Tipo de MIME aceptado (*/* --> cualquiera)
    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    xhr.send(request);
});

}
