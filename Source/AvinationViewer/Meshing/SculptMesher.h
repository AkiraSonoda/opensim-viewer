/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "PrimMesher.h"

/**
 * 
 */
enum SculptType { sphere = 1, torus = 2, plane = 3, cylinder = 4 };

class AVINATIONVIEWER_API SculptMesh {
public:
	TArray<FVector> coords;
	TArray<Face> faces;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;

	SculptMesh(TArray<TArray<FVector>>& rows, SculptType sculptType, bool viewerMode, int lod);

private:
	void calcVertexNormals(SculptType sculptType, int xSize, int ySize);
};
