// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MaterialInstanceHybridFactory.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API UMaterialInstanceHybridFactory : public UFactory
{
	GENERATED_BODY()
public:
	UMaterialInstanceHybridFactory();
	virtual UObject*	FactoryCreateNew						(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
