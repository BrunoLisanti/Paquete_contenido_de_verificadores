using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.ComponentModel;
using System.IO;
using OfficeOpenXml;
using OfficeOpenXml.Style;
using OfficeOpenXml.FormulaParsing.ExpressionGraph;
using OfficeOpenXml.Export.ToDataTable;
using System.Windows;
using System.Threading;
using System.Data.Common;
using System.Collections;
using OfficeOpenXml.FormulaParsing;
using System.Windows.Forms;
using System.Runtime.Remoting.Contexts;
using System.Data.OleDb;

namespace PruebaExcelGrafica
{
    internal class ConexionExcel
    { 

        static public DataTable obtenerTabla(string nombreArchivo, int numeroHoja)
        {
            ExcelPackage.LicenseContext = OfficeOpenXml.LicenseContext.NonCommercial;

            using (var ep = new ExcelPackage(nombreArchivo))
            {
                var theWorkbook = ep.Workbook;
                var theSheet = theWorkbook.Worksheets[numeroHoja];

                int colCount = 1;
                int rowCount = theSheet.Dimension.End.Row; //cantidad de filas del documento
                ArrayList columnas = new ArrayList();

                while (theSheet.Cells[1,colCount].Value != null) //recorre las celdas de la fila 1 que tengan datos.
                {
                    columnas.Add((theSheet.Cells[1,colCount].Value).ToString()); //agrega el nombre de la columna a una lista
                    colCount++; //cuenta la cantidad de columnas
                }
                colCount--;
                
                DataTable dt = new DataTable(); //crea la tabla que se retorna

                foreach(string columna in columnas) 
                {
                    dt.Columns.Add(columna, typeof(string)); //se agregan las columnas,todas de tipo string
                }

                for (int i = 2; i <= rowCount; i++)
                {
                    DataRow dr = dt.NewRow();
                    for (int j = 1; j <= colCount; j++) //se agregan las celdas del rango a la datatable
                    {
                        try
                        { 
                            dr[j - 1] = theSheet.Cells[i, j].Value;
                        }
                        catch(Exception)
                        {
                            
                        }
                    }
                    dt.Rows.Add(dr);
                }
                return dt;
            }
        }
    }
}
