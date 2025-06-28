// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialInstanceHybrid.h"

UMaterialInstanceHybrid::UMaterialInstanceHybrid()
{
	ConstantBase = CreateDefaultSubobject<UMaterialInstanceConstant_Instanced>(TEXT("ConstantBase"));
	Parent = ConstantBase;
}

void UMaterialInstanceHybrid::PostLoad()
{
	Super::PostLoad();
	UMaterialInterface* LastParent = Parent;
	Parent = nullptr;
	InitializeMID(LastParent);
}

void UMaterialInstanceHybrid::InitializeMID(class UMaterialInterface* ParentMaterial)
{
	Super::InitializeMID(ParentMaterial);
}
