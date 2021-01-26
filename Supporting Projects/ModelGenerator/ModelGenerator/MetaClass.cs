using System;
using System.Collections.Generic;
using System.Text;

namespace RingingMaster
{
    class MetaClass : List<MetaField>
    {
        public MetaClass()
        {
        
        }

        private string _className;
        public string ClassName
        {
            get { return _className; }
            set { _className = value; }
        }

    }
}
