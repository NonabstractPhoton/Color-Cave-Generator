#pragma once
#include "Header.h"
#include "Cave.cpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    constexpr int xDim = 1600, yDim = 900;
    constexpr int nodeRadius = 30, doorOffsetMult = 4;
    constexpr Color nodeColor = WHITE;
    std::string cavePath;
    ofstream file;
    int nextId = 0;
    vector<Cave> caves(40);
    int IdsToJoin[2] = { -1, - 1 };
    KeyboardKey doorColor = KEY_NULL;
    const KeyboardKey doorColors[5] = {KEY_R, KEY_G, KEY_B, KEY_P, KEY_Y};
    Vector2 mousePos = { 0, 0 };
    bool markedForDeletion = false;

    cout << "Welcome to the Cave Maker. Click to create a Room, hold R G B B Y and click on another room to create the respective color door.\n";
    cout << "The root node is always the start, and the final node placed is the end.\n";
    
    if (argc == 1)
    {
        cout << "Please Enter the Path to Cave.cave: ";
        cin >> cavePath;

        try
        {
            file.open(cavePath, ios::out | ios::trunc); // Output, truncate original contents
        }
        catch (...)
        {
            cout << "Invalid File";
            file.close();
            exit(1);
        }
        
    }
    else if (argc == 2)
    {
        try
        {
            file.open(string(argv[1]), ios::out | ios::trunc); // Output, truncate original contents
        }
        catch (...)
        {
            cout << "Invalid File";
            file.close();
            exit(1);
        }
    }
    else
        exit(1);

    InitWindow(xDim, yDim, "Cave Creator");
    SetTargetFPS(30);

    // Create Root
    caves[nextId++] = Cave(xDim/2, yDim/2);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            mousePos = GetMousePosition();

            for (KeyboardKey key : doorColors)
            {
                if (IsKeyDown(key)) 
                {
                    doorColor = key;
                }
            }
            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                markedForDeletion = true;
                doorColor = KEY_NULL;
            }

            if (doorColor == KEY_NULL && !markedForDeletion && nextId < 40) // No other button pressed and can make node
            {
                caves[nextId++] = Cave(mousePos);
            }
            
        }

        for (int i = 0; i < nextId; i++)
        {
            Cave& currentCave = caves.at(i);
            bool clickedOnCave = currentCave.isInsideOfNode(mousePos, nodeRadius);
            if (clickedOnCave)
            {
                if (markedForDeletion)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        if (currentCave.linkedCavesPtr[j] != nullptr) // If this door points to a room
                        {
                            currentCave.linkedCavesPtr[j]->linkedCavesPtr[j] = nullptr; // Set this door for that room to null
                        }
                    }
                    caves.erase(caves.begin() + i);
                    nextId--;
                    break;
                }

                if (doorColor != KEY_NULL) // If Door Key Selected
                {   
                    // Set id in appropriate position for joining
                    if (IdsToJoin[0] == -1)
                        IdsToJoin[0] = i;
                    else if (IdsToJoin[1] == -1)
                        IdsToJoin[1] = i;

                    if (IdsToJoin[0] != -1 && IdsToJoin[1] != -1) // If both filled
                    {
                        caves[IdsToJoin[0]].tryLinkTo(&caves[IdsToJoin[1]], doorColor); // Link caves

                        // Reset
                        IdsToJoin[0] = -1;
                        IdsToJoin[1] = -1;
                        doorColor = KEY_NULL;
                    }
                }
            }

            DrawCircle(currentCave.pos.x, currentCave.pos.y, nodeRadius, nodeColor); // Draw Node

            // Draw Doors
            for (int i = 0; i < 5; i++)
            {
                if (currentCave.linkedCavesPtr[i] != nullptr)
                {
                    DrawLine(currentCave.pos.x, currentCave.pos.y + doorOffsetMult*i,
                        currentCave.linkedCavesPtr[i]->pos.x, currentCave.linkedCavesPtr[i]->pos.y + doorOffsetMult*i,
                        Cave::getColorFromDoorIndex(i));
                }
            }

        }

        
        DrawText(to_string(nextId).c_str(), 50, 50, 24, PURPLE);

        doorColor = KEY_NULL;
        markedForDeletion = false;
        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();

    for (int i = 0; i < nextId; i++)
        caves[i].id = i;

    // Write to Cave file
    for (int i = 0; i < nextId; i++) // For each cave
    {
        file << "Name, Description, ";
        for (int j = 0; j < 5; j++) // For each door
        {
            auto linkedCave = caves[i].linkedCavesPtr[j];
            file << Cave::getColorLetterFromIndex(j) << ":";
            if (linkedCave != nullptr)
            {
                file << linkedCave->id << (j == 4 ? '\n' : ','); // If not last entry ends with comma, else new line
            }
            else
            {
                file << "-1" << (j == 4 ? '\n' : ',');
            }
        }
    }

    file.close();

    

    return 0;
}