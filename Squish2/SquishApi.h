#ifndef SQUISHAPI_H
#define SQUISHAPI_H

#ifdef SQUISH_EXPORTS
#define SQUISH_API __declspec(dllexport) _stdcall
#else
#define SQUISH_API __declspec(dllimport) _stdcall
#endif

typedef unsigned char u8;

enum
{
	//! Use DXT1 compression.
	kDxt1 = (1 << 0), 
	
	//! Use DXT3 compression.
	kDxt3 = (1 << 1), 
	
	//! Use DXT5 compression.
	kDxt5 = (1 << 2), 
	
	//! Use a very slow but very high quality colour compressor.
	kColourIterativeClusterFit = (1 << 8),	
	
	//! Use a slow but high quality colour compressor (the default).
	kColourClusterFit = (1 << 3),	
	
	//! Use a fast but low quality colour compressor.
	kColourRangeFit	= (1 << 4),
	
	//! Use a perceptual metric for colour error (the default).
	kColourMetricPerceptual = (1 << 5),

	//! Use a uniform metric for colour error.
	kColourMetricUniform = (1 << 6),
	
	//! Weight the colour by alpha during cluster fit (disabled by default).
	kWeightColourByAlpha = (1 << 7)
};

extern "C" {
void SQUISH_API SquishCompress(u8 const* rgba, void* block, int flags);
void SQUISH_API SquishCompressMasked(u8 const* rgba, int mask, void* block, int flags);
void SQUISH_API SquishDecompress(u8* rgba, void const* block, int flags);
int SQUISH_API SquishGetStorageRequirements(int width, int height, int flags);
void SQUISH_API SquishCompressImage(u8 const* rgba, int width, int height, void* blocks, int flags);
void SQUISH_API SquishDecompressImage(u8* rgba, int width, int height, void const* blocks, int flags);
}

#endif // SQUISH_H

