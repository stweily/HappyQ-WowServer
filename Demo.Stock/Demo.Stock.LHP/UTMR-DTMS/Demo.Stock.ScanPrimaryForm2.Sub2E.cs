﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Demo.Stock.LHP
{
    public partial class ScanPrimary2ControlSub2E : UserControl
    {
        ScanPrimary2ControlSub2ESub1 m_ScanControlSub2A = null;
        ScanPrimary2ControlSub2ESub2 m_ScanControlSub2B = null;
        ScanPrimary2ControlSub2ESub3 m_ScanControlSub2C = null;

        public ScanPrimary2ControlSub2E()
        {
            InitializeComponent();
            Initialize();
        }

        private void Initialize()
        {
            m_ScanControlSub2A = new ScanPrimary2ControlSub2ESub1();
            m_ScanControlSub2A.Parent = this.axTabControl;

            this.axTabControl.InsertItem( 0, "扫描设置A", m_ScanControlSub2A.Handle.ToInt32(), 0 );

            m_ScanControlSub2B = new ScanPrimary2ControlSub2ESub2();
            m_ScanControlSub2B.Parent = this.axTabControl;

            this.axTabControl.InsertItem( 1, "扫描设置B", m_ScanControlSub2B.Handle.ToInt32(), 0 );


            m_ScanControlSub2C = new ScanPrimary2ControlSub2ESub3();
            m_ScanControlSub2C.Parent = this.axTabControl;
            this.axTabControl.InsertItem( 2, "扫描设置C", m_ScanControlSub2C.Handle.ToInt32(), 0 );
        }

    }
}
