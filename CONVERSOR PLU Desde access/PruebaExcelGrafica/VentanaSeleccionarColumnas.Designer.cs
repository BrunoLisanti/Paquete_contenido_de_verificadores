namespace PruebaExcelGrafica
{
    partial class VentanaSeleccionarColumnas
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(VentanaSeleccionarColumnas));
            this.lblSeleccionarID = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnGenerarTxt = new System.Windows.Forms.Button();
            this.numericID = new System.Windows.Forms.NumericUpDown();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.numericDescripcion = new System.Windows.Forms.NumericUpDown();
            this.numericPrecio = new System.Windows.Forms.NumericUpDown();
            this.btnSeleccionar = new System.Windows.Forms.Button();
            this.dgvColumnas = new System.Windows.Forms.DataGridView();
            this.lblID = new System.Windows.Forms.Label();
            this.lblDescripcion = new System.Windows.Forms.Label();
            this.lblPrecio = new System.Windows.Forms.Label();
            this.chkID = new System.Windows.Forms.CheckBox();
            this.chkPrecio = new System.Windows.Forms.CheckBox();
            this.chkDescripcion = new System.Windows.Forms.CheckBox();
            this.btnSeleccionarAccess = new System.Windows.Forms.Button();
            this.ddlTablas = new System.Windows.Forms.ComboBox();
            this.lblSeleccion = new System.Windows.Forms.Label();
            this.lblOferta = new System.Windows.Forms.Label();
            this.numericOferta = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numericID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericDescripcion)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericPrecio)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvColumnas)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericOferta)).BeginInit();
            this.SuspendLayout();
            // 
            // lblSeleccionarID
            // 
            this.lblSeleccionarID.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblSeleccionarID.AutoSize = true;
            this.lblSeleccionarID.Location = new System.Drawing.Point(145, 314);
            this.lblSeleccionarID.Name = "lblSeleccionarID";
            this.lblSeleccionarID.Size = new System.Drawing.Size(129, 16);
            this.lblSeleccionarID.TabIndex = 1;
            this.lblSeleccionarID.Text = "Seleccionar Codigo:";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(145, 365);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(160, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "Seleccionar Descripcion: ";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(145, 417);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(124, 16);
            this.label3.TabIndex = 4;
            this.label3.Text = "Seleccionar Precio:";
            // 
            // btnGenerarTxt
            // 
            this.btnGenerarTxt.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnGenerarTxt.BackColor = System.Drawing.SystemColors.MenuBar;
            this.btnGenerarTxt.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnGenerarTxt.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnGenerarTxt.Location = new System.Drawing.Point(404, 520);
            this.btnGenerarTxt.Name = "btnGenerarTxt";
            this.btnGenerarTxt.Size = new System.Drawing.Size(169, 35);
            this.btnGenerarTxt.TabIndex = 7;
            this.btnGenerarTxt.Text = "Guardar configuracion";
            this.btnGenerarTxt.UseVisualStyleBackColor = false;
            this.btnGenerarTxt.Click += new System.EventHandler(this.btnGenerarTxt_Click);
            // 
            // numericID
            // 
            this.numericID.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.numericID.ContextMenuStrip = this.contextMenuStrip1;
            this.numericID.Cursor = System.Windows.Forms.Cursors.Hand;
            this.numericID.Location = new System.Drawing.Point(667, 312);
            this.numericID.Name = "numericID";
            this.numericID.Size = new System.Drawing.Size(50, 22);
            this.numericID.TabIndex = 17;
            this.numericID.ValueChanged += new System.EventHandler(this.numericID_ValueChanged);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // numericDescripcion
            // 
            this.numericDescripcion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.numericDescripcion.Cursor = System.Windows.Forms.Cursors.Hand;
            this.numericDescripcion.Location = new System.Drawing.Point(667, 363);
            this.numericDescripcion.Name = "numericDescripcion";
            this.numericDescripcion.Size = new System.Drawing.Size(50, 22);
            this.numericDescripcion.TabIndex = 18;
            this.numericDescripcion.ValueChanged += new System.EventHandler(this.numericDescripcion_ValueChanged);
            // 
            // numericPrecio
            // 
            this.numericPrecio.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.numericPrecio.Cursor = System.Windows.Forms.Cursors.Hand;
            this.numericPrecio.Location = new System.Drawing.Point(667, 415);
            this.numericPrecio.Name = "numericPrecio";
            this.numericPrecio.Size = new System.Drawing.Size(50, 22);
            this.numericPrecio.TabIndex = 19;
            this.numericPrecio.ValueChanged += new System.EventHandler(this.numericPrecio_ValueChanged);
            // 
            // btnSeleccionar
            // 
            this.btnSeleccionar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSeleccionar.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSeleccionar.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnSeleccionar.Location = new System.Drawing.Point(12, 303);
            this.btnSeleccionar.Name = "btnSeleccionar";
            this.btnSeleccionar.Size = new System.Drawing.Size(107, 52);
            this.btnSeleccionar.TabIndex = 20;
            this.btnSeleccionar.Text = "Seleccionar Excel";
            this.btnSeleccionar.UseVisualStyleBackColor = true;
            this.btnSeleccionar.Click += new System.EventHandler(this.btnSeleccionar_Click);
            // 
            // dgvColumnas
            // 
            this.dgvColumnas.AllowUserToAddRows = false;
            this.dgvColumnas.AllowUserToDeleteRows = false;
            this.dgvColumnas.AllowUserToResizeColumns = false;
            this.dgvColumnas.AllowUserToResizeRows = false;
            this.dgvColumnas.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvColumnas.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvColumnas.Location = new System.Drawing.Point(12, 12);
            this.dgvColumnas.MultiSelect = false;
            this.dgvColumnas.Name = "dgvColumnas";
            this.dgvColumnas.ReadOnly = true;
            this.dgvColumnas.RowHeadersWidth = 51;
            this.dgvColumnas.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.White;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.Color.Black;
            this.dgvColumnas.RowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dgvColumnas.RowTemplate.Height = 24;
            this.dgvColumnas.Size = new System.Drawing.Size(964, 285);
            this.dgvColumnas.TabIndex = 21;
            this.dgvColumnas.SelectionChanged += new System.EventHandler(this.dgvColumnas_SelectionChanged);
            // 
            // lblID
            // 
            this.lblID.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.lblID.AutoEllipsis = true;
            this.lblID.AutoSize = true;
            this.lblID.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblID.Location = new System.Drawing.Point(387, 314);
            this.lblID.Name = "lblID";
            this.lblID.Size = new System.Drawing.Size(12, 16);
            this.lblID.TabIndex = 23;
            this.lblID.Text = "-";
            // 
            // lblDescripcion
            // 
            this.lblDescripcion.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.lblDescripcion.AutoEllipsis = true;
            this.lblDescripcion.AutoSize = true;
            this.lblDescripcion.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDescripcion.Location = new System.Drawing.Point(387, 365);
            this.lblDescripcion.Name = "lblDescripcion";
            this.lblDescripcion.Size = new System.Drawing.Size(12, 16);
            this.lblDescripcion.TabIndex = 24;
            this.lblDescripcion.Text = "-";
            // 
            // lblPrecio
            // 
            this.lblPrecio.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.lblPrecio.AutoEllipsis = true;
            this.lblPrecio.AutoSize = true;
            this.lblPrecio.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPrecio.Location = new System.Drawing.Point(387, 417);
            this.lblPrecio.Name = "lblPrecio";
            this.lblPrecio.Size = new System.Drawing.Size(12, 16);
            this.lblPrecio.TabIndex = 25;
            this.lblPrecio.Text = "-";
            // 
            // chkID
            // 
            this.chkID.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.chkID.Location = new System.Drawing.Point(791, 319);
            this.chkID.Name = "chkID";
            this.chkID.Size = new System.Drawing.Size(138, 27);
            this.chkID.TabIndex = 26;
            this.chkID.Text = "Ignorar Codigo 0";
            this.chkID.UseVisualStyleBackColor = true;
            // 
            // chkPrecio
            // 
            this.chkPrecio.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.chkPrecio.Location = new System.Drawing.Point(791, 374);
            this.chkPrecio.Name = "chkPrecio";
            this.chkPrecio.Size = new System.Drawing.Size(177, 28);
            this.chkPrecio.TabIndex = 27;
            this.chkPrecio.Text = "Ignorar Precio 0";
            this.chkPrecio.UseVisualStyleBackColor = true;
            // 
            // chkDescripcion
            // 
            this.chkDescripcion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.chkDescripcion.Location = new System.Drawing.Point(791, 408);
            this.chkDescripcion.Name = "chkDescripcion";
            this.chkDescripcion.Size = new System.Drawing.Size(262, 70);
            this.chkDescripcion.TabIndex = 28;
            this.chkDescripcion.Text = "Limitar largo de descripcion\r\n a 40 caracteres\r\n ";
            this.chkDescripcion.UseVisualStyleBackColor = true;
            // 
            // btnSeleccionarAccess
            // 
            this.btnSeleccionarAccess.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSeleccionarAccess.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSeleccionarAccess.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btnSeleccionarAccess.Location = new System.Drawing.Point(12, 372);
            this.btnSeleccionarAccess.Name = "btnSeleccionarAccess";
            this.btnSeleccionarAccess.Size = new System.Drawing.Size(107, 52);
            this.btnSeleccionarAccess.TabIndex = 29;
            this.btnSeleccionarAccess.Text = "Seleccionar Access";
            this.btnSeleccionarAccess.UseVisualStyleBackColor = true;
            this.btnSeleccionarAccess.Click += new System.EventHandler(this.btnSeleccionarAccess_Click);
            // 
            // ddlTablas
            // 
            this.ddlTablas.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.ddlTablas.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ddlTablas.FormattingEnabled = true;
            this.ddlTablas.Location = new System.Drawing.Point(12, 529);
            this.ddlTablas.Name = "ddlTablas";
            this.ddlTablas.Size = new System.Drawing.Size(208, 24);
            this.ddlTablas.TabIndex = 30;
            this.ddlTablas.Visible = false;
            this.ddlTablas.SelectedIndexChanged += new System.EventHandler(this.ddlTablas_SelectedIndexChanged);
            // 
            // lblSeleccion
            // 
            this.lblSeleccion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblSeleccion.AutoSize = true;
            this.lblSeleccion.Location = new System.Drawing.Point(13, 507);
            this.lblSeleccion.Name = "lblSeleccion";
            this.lblSeleccion.Size = new System.Drawing.Size(136, 16);
            this.lblSeleccion.TabIndex = 31;
            this.lblSeleccion.Text = "Seleccione una tabla:";
            this.lblSeleccion.Visible = false;
            // 
            // lblOferta
            // 
            this.lblOferta.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.lblOferta.AutoEllipsis = true;
            this.lblOferta.AutoSize = true;
            this.lblOferta.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblOferta.Location = new System.Drawing.Point(387, 468);
            this.lblOferta.Name = "lblOferta";
            this.lblOferta.Size = new System.Drawing.Size(12, 16);
            this.lblOferta.TabIndex = 34;
            this.lblOferta.Text = "-";
            // 
            // numericOferta
            // 
            this.numericOferta.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.numericOferta.Cursor = System.Windows.Forms.Cursors.Hand;
            this.numericOferta.Location = new System.Drawing.Point(667, 466);
            this.numericOferta.Name = "numericOferta";
            this.numericOferta.Size = new System.Drawing.Size(50, 22);
            this.numericOferta.TabIndex = 33;
            this.numericOferta.ValueChanged += new System.EventHandler(this.numericOferta_ValueChanged);
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(145, 468);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(121, 16);
            this.label4.TabIndex = 32;
            this.label4.Text = "Seleccionar Oferta:";
            // 
            // VentanaSeleccionarColumnas
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(988, 576);
            this.Controls.Add(this.lblOferta);
            this.Controls.Add(this.numericOferta);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.lblSeleccion);
            this.Controls.Add(this.ddlTablas);
            this.Controls.Add(this.btnSeleccionarAccess);
            this.Controls.Add(this.chkDescripcion);
            this.Controls.Add(this.chkPrecio);
            this.Controls.Add(this.chkID);
            this.Controls.Add(this.lblPrecio);
            this.Controls.Add(this.lblDescripcion);
            this.Controls.Add(this.lblID);
            this.Controls.Add(this.dgvColumnas);
            this.Controls.Add(this.btnSeleccionar);
            this.Controls.Add(this.numericPrecio);
            this.Controls.Add(this.numericDescripcion);
            this.Controls.Add(this.numericID);
            this.Controls.Add(this.btnGenerarTxt);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lblSeleccionarID);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(860, 623);
            this.Name = "VentanaSeleccionarColumnas";
            this.Text = "Conversor a PLU - Configuración";
            ((System.ComponentModel.ISupportInitialize)(this.numericID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericDescripcion)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericPrecio)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvColumnas)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericOferta)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label lblSeleccionarID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnGenerarTxt;
        private System.Windows.Forms.NumericUpDown numericID;
        private System.Windows.Forms.NumericUpDown numericDescripcion;
        private System.Windows.Forms.NumericUpDown numericPrecio;
        private System.Windows.Forms.Button btnSeleccionar;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.DataGridView dgvColumnas;
        private System.Windows.Forms.Label lblID;
        private System.Windows.Forms.Label lblDescripcion;
        private System.Windows.Forms.Label lblPrecio;
        private System.Windows.Forms.CheckBox chkID;
        private System.Windows.Forms.CheckBox chkPrecio;
        private System.Windows.Forms.CheckBox chkDescripcion;
        private System.Windows.Forms.Button btnSeleccionarAccess;
        private System.Windows.Forms.ComboBox ddlTablas;
        private System.Windows.Forms.Label lblSeleccion;
        private System.Windows.Forms.Label lblOferta;
        private System.Windows.Forms.NumericUpDown numericOferta;
        private System.Windows.Forms.Label label4;
    }
}

