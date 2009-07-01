#include <iostream>
#include <cstdlib>
#include <scim_panelcontrol_client.h>

using namespace std;

int main (int argc, char *argv [])
{
	while(1){
	vector<KeyboardProperties> *factoryList = new vector<KeyboardProperties>();
	//PanelControlClient *panelControl = new PanelControlClient();

	cout<<"Opening Connection"<<endl;

	if(!open_connection()){
		cout << "Failed to open connection to Panel";
		return 0;
	}

	cout<<"Asking for current Factory:"<<endl;
	cout<<request_current_factory();

	cout<<"Requesting Factory Menu"<<endl;
	cout << request_factory_menu(factoryList)<<endl;
	KeyboardProperties keyboardProperties;
	for(unsigned int i=0;i < factoryList->size(); ++i){
		keyboardProperties = (*factoryList)[i];
		cout << i << " " << keyboardProperties.name << endl;
	}
	cout << "Anything else:" << " " << (*factoryList)[0].name << endl;
	cout << "Please select the factory you would like";
	char selected_keyboard[20];
	scanf("%s", selected_keyboard);
	int selected_keyboard_as_int = atoi(selected_keyboard);
	cout << change_factory((*factoryList)[selected_keyboard_as_int].uuid);

	cout<<"Closing Connection"<<endl;
	close_connection();

	delete factoryList;
	//delete panelControl;
	factoryList = 0;
	//panelControl = 0;

	char quit[80];
	cout << "Quit?\n 1.Yes\n Anything else: No\n" ;
	scanf("%s", quit);
	if(atoi(quit) == 1)break;
	}

	return 0;
}