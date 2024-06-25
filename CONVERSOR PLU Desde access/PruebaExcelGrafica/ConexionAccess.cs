using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Data.Sql;
using System.Data.OleDb;
using static OfficeOpenXml.ExcelErrorValue;
using OfficeOpenXml.Style;

namespace PruebaExcelGrafica
{
    internal class ConexionAccess //Clase destinada al proceso de conexión con una base de datos Access
    {
        static private string nombreDeTabla = ""; //Variable estatica para guardar el nombre de tabla a leer
        static private string contrasenia = ""; //Variable estática para guardar la contraseña de la bd

        public static string NombreDeTabla { get => nombreDeTabla; set => nombreDeTabla = value; }
        public static string Contrasenia { get => contrasenia; set => contrasenia = value; }

        static public DataTable obtenerNombres(string path) //Consulta a la base de datos para obtener una tabla que contiene los nombres de las tablas de la bd
        {
            string connString = $"Provider=Microsoft.ACE.OLEDB.12.0;Data Source={path};Jet OLEDB:Database Password={Contrasenia};";

            DataTable results = new DataTable();

            using (OleDbConnection conn = new OleDbConnection(connString))
            {
                conn.Open();

                results = conn.GetOleDbSchemaTable(OleDbSchemaGuid.Tables, new object[] { null, null, null, "TABLE" });
                conn.Close();
            }
            return results;
        }
        static public DataTable obtenerTabla(string path,string nombreTabla) //a partir de la tabla guardada en la variable estática se trae su información en una datatable
        {
            string connString = $"Provider=Microsoft.ACE.OLEDB.12.0;Data Source={path};Jet OLEDB:Database Password={Contrasenia};";

            DataTable results = new DataTable();

            using (OleDbConnection conn = new OleDbConnection(connString))
            {
                OleDbCommand cmd = new OleDbCommand($"SELECT * FROM {nombreTabla}", conn);

                conn.Open();

                OleDbDataAdapter adapter = new OleDbDataAdapter(cmd);

                adapter.Fill(results);

                conn.Close();
            }
            return results;
        }
    }
}
