using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace PruebaExcelGrafica
{
    internal static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            if (!Directory.Exists("TxtGenerados"))
                Directory.CreateDirectory("TxtGenerados");

            if (args.Length == 0) // Si no recibe ningun argumento, el programa se abre en la interfaz gráfica de configuración.
            {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    Application.Run(new VentanaSeleccionarColumnas());
            }
            else //si se le manda algun parámetro, solo accede al archivo y escribe el plu haciendo uso del archivo de configuracion.
            {
                ConexionAccess.NombreDeTabla = ProcesarTabla.config()[8];
                ConexionAccess.Contrasenia = ProcesarTabla.config()[9];
                DataTable dt = ProcesarTabla.obtenerTabla(ProcesarTabla.config()[0]);

                if (ProcesarTabla.config()[4] != "")
                {
                    dt = ProcesarTabla.filtrarColumnas(dt,
                        ProcesarTabla.config()[1],
                        ProcesarTabla.config()[2],
                        ProcesarTabla.config()[3],
                        ProcesarTabla.config()[4]
                        );

                }
                else
                {
                    dt = ProcesarTabla.filtrarColumnas(dt,
                        ProcesarTabla.config()[1],
                        ProcesarTabla.config()[2],
                        ProcesarTabla.config()[3]
                        );
                }

                bool[] checkBoxes =
                {
                    bool.Parse(ProcesarTabla.config()[5]),
                    bool.Parse(ProcesarTabla.config()[6]),
                    bool.Parse(ProcesarTabla.config()[7])
                };

                if (dt != null)
                {
                    ProcesarTabla.escribirTXT(dt, checkBoxes);
                }
            }
        }
    }
}
