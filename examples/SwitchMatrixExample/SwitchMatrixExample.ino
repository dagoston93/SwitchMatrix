/*
* file: SwitchMatrixTest.ino
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
* This file is an example of how to use Switch Matrix library.
* 
* In the example we will call the Scan() in each loop,
* And call the GetAllSwitchStates() and GetSwitchState() every three seconds,
* and write their values to the serial port.
*
* We also add an event listener, which will write to serial
* which switch has been flipped, and the new state of it.
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
//Create an instance of the switch matrix
SwitchMatrix swPanel = SwitchMatrix();
//Declare the function we will use as event listener
void testEventListener(STATUS sData);
//We use this to call the GetAllSwitchStates() and GetSwitchState() every three seconds
unsigned long lastTime = millis();

void setup()
{
	Serial.begin(9600);
	//Adding the event listener
	swPanel.AddEventListener(testEventListener);

}

void loop()
{
	//Scan the switches
	swPanel.Scan();
	//Check if the 3 seconds is over
	if(millis() > lastTime + 3000)
	{
		//If so create an array thats bounds match the rows and the cols of the 
		//switch matrix
		SwitchState states[2][8];
		//Then pass this array to the GetAllSwitchStates()
		//to have it filled with the values of each switches
		swPanel.GetAllSwitchStates(states);
		//Then loop through the array and write everything to serial
		for (int r = 0; r < 2; r++)
		{
			for (int c = 0; c < 8; c++)
			{
				Serial.print(r);
				Serial.print(", ");
				Serial.print(c);
				Serial.print(": ");
				Serial.print((bool)states[r][c]);
				Serial.print(" | ");
			}

			Serial.println();
		}
		//Next we get the state of switch 5 in row 0 and show its state in the serial
		SwitchState mySwitchState = swPanel.GetSwitchState(0, 5);
		Serial.print("The state of 0,5: ");
		Serial.println((bool)mySwitchState);
		//Finallay update the variable that stores when this was done last time
		lastTime = millis();
	}

	
}

//This is the definition of the earlier declared event handler function
void testEventListener(STATUS sData)
{
	Serial.print(sData.row);
	Serial.print(", ");
	Serial.print(sData.col);
	Serial.print(": ");
	Serial.println((bool)sData.state); 
}
