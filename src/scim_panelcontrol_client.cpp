#define Uses_SCIM_SOCKET
#define Uses_SCIM_TRANSACTION
#define Uses_SCIM_TRANS_COMMANDS

#include <X11/Xlib.h>
#include <scim.h>
#include "scim_panelcontrol_client.h"

namespace scim
{

class PanelControlClient::PanelControlClientImpl{
	int				m_socket_timeout;
	uint32			m_socket_magic_key;
	Transaction     m_send_trans;

	SocketClient	m_socket;
	Display			*m_display;
	int 			m_send_refcount;

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

	void request_factory_menu (void)
	{
		SCIM_DEBUG_MAIN(1) << "PanelControlClient::request_factory_menu ()\n";
		//focus_in()?
		prepare();
		m_send_trans.put_command (SCIM_TRANS_CMD_PANEL_REQUEST_FACTORY_MENU);
		send();
		//focus_out()?
	}

	bool prepare ()
	{
		if (!m_socket.is_connected ()) return false;

		int cmd;
		uint32 data;
		m_send_trans.clear ();
		m_send_trans.put_command (SCIM_TRANS_CMD_REQUEST);
		m_send_trans.put_data (m_socket_magic_key);

		//m_send_trans.get_command (cmd);
		//m_send_trans.get_data (data);
		return true;
	}

	bool send                   ()
	{
		if (!m_socket.is_connected ()) return false;

		if (m_send_trans.get_data_type () != SCIM_TRANS_DATA_UNKNOWN)
			return m_send_trans.write_to_socket (m_socket, 0x4d494353);

		return false;
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

bool
PanelControlClient::request_factory_menu (void)
{
	m_impl->request_factory_menu();
}
}	//namespace scim