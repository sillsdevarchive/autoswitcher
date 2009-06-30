#ifndef SCIM_PANELCONTROL_CLIENT_H_
#define SCIM_PANELCONTROL_CLIENT_H_

#include <vector>
#include <string>

using namespace std;

const int SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR							= 1;
const int SCIM_AUTOSWITCHER_PANEL_MESSAGE_TIMEOUT						= 2;
const int SCIM_AUTOSWITCHER_PANEL_RECEIVED_FAULTY_TRANSACTION_HEADER	= 3;

typedef basic_string<char> SimpleString;

struct KeyboardProperties
{
	SimpleString uuid;
	SimpleString name;
	SimpleString language;
	SimpleString pathToIcon;
};

class PanelControlClient
{
	class PanelControlClientImpl;
	PanelControlClientImpl *m_impl;

	PanelControlClient (const PanelControlClient &);
	const PanelControlClient & operator = (const PanelControlClient &);

	public:
			PanelControlClient		();
			~PanelControlClient		();
	int 	open_connection			();
	void 	close_connection		();
	int 	request_factory_menu	(std::vector <KeyboardProperties>* FactoryList);
	int 	change_factory			(SimpleString uuidToChangeTo);
	int		request_current_factory ();
};

#endif /*SCIM_PANELCONTROL_CLIENT_H_*/