/*
* file: SwitchMatrix.h
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
* This file is the header file of Switch Matrix library.
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

#ifndef _SWICHMATRIX_h
#define _SWICHMATRIX_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


//The possible states of a switch
typedef enum { FALSE, TRUE, DEBOUNCE, NONSET } SwitchState;
//And we need to overload the == operator to be able to compare it to a logic variable
inline bool operator== (SwitchState lhs, bool rhs)
{
	if (lhs == FALSE && !rhs)
		return true;
	if (lhs == TRUE && rhs)
		return true;

	return false;
}
//And overload also the !=
//We are not supposed to use the DEBOUNCE or NONSET in neither overload
inline bool operator!= (SwitchState lhs, bool rhs)
{
	if (lhs == FALSE && !rhs)
		return false;
	if (lhs == TRUE && rhs)
		return false;

	return true;
}
//This struct represents a switch
typedef struct
{
	SwitchState currentState;
	unsigned long debounceBeginTime;
	bool debounceState;
} Switch;

//This struct is used when returning the status of an actual switch on user request
typedef struct
{
	byte row;
	byte col;
	SwitchState state;
} STATUS;

//This is the class for the switchpanel
class SwitchMatrix
{

private:
	//Setup the size of the switch panel
	static const byte numberOfCols = 8;
	static const byte numberOfRows = 2;
	//Setup the pins 
	const byte rowPins[numberOfRows] = { 2, 3 };
	const byte colPins[numberOfCols] = { 14,15,16,17,18,19,20,21 };
	//The debounce time in milliseconds
	static const byte debounceTime = 10;
	//Create the matrix for the switches
	Switch switches[numberOfRows][numberOfCols];
	//The pointer to an event handler function
	void(*eventListener)(STATUS);

public:
	SwitchMatrix();
	void Scan();
	SwitchState GetSwitchState(int row, int col);
	void GetAllSwitchStates(SwitchState (&arrayToFill)[numberOfRows][numberOfCols]);
	void AddEventListener(void (*listener)(STATUS));
};



#endif

