var s_saver;

tday=new Array("Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado");
tmonth=new Array("Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre");

function GetClock(){
	var d=new Date();
	var nday=d.getDay(),nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getFullYear();
	var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();
	if(nmin<=9) nmin="0"+nmin
	if(nsec<=9) nsec="0"+nsec;

	$("#hora").html(""+tday[nday]+ " "+ ndate + " de " + tmonth[nmonth]+" "+nhour+":"+nmin+"");
}

//Pone screensaver
function setScreenSaver() {
    $('#screensaver').fadeIn(900);
}

$(function() {
	GetClock();
	setInterval(GetClock,1000);
    var barcode="";

    $('#screensaver').fadeOut(100);

    s_saver = setTimeout(setScreenSaver,5000);
    

    $(document).keydown(function(e) {
        clearTimeout(s_saver);

        $('#screensaver').fadeOut(100);

        s_saver = setTimeout(setScreenSaver,5000);
        
        var code = (e.keyCode ? e.keyCode : e.which);
        if(code==13)// Al hacer enter
        {
            $("#promotion").text("");
            $("#price").text("");
            //alert(barcode);
            var path = "/";
            var barcodeData = { barcode : barcode}

            $.ajax({
            type: "POST",
            url: path,
            data: barcodeData,
            barcode:barcode
            }).done(function(res) {
                //Spliteamos el mensaje
                keys = res.split('|');

                var codigo = "";
                var descr = "Articulo no encontrado";
                var precio = "";

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
                    }
                }

                $("#descripcion").text(descr);
                if(precio != "")
                {
                    $("#precio").text("$" + precio);
                }
                else
                {
                    $("#precio").text(precio);
                }

                $("#fotoprod").attr("src","productos/" + codigo + ".jpeg");

            });

            barcode = "";
        }
        else
        {
            barcode=barcode+String.fromCharCode(code);
        }
    });

});