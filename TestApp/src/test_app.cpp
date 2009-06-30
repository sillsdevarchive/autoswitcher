#include <scim.h>
#include "scim_panelcontrol_client.h"

namespace scim{

class ScimAutoSwitcher
{
	ScimAutoSwitcher();
	~ScimAutoSwitcher();
};

ScimAutoSwitcher::ScimAutoSwitcher()
{
};

ScimAutoSwitcher::~ScimAutoSwitcher()
{
};

}

using namespace scim;
using namespace std;

int main (int argc, char *argv [])
{
	while(1){
	vector<PanelFactoryInfo> *factoryList = new vector<PanelFactoryInfo>();
	PanelControlClient *panelControl = new PanelControlClient();


	cout<<"Opening Connection"<<endl;

	if(!panelControl->open_connection()){
		cout << "Failed to open connection to Panel";
		return 0;
	}

	cout<<"Asking for current Factory:"<<endl;
	cout<<panelControl->request_current_factory();

	cout<<"Requesting Factory Menu"<<endl;
	cout << panelControl->request_factory_menu(factoryList)<<endl;
	PanelFactoryInfo factoryInfo;
	for(unsigned int i=0;i < factoryList->size(); ++i){
		factoryInfo = (*factoryList)[i];
		cout << i << " " << factoryInfo.name << endl;
	}
	cout << "Anything else:" << " " << (*factoryList)[0].name << endl;
	cout << "Please select the factory you would like";
	char selected_factory[20];
	scanf("%s", selected_factory);
	int selected_factory_as_int = atoi(selected_factory);
	cout << panelControl->change_factory((*factoryList)[selected_factory_as_int].uuid);

	cout<<"Closing Connection"<<endl;
	panelControl->close_connection();

	delete factoryList;
	delete panelControl;
	factoryList = 0;
	panelControl = 0;

	char quit[80];
	cout << "Quit?\n 1.Yes\n Anything else: No\n" ;
	scanf("%s", quit);
	if(atoi(quit) == 1)break;
	}

	return 0;
}