/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "TextureAsset.h"
#include "PotamosGameMode.h"


struct MBB : FBulkData {
	void SetFilename(FString name);
#if WITH_EDITOR
	void setAr(FArchive* ar);
#endif
};

void MBB::SetFilename(FString name) {
	// TODO check if this works
	// Filename = name;
	FStringBuilderBase sb;
	sb.Append(name);
	GetPackagePath().AppendLocalFullPath(sb);
}

#if WITH_EDITOR
void MBB::setAr(FArchive* ar) {
	AttachedAr = ar;
}
#endif

TextureAsset::TextureAsset() {
	decode.BindRaw(this, &TextureAsset::DecodeImage);
	preProcess.BindRaw(this, &TextureAsset::PreProcess);
	mainProcess.BindRaw(this, &TextureAsset::Process);
	postProcess.BindRaw(this, &TextureAsset::PostProcess);
}

TextureAsset::~TextureAsset() {
	if (image)
		opj_image_destroy(image);
	image = 0;
}

void TextureAsset::DecodeImage() {
	AssetBase::Decode();

	J2KDecode idec;
	if (!idec.Decode(stageData)) {
		throw std::exception();
	}

	stageData.Reset();

	image = idec.image;
	idec.image = 0;

	if (!image)
		throw std::exception();

	if (image->numcomps < 1)
		throw std::exception();

	w = image->comps[0].w;
	h = image->comps[0].h;
	if (w < 0 || h < 0) {
		opj_image_destroy(image);
		image = 0;
		throw std::exception();
	}
}

void TextureAsset::PreProcess() {
	//    worktex->Serialize();
}

void TextureAsset::Process() {
	texBuffer.Empty();
	texBuffer.AddZeroed(13);

	texBuffer[0] = new uint8_t[w * h * 4];

	int32_t *r, *g, *b, *a;
	r = 0;
	g = 0;
	b = 0;
	a = 0;

	switch (image->numcomps) {
	case 1: // Straight greyscale
		r = image->comps[0].data;
		break;
	case 2: // Greyscale w/alpha
		r = image->comps[0].data;
		a = image->comps[1].data;
		break;
	case 3: // RGB
		r = image->comps[0].data;
		g = image->comps[1].data;
		b = image->comps[2].data;
		break;
	case 4: // RGBA
		r = image->comps[0].data;
		g = image->comps[1].data;
		b = image->comps[2].data;
		a = image->comps[3].data;
		break;
	}

	hasAlpha = false;
	int pixels = w * h;

	uint8_t* dest = texBuffer[0];
	for (int i = 0; i < pixels; i++) {
		if (!g) {
			*dest++ = (uint8_t)*r;
			*dest++ = (uint8_t)*r;
		} else {
			*dest++ = (uint8_t)*(b++);
			*dest++ = (uint8_t)*(g++);
		}
		*dest++ = (uint8_t)*(r++);

		if (a) {
			if (*a < 0xf8)
				hasAlpha = true;
			*dest++ = (uint8_t)*(a++);
		} else {
			*dest++ = 0xff;
		}
	}

	opj_image_destroy(image);
	image = 0;

	nlevels = 1;

	int mindimension = h;
	if (mindimension > w)
		mindimension = w;
	int sw = w;
	int sh = h;

	unsigned int* tmpline = new unsigned int[w * 4];

	while (mindimension > 1 && nlevels < 13) {
		texBuffer[nlevels] = new uint8_t[sw * sh]; // reducion will be by 2x2 so number of components included

		dest = texBuffer[nlevels];
		uint8_t* src = texBuffer[nlevels - 1];

		unsigned int* destline;

		sh >>= 1;
		sw >>= 1;
		mindimension >>= 1;

		for (int j = 0; j < sh; j++) {
			destline = tmpline;
			for (int i = 0; i < sw; i++) {
				*destline++ = *src++;
				*destline++ = *src++;
				*destline++ = *src++;
				*destline = *src++;

				destline -= 3;
				*destline++ += *src++;
				*destline++ += *src++;
				*destline++ += *src++;
				*destline++ += *src++;
			}

			destline = tmpline;
			for (int i = 0; i < sw; i++) {
				*destline++ += *src++;
				*destline++ += *src++;
				*destline++ += *src++;
				*destline += *src++;

				destline -= 3;
				*destline++ += *src++;
				*destline++ += *src++;
				*destline++ += *src++;
				*destline += *src++;

				destline -= 3;
				*dest++ = (uint8_t)(*destline++ >> 2);
				*dest++ = (uint8_t)(*destline++ >> 2);
				*dest++ = (uint8_t)(*destline++ >> 2);
				*dest++ = (uint8_t)(*destline++ >> 2);
			}
		}

		nlevels++;
	}
	FMemory::Free(tmpline);

	tex = TextureAsset::CreateTexture();
	if (tex == nullptr)
		state = AssetBase::Failed;

	if (nlevels > 0) {
		for (int i = 0; i < nlevels; i++) {
			if (texBuffer[i])
				FMemory::Free(texBuffer[i]);
			texBuffer[i] = nullptr;
		}
	}
	texBuffer.Empty();
	nlevels = 0;
}

void TextureAsset::PostProcess() {
	if (state == AssetBase::Failed)
		return;

	tex->UpdateResource();

	tex->AddToRoot();
}

UTexture2D* TextureAsset::CreateTexture() {
	bool normalMap = false;
	FString sid = id.ToString(EGuidFormats::DigitsWithHyphens).ToLower();
	FString ppath = FPaths::ProjectDir();

	ppath = FPaths::Combine(*ppath, TEXT("cache/"), *sid);
	FPaths::MakeStandardFilename(ppath);

	//    UE_LOG(LogTemp, Warning, TEXT("%s"), *ppath);

	FArchive* archive = 0;

	bool retried = false;

	do {
		archive = IFileManager::Get().CreateFileWriter(*ppath);
		if (!archive && retried)
			return 0;
		retried = true;
	} while (!archive);

	// ar->ArIsSaving = true;
	archive->GetArchiveState().SetIsSaving(true);
	// ar->ArIsPersis   tent = true;
	archive->GetArchiveState().SetIsPersistent(true);

	UTexture2D* Texture = nullptr;
	Texture = NewObject<UTexture2D>(GetTransientPackage(), *sid, RF_Standalone | RF_Public);

	if (!Texture) {
		return nullptr;
	}

	*archive << sid;
	*archive << nlevels;
	*archive << w;
	*archive << h;
	int pixelFormat = (int)EPixelFormat::PF_B8G8R8A8;
	*archive << pixelFormat;
	*archive << normalMap;
	*archive << hasAlpha;


	FTexturePlatformData* platformData = new FTexturePlatformData();
	platformData->SizeX = w;
	platformData->SizeY = h;
	platformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	platformData->SetNumSlices(1);
	
	// Texture->PlatformData->SizeX = w;
	// Texture->PlatformData->SizeY = h;
	// Texture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	// Texture->PlatformData->SetNumSlices(1);
	// Texture->bForceMiplevelsToBeResident = false;
	// TODO Texture->bIsStreamable = true; 


	for (int level = 0; level < nlevels; level++) {
		int lw = w >> level;
		int lh = h >> level;

		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		void* NewMipData = Mip->BulkData.Lock(LOCK_READ_WRITE);
		Mip->SizeX = lw;
		Mip->SizeY = lh;
		int tsize = lw * lh * 4;
		NewMipData = Mip->BulkData.Realloc(tsize);
		FMemory::Memcpy(NewMipData, (void*)texBuffer[level], tsize);
		// Texture->PlatformData->Mips.Add(Mip);
		Mip->BulkData.Unlock();
		MBB* b = (MBB*)&Mip->BulkData;
		b->SetFilename(ppath);
		Mip->BulkData.SetBulkDataFlags(EBulkDataFlags::BULKDATA_SerializeCompressedZLIB);
		platformData->Mips.Add(Mip);
		Mip->BulkData.Serialize(*archive, 0);
	}

	archive->FlushCache();
	archive->Close();
	delete archive;

	if (nlevels > 6) {
		int min = nlevels - 6;
		for (int level = 0; level < min; level++) {
			FTexture2DMipMap* Mip = &Texture->GetPlatformData()->Mips[level];
			// MBB* b = (MBB*)&Mip->BulkData;
			Mip->BulkData.SetBulkDataFlags(EBulkDataFlags::BULKDATA_SingleUse);
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			Mip->BulkData.Unlock();
			Mip->BulkData.SetBulkDataFlags(EBulkDataFlags::BULKDATA_SerializeCompressedZLIB);
		}
	}

	Texture->SetPlatformData(platformData);

#if WITH_EDITOR
	FArchive* readAr = IFileManager::Get().CreateFileReader(*ppath, FILEREAD_Silent);
	for (int level = 0; level < nlevels; level++) {
		MBB* b = (MBB*)&Texture->GetPlatformData()->Mips[level].BulkData;
		readAr->AttachBulkData(tex, &Texture->GetPlatformData()->Mips[level].BulkData);
		b->setAr(readAr);
	}
#endif

	if (normalMap) {
		Texture->LODGroup = TEXTUREGROUP_WorldNormalMap;
		Texture->CompressionSettings = TC_Normalmap;
		Texture->SRGB = false;
	} else {
		Texture->LODGroup = TEXTUREGROUP_World;
		Texture->CompressionSettings = TC_Default;
		Texture->SRGB = true;
	}

	return Texture;
}

bool TextureAsset::GetFromCache(const TCHAR* file) {
	// Bend the pointers so the texture is pulled from cache instead of decoding from
	// network
	FString sfile = file;

	decode.BindRaw(this, &TextureAsset::LoadFromFile, sfile);
	preProcess.Unbind();
	mainProcess.Unbind();
	postProcess.BindRaw(this, &TextureAsset::PostProcess);

	// Returning true here simply means that we support loading from file for this
	// asset type
	return true;
}

void TextureAsset::LoadFromFile(FString file) {
	//    UE_LOG(LogTemp, Warning, TEXT("%s"), *ppath);
	FString sid;

	FArchive* ar = IFileManager::Get().CreateFileReader(*file);
	if (!ar) {
		state = AssetBase::Failed;
		return;
	}

	// ar->ArIsSaving = true;
	ar->GetArchiveState().SetIsSaving(true);
	// ar->ArIsPersis   tent = true;
	ar->GetArchiveState().SetIsPersistent(true);
	*ar << sid;

	tex = nullptr;
	tex = NewObject<UTexture2D>(GetTransientPackage(), *sid, RF_Standalone | RF_Public);

	if (!tex) {
		state = AssetBase::Failed;
		return;
	}

	int pixelFormat;
	bool normalMap;

	*ar << nlevels;
	*ar << w;
	*ar << h;
	*ar << pixelFormat;
	*ar << normalMap;
	*ar << hasAlpha;

	FTexturePlatformData* platformData = new FTexturePlatformData();
	platformData->SizeX = w;
	platformData->SizeY = h;
	platformData->PixelFormat = (EPixelFormat)pixelFormat;
	platformData->SetNumSlices(1);

	tex->bForceMiplevelsToBeResident = false;
	// TODO tex->bIsStreamable = true; 

	int minloadlevel = 0;
	if (nlevels > 6)
		minloadlevel = nlevels - 6;

	for (int level = 0; level < nlevels; level++) {
		int lw = w >> level;
		int lh = h >> level;

		FTexture2DMipMap* Mip = new FTexture2DMipMap();
		Mip->SizeX = lw;
		Mip->SizeY = lh;
		// we should avoid this load and delete
		// possible cloning the serializer
		Mip->BulkData.Serialize(*ar, 0);
		// tex->PlatformData->Mips.Add(Mip);
		
		MBB* b = (MBB*)&Mip->BulkData;
		b->SetFilename(file); // not in serialize
		if (level < minloadlevel) {
			b->SetBulkDataFlags(EBulkDataFlags::BULKDATA_SingleUse);
			Mip->BulkData.Lock(LOCK_READ_WRITE);
			Mip->BulkData.Unlock();
		}
		b->SetBulkDataFlags(EBulkDataFlags::BULKDATA_SerializeCompressedZLIB);
		platformData->Mips.Add(Mip);
	}

	ar->Close();
	delete ar;

	tex->SetPlatformData(platformData);
	
#if WITH_EDITOR
	FArchive* readAr = IFileManager::Get().CreateFileReader(*file, FILEREAD_Silent);
	for (int level = 0; level < nlevels; level++) {
		MBB* b = (MBB*)&tex->GetPlatformData()->Mips[level].BulkData;
		readAr->AttachBulkData(tex, &tex->GetPlatformData()->Mips[level].BulkData);
		b->setAr(readAr);
	}
#endif

	if (normalMap) {
		tex->LODGroup = TEXTUREGROUP_WorldNormalMap;
		tex->CompressionSettings = TC_Normalmap;
		tex->SRGB = false;
	} else {
		tex->LODGroup = TEXTUREGROUP_World;
		tex->CompressionSettings = TC_Default;
		tex->SRGB = true;
	}
	nlevels = 0;
}
