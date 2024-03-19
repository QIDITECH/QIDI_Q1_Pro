#ifndef MAKERBASE_CLIENT_H
#define MAKERBASE_CLIENT_H

// 不包含TLS Client
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

// 包含TLS Client
// #include <websocketpp/config/asio_client.hpp>
// #include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>


#include <string>
#include <locale>
#include <codecvt>

#include "./mks_log.h"
#include "./MoonrakerAPI.h"
#include "./MakerbaseParseMessage.h"

extern std::string message;
extern bool is_get_message;

typedef websocketpp::client<websocketpp::config::asio_client> client;


// 保存一个连接的metadata
class connection_metadata {
public:
	typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

	connection_metadata(websocketpp::connection_hdl hdl, std::string url) {
		this->m_Hdl = hdl;
		this->m_Status = "Connecting";
		this->m_Url = url;
		this->m_Server = "N/A";
	}

	void on_open(client * c, websocketpp::connection_hdl hdl) {
		this->m_Status = "Open";
		// c->get_alog().write(websocketpp::log::alevel::app, "Open client, the url is" + this->m_Url);
		client::connection_ptr con = c->get_con_from_hdl(hdl);
		m_Server = con->get_response_header("Server");
		MKSLOG("websocket连接成功\n");
		// std::cout << this->m_Status << "the m_Server is" + m_Server << std::endl;
	}

	void on_fail(client * c, websocketpp::connection_hdl hdl)
	{
		this->m_Status = "Failed";
		c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
		client::connection_ptr con = c->get_con_from_hdl(hdl);
		m_Server = con->get_response_header("Server");
		m_Error_reason = con->get_ec().message();
		// std::cout << this->m_Status << "the m_Server is " + m_Server << std::endl;
		std::cout << "连接服务端失败，请重新连接" << std::endl;
	}

	void on_close(client * c, websocketpp::connection_hdl hdl)
	{
		this->m_Status = "Closed";

		client::connection_ptr con = c->get_con_from_hdl(hdl);
		std::stringstream s;
		s << "close code: " << con->get_remote_close_code() << " ("
			<< websocketpp::close::status::get_string(con->get_remote_close_code())
			<< "), close reason: " << con->get_remote_close_reason();
		m_Error_reason = s.str();
		// std::cout << this->m_Status << "the m_Server is " + m_Server << std::endl;
	}

	void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
		if (msg->get_opcode() == websocketpp::frame::opcode::text) {
			// std::string message = msg->get_payload();
			// std::cout << "收到来自服务器的消息：" << message << std::endl;
			// this->m_Message = msg->get_payload();
			message = msg->get_payload();
			is_get_message = true;
		} else {
			std::string message = websocketpp::utility::to_hex(msg->get_payload());
		}
	}

	websocketpp::connection_hdl get_hdl() const {
		return m_Hdl;
	}

	std::string get_status() const {
		return m_Status;
	}
private:
	websocketpp::connection_hdl m_Hdl;  // websocketpp表示连接的编号
	std::string m_Status;               // 连接自动状态
	std::string m_Url;                  // 连接的URI
	std::string m_Server;               // 服务器信息
	std::string m_Error_reason;         // 错误原因

	std::string m_Message;				// 回收到的消息

	bool is_recv_result = false;		// 是否已经收到 result 为 "OK" 的回调信息
};


class MakerbaseClient
{
public:
	MakerbaseClient(std::string host, std::string port);
	virtual~MakerbaseClient();

public:
	bool Connect(std::string const & url);
	bool Close(std::string reason = "");
	bool Send(std::string message);
	bool GetIsConnected();
	std::string GetStatus();

	connection_metadata::ptr GetConnectionMetadataPtr();
	
	std::string GetURL();

private:
	bool is_connected = false;			// 连接判断
	std::string status = "none";					// 连接的状态，用字符串表示
	std::string host;			// 目标地址 host
	std::string port;			// 目标地址 port
	std::string sending_message;	// 发送的消息
	connection_metadata::ptr m_ConnectionMetadataPtr;
	client m_WebsocketClient;
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_Thread; // 线程
};


#endif // !MAKERBASE_CLIENT_H

