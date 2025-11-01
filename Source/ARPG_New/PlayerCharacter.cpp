#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"  // For overlap detection
#include "GameFramework/CharacterMovementComponent.h"  // Movement basics
#include "Kismet/GameplayStatics.h"  // For damage application

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;  // Tick for input checks

    // Attack overlap sphere—like a bubble around the sword
    AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
    AttackSphere->SetupAttachment(RootComponent);
    AttackSphere->SetSphereRadius(100.0f);  // Small radius for melee
    AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Off by default
}

// Called every frame—efficient, only checks input
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsAttacking) {
        // Simple timer: Attack lasts 0.5s, then off
        AttackTime += DeltaTime;
        if (AttackTime >= 0.5f) {
            EndAttack();
        }
    }
}

// Input hook—binds to "Attack" action
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::StartAttack);
}

// Start attack: Enable sphere, deal damage
void APlayerCharacter::StartAttack()
{
    if (!bIsAttacking) {  // Prevent spam—cooldown feel
        bIsAttacking = true;
        AttackTime = 0.0f;
        AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // Detect overlaps

        // Overlap event: Like a net catching fish
        AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnAttackOverlap);

        // Visual: Rotate weapon (call BP event for animation later)
        UE_LOG(LogTemp, Warning, TEXT("Player Attacks!"));  // Debug print
    }
}

void APlayerCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Assume enemy has AEnemy class with TakeDamage func
    if (AActor* Enemy = OtherActor) {
        UGameplayStatics::ApplyDamage(Enemy, 10.0f, nullptr, this, nullptr);  // 10 base damage
    }
}

void APlayerCharacter::EndAttack()
{
    bIsAttacking = false;
    AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AttackSphere->OnComponentBeginOverlap.RemoveDynamic(this, &APlayerCharacter::OnAttackOverlap);
}