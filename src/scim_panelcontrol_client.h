#ifndef SCIM_PANELCONTROL_CLIENT_H_
#define SCIM_PANELCONTROL_CLIENT_H_

#include <scim_panel_common.h>

namespace scim {

const int SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR							= 1;
const int SCIM_AUTOSWITCHER_PANEL_MESSAGE_TIMEOUT						= 2;
const int SCIM_AUTOSWITCHER_PANEL_RECEIVED_FAULTY_TRANSACTION_HEADER	= 3;

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
	int 	request_factory_menu	(std::vector <PanelFactoryInfo>* FactoryList);
};

} //namespace scim

#endif /*SCIM_PANELCONTROL_CLIENT_H_*/