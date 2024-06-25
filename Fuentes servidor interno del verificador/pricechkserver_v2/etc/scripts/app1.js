var s_saver;

//Pone screensaver
function setScreenSaver() {
    $('#screensaver').fadeIn(900);
}

$(function() {
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

                $("#promotion").text(descr);
                if(precio != "")
                {
                    $("#price").text("$" + precio);
                }
                else
                {
                    $("#price").text(precio);
                }

                $("#pluimage").attr("src","productos/" + codigo + ".jpeg");

            });

            barcode = "";
        }
        else
        {
            barcode=barcode+String.fromCharCode(code);
        }
    });

    $("#price").click(function() {
        var path = "/";
        var barcodeData = { barcode : "7792798007400"}

        $.ajax({
        type: "POST",
        url: path,
        data: barcodeData
        }).done(function(res) {
            //Spliteamos el mensaje
            keys = res.split('|');

            for (i = 0; i < keys.length; i++) {
                var elm = keys[i];
                keyval = elm.split('=');
                if(keyval[0] == "Descripcion"){
                    $("#promotion").text(keyval[1]);
                }
                if(keyval[0] == "PrecioUnit")
                {
                    $("#price").text("$" + keyval[1]);
                }
            }
            //alert(res);
        });
       

    });

});