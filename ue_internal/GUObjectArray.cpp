#include "GUObjectArray.hpp"
#include <Windows.h>
#include <unordered_map>

namespace UE503 {
	std::unordered_map<std::string, FUObjectItem*> FUObjectArray::Cache = {};

	FUObjectItem* TUObjectArray::GetObjectPtr(int32_t Index) {
		const int32_t ChunkIndex = Index / NumElementsPerChunk;
		const int32_t WithinChunkIndex = Index % NumElementsPerChunk;

		if (!IsValidIndex(Index) || ChunkIndex >= NumChunks || Index >= MaxElements)
			return nullptr;

		FUObjectItem* Chunk = Objects[ChunkIndex];

		return Chunk + WithinChunkIndex;
	}
}