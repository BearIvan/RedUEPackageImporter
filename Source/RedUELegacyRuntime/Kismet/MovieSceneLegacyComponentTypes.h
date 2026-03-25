#pragma once
#include "EntitySystem/MovieSceneEntityIDs.h"
#include "Materials/Hybrid/MaterialInstanceHybrid.h"

class FMovieSceneLegacyComponentTypes
{
public:
	UE::MovieScene::TComponentTypeID<TObjectPtr<UMaterialInstanceHybrid>>		HybridMaterialHandle;
	
	static  void																Destroy							();
	static  FMovieSceneLegacyComponentTypes*									Get								();
private:								
																				FMovieSceneLegacyComponentTypes	();
};

