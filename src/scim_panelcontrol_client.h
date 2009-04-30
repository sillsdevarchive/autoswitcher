#ifndef SCIM_PANELCONTROL_CLIENT_H_
#define SCIM_PANELCONTROL_CLIENT_H_

#endif /*SCIM_PANELCONTROL_CLIENT_H_*/

namespace scim {

class PanelControlClient
{
	class PanelControlClientImpl;
	PanelControlClientImpl *m_impl;

	PanelControlClient (const PanelControlClient &);
	const PanelControlClient & operator = (const PanelControlClient &);

	public:
			PanelControlClient	();
			~PanelControlClient	();
	int 	open_connection();
	void 	close_connection	();
};

}