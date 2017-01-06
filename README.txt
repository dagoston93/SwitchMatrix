# SwitchMatrix
Switch matrix library

 file: ReadMe.txt
 author: Agoston Diamont
 date: 06 January 2017
 contact: dagoston93@gmail.com

 description of library:

 Switch matrix library is designed to read tactile and toggle swithes
 arranged in matrix. In order to avoid "ghosting" and masking
 use diodes e.g.: 1N4148 with each switch. 
 The diodes are crutial!
 I created the first version for my own purposes and then thought it might be useful
 for someone so I modified it so that it is a bit more universal, and I share it happily with everyone
 I hope it will be useful for some of you.
 I am not a professional programmer, I am just a hobbyist, so this library may contain some mistakes,
 and might not be the best solution, but for me it works perfectly, and i think if you use it 
 as in the description, it will work for you too.
 Feel free to modify anything, and if you have a good idea, I am happy to hear about,
 just contact me via the email above.

 To use this library you have to set the size of the matrix and the
 pins used in the SwitchMatrix.h, because I could not make it possible
 to set them from outside the class
 So obviously if you wish to use it in multiple projects,
 you need to make a copy of the lib and give it a different name.
 If you have any idea to overcome this problem, please contact me
 on the e-mail above.

 If you need a library in which you can give char type 'names'
 of each buttons, or this library doesn't provide enough functionality for you,
 I recommend the Keypad library <http://playground.arduino.cc/Code/Keypad>.


 This library has four functions:

 void Scan() :

 This function is used to check the state of each scan
 and call an event listner if the state of the switch has changed
 Call this function in each loop to keep monitoring the switches

 SwitchState GetSwitchState(int row, int col):

 This function returns the state of the switch specified in the call.
 Parameters are the row and coloumn of the requested switch int the matrix.
 The return type SwitchState is an enumerated type, defined in the header file.
 It can have four values: FALSE, TRUE, DEBOUNCE, NONSET.
 This actually extends the standard bool type, and can be compared and converted to
 logical bool.

 void GetAllSwitchStates(SwitchState (&arrayToFill)[numberOfRows][numberOfCols]);

 This functions takes an array thats bounds match the number of rows and the
 number of coloumns of the matrix by reference, and fills it with the state of each
 switches in the matrix.

 void AddEventListener(void (*listener)(STATUS)):
 This function sets an event listener which is being called by the Scan() function
 in case a switch has been turned on or off.
 The function has to take one STATUS type parameter.
 STATUS is a struct defined in the header file.
 it contains two byte type values: row, col, which are the coloumn and row of the switches
 that have been flipped, and a SwitchState type which is the new state of the switch


 license:

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
