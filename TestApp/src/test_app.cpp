#include <iostream>
#include <cstdlib>
#include <scim_panelcontrol_client.h>

using namespace std;

int main (int argc, char *argv [])
{
	while(1){
	KeyboardProperties listOfAvailableKeyboards[20];

	cout << "Opening Connection" << endl;

	if(!OpenConnectionToScimPanel()){
		cout << "Failed to open connection to Panel";
		return 0;
	}

	cout << "Asking for current Factory:" << endl;
	cout << GetCurrentKeyboard();

	cout << "Requesting Factory Menu" << endl;
	int numAvailableKeyboards = 0;
	GetListOfSupportedKeyboards(listOfAvailableKeyboards, 20, &numAvailableKeyboards) << endl;
	cout << "Number of supported keyboards:" << numAvailableKeyboards << endl;
	KeyboardProperties keyboardProperties;
	for(int i=0;i < numAvailableKeyboards; ++i){
		keyboardProperties = listOfAvailableKeyboards[i];
		cout << i << " " << keyboardProperties.name << endl;
	}
	cout << "Anything else:" << " " << listOfAvailableKeyboards[0].name << endl;
	cout << "Please select the factory you would like";
	char selected_keyboard[20];
	scanf("%s", selected_keyboard);
	int selected_keyboard_as_int = atoi(selected_keyboard);
	cout << SetKeyboard(listOfAvailableKeyboards[selected_keyboard_as_int].uuid);

	cout << "Closing Connection" << endl;
	CloseConnectionToScimPanel();

	char quit[80];
	cout << "Quit?\n 1.Yes\n Anything else: No\n" ;
	scanf("%s", quit);
	if(atoi(quit) == 1)break;
	}

	return 0;
}