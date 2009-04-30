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


	PanelControlClient *panelControl = new PanelControlClient();

	cout<<"Opening Connection"<<endl;
	panelControl->open_connection();
	cout<<"Closing Connection"<<endl;
	panelControl->close_connection();
	delete panelControl;
	return 0;
}