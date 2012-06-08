using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using Playroom;

namespace SquishPng
{
    class Program
    {
        private unsafe static byte[] GetRgbaData(Bitmap bitmap)
        {
            byte[] rgba = new byte[4 * bitmap.Width * bitmap.Height];
            BitmapData bitmapData = null;

            try
            {
                bitmapData = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);

                for (int y = 0; y < bitmap.Height; y++)
                {
                    for (int x = 0; x < bitmap.Width; x++)
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            rgba[(y * bitmap.Width + x) * 4 + i] = ((byte*)bitmapData.Scan0)[y * bitmapData.Stride + x * 4 + i];
                        }
                    }
                }
            }
            finally
            {
                if (bitmapData != null)
                    bitmap.UnlockBits(bitmapData);
            }

            return rgba;
        }

        private unsafe static void SetRgbaData(Bitmap bitmap, byte[] rgba)
        {
            fixed (byte* pRgba = rgba)
            {
                BitmapData bitmapData = new BitmapData();

                bitmapData.Width = bitmap.Width;
                bitmapData.Height = bitmap.Height;
                bitmapData.PixelFormat = PixelFormat.Format32bppArgb;
                bitmapData.Stride = bitmapData.Width * 4;
                bitmapData.Scan0 = (IntPtr)pRgba;
                bitmapData.Reserved = 0;

                bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                    ImageLockMode.WriteOnly | ImageLockMode.UserInputBuffer, PixelFormat.Format32bppArgb, bitmapData);

                bitmap.UnlockBits(bitmapData);
            }
        }

        public static void Main(string[] args)
        {
            Bitmap bitmap = (Bitmap)Image.FromFile(args[0]);

            byte[] rgba = GetRgbaData(bitmap);

            byte[] blocks = Squish.CompressImage(
                rgba, bitmap.Width, bitmap.Height, SquishMethod.Dxt5, SquishFit.IterativeCluster, SquishMetric.Default, SquishExtra.None);

            rgba = Squish.DecompressImage(bitmap.Width, bitmap.Height, blocks, SquishMethod.Dxt5);

            SetRgbaData(bitmap, rgba);

            bitmap.Save(args[1]);
        }
    }
}
