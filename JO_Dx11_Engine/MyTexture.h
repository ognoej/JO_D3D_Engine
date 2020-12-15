#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

class MyTexture
{
};

