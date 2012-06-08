#include "stdafx.h"
#include "SquishApi.h"

void SQUISH_API SquishyCompress(u8 const* rgba, void* block, int flags)
{
    squish::Compress(rgba, block, flags);
}

void SQUISH_API SquishyCompressMasked(u8 const* rgba, int mask, void* block, int flags)
{
    squish::CompressMasked(rgba, mask, block, flags);
}

void SQUISH_API SquishyDecompress(u8* rgba, void const* block, int flags)
{
    squish::Decompress(rgba, block, flags);
}

int SQUISH_API SquishyGetStorageRequirements(int width, int height, int flags)
{
    return squish::GetStorageRequirements(width, height, flags);
}

void SQUISH_API SquishyCompressImage(u8 const* rgba, int width, int height, void* blocks, int flags)
{
    squish::CompressImage(rgba, width, height, blocks, flags);
}

void SQUISH_API SquishyDecompressImage(u8* rgba, int width, int height, void const* blocks, int flags)
{
}
