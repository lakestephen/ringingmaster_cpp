using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

namespace RingingMaster
{
    class ModelBuilder
    {

        protected string _input;
        protected string _output;

        public ModelBuilder(string input, string output)
        {
            _input = input;
            _output = output;
        }
        
        internal void run()
        {
            DirectoryInfo inputDir = new DirectoryInfo(_input);
            FileInfo[] files = inputDir.GetFiles("*.xml");
            List<MetaClass> metaClasses = new List<MetaClass>();

            foreach (FileInfo file in files)
            {
                metaClasses.Add(extractMetaData(file));
            }

            generateInfoClassFiles(metaClasses);
            generateInfoListDefsFiles(metaClasses);
            generateInfoManagerBaseFiles(metaClasses);
        }

        private void generateInfoManagerBaseFiles(List<MetaClass> metaClasses)
        {
            StreamWriter infoManagerBaseH = new StreamWriter(_output + "InfoManagerBase.h");
            infoManagerBaseH.WriteLine("#pragma once");
            infoManagerBaseH.WriteLine();
            infoManagerBaseH.WriteLine("#include \"InfoListDefs.h\"");
            infoManagerBaseH.WriteLine();
            infoManagerBaseH.WriteLine("class InfoManagerBase");
            infoManagerBaseH.WriteLine("{");
            infoManagerBaseH.WriteLine("public:");
            infoManagerBaseH.WriteLine("\tInfoManagerBase();");
            infoManagerBaseH.WriteLine("\t~InfoManagerBase();");
            infoManagerBaseH.WriteLine();

            StreamWriter infoManagerBaseC = new StreamWriter(_output + "InfoManagerBase.cpp");
            infoManagerBaseC.WriteLine("#include \"stdafx.h\"");
            infoManagerBaseC.WriteLine("#include \"InfoManagerBase.h\"");
            infoManagerBaseC.WriteLine();
            
              infoManagerBaseC.WriteLine("InfoManagerBase::InfoManagerBase()");
            infoManagerBaseC.WriteLine("{");
            infoManagerBaseC.WriteLine("}");
            infoManagerBaseC.WriteLine();
            infoManagerBaseC.WriteLine("InfoManagerBase::~InfoManagerBase()");
            infoManagerBaseC.WriteLine("{");
            infoManagerBaseC.WriteLine("}");
            infoManagerBaseC.WriteLine();

            foreach (MetaClass metaClass in metaClasses)
            {
                infoManagerBaseH.WriteLine("\t{0}Info* find{0}(long key);", metaClass.ClassName);
                infoManagerBaseC.WriteLine("{0}Info* InfoManagerBase::find{0}(long key)", metaClass.ClassName);
                infoManagerBaseC.WriteLine("{");
                infoManagerBaseC.WriteLine("\treturn _all{0}s.Find(key);", metaClass.ClassName.ToLower());
                infoManagerBaseC.WriteLine("}");
                infoManagerBaseC.WriteLine();

                infoManagerBaseH.WriteLine("\tint get{0}Count();", metaClass.ClassName);
                infoManagerBaseC.WriteLine("int InfoManagerBase::get{0}Count()", metaClass.ClassName);
                infoManagerBaseC.WriteLine("{");
                infoManagerBaseC.WriteLine("\treturn _all{0}s.Count();", metaClass.ClassName.ToLower());
                infoManagerBaseC.WriteLine("}");
                infoManagerBaseC.WriteLine();

            }

            infoManagerBaseH.WriteLine();
            infoManagerBaseH.WriteLine("private:");


            foreach (MetaClass metaClass in metaClasses)
            {
                infoManagerBaseH.WriteLine("\t{0}InfoCollection _all{1}s;", metaClass.ClassName, metaClass.ClassName.ToLower());
            }

            infoManagerBaseH.WriteLine("};");


            infoManagerBaseH.Close();
            infoManagerBaseC.Close();
        }

        private void generateInfoListDefsFiles(List<MetaClass> metaClasses)
        {
            StreamWriter infoListDefs = new StreamWriter(_output + "InfoListDefs.h");
            infoListDefs.WriteLine("#pragma once");
            infoListDefs.WriteLine();
            infoListDefs.WriteLine("#include \"InfoListBase.h\"");
            infoListDefs.WriteLine();

            foreach (MetaClass metaClass in metaClasses)
            {
                infoListDefs.WriteLine("class {0}Info;", metaClass.ClassName);
                infoListDefs.WriteLine("typedef InfoListBase<{0}Info*> {0}InfoCollection;", metaClass.ClassName);
                infoListDefs.WriteLine();
            }

            infoListDefs.Close();
        }

        private MetaClass extractMetaData(FileInfo file)
        {
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.ProhibitDtd = false;
            XmlReader reader = XmlReader.Create(file.FullName, settings);
            XmlDocument doc = new XmlDocument();
            doc.Load(reader);

            MetaClass metaClass = new MetaClass();

            metaClass.ClassName = doc.LastChild.Attributes.GetNamedItem("classname").Value;

            //fields
            XmlNodeList fieldNodes = doc.GetElementsByTagName("field");
            foreach (XmlNode fieldNode in fieldNodes)
            {
                MetaField field = new MetaField();

                foreach (XmlNode node in fieldNode.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "variableName":
                            field.VariableName = node.InnerText;
                            break;
                        case "customType":
                        case "enumName":
                        case "infoName":
                            field.CustomType = node.InnerText;
                            break;
                    }
                }
                field.TypeString = fieldNode.Attributes.GetNamedItem("type").Value;

                metaClass.Add(field);
            }

            return metaClass;
        }

          

        private void generateInfoClassFiles(List<MetaClass> metaClasses)
        {
            foreach (MetaClass metaClass in metaClasses)
            {

                StreamWriter h = new StreamWriter(_output + metaClass.ClassName + "Info.h");
                StreamWriter c = new StreamWriter(_output + metaClass.ClassName + "Info.cpp");

                writeInfoIncludes(metaClass, h, c);
                writeInfoConstructors(metaClass, h, c);
                writeInfoOperators(metaClass, h, c);
                writeInfoSerialize(metaClass, h, c);
                writeInfoGettersSetters(metaClass, h, c);
                writeInfoDataMembers(metaClass, h, c);

                h.WriteLine("};");

                h.Close();
                c.Close();
            }
        }

        private void writeInfoOperators(MetaClass metaClass, StreamWriter h, StreamWriter c)
        {
            //assignment operator
            h.WriteLine("\t{0}Info& operator =(const {0}Info& {1}Info);", metaClass.ClassName, metaClass.ClassName.ToLower());
            c.WriteLine("{0}Info& {0}Info::operator =(const {0}Info& {1}Info)", metaClass.ClassName, metaClass.ClassName.ToLower());
            c.WriteLine("{");
            foreach (MetaField field in metaClass)
            {
                if (field.Type ==MetaField.MetaFieldType.mft_info)
                    c.WriteLine("\t_{0}Key = {1}Info._{0}Key;", field.VariableName.ToLower(), metaClass.ClassName.ToLower());

                c.WriteLine("\t_{0} = {1}Info._{0};", field.VariableName.ToLower(), metaClass.ClassName.ToLower());
            }
            c.WriteLine();
            c.WriteLine("\treturn *this;");
            c.WriteLine("}");
            c.WriteLine();
         
        }

        private void writeInfoDataMembers(MetaClass metaClass, StreamWriter h, StreamWriter c)
        {
            h.WriteLine("private:");
            foreach (MetaField field in metaClass)
            {
                if (field.Type == MetaField.MetaFieldType.mft_info)
                {
                    h.WriteLine("\tlong _{0}Key;",field.VariableName.ToLower());
                    h.WriteLine("\t{0}Info* _{1};", field.CustomType, field.VariableName.ToLower());
                }
                else
                    h.WriteLine("\t{0} _{1};", field.TypeString, field.VariableName.ToLower());
            } 
        }

        private void writeInfoSerialize(MetaClass metaClass, StreamWriter h, StreamWriter c)
        {
            h.WriteLine("public:");
            h.WriteLine("\tvoid Serialize(CArchive &ar);");
            h.WriteLine("");

            c.WriteLine("void {0}Info::Serialize(CArchive &ar)", metaClass.ClassName);
            c.WriteLine("{");

            c.WriteLine("\tint version = 1;"); //If version changes, will need to write the framework to manage this in the data generator
	        c.WriteLine("\tif (ar.IsStoring())");
	        c.WriteLine("\t{");
            c.WriteLine("\t\tar << version;");
            c.WriteLine();
            c.WriteLine("\t\t__super::Serialize(ar);");
            c.WriteLine();

            foreach (MetaField field in metaClass)
            {
                switch (field.Type)
                {
                    case MetaField.MetaFieldType.mft_custom:
                        c.WriteLine("\t\t_{0}.Serialize(ar);", field.VariableName.ToLower());
                        break;
                    case MetaField.MetaFieldType.mft_info:
                        c.WriteLine("\t\tar << _{0}Key;", field.VariableName.ToLower());
                        break;
                    default:
                        c.WriteLine("\t\tar << _{0};", field.VariableName.ToLower());
                        break;
                }

            }

            c.WriteLine("\t}");
        	c.WriteLine("\telse");
	        c.WriteLine("\t{");
            c.WriteLine("\t\tint thisVersion;");
		    c.WriteLine("\t\tar >> thisVersion;");
            c.WriteLine("\t\tASSERT(thisVersion == version);");
            c.WriteLine("\t\tif (thisVersion != version)");
            c.WriteLine("\t\t\tAfxThrowArchiveException(CArchiveException::badSchema);");
            c.WriteLine();
            c.WriteLine("\t\t__super::Serialize(ar);");
            c.WriteLine();


            foreach (MetaField field in metaClass)
            {
                switch (field.Type)
                {
                    case MetaField.MetaFieldType.mft_custom:
                        c.WriteLine("\t\t_{0}.Serialize(ar);", field.VariableName.ToLower());
                        break;
                    case MetaField.MetaFieldType.mft_enum:
                        c.WriteLine("\t\tar >> (int&)_{0};", field.VariableName.ToLower());
                        break;
                    case MetaField.MetaFieldType.mft_info:
                        c.WriteLine("\t\tar >> _{0}Key;", field.VariableName.ToLower());
                        break;
                    default:
                        c.WriteLine("\t\tar >> _{0};", field.VariableName.ToLower());
                        break;
                }

            }

            c.WriteLine("\t}");

            c.WriteLine("}");
            c.WriteLine();

        }

        private void writeInfoIncludes(MetaClass metaClass, StreamWriter h, StreamWriter c)
        {
            h.WriteLine("#pragma once");
            h.WriteLine();
            h.WriteLine("#include \"SaveableObject.h\"");
            h.WriteLine("#include \"InfoListDefs.h\"");

            c.WriteLine("#include \"stdafx.h\"");
            c.WriteLine("#include \"{0}Info.h\"", metaClass.ClassName);
            c.WriteLine();

            foreach (MetaField field in metaClass)
            {
                switch (field.Type)
                {
                    case MetaField.MetaFieldType.mft_custom:
                        h.WriteLine("#include \"{0}.h\"", field.TypeString);
                        break;
                }
            }

            h.WriteLine();
            
            foreach (MetaField field in metaClass)
            {
                switch (field.Type)
                {
                    case MetaField.MetaFieldType.mft_enum:
                        h.WriteLine("enum {0};", field.TypeString);
                        break;
                }
            }

            h.WriteLine();
            h.WriteLine("class {0}Info : SaveableObject", metaClass.ClassName);
            h.WriteLine("{");

            c.WriteLine();
            c.WriteLine();
        }

        private void writeInfoConstructors(MetaClass metaClass, StreamWriter h, StreamWriter c)
        {  
            //main constructor
            h.WriteLine("public:");
            h.WriteLine("\t{0}Info(long key);", metaClass.ClassName);
            c.WriteLine("{0}Info::{0}Info(long key) :", metaClass.ClassName);
            c.WriteLine("SaveableObject(key)");
            foreach (MetaField field in metaClass)
            {
                if (field.Type == MetaField.MetaFieldType.mft_info)
                    c.WriteLine(", _{0}(0)", field.VariableName.ToLower());
            }
            c.WriteLine("{");
            c.WriteLine("}");
            c.WriteLine();

            //archive constructor
            h.WriteLine("\t{0}Info(CArchive& ar);", metaClass.ClassName);
            c.WriteLine("{0}Info::{0}Info(CArchive& ar) :", metaClass.ClassName);
            c.WriteLine("SaveableObject(-1)");
            foreach (MetaField field in metaClass)
            {
                if (field.Type == MetaField.MetaFieldType.mft_info)
                    c.WriteLine(", _{0}(0)", field.VariableName.ToLower());
            }
            c.WriteLine("{");
            c.WriteLine("\tASSERT(!ar.IsStoring());");
            c.WriteLine("\tSerialize(ar);"); 
            c.WriteLine("\tASSERT(getKey() != -1);");
            c.WriteLine("}");
            c.WriteLine();

            //copy constructor
            h.WriteLine("\t{0}Info(const {0}Info& {1}Info);", metaClass.ClassName, metaClass.ClassName.ToLower());
            c.WriteLine("{0}Info::{0}Info(const {0}Info& {1}Info) : ", metaClass.ClassName, metaClass.ClassName.ToLower());
            c.WriteLine("SaveableObject({0}Info)", metaClass.ClassName.ToLower());
            foreach (MetaField field in metaClass)
            {
                if (field.Type == MetaField.MetaFieldType.mft_info)
                    c.WriteLine(", _{0}(0)", field.VariableName.ToLower());
            }
            c.WriteLine("{");
            c.WriteLine("\toperator = ({0}Info);", metaClass.ClassName.ToLower());
            c.WriteLine("}");
            c.WriteLine();
            
            //destructor
            h.WriteLine("\t~{0}Info();", metaClass.ClassName);
            h.WriteLine();
            c.WriteLine("{0}Info::~{0}Info()", metaClass.ClassName);
            c.WriteLine("{");
            c.WriteLine("}");
            c.WriteLine();

        }

        private void writeInfoGettersSetters(MetaClass metaClass, StreamWriter h, StreamWriter c)
        {
            h.WriteLine("public:"); 

            foreach (MetaField field in metaClass)
            {
                if (field.Type == MetaField.MetaFieldType.mft_info)
                {
                    h.WriteLine("\t{0}Info* get{1}();", field.CustomType, field.VariableName);

                    c.WriteLine("{0}Info* {1}Info::get{2}()", field.CustomType, metaClass.ClassName, field.VariableName);
                    c.WriteLine("{");
                    c.WriteLine("\treturn _{0};", field.VariableName.ToLower());
                    c.WriteLine("}");

                    h.WriteLine("\tlong get{0}Key();", field.VariableName);
                    c.WriteLine("long {0}Info::get{1}Key()", metaClass.ClassName, field.VariableName);
                    c.WriteLine("{");
                    c.WriteLine("\treturn _{0}Key;", field.VariableName.ToLower());
                    c.WriteLine("}");
                }
                else
                {
                    h.WriteLine("\t{0} get{1}();", field.TypeString, field.VariableName);
                    c.WriteLine("{0} {1}Info::get{2}()", field.TypeString, metaClass.ClassName, field.VariableName);
                    c.WriteLine("{");   
                    c.WriteLine("\treturn _{0};",field.VariableName.ToLower() );
                    c.WriteLine("}");
                }

                if (field.Type == MetaField.MetaFieldType.mft_info)
                {
                    h.WriteLine("\tvoid set{0}({1}Info* {2});", field.VariableName, field.CustomType, field.VariableName.ToLower());
                    c.WriteLine("void {0}Info::set{1}({2}Info* {3})", metaClass.ClassName, field.VariableName, field.CustomType, field.VariableName.ToLower());
                    c.WriteLine("{");
                    c.WriteLine("\t_{0} = {0};", field.VariableName.ToLower());
                    c.WriteLine("}");

                    h.WriteLine("\tvoid set{0}Key(long {1}Key);", field.VariableName, field.VariableName.ToLower());
                    c.WriteLine("void {0}Info::set{1}Key(long {2}Key)", metaClass.ClassName, field.VariableName, field.VariableName.ToLower());
                    c.WriteLine("{");
                    c.WriteLine("\t_{0}Key = {0}Key;", field.VariableName.ToLower());
                    c.WriteLine("}");
                }
                else
                {
                    h.WriteLine("\tvoid set{0}({1} {2});", field.VariableName, field.TypeString, field.VariableName.ToLower());
                    c.WriteLine("void {0}Info::set{1}({2} {3})", metaClass.ClassName, field.VariableName, field.TypeString, field.VariableName.ToLower());
                    c.WriteLine("{");
                    c.WriteLine("\t_{0} = {0};", field.VariableName.ToLower());
                    c.WriteLine("}");
                }

                h.WriteLine();
                c.WriteLine();
            }
            
            h.WriteLine();
        }
    
    }
}
