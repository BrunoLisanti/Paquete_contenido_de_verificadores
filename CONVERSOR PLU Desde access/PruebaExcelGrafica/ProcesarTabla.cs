using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PruebaExcelGrafica
{
    internal class ProcesarTabla //Clase intermedia para generar tablas, filtrarlas y escribir.
    {
        private static string path = ""; //direccion del archivo a leer, sin importar su extension

        public static string Path { get => path; set => path = value; }

        private static bool restricciones(string[] campos, bool[] checkBoxes) //Recibe los 3 campos de la fila y 3 booleanos que representan los checkboxes
                                                                            //si la fila cumple con las restricciones, devuelve true
        {
            bool aux = campos[0] != "" && campos[1] != "" && campos[2] != ""; //Pregunta si la fila tiene algun campo vacío

            if (checkBoxes[0] && aux) //pregunta si esta checkeado
            {
                aux = campos[0] != "0"; //si es que esta checkeado, pregunta si ya esta en true para no pisar el filtro anterior
            }
            if (checkBoxes[1] && aux)
            {
                aux = campos[2] != "0";
            }

            return aux;
        }

        public static DataTable obtenerTabla(string path) //dependiendo de la extensión del archivo llamará a la función correspondiente para obtener la tabla.
        {
            if (path.EndsWith("xlsx",StringComparison.OrdinalIgnoreCase))
                return ConexionExcel.obtenerTabla(path, 0);
            if (path.EndsWith("accdb", StringComparison.OrdinalIgnoreCase) || path.EndsWith("mdb", StringComparison.OrdinalIgnoreCase))
                return ConexionAccess.obtenerTabla(path, ConexionAccess.NombreDeTabla);
            return null;
        }

        static public DataTable filtrarColumnas(DataTable dt, params string[] columns)
        {
            //Toma la dt, la transforma en una view y luego la vuelve a convertir en dt, a traves de un metodo que
            //recibe las columnas que queremos seleccionar de la tabla original.
            DataView view = new DataView(dt);

            return view.ToTable("Selected", false, columns);
        }

        static public void escribirTXT(DataTable dt, bool[] checkBoxes) //Toma la dt y escribe linea por linea cada fila
                                                                        //Recibe por parametro un array de booleanos que representan los checkboxes
        {
            using (StreamWriter writer = new StreamWriter($"TxtGenerados/plu.asc"))
            {
                if (dt == null)
                    return;

                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    string id = dt.Rows[i][0].ToString();
                    string descripcion = dt.Rows[i][1].ToString();
                    string precio = dt.Rows[i][2].ToString();
                    string oferta = "";
                    if (dt.Columns.Count == 4)
                        oferta = dt.Rows[i][3].ToString();

                    if (checkBoxes[2]) //Limitar la descripcion a 40 caracteres
                    {
                        if (dt.Rows[i][1].ToString().Length > 39)
                            descripcion = dt.Rows[i][1].ToString().Substring(0, 39);
                    }

                    for (int x = 0; x < descripcion.Length; x++) //Quitarle a la descripcion los caracteres especiales
                    {
                        if (((int)descripcion[x] < 48 || (int)descripcion[x] > 127) && ((int)descripcion[x] < 160 || (int)descripcion[x] > 165) && (int)descripcion[x] != ' ')
                        {
                            descripcion = descripcion.Remove(x, 1);
                            x--;
                        }
                    }

                    for (int x = 0; x < id.Length; x++) //Si el id tiene caracteres que no sean numeros, se borran
                    {
                        if (ProcesarTabla.config()[11] == "False")
                        {
                            if ((int)id[x] < 48 || (int)id[x] > 57)
                            {
                                id = id.Remove(x, 1);
                                x--;
                            }
                        }
                    }

                    for (int x = 0; x < precio.Length; x++) //si el precio tiene caracteres que no sean numeros, se borran
                    {
                        if ((int)precio[x] < 44 || (int)precio[x] > 57)
                        {
                            precio = precio.Remove(x, 1);
                            x--;
                        }

                    }

                    if (id.Length > 14) //Si el cod de barra se pasa se borra totalmente asi la linea no se escribe.
                        id = "";

                    if (restricciones(new string[] { id, descripcion, precio }, checkBoxes)) //Si la fila cumple con las restricciones, se escribe, sino se saltea
                    {
                        if (oferta == "")
                            writer.Write($"Codigo={id}|Descripcion={descripcion}|PrecioUnit={precio}\n");
                        else
                            writer.Write($"Codigo={id}|Descripcion={descripcion}|PrecioUnit={precio}|Oferta={oferta}\n");
                    }    
                }
            }
        }

        static public string[] config() //Devuelve en un array todos los valores del archivo de configuracion
        {
            List<string> configuraciones = new List<string>();
            foreach (string s in File.ReadAllText($"TxtGenerados/config.txt").Split('|'))
            {
                if (s != "")
                    configuraciones.Add(s.Split('=')[1]);
            }
            return configuraciones.ToArray();
        }
    }
}
