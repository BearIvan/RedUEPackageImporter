#pragma once
#include "XMatineeCameraActor.generated.h"

class UCameraComponent;
UCLASS()
class REDUELEGACYRUNTIME_API AXMatineeCameraActor : public AActor
{
	GENERATED_BODY()

public:
					AXMatineeCameraActor	();
	virtual void	Tick					(float DeltaTime) override;

protected:
	virtual void	BeginPlay				() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent*	Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent*	SkeletalMesh;
};
