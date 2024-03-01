/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "PrimAsset.h"

PrimAsset::PrimAsset() {
	mainProcess.BindRaw(this, &PrimAsset::Process);
}

PrimAsset::~PrimAsset() {}

void PrimAsset::Process() {
	stageData->Add(0);
	xmlData.Append(stageData->GetData(), stageData->Num());
	xmlData.Add(0);
	stageData.Reset();
}
