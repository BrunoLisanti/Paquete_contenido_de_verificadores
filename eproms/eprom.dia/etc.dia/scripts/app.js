var s_saver;
var Barcode="";


//Pone screensaver
function setScreenSaver() {
    $('#screensaver').fadeIn(900);
    $("#oferta").hide();
    $("#descripcion").text("");
    $("#codigo").text("");
    $("#precio_valor").text("");
    $("#precio_centavos").text("");
    $("#precio_oferta_valor").text("");
    $("#precio_oferta_centavos").text("");
    $("#desc_oferta").text("");
    $("#consultando_precio_title").text("Consultando precio");
}

function reloadVideo() {
	$('#video')[0].load();
} 

$(function() {
	//GetClock();
	//setInterval(GetClock,1000);

	//$('#screensaver').fadeOut(100);
	
	//s_saver = setTimeout(setScreenSaver,5000);
    
    	// la opcion loop del tag de video de este chromium
	// no funciona bien le pusimos un timer de 60 seg. 
	// para este video En caso de poner otro 
	// Tomar en cuenta y cambiar la duracion
	//var reload = setInterval(reloadVideo, 60000);
	
    $(document).keydown(function(e) {
        clearTimeout(s_saver);

        $('#screensaver').fadeOut(100);

        $('#consultando').fadeIn(100);

        var code = (e.keyCode ? e.keyCode : e.which);
        if(code==13)// Al hacer enter
        {
            $("#descripcion").text("");
            $("#codigo").text("");
            $("#precio_valor").text("");
            $("#precio_centavos").text("");
            $("#precio_oferta_valor").text("");
            $("#precio_oferta_centavos").text("");
            $("#desc_oferta").text("");
            $("#consultando_precio_title").text("Consultando precio");
            $("#oferta").hide();

            //alert(barcode);
            var path = "/";
            var barcodeData = { barcode : Barcode}

            $.ajax({
            type: "POST",
            url: path,
            data: barcodeData,
            barcode:Barcode
            }).done(function(res) {
                clearTimeout(s_saver);
                //Spliteamos el mensaje
                keys = res.split('|');

                var codigo = "";
                var descr = "Articulo no encontrado";

                var precio = "";
                var centavos = "";
                
                var precio_oferta = "";
                var centavos_oferta = "";
                
                var info_ofertas = "";

                for (i = 0; i < keys.length; i++) {
                    var elm = keys[i];
                    keyval = elm.split('=');

                    switch (keyval[0])
                    {
                        case "Codigo":
                            codigo = keyval[1];
                        break;
                        case "Descripcion":
                            descr = keyval[1]
                        break;
                        case "PrecioUnit":
                            precio = keyval[1];
                        break;
                        case "PrecioUnitF":
                            precio_oferta = keyval[1];
                        break;
                        case "Ofertas":
                            info_ofertas = keyval[1];
                        break;
                        
                    }
                }

                if(precio != precio_oferta || info_ofertas != "") {
                    $("#oferta").show(500);
                    //Si vino precio verificamos que vino con centavos (,)
                    if (precio_oferta.split(",").length > 1) {
                        centavos_oferta = precio_oferta.split(",")[1];
                        precio_oferta = precio_oferta.split(",")[0];
                    } else if (precio_oferta.split(".").length > 1) {
                        centavos_oferta = precio_oferta.split(".")[1];
                        precio_oferta = precio_oferta.split(".")[0];
                    }
                } else {
                    $("#oferta").hide(500);
                }

                if(precio != "") {
                    //Si vino precio verificamos que vino con centavos (,)
                    if (precio.split(",").length > 1) {
                        centavos = precio.split(",")[1];
                        precio = precio.split(",")[0];
                    } else if (precio.split(".").length > 1) {
                        centavos = precio.split(".")[1];
                        precio = precio.split(".")[0];
                    }
                } else {
                    Barcode = "";
                    $("#consultando_precio_title").text("Precio no encontrado");
                    s_saver = setTimeout(setScreenSaver,10000);
                    return;
                }
                
                //Ocultamos el cartel
                $('#consultando').fadeOut(100);
                
                $("#descripcion").text(descr);
                $("#codigo").text(Barcode);
                $("#precio_valor").text(precio);
                $("#precio_centavos").text(centavos);
                $("#precio_oferta_valor").text(precio_oferta);
                $("#precio_oferta_centavos").text(centavos_oferta);
                $("#desc_oferta").text(info_ofertas);

                s_saver = setTimeout(setScreenSaver,10000);
                Barcode = "";
                
            });
            
        }
        else
        {
            Barcode=Barcode+String.fromCharCode(code);
        }
    });

});