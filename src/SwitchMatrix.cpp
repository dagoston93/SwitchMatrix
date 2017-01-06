/*
* file: SwitchMatrix.cpp
* author: Agoston Diamont
* date: 06 January 2017
* contact: dagoston93@gmail.com
*
* description of library:
*
* Switch matrix library is designed to read tactile and toggle swithes
* arranged in matrix. In order to avoid "ghosting" and masking
* use diodes e.g.: 1N4148 with each switch
*
* To use this library you have to set the size of the matrix and the
* pins used in the SwitchMatrix.h, because I could not make it possible
* to set them from outside the class
* So obviously if you wish to use it in multiple projects,
* you need to make a copy of the lib and give it a different name.
* If you have any idea to overcome this problem, feel free to contact me
* on the e-mail above
*
* This library has four functions:
*
* ***** void Scan() :
*
* This function is used to check the state of each scan
* and call an event listner if the state of the switch has changed
* Call this function in each loop to keep monitoring the switches
*
* ***** SwitchState GetSwitchState(int row, int col):
*
* This function returns the state of the switch specified in the call.
* Parameters are the row and coloumn of the requested switch int the matrix.
* The return type SwitchState is an enumerated type, defined in the header file.
* It can have four values: FALSE, TRUE, DEBOUNCE, NONSET.
* This actually extends the standard bool type, and can be compared and converted to
* logical bool.
*
* *****	void GetAllSwitchStates(SwitchState (&arrayToFill)[numberOfRows][numberOfCols]);
*
* This functions takes an array thats bounds match the number of rows and the
* number of coloumns of the matrix by reference, and fills it with the state of each
* switches in the matrix.
*
* ***** void AddEventListener(void (*listener)(STATUS)):
*
* This function sets an event listener which is being called by the Scan() function
* in case a switch has been turned on or off.
* The function has to take one STATUS type parameter.
* STATUS is a struct defined in the header file.
* it contains two byte type values: row, col, which are the coloumn and row of the switches
* that have been flipped, and a SwitchState type which is the new state of the switch
*
* description of file:
*
* This file is the source file of Switch Matrix library.
*
* license:
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "SwitchMatrix.h"

//The constructor
SwitchMatrix::SwitchMatrix()
{
	//Initialize the pins and the switch structs
	for (byte r = 0; r < numberOfRows; r++)
	{
		//Activate the internal pullup resistors
		pinMode(rowPins[r], INPUT_PULLUP);

		for (byte c = 0; c < numberOfCols; c++)
		{
			//Set the cols as high impedance input
			pinMode(colPins[c], OUTPUT);
			digitalWrite(colPins[c], HIGH);
			pinMode(colPins[c], INPUT);

			//Set the state of the switch NONSET
			switches[r][c].currentState = NONSET;
		}
	}

	//Initially set the event listener to 0
	eventListener = 0;

}

//Scanning the switches
void SwitchMatrix::Scan()
{
	//Loop through the cols and activate them one by one
	//Then read the state of the rows
	//Then reset the state of the cols
	for (byte c = 0; c < numberOfCols; c++)
	{
		pinMode(colPins[c], OUTPUT);
		digitalWrite(colPins[c], LOW);

		for (byte r = 0; r < numberOfRows; r++)
		{
			//We save the current read value of the switch
			//We invert because we read LOW if the switch is active
			bool switchState = (bool)!(digitalRead(rowPins[r]));
			//If the switches haven't been given the initial value yet, we give them
			//Here we don't need to debounce yet
			if (switches[r][c].currentState == NONSET)
			{
				switches[r][c].currentState = (SwitchState)switchState;
			}
			//If the switch is being debounced
			else if (switches[r][c].currentState == DEBOUNCE)
			{
				//We check if the debounce time is over or not
				if (millis() >= (switches[r][c].debounceBeginTime + debounceTime))
				{
					//Then we check if the debounce state is the same as current or not
					if (switches[r][c].debounceState == switchState)
					{
						//If yes we save that state
						switches[r][c].currentState = (SwitchState)switches[r][c].debounceState;
						//If an event listener is set, we call it
						if (eventListener != NULL)
						{
							eventListener({r, c, switches[r][c].currentState });
						}
					}
					else
					{
						//If not, we reset the original state which is the opposite of the
						//state we are debouncing
						switches[r][c].currentState = (SwitchState)!switches[r][c].debounceState;
					}
				}
			}
			//If it is TRUE OR FALSE
			else
			{
				//If it doesn't match the prev state, we start debouncing
				if (switches[r][c].currentState != switchState)
				{
					switches[r][c].currentState = DEBOUNCE;
					switches[r][c].debounceBeginTime = millis();
					switches[r][c].debounceState = switchState;
				}
			}
		}
		//Reset the col pins
		digitalWrite(colPins[c], HIGH);
		pinMode(colPins[c], INPUT);
	}
}

//Get the state of an individual switch
SwitchState SwitchMatrix::GetSwitchState(int row, int col)
{
	return switches[row][col].currentState;
}


//Take an array by reference and fill it with the states of the switches
void SwitchMatrix::GetAllSwitchStates(SwitchState(&arrayToFill)[numberOfRows][numberOfCols])
{
	for (int r = 0; r < numberOfRows; r++)
	{
		for (int c = 0; c < numberOfCols; c++)
		{
			arrayToFill[r][c] = switches[r][c].currentState;
		}
	}
}

//Add an event listener
void SwitchMatrix::AddEventListener(void(*listener)(STATUS))
{
	eventListener = listener;
}

