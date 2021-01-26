using System;
using System.Collections.Generic;
using System.Text;

namespace RingingMaster
{
    class MetaField
    {

        public enum MetaFieldType
        {
            mft_bool,
            mft_long,
            mft_double,
            mft_string,
            mft_date,
            mft_time,
            mft_dateTime,
            mft_custom,
            mft_enum,
            mft_info,
        };

        private MetaFieldType _type;
        public MetaFieldType Type
        {
            get { return _type; }
        }

        private string _stringType;
        public string TypeString
        {
            get
            {
                switch (_type)
                {
                    case MetaFieldType.mft_custom:
                    case MetaFieldType.mft_enum:
                        return _customType;
                    default:
                        return _stringType;
                }
            }
            set { 
                _stringType = value;

                switch (value)
                {
                    case "BOOL":
                        _type = MetaFieldType.mft_bool;
                        break;
                    case "long":
                        _type = MetaFieldType.mft_long;
                        break;
                    case "double":
                        _type = MetaFieldType.mft_double;
                        break;
                    case "CString":
                        _type = MetaFieldType.mft_string;
                        break;
                    case "date":
                        _type = MetaFieldType.mft_date;
                        break;
                    case "time":
                        _type = MetaFieldType.mft_time;
                        break;
                    case "dateTime":
                        _type = MetaFieldType.mft_dateTime;
                        break;
                    case "custom":
                        _type = MetaFieldType.mft_custom;
                        break;
                    case "enum":
                        _type = MetaFieldType.mft_enum;
                        break;
                    case "Info":
                        _type = MetaFieldType.mft_info;
                        break;

                    default:
                        break;
                }
            }
        }

        private string _variableName;
        public string VariableName
        {
            get { return _variableName; }
            set { _variableName = value; }
        }

        private string _customType;
        public string CustomType
        {
            get { return _customType; }
            set { _customType = value; }
        }

        public MetaField()
        {

        }
    }
}
