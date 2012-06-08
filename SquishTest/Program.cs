using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Playroom;

namespace SquishTest
{
    class Program
    {
        private static unsafe double GetColourError(byte[] a, byte[] b)
        {
	        double error = 0.0;
	        
            for (int i = 0; i < 16; ++i)
	        {
		        for (int j = 0; j < 3; ++j)
		        {
			        int index = 4*i + j;
			        int diff = (int)a[index] - (int)b[index];
			        error += (double)(diff*diff);
		        }
	        }
	        
            return error / 16.0;
        }

        public static void TestOneColour(SquishMethod method)
        {
	        byte[] input = new byte[4*16];
	        double avg = 0.0, min = Double.MaxValue, max = -Double.MaxValue;
	        int counter = 0;
	
	        // test all single-channel colours
	        for (int i = 0; i < 16*4; ++i)
		        input[i] = ((i % 4) == 3) ? (byte)255 : (byte)0;
	        
            for (int channel = 0; channel < 3; ++channel)
	        {
		        for(int value = 0; value < 255; ++value)
		        {
			        // set the channnel value
			        for(int i = 0; i < 16; ++i)
				        input[4*i + channel] = (byte)value;
			
			        // compress and decompress
			        byte[] block = Squish.CompressImage(input, 4, 4, method, SquishFit.Default, SquishMetric.Default, SquishExtra.None);
			        byte[] output = Squish.DecompressImage(4, 4, block, method);
			
			        // test the results
			        double rm = GetColourError(input, output);
			        double rms = Math.Sqrt(rm);
			
			        // accumulate stats
			        min = Math.Min(min, rms);
			        max = Math.Max(max, rms);
			        avg += rm;
			        ++counter;
		        }
		
		        // reset the channel value
		        for(int i = 0; i < 16; ++i)
			        input[4 * i + channel] = 0;
	        }
	
	        // finish stats
	        avg = Math.Sqrt(avg / counter);
	
	        // show stats
            Console.WriteLine("One colour error (min, max, avg): {0:F5}, {1:F5}, {2:F5}", min, max, avg);
        }

        public static void TestOneColourRandom(SquishMethod method, SquishFit fit)
        {
	        byte[] input = new byte[4*16];
	
	        double avg = 0.0, min = Double.MaxValue, max = -Double.MaxValue;
	        int counter = 0;
	
	        // test all single-channel colours
            Random rand = new Random();
	        for (int test = 0; test < 1000; ++test)
	        {
		        // set a constant random colour
		        for(int channel = 0; channel < 3; ++channel)
		        {
			        byte value = (byte)(rand.Next() & 0xff);
			        
                    for(int i = 0; i < 16; ++i)
				        input[4*i + channel] = value;
		        }
		        
                for(int i = 0; i < 16; ++i)
			        input[4*i + 3] = 255;
		
		        // compress and decompress
		        byte[] block = Squish.CompressImage(input, 4, 4, method, fit, SquishMetric.Default, SquishExtra.None);
		        byte[] output = Squish.DecompressImage(4, 4, block, method);
		
		        // test the results
		        double rm = GetColourError(input, output);
		        double rms = Math.Sqrt(rm);
		
		        // accumulate stats
		        min = Math.Min(min, rms);
		        max = Math.Max(max, rms);
		        avg += rm;
		        ++counter;
	        }
	
	        // finish stats
	        avg = Math.Sqrt(avg/counter);
	
	        // show stats
            Console.WriteLine("Random one colour error (min, max, avg): {0:F5}, {1:F5}, {2:F5}", min, max, avg);
        }

        public static void TestTwoColour(SquishMethod method)
        {
	        byte[] input = new byte[4*16];

	        double avg = 0.0, min = Double.MaxValue, max = -Double.MaxValue;
	        int counter = 0;
	
	        // test all single-channel colours
	        for( int i = 0; i < 16*4; ++i )
		        input[i] = ((i % 4) == 3) ? (byte)255 : (byte)0;

	        for (int channel = 0; channel < 3; ++channel)
	        {
		        for (int value1 = 0; value1 < 255; ++value1)
		        {
			        for (int value2 = value1 + 1; value2 < 255; ++value2)
			        {
				        // set the channnel value
				        for (int i = 0; i < 16; ++i)
					        input[4*i + channel] = (byte)((i < 8) ? value1 : value2);
				
				        // compress and decompress
				        byte[] block = Squish.CompressImage(input, 4, 4, method, SquishFit.Default, SquishMetric.Default, SquishExtra.None);
                        byte[] output = Squish.DecompressImage(4, 4, block, method);
				
				        // test the results
				        double rm = GetColourError(input, output);
				        double rms = Math.Sqrt(rm);
				
				        // accumulate stats
				        min = Math.Min(min, rms);
				        max = Math.Max(max, rms);
				        avg += rm;
				        ++counter;
			        }
		        }
				
		        // reset the channel value
		        for (int i = 0; i < 16; ++i)
			        input[4*i + channel] = 0;
	        }
	
	        // finish stats
	        avg = Math.Sqrt(avg / counter);
	
	        // show stats
            Console.WriteLine("Two colour error (min, max, avg): {0:F5}, {1:F5}, {2:F5}", min, max, avg);
        }

        public static void Main()
        {
	        TestOneColourRandom(SquishMethod.Dxt1, SquishFit.Range);
            TestOneColour(SquishMethod.Dxt1);
            TestTwoColour(SquishMethod.Dxt1);
        }
    }
}
