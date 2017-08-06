#pragma once

//Grid map
#define GRID_W 20
#define GRID_H 20

//Character
#define CSPEED 150

#define JSPEED 350	//jump speed
#define GRAVITY 1300
#define JUMPLOCK 1
#define GRAVITY_ENABLED 0

#define CHAR_H 32
#define CHAR_W 24
#define CHAR_HITBOX_H 32
#define CHAR_HITBOX_W 24


//Text printer
#define PRINT_SPEED 0


/*
ShowMessage("Hello " + hero.name + ", how can I help you?")
choices = { "Open the door for me", "Tell me about yourself", "Nevermind" }
chosen = ShowChoices(choices)
if chosen == 0
    if hero.inventory["gold key"] > 0
        ShowMessage("You have the key! I'll open the door for you!")
        isGateOpen = true
    else
        ShowMessage("I'm sorry, but you need the gold key")
    end if
else if chosen == 1
    if isGateOpen
        ShowMessage("I'm the gate keeper, and the gate is open")
    else
        ShowMessage("I'm the gate keeper and you need gold key to pass")
    end if
else
    ShowMessage("Okay, tell me if you need anything")
	end if
*/