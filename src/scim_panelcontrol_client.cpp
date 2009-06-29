#define Uses_SCIM_SOCKET
#define Uses_SCIM_TRANSACTION
#define Uses_SCIM_TRANS_COMMANDS

#include <X11/Xlib.h>
#include <scim.h>
#include "scim_panelcontrol_client.h"

using namespace std;

namespace scim
{

class PanelControlClient::PanelControlClientImpl{
	int				m_socket_timeout;
	uint32			m_socket_magic_key;
	Transaction     m_send_trans;
	Transaction     m_recv_trans;

	SocketClient	m_socket;
	Display			*m_display;
	int 			m_send_refcount;

public:
	PanelControlClientImpl()
		:	m_socket_timeout (scim_get_default_socket_timeout ()),
			m_socket_magic_key (0)
	{
	}

bool  open_connection        ()
	{
		bool connection_opened = false;

		m_display = XOpenDisplay (NULL);

		if (!m_display)
			throw Exception (String ("X11 -- Cannot open Display!"));

		SocketAddress addr (scim_get_default_panel_socket_address (String DisplayString(m_display)));

		if (m_socket.connect (addr)){
			if (scim_socket_open_connection (m_socket_magic_key, String ("PanelController"), String ("Panel"), m_socket, m_socket_timeout)){
				connection_opened = true;
			}
			else{
				if (m_socket.is_connected ()) close_connection ();
			}
		}

		return connection_opened;
	}

	void close_connection       ()
	{
		m_socket.close ();
		m_socket_magic_key = 0;
	}

	int request_factory_menu (vector <PanelFactoryInfo>* FactoryList)
	{
		int return_status = 0;
		SCIM_DEBUG_MAIN(1) << "PanelControlClient::request_factory_menu ()\n";
		prepare();
		m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_REQUEST_FACTORY_MENU);
		send();

		cout << "Transaction sent!" << endl;

		return_status = wait_for_response_from_agent();

		if (return_status == 0){
			cout << "Got a response from the panel!" << endl;

			return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_SHOW_FACTORY_MENU);
			if (return_status == 0){
				cout << "Happily read the transaction header!" << endl;

				PanelFactoryInfo info;
				while (m_recv_trans.get_data (info.uuid) && m_recv_trans.get_data (info.name) &&
					   m_recv_trans.get_data (info.lang) && m_recv_trans.get_data (info.icon)) {
					info.lang = scim_get_normalized_language (info.lang);
					FactoryList->push_back (info);
					cout << "Happily read a factory!" << endl;
					cout << info.name << info.uuid << endl;
				}
			}
		}

		return return_status;
	}

	int change_factory (String uuid_to_change_to)
	{
		int return_status = 0;
		cout << "PanelControlClient::change_factory () to " << uuid_to_change_to << "\n";
		prepare();
		m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_CHANGE_FACTORY);
		m_send_trans.put_data (uuid_to_change_to);
		send();

		return_status = wait_for_response_from_agent();

		if (return_status == 0){
			cout << "Got a response from the panel!" << endl;

			return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_UPDATE_FACTORY_INFO);
			if (return_status == 0){
				cout << "Happily read the transaction header!" << endl;

				PanelFactoryInfo info;
				if (m_recv_trans.get_data (info.uuid) && m_recv_trans.get_data (info.name) &&
					m_recv_trans.get_data (info.lang) && m_recv_trans.get_data (info.icon)) {
					cout << "Switched to new Factory:  uuid=" << info.uuid << " name=" << info.name << "\n";
					info.lang = scim_get_normalized_language (info.lang);
				}
			}
		}

		return return_status;
	}

	int request_current_factory ()
	{
		int return_status = 0;
		cout << "PanelControlClient::request_current_factory ()\n";
		prepare();
		m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_GET_CURRENT_FACTORY);
		send();

		return_status = wait_for_response_from_agent();

		if (return_status == 0){
			cout << "Got a response from the panel!" << endl;

			return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_RETURN_CURRENT_FACTORY_INFO);
			if (return_status == 0){
				cout << "Happily read the transaction header!" << endl;

				PanelFactoryInfo info;
				if (m_recv_trans.get_data (info.uuid) && m_recv_trans.get_data (info.name) &&
					m_recv_trans.get_data (info.lang) && m_recv_trans.get_data (info.icon)) {
					info.lang = scim_get_normalized_language (info.lang);
					cout << "Current Factory is:  uuid=" << info.uuid << " name=" << info.name << "\n";
				}
			}
		}

		return return_status;
	}

	int wait_for_response_from_agent(){
		int return_status = 0;

		fd_set active_fds;
		timeval	timeout;
		int panel_fd = get_connection_number();

		FD_ZERO(&active_fds);
		FD_SET(panel_fd, &active_fds);

		timeout.tv_sec = 0;
		timeout.tv_usec = 3000 * 1000; //1000 milliseconds timeout

		int select_result = select (panel_fd + 1, &active_fds, NULL, NULL, &timeout);

		if(select_result < 0){
			return_status = SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR;
		}
		else if(select_result == 0){
			return_status = SCIM_AUTOSWITCHER_PANEL_MESSAGE_TIMEOUT;
		}
		return return_status;
	}

	int verify_transaction_header(int expected_cmd){
		int return_status = 0;
		if (!m_socket.is_connected () || !m_recv_trans.read_from_socket (m_socket, m_socket_timeout))
			return SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR;

		int cmd_type;
		int cmd;
		uint32 context = (uint32)(-1);

		bool command_is_a_reply = (m_recv_trans.get_command (cmd_type)) && (cmd_type == SCIM_TRANS_CMD_REPLY);
		bool context_is_correct = (m_recv_trans.get_data (context)) /*&& context != SCIM_TRANS_CMD_REPLY &&  Fix Me! --TA*/;
		bool command_is_expected_command = (m_recv_trans.get_command (cmd))  && (cmd == expected_cmd);

		if ( !(command_is_a_reply &&
			 context_is_correct &&
			 command_is_expected_command))
			return_status = SCIM_AUTOSWITCHER_PANEL_RECEIVED_FAULTY_TRANSACTION_HEADER;
		return return_status;
	}

	int  get_connection_number  () const
	{
		return m_socket.get_id ();
	}

	bool prepare ()
	{
		if (!m_socket.is_connected ()) return false;

		m_send_trans.clear ();
		m_send_trans.put_command (SCIM_TRANS_CMD_REQUEST);
		m_send_trans.put_data (m_socket_magic_key);
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

int
PanelControlClient::request_factory_menu (std::vector <PanelFactoryInfo>* FactoryList)
{
	return m_impl->request_factory_menu(FactoryList);
}

int
PanelControlClient::change_factory (String uuid){
	return m_impl->change_factory(uuid);
}

int
PanelControlClient::request_current_factory (){
	return m_impl->request_current_factory();
}

}	//namespace scim