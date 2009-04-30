#define Uses_SCIM_SOCKET

#include <X11/Xlib.h>
#include <scim.h>
#include "scim_panelcontrol_client.h"

namespace scim
{

class PanelControlClient::PanelControlClientImpl{
	int				m_socket_timeout;
	uint32			m_socket_magic_key;

	SocketClient	m_socket;
	Display			*m_display;

public:
	PanelControlClientImpl()
		:	m_socket_timeout (scim_get_default_socket_timeout ()),
			m_socket_magic_key (0)
	{
	}

int  open_connection        ()
	{
		m_display = XOpenDisplay (NULL);

		if (!m_display)
			throw Exception (String ("X11 -- Cannot open Display!"));

		SocketAddress addr (scim_get_default_panel_socket_address (String DisplayString(m_display)));

		if (m_socket.is_connected ()) close_connection ();

		bool ret;
		int count = 0;

		// Try three times.
		while (1) {
			if ((ret = m_socket.connect (addr)) == false) {
				scim_usleep (100000);
				for (int i = 0; i < 200; ++i) {
					if (m_socket.connect (addr)) {
						ret = true;
						break;
					}
					scim_usleep (100000);
				}
			}

			if (ret && scim_socket_open_connection (m_socket_magic_key, String ("PanelController"), String ("Panel"), m_socket, m_socket_timeout))
				break;

			m_socket.close ();

			if (count++ >= 3) break;

			scim_usleep (100000);
		}

		return m_socket.get_id ();
	}

	void close_connection       ()
	{
		m_socket.close ();
		m_socket_magic_key = 0;
	}
};

PanelControlClient::PanelControlClient ()
	: m_impl (new PanelControlClientImpl ())
{
}

PanelControlClient::~PanelControlClient ()
{
	delete m_impl;
}

int
PanelControlClient::open_connection ()
{
	return m_impl->open_connection ();
}

void
PanelControlClient::close_connection ()
{
	m_impl->close_connection ();
}

}	//namespace scim