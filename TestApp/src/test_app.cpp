#include <iostream>
#include <cstdlib>
#include <scim_panelcontrol_client.h>

using namespace std;

int main (int argc, char *argv [])
{
	while(1){
	KeyboardProperties listOfAvailableKeyboards[20];

	cout << "Checking if connection is open..." << endl;
	cout << ConnectionToScimPanelIsOpen() << endl;
	cout << "Opening Connection" << endl;

	if(!OpenConnectionToScimPanel()){
		cout << "Failed to open connection to Panel";
		return 0;
	}


	cout << "Checking if connection is open..." << endl;
	cout << ConnectionToScimPanelIsOpen() << endl;

	KeyboardProperties currentKeyboard;
	if(GetCurrentKeyboard(&currentKeyboard) != 0){
		cout << "Failed to get the current keyboard.";
		return 0;
	}
	cout << "Current keyboard is:" << currentKeyboard.name << endl;

	cout << "Requesting Factory Menu" << endl;
	int numAvailableKeyboards = 0;
	GetListOfSupportedKeyboards(listOfAvailableKeyboards, 20, &numAvailableKeyboards);
	cout << "Number of supported keyboards:" << numAvailableKeyboards << endl;
	KeyboardProperties keyboardProperties;
	for(int i=0;i < numAvailableKeyboards; ++i){
		keyboardProperties = listOfAvailableKeyboards[i];
		cout << i << " " << keyboardProperties.name << endl;
	}
	cout << "Anything else:" << " " << listOfAvailableKeyboards[0].name << endl;
	cout << "Please select the factory you would like ";
	char selected_keyboard[20];
	scanf("%s", selected_keyboard);
	int selected_keyboard_as_int = atoi(selected_keyboard);
	if(numAvailableKeyboards< selected_keyboard_as_int) selected_keyboard_as_int = 0;
	if(SetKeyboard(listOfAvailableKeyboards[selected_keyboard_as_int].uuid) != 0){
		cout << "Error trying to set keyboard!" << endl;
		return 0;
	}

	if(GetCurrentKeyboard(&currentKeyboard) != 0){
		cout << "Failed to get the current keyboard.";
		return 0;
	}
	cout << "Switched keyboard to: " << currentKeyboard.name << endl;

	cout << "Closing Connection" << endl;
	cout << "Closing status: " << CloseConnectionToScimPanel() << endl;

	char quit[80];
	cout << "Quit?\n 1.Yes\n Anything else: No\n" ;
	scanf("%s", quit);
	if(atoi(quit) == 1)break;
	}

	return 0;
}