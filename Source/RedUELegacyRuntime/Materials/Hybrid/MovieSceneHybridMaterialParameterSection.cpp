#include "MovieSceneHybridMaterialParameterSection.h"

#include "MovieSceneHybridMaterialTrack.h"
#include "MovieSceneTracksComponentTypes.h"
#include "EntitySystem/BuiltInComponentTypes.h"
#include "Kismet/MovieSceneLegacyComponentTypes.h"
#include "Tracks/MovieSceneMaterialTrack.h"
#define LOCTEXT_NAMESPACE "RedUELegacy"

namespace Rbmk::MovieScene
{

	/* Entity IDs are an encoded type and index, with the upper 8 bits being the type, and the lower 24 bits as the index */
	uint32 EncodeMaterialParameterEntityID(int32 InIndex, uint8 InType)
	{
		check(InIndex >= 0 && InIndex < int32(0x00FFFFFF));
		return static_cast<uint32>(InIndex) | (uint32(InType) << 24);
	}
	void DecodeMaterialParameterEntityID(uint32 InEntityID, int32& OutIndex, uint8& OutType)
	{
		// Mask out the type to get the index
		OutIndex = static_cast<int32>(InEntityID & 0x00FFFFFF);
		OutType = InEntityID >> 24;
	}


}// namespace UE::MovieScene

void UMovieSceneHybridMaterialParameterSection::ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity)
{
	using namespace UE::MovieScene;

	uint8 ParameterType = 0;
	int32 EntityIndex = 0;
	Rbmk::MovieScene::DecodeMaterialParameterEntityID(Params.EntityID, EntityIndex, ParameterType);

	FBuiltInComponentTypes* BuiltInComponentTypes = FBuiltInComponentTypes::Get();
	FMovieSceneTracksComponentTypes* TracksComponentTypes = FMovieSceneTracksComponentTypes::Get();
	FMovieSceneLegacyComponentTypes* LegacyComponentTypes = FMovieSceneLegacyComponentTypes::Get();

	FGuid ObjectBindingID = Params.GetObjectBindingID();
	
	// Find material info from the outer track
	TObjectPtr<UMaterialInstanceHybrid> MaterialHybrid;
	if (UMovieSceneHybridMaterialTrack* MaterialTrack = GetTypedOuter<UMovieSceneHybridMaterialTrack>())
	{
		MaterialHybrid = MaterialTrack->GetMaterialHybrid();
	}

	TEntityBuilder<TAddConditional<FGuid>> BaseBuilder = FEntityBuilder()
		.AddConditional(BuiltInComponentTypes->GenericObjectBinding, ObjectBindingID, ObjectBindingID.IsValid());

	switch (ParameterType)
	{
	case 0:
	{
		const FScalarMaterialParameterInfoAndCurve& Scalar = ScalarParameterInfosAndCurves[EntityIndex];

		if (Scalar.ParameterCurve.HasAnyData())
		{
			OutImportedEntity->AddBuilder(
				BaseBuilder
				.Add(TracksComponentTypes->ScalarMaterialParameterInfo, Scalar.ParameterInfo)
				.Add(BuiltInComponentTypes->FloatChannel[0], &Scalar.ParameterCurve)
				.Add(LegacyComponentTypes->HybridMaterialHandle, MaterialHybrid)
				// If the section has no valid blend type (legacy data), make it use absolute blending.
				// Otherwise, the base section class will add the appropriate blend type tag in BuildDefaultComponents.
				.AddTagConditional(BuiltInComponentTypes->Tags.AbsoluteBlend, !GetBlendType().IsValid())
			);
		}
		break;
	}
	case 1:
	{
		const FColorMaterialParameterInfoAndCurves& Color = ColorParameterInfosAndCurves[EntityIndex];
		if (Color.RedCurve.HasAnyData() || Color.GreenCurve.HasAnyData() || Color.BlueCurve.HasAnyData() || Color.AlphaCurve.HasAnyData())
		{
			OutImportedEntity->AddBuilder(
				BaseBuilder
				.Add(TracksComponentTypes->ColorMaterialParameterInfo, Color.ParameterInfo)
				.AddConditional(BuiltInComponentTypes->FloatChannel[0], &Color.RedCurve, Color.RedCurve.HasAnyData())
				.AddConditional(BuiltInComponentTypes->FloatChannel[1], &Color.GreenCurve, Color.GreenCurve.HasAnyData())
				.AddConditional(BuiltInComponentTypes->FloatChannel[2], &Color.BlueCurve, Color.BlueCurve.HasAnyData())
				.AddConditional(BuiltInComponentTypes->FloatChannel[3], &Color.AlphaCurve, Color.AlphaCurve.HasAnyData())
				.Add(LegacyComponentTypes->HybridMaterialHandle, MaterialHybrid)
				// If the section has no valid blend type (legacy data), make it use absolute blending.
				// Otherwise, the base section class will add the appropriate blend type tag in BuildDefaultComponents.
				.AddTagConditional(BuiltInComponentTypes->Tags.AbsoluteBlend, !GetBlendType().IsValid())
			);
		}
		break;
	}
	}
}

EMovieSceneChannelProxyType UMovieSceneHybridMaterialParameterSection::CacheChannelProxy()
{
	FMovieSceneChannelProxyData Channels;

#if WITH_EDITOR

	auto GetMaterialParameterDisplayText = [](const FText& ParameterName, const FString& InLayerName, const FString& InAssetName)
	{
		FText DisplayName = ParameterName;
		if (!InLayerName.IsEmpty() && !InAssetName.IsEmpty())
		{
			DisplayName = FText::Format(LOCTEXT("MaterialParameterDisplayText", "{0} ({1}.{2})"), ParameterName, FText::FromString(InLayerName), FText::FromString(InAssetName));
		}
		return DisplayName;
	};
	auto GetMaterialParameterPath = [](const FName& ParameterName, const FString& InLayerName, const FString& InAssetName)
	{
		FString Path = ParameterName.ToString();
		if (!InLayerName.IsEmpty() && !InAssetName.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s.%s.%s"), *InLayerName, *InAssetName, *ParameterName.ToString());
		}
		return Path;
	};
	auto GetMaterialParameterTooltipText = [](IMovieScenePlayer* Player, FGuid BindingID, FMovieSceneSequenceID SequenceID, FString ParameterPath)
	{
		return FText::Format(LOCTEXT("MaterialParameterPath", "Path: {0}"), FText::FromString(ParameterPath));
	};
	int32 SortOrder = 0;
	for (FScalarMaterialParameterInfoAndCurve& Scalar : ScalarParameterInfosAndCurves)
	{
		FString ParameterPath = *GetMaterialParameterPath(Scalar.ParameterInfo.Name, Scalar.ParameterLayerName, Scalar.ParameterAssetName);
		FText ParameterDisplayName = GetMaterialParameterDisplayText(FText::FromName(Scalar.ParameterInfo.Name), Scalar.ParameterLayerName, Scalar.ParameterAssetName);
		FMovieSceneChannelMetaData MetaData(*ParameterPath, ParameterDisplayName);
		// Prevent single channels from collapsing to the track node
		MetaData.GetTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath);
		MetaData.bCanCollapseToTrack = false;
		MetaData.SortOrder = SortOrder++;
		Channels.Add(Scalar.ParameterCurve, MetaData, TMovieSceneExternalValue<float>());
	}
	for (FColorMaterialParameterInfoAndCurves& Color : ColorParameterInfosAndCurves)
	{
		FString ParameterPath = GetMaterialParameterPath(Color.ParameterInfo.Name, Color.ParameterLayerName, Color.ParameterAssetName);
		FText ParameterDisplayName = GetMaterialParameterDisplayText(FText::FromName(Color.ParameterInfo.Name), Color.ParameterLayerName, Color.ParameterAssetName);
		FText Group = FText::FromString(ParameterPath);

		FMovieSceneChannelMetaData MetaData_R(*(ParameterPath + TEXT("R")), FCommonChannelData::ChannelR, Group);
		MetaData_R.GetTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath + TEXT(".R"));
		MetaData_R.SortOrder = SortOrder++;
		MetaData_R.Color = FCommonChannelData::RedChannelColor;
		MetaData_R.PropertyMetaData.Add(FCommonChannelData::GroupDisplayName, ParameterDisplayName.ToString());
		MetaData_R.GetGroupTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath);

		FMovieSceneChannelMetaData MetaData_G(*(ParameterPath + TEXT("G")), FCommonChannelData::ChannelG, Group);
		MetaData_G.GetTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath + TEXT(".G"));
		MetaData_G.SortOrder = SortOrder++;
		MetaData_G.Color = FCommonChannelData::GreenChannelColor;
		MetaData_G.PropertyMetaData.Add(FCommonChannelData::GroupDisplayName, ParameterDisplayName.ToString());
		MetaData_G.GetGroupTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath);

		FMovieSceneChannelMetaData MetaData_B(*(ParameterPath + TEXT("B")), FCommonChannelData::ChannelB, Group);
		MetaData_B.GetTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath + TEXT(".B"));
		MetaData_B.SortOrder = SortOrder++;
		MetaData_B.Color = FCommonChannelData::BlueChannelColor;
		MetaData_B.PropertyMetaData.Add(FCommonChannelData::GroupDisplayName, ParameterDisplayName.ToString());
		MetaData_B.GetGroupTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath);

		FMovieSceneChannelMetaData MetaData_A(*(ParameterPath + TEXT("A")), FCommonChannelData::ChannelA, Group);
		MetaData_A.GetTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath + TEXT(".A"));
		MetaData_A.SortOrder = SortOrder++;
		MetaData_A.PropertyMetaData.Add(FCommonChannelData::GroupDisplayName, ParameterDisplayName.ToString());
		MetaData_A.GetGroupTooltipTextDelegate.BindLambda(GetMaterialParameterTooltipText, ParameterPath);

		Channels.Add(Color.RedCurve, MetaData_R, TMovieSceneExternalValue<float>());
		Channels.Add(Color.GreenCurve, MetaData_G, TMovieSceneExternalValue<float>());
		Channels.Add(Color.BlueCurve, MetaData_B, TMovieSceneExternalValue<float>());
		Channels.Add(Color.AlphaCurve, MetaData_A, TMovieSceneExternalValue<float>());
	}
#else

	for (FScalarMaterialParameterInfoAndCurve& Scalar : ScalarParameterInfosAndCurves)
	{
		Channels.Add(Scalar.ParameterCurve);
	}

	for (FColorMaterialParameterInfoAndCurves& Color : ColorParameterInfosAndCurves)
	{
		Channels.Add(Color.RedCurve);
		Channels.Add(Color.GreenCurve);
		Channels.Add(Color.BlueCurve);
		Channels.Add(Color.AlphaCurve);
	}
#endif

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));

	return EMovieSceneChannelProxyType::Dynamic;
}

FMovieSceneFloatChannel* UMovieSceneHybridMaterialParameterSection::FindOrAddScalarParameterKey(const FName& ParametersName)
{
	FMovieSceneFloatChannel* ExistingChannel = nullptr;
	for (FScalarMaterialParameterInfoAndCurve& ScalarParameterInfoAndCurve : ScalarParameterInfosAndCurves)
	{
		if (ScalarParameterInfoAndCurve.ParameterInfo.Name == ParametersName)
		{
			ExistingChannel = &ScalarParameterInfoAndCurve.ParameterCurve;
			break;
		}
	}
	if (ExistingChannel == nullptr)
	{
		const int32 NewIndex = ScalarParameterInfosAndCurves.Add(FScalarMaterialParameterInfoAndCurve(ParametersName));
		ExistingChannel = &ScalarParameterInfosAndCurves[NewIndex].ParameterCurve;
		CacheChannelProxy();
	}
	return ExistingChannel;
}

void UMovieSceneHybridMaterialParameterSection::FindOrAddColorParameterKey(const FName& ParametersName, FMovieSceneFloatChannel*& RChannel, FMovieSceneFloatChannel*& GChannel, FMovieSceneFloatChannel*& BChannel, FMovieSceneFloatChannel*& AChannel)
{
	RChannel = nullptr;
	GChannel = nullptr;
	BChannel = nullptr;
	AChannel = nullptr;
	for (FColorMaterialParameterInfoAndCurves& ColorParameterInfoAndCurve : ColorParameterInfosAndCurves)
	{
		if (ColorParameterInfoAndCurve.ParameterInfo.Name == ParametersName)
		{
			RChannel = &ColorParameterInfoAndCurve.RedCurve;
			GChannel = &ColorParameterInfoAndCurve.GreenCurve;
			BChannel = &ColorParameterInfoAndCurve.BlueCurve;
			AChannel = &ColorParameterInfoAndCurve.AlphaCurve;
			return;
		}
	}
	{
		const int32 NewIndex = ColorParameterInfosAndCurves.Add(FColorMaterialParameterInfoAndCurves(ParametersName));
		RChannel = &ColorParameterInfosAndCurves[NewIndex].RedCurve;
		GChannel = &ColorParameterInfosAndCurves[NewIndex].GreenCurve;
		BChannel = &ColorParameterInfosAndCurves[NewIndex].BlueCurve;
		AChannel = &ColorParameterInfosAndCurves[NewIndex].AlphaCurve;
		CacheChannelProxy();
	}
}

#undef LOCTEXT_NAMESPACE
