#include "XMatineeCameraActor.h"
#include "Camera/CameraComponent.h"

AXMatineeCameraActor::AXMatineeCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SetRootComponent(SkeletalMesh);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SkeletalMesh,"CameraOffset");
}

void AXMatineeCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

void AXMatineeCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

