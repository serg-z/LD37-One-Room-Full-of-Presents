#include "LD37OneRoom.h"
#include "LD37OneRoomGameMode.h"

ALD37OneRoomGameMode::ALD37OneRoomGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/LD37/Boy/BoyCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
