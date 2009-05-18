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
	vector<PanelFactoryInfo>* FactoryList = new vector<PanelFactoryInfo>();

	PanelControlClient *panelControl = new PanelControlClient();

	cout<<"Opening Connection"<<endl;
	panelControl->open_connection();

	cout<<"Requesting Factory Menu"<<endl;
	cout << panelControl->request_factory_menu(FactoryList)<<endl;
	delete FactoryList;
	FactoryList = 0;
	cout<<"Closing Connection"<<endl;
	panelControl->close_connection();
	delete panelControl;
	return 0;
}