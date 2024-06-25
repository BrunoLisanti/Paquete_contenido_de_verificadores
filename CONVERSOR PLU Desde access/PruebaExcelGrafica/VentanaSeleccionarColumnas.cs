using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace PruebaExcelGrafica
{
    public partial class VentanaSeleccionarColumnas : Form
    {
        public VentanaSeleccionarColumnas()
        {
            InitializeComponent();
        }

        private void cargarListasDeColumnas() //Llena la gridview con la tabla
        {
            try
            {
                dgvColumnas.DataSource = ProcesarTabla.obtenerTabla(ProcesarTabla.Path);

                foreach (DataGridViewColumn column in dgvColumnas.Columns)
                {
                    column.SortMode = DataGridViewColumnSortMode.NotSortable; //les quita la posibilidad de ordenar la columna
                }
            }
            catch (InvalidOperationException ioEx)
            {
                MessageBox.Show(ioEx.Message);
                MessageBox.Show(ioEx.HelpLink);
            }
        }

        private void cargarDdlTablas() //Llena el ddl con las tablas de la base de datos elegida
        {
            foreach(DataRow dr in ConexionAccess.obtenerNombres(ProcesarTabla.Path).Rows)
            {
                ddlTablas.Items.Add(dr["TABLE_NAME"]);
            }
        }


        private void btnGenerarTxt_Click(object sender, EventArgs e) //Pasa por todas las validaciones necesarias y al final si todo es correcto guarda los datos de configuracion
        {
            if (!(numericID.Value > 0 && numericDescripcion.Value > 0 && numericPrecio.Value > 0))
            {
                MessageBox.Show("Hay columnas sin seleccionar", "Error");
                return;
            }                   
            if (!(numericID.Value != numericDescripcion.Value && numericID.Value != numericPrecio.Value && numericPrecio.Value != numericDescripcion.Value && numericOferta.Value != numericID.Value && numericOferta.Value != numericDescripcion.Value && numericOferta.Value != numericPrecio.Value))
            {
                MessageBox.Show("Se eligieron columnas repetidas", "Error");
                return;
            }
            try
            {
                DataTable dt = ProcesarTabla.obtenerTabla(ProcesarTabla.Path); //Guarda la tabla en una datatable

                if (numericOferta.Value == 0)
                {
                    dt = ProcesarTabla.filtrarColumnas(dt,
                        dt.Columns[Convert.ToInt32(numericID.Value - 1)].ColumnName,
                        dt.Columns[Convert.ToInt32(numericDescripcion.Value - 1)].ColumnName,
                        dt.Columns[Convert.ToInt32(numericPrecio.Value - 1)].ColumnName
                        ); //filtra la tabla para dejarla solo con las columnas elegidas
                }
                else
                {
                    dt = ProcesarTabla.filtrarColumnas(dt,
                        dt.Columns[Convert.ToInt32(numericID.Value - 1)].ColumnName,
                        dt.Columns[Convert.ToInt32(numericDescripcion.Value - 1)].ColumnName,
                        dt.Columns[Convert.ToInt32(numericPrecio.Value - 1)].ColumnName,
                        dt.Columns[Convert.ToInt32(numericOferta.Value - 1)].ColumnName
                        ); //filtra la tabla para dejarla solo con las columnas elegidas incluida la oferta
                }



                DataView view = new DataView(dt);

                //Acá se consulta si existen codigos de barra repetidos.

                List<string> listaFilas = new List<string>();

                foreach (DataRow dr in view.ToTable(false, dt.Columns[0].ColumnName).Rows) //Se cuenta la cantidad de filas original
                {
                    listaFilas.Add(dr[0].ToString());
                }

                var seen = new HashSet<string>();
                var duplicados = new List<string>();

                int i = 2;

                using (StreamWriter writer = new StreamWriter($"TxtGenerados/errores.txt"))
                {
                    foreach (var number in listaFilas) //Se guardan los codigos en un hashset
                    {
                        if (chkID.Checked)
                        {
                            if (!string.IsNullOrEmpty(number) && number != "0")
                            {
                                if (!seen.Add(number)) //Un hashset no puede contener elementos repetidos.
                                {
                                    writer.WriteLine("SE REPITE: " + number + " EN LA FILA: " + (i - 1)); //Se escribe un log con los registros repetidos
                                }  
                            }
                        }
                        else
                        {
                            if (!string.IsNullOrEmpty(number))
                            {
                                if (!seen.Add(number)) // only care about numbers that appear multiple times
                                {
                                    writer.WriteLine("SE REPITE: " + number + " EN LA FILA: " + (i - 1));
                                }
                            }
                        }
                        i++;
                    }
                }

                if (chkID.Checked)
                {
                    if (seen.Count(c => c != "" && c != "#REF!" && c != "0") != listaFilas.Count(c => c != "" && c != "#REF!" && c != "0"))
                    {
                        MessageBox.Show("Hay registros con ID repetido", "Aviso");
                    }
                }
                else
                {
                    if (seen.Count(c => c != "" && c != "#REF!") != listaFilas.Count(c => c != "" && c != "#REF!"))
                    {
                        MessageBox.Show("Hay registros con ID repetido", "Aviso");
                    }
                }

                if (dt != null)
                {
                    //Se escriben las configuraciones.
                    using (StreamWriter writer = new StreamWriter($"TxtGenerados/config.txt"))
                    {
                        writer.Write($"|PATH={ProcesarTabla.Path}");
                        writer.Write($"|ID={dt.Columns[0].ColumnName}");
                        writer.Write($"|DESCRIPCION={dt.Columns[1].ColumnName}");
                        writer.Write($"|PRECIO={dt.Columns[2].ColumnName}");
                        if (numericOferta.Value != 0)
                            writer.Write($"|OFERTA={dt.Columns[3].ColumnName}");
                        else
                            writer.Write($"|OFERTA=");
                        writer.Write($"|IGNORARID0={chkID.Checked}");
                        writer.Write($"|IGNORARPRECIO0={chkPrecio.Checked}");
                        writer.Write($"|LIMITARDESCRIPCION={chkDescripcion.Checked}");
                        writer.Write($"|NOMBRETABLA={ddlTablas.Text}");
                        writer.Write($"|CONTRASENIA={ConexionAccess.Contrasenia}");
                        writer.Write($"|CODACEPTALETRAS=True");
                        writer.Write($"|LETRASENMINUSCULA=True");
                        MessageBox.Show("Se guardaron las configuraciones correctamente", "Tarea exitosa");
                    }
                }
            }
            catch (ArgumentException)
            {
                MessageBox.Show("No seleccionó un archivo.", "Error");
            }
            catch (IndexOutOfRangeException)
            {
                MessageBox.Show("Las columnas seleccionadas no existen", "Error");
            }
        }

        private void btnSeleccionar_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog(); //Se abre un cuadro de dialogo para elegir el archivo de excel

            openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "Excel sheets (*.xlsx)|*.xlsx";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                ProcesarTabla.Path = openFileDialog1.FileName; //La direccion del archivo elegido se guarda en una variable global
                cargarListasDeColumnas();
                ddlTablas.Items.Clear();
                ddlTablas.Visible = false;
                lblSeleccion.Visible = false;
                numericID.Value = 0;
                numericDescripcion.Value = 0;
                numericPrecio.Value = 0;
                numericOferta.Value = 0;
            }
        }

        private void numericID_ValueChanged(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dgvColumnas.DataSource;
            try
            {
                lblID.Text = dt.Columns[(int)numericID.Value - 1].ColumnName; //Que se muestre el nombre de la columna en un label
            }
            catch(Exception)
            {
                lblID.Text = "-";
            }
        }

        private void numericDescripcion_ValueChanged(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dgvColumnas.DataSource;
            try
            {
                lblDescripcion.Text = dt.Columns[(int)numericDescripcion.Value - 1].ColumnName; //Que se muestre el nombre de la columna en un label
            }
            catch (Exception)
            {
                lblDescripcion.Text = "-";
            }
        }

        private void numericPrecio_ValueChanged(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dgvColumnas.DataSource;
            try
            {
                lblPrecio.Text = dt.Columns[(int)numericPrecio.Value - 1].ColumnName; //Que se muestre el nombre de la columna en un label
            }
            catch (Exception)
            {
                lblPrecio.Text = "-";
            }
        }
        private void numericOferta_ValueChanged(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dgvColumnas.DataSource;
            try
            {
                lblOferta.Text = dt.Columns[(int)numericOferta.Value - 1].ColumnName;
            }
            catch(Exception)
            {
                lblOferta.Text = "-";
            }

        }

        private void dgvColumnas_SelectionChanged(object sender, EventArgs e)
        {
            dgvColumnas.ClearSelection(); //que no se pueda seleccionar una celda de la gridview.
        }

        private void btnSeleccionarAccess_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.InitialDirectory = "c:\\"; //Se selecciona un archivo de Access mediante un cuadro de dialogo
            openFileDialog1.Filter = "Access file|*.accdb;*.mdb";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                ProcesarTabla.Path = openFileDialog1.FileName;

                ddlTablas.Items.Clear();
                dgvColumnas.DataSource = null;
                bool contraseniaIngresada = false;
                while (true)
                {
                    try
                    {
                        cargarDdlTablas(); //Se intenta cargar la gridview con los datos de la bd, si requieriese contraseña,
                        ddlTablas.Visible = true; //generará una excepción.
                        lblSeleccion.Visible = true;
                        numericID.Value = 0;
                        numericDescripcion.Value = 0;
                        numericPrecio.Value = 0;
                        numericOferta.Value = 0;
                    }
                    catch (Exception ex)
                    {
                        if (contraseniaIngresada)
                        {
                            MessageBox.Show("Contraseña incorrecta" + ex.Message, "Error");
                        }
                        using (ContraseñaForm cForm = new ContraseñaForm()) //Se abre un formulario para ingresar la contraseña.
                        {
                            if (cForm.ShowDialog() == DialogResult.OK)
                            {
                                ConexionAccess.Contrasenia = cForm.Contrasenia;
                                contraseniaIngresada = true;
                            }
                            else
                                break;
                        }
                        continue;
                    }
                    break;
                }
            }
        }

        private void ddlTablas_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ddlTablas.Text != "")
            {
                ConexionAccess.NombreDeTabla = ddlTablas.Text;
                cargarListasDeColumnas();
            }
        }

    }
}
