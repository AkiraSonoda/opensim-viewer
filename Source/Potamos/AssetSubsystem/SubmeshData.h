/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "ProceduralMeshComponent.h"
#include "../Meshing/PrimMesher.h"

/**
 * 
 */
class POTAMOS_API SubmeshData {
public:
	SubmeshData();
	~SubmeshData();

	TArray<PrimFaceMeshData> meshFaces;
	int numFaces = 0;
};
