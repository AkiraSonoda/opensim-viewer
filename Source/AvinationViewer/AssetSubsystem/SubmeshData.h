/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "ProceduralMeshComponent.h"
#include "../Meshing/PrimMesher.h"

/**
 * 
 */
class AVINATIONVIEWER_API SubmeshData {
public:
	SubmeshData();
	~SubmeshData();

	TArray<PrimFaceMeshData> meshFaces;
	int numFaces = 0;
};
