#include "Header.h"
#include <format>
struct Cave
{
	Vector2 pos;
	int id = -1;
	std::unique_ptr<Cave*[]> linkedCavesPtr;

	Cave(int x, int y)
	{
		pos.x = x;
		pos.y = y;
		linkedCavesPtr = std::make_unique<Cave*[]>(5);
	}

	Cave(Vector2 center)
	{
		pos = center;
		linkedCavesPtr = std::make_unique<Cave * []>(5);
	}

	Cave()
	{
		linkedCavesPtr = std::make_unique<Cave*[]>(5);
	}

	bool isInsideOfNode(Vector2 mousePos, int nodeRadius)
	{
		return Vector2DistanceSqr(mousePos, pos) < nodeRadius * nodeRadius;
	}

	bool tryLinkTo(Cave* cavePtr, KeyboardKey color)
	{
		int index = getDoorIndexFromColorKey(color);
		if (linkedCavesPtr[index] != nullptr || cavePtr->linkedCavesPtr[index] != nullptr)
			return false;
		linkedCavesPtr[index] = cavePtr;
		cavePtr->linkedCavesPtr[index] = this;
		return true;
	}

	static int getDoorIndexFromColorKey(KeyboardKey k)
	{
		switch (k)
		{
			case KEY_R:
				return 0;
			case KEY_G:
				return 1;
			case KEY_B:
				return 2;
			case KEY_P:
				return 3;
			case KEY_Y:
				return 4;
			default:
				return -1;
		}
	}

	static Color getColorFromDoorIndex(int index)
	{
		switch (index)
		{
			case 0:
				return RED;
			case 1:
				return GREEN;
			case 2:
				return BLUE;
			case 3:
				return PINK;
			case 4:
				return YELLOW;
			default:
				return WHITE;
		}
	}
	static char getColorLetterFromIndex(int index)
	{
		switch (index)
		{
		case 0:
			return 'R';
		case 1:
			return 'G';
		case 2:
			return 'B';
		case 3:
			return 'P';
		case 4:
			return 'Y';
		default:
			return 'W';
		}
	}
};
