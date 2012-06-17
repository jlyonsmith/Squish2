/* -----------------------------------------------------------------------------

	Copyright (c) 2006 Simon Brown                          si@sjbrown.co.uk

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the 
	"Software"), to	deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to 
	permit persons to whom the Software is furnished to do so, subject to 
	the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
   -------------------------------------------------------------------------- */
   
#ifndef SQUISH_H
#define SQUISH_H

#ifdef WINDOWS
  #ifdef SQUISH_EXPORTS
    #define SQUISH_API __declspec(dllexport) _stdcall
  #else
    #define SQUISH_API __declspec(dllimport) _stdcall
  #endif
#else
  #define SQUISH_API
#endif

// -----------------------------------------------------------------------------

//! Typedef a quantity that is a single unsigned byte.
typedef unsigned char u8;

// -----------------------------------------------------------------------------

enum
{
	//! Use DXT1 compression.
	squishDxt1 = ( 1 << 0 ), 
	
	//! Use DXT3 compression.
	squishDxt3 = ( 1 << 1 ), 
	
	//! Use DXT5 compression.
	squishDxt5 = ( 1 << 2 ), 
	
	//! Use a very slow but very high quality colour compressor.
	squishColourIterativeClusterFit = ( 1 << 8 ),	
	
	//! Use a slow but high quality colour compressor (the default).
	squishColourClusterFit = ( 1 << 3 ),	
	
	//! Use a fast but low quality colour compressor.
	squishColourRangeFit	= ( 1 << 4 ),
	
	//! Use a perceptual metric for colour error (the default).
	squishColourMetricPerceptual = ( 1 << 5 ),

	//! Use a uniform metric for colour error.
	squishColourMetricUniform = ( 1 << 6 ),
	
	//! Weight the colour by alpha during cluster fit (disabled by default).
	squishWeightColourByAlpha = ( 1 << 7 )
};

// -----------------------------------------------------------------------------

/*! @brief Compresses a 4x4 block of pixels.

	@param rgba		The rgba values of the 16 source pixels.
	@param block	Storage for the compressed DXT block.
	@param flags	Compression flags.
	
	The source pixels should be presented as a contiguous array of 16 rgba
	values, with each component as 1 byte each. In memory this should be:
	
		{ r1, g1, b1, a1, .... , r16, g16, b16, a16 }
	
	The flags parameter should specify either squishDxt1, squishDxt3 or squishDxt5 compression, 
	however, DXT1 will be used by default if none is specified. When using DXT1 
	compression, 8 bytes of storage are required for the compressed DXT block. 
	DXT3 and DXT5 compression require 16 bytes of storage per block.
	
	The flags parameter can also specify a preferred colour compressor and 
	colour error metric to use when fitting the RGB components of the data. 
	Possible colour compressors are: squishColourClusterFit (the default), 
	squishColourRangeFit or squishColourIterativeClusterFit. Possible colour error metrics 
	are: squishColourMetricPerceptual (the default) or squishColourMetricUniform. If no 
	flags are specified in any particular category then the default will be 
	used. Unknown flags are ignored.
	
	When using squishColourClusterFit, an additional flag can be specified to
	weight the colour of each pixel by its alpha value. For images that are
	rendered using alpha blending, this can significantly increase the 
	perceived quality.
*/
extern "C" void SQUISH_API SquishCompress( u8 const* rgba, void* block, int flags );

// -----------------------------------------------------------------------------

/*! @brief Compresses a 4x4 block of pixels.

	@param rgba		The rgba values of the 16 source pixels.
	@param mask		The valid pixel mask.
	@param block	Storage for the compressed DXT block.
	@param flags	Compression flags.
	
	The source pixels should be presented as a contiguous array of 16 rgba
	values, with each component as 1 byte each. In memory this should be:
	
		{ r1, g1, b1, a1, .... , r16, g16, b16, a16 }
		
	The mask parameter enables only certain pixels within the block. The lowest
	bit enables the first pixel and so on up to the 16th bit. Bits beyond the
	16th bit are ignored. Pixels that are not enabled are allowed to take
	arbitrary colours in the output block. An example of how this can be used
	is in the CompressImage function to disable pixels outside the bounds of
	the image when the width or height is not divisible by 4.
	
	The flags parameter should specify either squishDxt1, squishDxt3 or squishDxt5 compression, 
	however, DXT1 will be used by default if none is specified. When using DXT1 
	compression, 8 bytes of storage are required for the compressed DXT block. 
	DXT3 and DXT5 compression require 16 bytes of storage per block.
	
	The flags parameter can also specify a preferred colour compressor and 
	colour error metric to use when fitting the RGB components of the data. 
	Possible colour compressors are: squishColourClusterFit (the default), 
	squishColourRangeFit or squishColourIterativeClusterFit. Possible colour error metrics 
	are: squishColourMetricPerceptual (the default) or squishColourMetricUniform. If no 
	flags are specified in any particular category then the default will be 
	used. Unknown flags are ignored.
	
	When using squishColourClusterFit, an additional flag can be specified to
	weight the colour of each pixel by its alpha value. For images that are
	rendered using alpha blending, this can significantly increase the 
	perceived quality.
*/
extern "C" void SQUISH_API SquishCompressMasked( u8 const* rgba, int mask, void* block, int flags );

// -----------------------------------------------------------------------------

/*! @brief Decompresses a 4x4 block of pixels.

	@param rgba		Storage for the 16 decompressed pixels.
	@param block	The compressed DXT block.
	@param flags	Compression flags.

	The decompressed pixels will be written as a contiguous array of 16 rgba
	values, with each component as 1 byte each. In memory this is:
	
		{ r1, g1, b1, a1, .... , r16, g16, b16, a16 }
	
	The flags parameter should specify either squishDxt1, squishDxt3 or squishDxt5 compression, 
	however, DXT1 will be used by default if none is specified. All other flags 
	are ignored.
*/
extern "C" void SQUISH_API SquishDecompress( u8* rgba, void const* block, int flags );

// -----------------------------------------------------------------------------

/*! @brief Computes the amount of compressed storage required.

	@param width	The width of the image.
	@param height	The height of the image.
	@param flags	Compression flags.
	
	The flags parameter should specify either squishDxt1, squishDxt3 or squishDxt5 compression, 
	however, DXT1 will be used by default if none is specified. All other flags 
	are ignored.
	
	Most DXT images will be a multiple of 4 in each dimension, but this 
	function supports arbitrary size images by allowing the outer blocks to
	be only partially used.
*/
extern "C" int SQUISH_API SquishGetStorageRequirements( int width, int height, int flags );

// -----------------------------------------------------------------------------

/*! @brief Compresses an image in memory.

	@param rgba		The pixels of the source.
	@param width	The width of the source image.
	@param height	The height of the source image.
	@param blocks	Storage for the compressed output.
	@param flags	Compression flags.
	
	The source pixels should be presented as a contiguous array of width*height
	rgba values, with each component as 1 byte each. In memory this should be:
	
		{ r1, g1, b1, a1, .... , rn, gn, bn, an } for n = width*height
		
	The flags parameter should specify either squishDxt1, squishDxt3 or squishDxt5 compression, 
	however, DXT1 will be used by default if none is specified. When using DXT1 
	compression, 8 bytes of storage are required for each compressed DXT block. 
	DXT3 and DXT5 compression require 16 bytes of storage per block.
	
	The flags parameter can also specify a preferred colour compressor and 
	colour error metric to use when fitting the RGB components of the data. 
	Possible colour compressors are: squishColourClusterFit (the default), 
	squishColourRangeFit or squishColourIterativeClusterFit. Possible colour error metrics 
	are: squishColourMetricPerceptual (the default) or squishColourMetricUniform. If no 
	flags are specified in any particular category then the default will be 
	used. Unknown flags are ignored.
	
	When using squishColourClusterFit, an additional flag can be specified to
	weight the colour of each pixel by its alpha value. For images that are
	rendered using alpha blending, this can significantly increase the 
	perceived quality.
	
	Internally this function calls squish::Compress for each block. To see how
	much memory is required in the compressed image, use
	squish::GetStorageRequirements.
*/
extern "C" void SQUISH_API SquishCompressImage( u8 const* rgba, int width, int height, void* blocks, int flags );

// -----------------------------------------------------------------------------

/*! @brief Decompresses an image in memory.

	@param rgba		Storage for the decompressed pixels.
	@param width	The width of the source image.
	@param height	The height of the source image.
	@param blocks	The compressed DXT blocks.
	@param flags	Compression flags.
	
	The decompressed pixels will be written as a contiguous array of width*height
	16 rgba values, with each component as 1 byte each. In memory this is:
	
		{ r1, g1, b1, a1, .... , rn, gn, bn, an } for n = width*height
		
	The flags parameter should specify either squishDxt1, squishDxt3 or squishDxt5 compression, 
	however, DXT1 will be used by default if none is specified. All other flags 
	are ignored.

	Internally this function calls SquishDecompress for each block.
*/
extern "C" void SQUISH_API SquishDecompressImage( u8* rgba, int width, int height, void const* blocks, int flags );

// -----------------------------------------------------------------------------

#endif // ndef SQUISH_H

