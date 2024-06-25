function request(method, endpoint, data, done)
// Metodo para hacer peticiones http
{
	var xhr = new XMLHttpRequest();
	xhr.open(method, endpoint);
	xhr.onload = function() {
		if (xhr.status == 404)
			done(xhr.response);
		else
			done(null, xhr.response);
	}
	xhr.onerror = function() {
		done(xhr.response);
	}
	xhr.send(data);
}

function read_config_file(path, done)
{
	/* 	Obtener un diccionario a partir de un archivo de configuración
	*/
	request("GET", path, null, function(err, response) {
		var d = {};
		if (!err)  // El response sería un mensaje 404
		{
			var cfg = response.split(',');
			for (var line of cfg) {
				var [key, value] = line.split('=');
				d[key] = value;
			}
		} else { console.log("ERROR leyendo cfg: " + path) }
		done(d);
	});
}

function make_form(dict)
{
	/*	Crear FormData a partir de un diccionario. El servidor procesa FormData 
		a través de la API de Flask "request".
		Utilizado para formularios HTML a los que no se les puede aplicar el 
		formato "form" (e.g. una selección de botones tipo radio).
	*/
	form = new FormData();
	for (var key in dict) 
		form.append(key, dict[key]);
	return form;
}

function updateDBConfigDisplay()
{
	/*	Muestra la información del archivo de configuración en el formulario HTML
		Llamado también desde el DOM
	*/
	document.getElementById("previews").style.visibility = "hidden";
	document.getElementById("previews_header").style.visibility = "hidden";

	read_config_file("/eprom/getplu.cfg", function(cfg){
		if (cfg["mode"] === "plu") 
		{
			console.log("Esta en modo plu!!");
			document.getElementById("campos_base_de_datos").classList.add("disable_section");
			document.getElementById("rdb_disable_bd").checked = true;
		}
		else
		{
			console.log("Esta en modo sql!!");
			document.getElementById("campos_base_de_datos").classList.remove("disable_section");
			document.getElementById("rdb_enable_bd").checked = true;
		}
	});


	read_config_file("/db.cfg", function(cfg) {
		for (var key in cfg) {
			var column = document.getElementById(key);
			if (column)
				column.value = cfg[key];
		}
		document.getElementById("chkOferta").checked= cfg.hasOwnProperty("Oferta");
		document.getElementById("Oferta").disabled 	= !cfg.hasOwnProperty("Oferta");
	});
}

function updateLogosDisplay()
{
	/* 	Mostrar imágenes de logos en el grid
	*/
	// File listing de la carpeta /resources/logos/
	document.getElementById("previews").style.visibility = "visible";
	document.getElementById("previews_header").style.visibility = "visible";

	request('GET', '/Logo', null, function(err, response) {
		var logos = JSON.parse(response);
		var logoGrid = document.getElementById("logoGrid");
		logoGrid.innerHTML = "";
		logos.forEach(function(logo) {
			var img_div = document.createElement('div');
			img_div.setAttribute('class', 'grid_element_div');

			var img = document.createElement('img');
			img.setAttribute('id', logo);
			img.setAttribute('class', 'grid_element');
			img.setAttribute('src', '/resources/logos/' + logo);
			
			var delete_button = document.createElement('button');
			delete_button.setAttribute('id','delete_btn');
			delete_button.innerHTML = 'x';
			delete_button.setAttribute('class', 'delete_element');
			
			delete_button.addEventListener('click', function(){
				if (confirm("¿Seguro que desea borrar el logo?")){
					request("DELETE", "/Logo/Delete", logo, function(err,response){
						if (err){
							alert(err)
						} else {
							document.getElementById("preview").src += '';
							updateLogosDisplay();
							alert(response);
						}
					})
				}
			});
			
			img_div.appendChild(delete_button);

			img_div.appendChild(img);
			logoGrid.appendChild(img_div);

			// Click en imagen --> setear logo en EPROM
			img.addEventListener('click', function() {
				request("POST", "/Logo/Set", logo, function(err, response) {
					if (err) 
						alert(err);
					else {
						updatePreview();
						alert(response);
					}
				});
			});
			
		});
	});
}

function check_rdb_database()
/* 	Habilita y deshabilita los campos de texto dependiendo si esta elegido
	el modo plu o el modo sql en los radio buttons de la configuracion de bd.
*/ 
{
	if(document.getElementById("rdb_enable_bd").checked)
	{
		document.getElementById("campos_base_de_datos").classList.remove("disable_section");
		request("POST", "/DB/ModeConfigUpload", make_form({mode: "sql"}), function(err,response){
			if(err)
				alert(err);
			else {
				alert(response);
			}
		})
	}

	if(document.getElementById("rdb_disable_bd").checked)
	{
		document.getElementById("campos_base_de_datos").classList.add("disable_section");
		request("POST", "/DB/ModeConfigUpload", make_form({mode: "plu"}), function(err,response){
			if(err)
				alert(err);
			else{
				alert(response);
			}
		})
	}
}

function openTab(evt, tabName) 
{
	/* Funcion para tener divida la pagina en pestañas
	*/
	var tabcontent = document.getElementsByClassName("tabcontent");
	for (var i = 0; i < tabcontent.length; i++) 
	  tabcontent[i].style.display = "none";
  
	// Get all elements with class="tablinks" and remove the class "active"
	var tablinks = document.getElementsByClassName("tablinks");
	for (var i = 0; i < tablinks.length; i++) 
	  tablinks[i].className = tablinks[i].className.replace(" active", "");
  
	// Show the current tab, and add an "active" class to the button that opened the tab
	document.getElementById(tabName).style.display = "block";
	evt.currentTarget.className += " active";
}

function updatePreview()
{
	/*	Actualizar la preview. Se usa cuando se cambian los archivos HTML y CSS de eprom
	*/
	document.getElementById("preview").src += '';
}

/*	Inicio de la página
*/
updateLogosDisplay();
updateDBConfigDisplay();
read_config_file("/eprom/ui.cfg", function(cfg) {
	/*	Mostrar la UI seleccionada actualmente (en preview y en el configurador)
	*/
	document.getElementById("ui_" + cfg["name"]).checked = true; 	// Boton de selección
	request("POST", "/UI/Set/", make_form(cfg), function(err, response) {
		updatePreview();											// Preview
	});
});

document.getElementById("index").click(); // Pestaña inicial

/* 	Interacción del usuario
*/
// Subida de logos
document.getElementById("logoUpload").addEventListener("change", function() {
	var form = new FormData(this);
	// if (!form.get("file"))
	// 	return;
	console.log(form);
	request("POST", "/Logo/Upload", form, function(err, response) {
		if (err) 
			alert("ERROR: " + err);
		else { 
			updateLogosDisplay();
			alert(response);
		}
	});
});

// Subida de configuración de BD
document.getElementById("DBConfig").addEventListener("submit", function() {
	event.preventDefault();
	request("POST", "/DB/ConfigUpload", new FormData(this), function(err, response) {
		if (err)
			alert("ERR");
		else
			alert(response);
	});
});





// Escribir archivo de configuración de estilo
document.getElementById("uiConfig").addEventListener("click", function() {
	var ui 	= event.target.value; // Botón clickeado
	request(
		"POST", "/UI/Set", 
		make_form({
			"name" 	: ui,
			"html" 	: ui + ".html",
			"css"	: ui + ".css"
		}), 
		function(err, response) {
			if (err)
				alert("Error aplicando estilo: " + err);
			else
				updatePreview();
		}
	);
});