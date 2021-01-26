using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using RingingMaster;

namespace RingignMaster
{
    class ModelGenerator
    {
        static void Main(string[] args)
        {

            string output = "";
            string input = "";
            bool pause = false;

            foreach (string arg in args)
            {
                if (arg.StartsWith("/output:"))
                    output = arg.Substring(8, arg.Length - 8);
                if (arg.StartsWith("/input:"))
                    input = arg.Substring(7, arg.Length - 7);
                if ("/pause".Equals(arg))
                    pause = true;
            }

            if (input.Length == 0 ||
                output.Length == 0)
            {
                System.Console.WriteLine("Usage: ModelGenerator [/input:directory] [/output:directory] [/pause]");
                pause = true; 
            }
            else
            {
                try
                {
                    ModelBuilder bm = new ModelBuilder(input, output);
                    bm.run();
                }
                catch (Exception e)
                {
                    System.Console.WriteLine(e);

                    pause = true;                
                }
            }
            
            while (pause && !Console.KeyAvailable)
                Thread.Sleep(250); 

        }
    }
}
