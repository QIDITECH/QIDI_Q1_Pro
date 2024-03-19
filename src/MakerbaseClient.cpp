#include "../include/MakerbaseClient.h"

MakerbaseClient::MakerbaseClient(std::string host = "localhost", std::string port = "7125") {
	m_WebsocketClient.clear_access_channels(websocketpp::log::alevel::all);  // 开启全部接入日志级别
	m_WebsocketClient.clear_error_channels(websocketpp::log::elevel::all);   // 开启全部错误日志级别
    m_WebsocketClient.set_error_channels(websocketpp::log::elevel::none);

	m_WebsocketClient.init_asio();       // 初始化asio
	m_WebsocketClient.start_perpetual(); // 避免请求为空时退出，实际上，也是避免asio退出

	// 独立运行client::run()的线程，主要是避免阻塞
	m_Thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_WebsocketClient);
    this->host = host;
    this->port = port;
    this->is_connected = false;
}

MakerbaseClient::~MakerbaseClient()
{
	m_WebsocketClient.stop_perpetual();

	if (m_ConnectionMetadataPtr != nullptr && m_ConnectionMetadataPtr->get_status() == "Open")
	{
		websocketpp::lib::error_code ec;
		m_WebsocketClient.close(m_ConnectionMetadataPtr->get_hdl(), websocketpp::close::status::going_away, "", ec); // 关闭连接

		if (ec) {
			std::cout << "> Error initiating close: " << ec.message() << std::endl;
		}
	}

	m_Thread->join();
}

bool MakerbaseClient::Connect(std::string const & url) {
	websocketpp::lib::error_code ec;

	// 创建connect的共享指针，注意，此时创建并没有实际建立
	client::connection_ptr con = m_WebsocketClient.get_connection(url, ec);  

	if (ec) {
		std::cout << "> Connect initialization error: " << ec.message() << std::endl;
		return false;
	}

	// 创建连接的metadata信息，并保存
	connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(con->get_handle(), url);
	m_ConnectionMetadataPtr = metadata_ptr;

	

    try {

            // 注册连接打开的Handler
        con->set_open_handler(websocketpp::lib::bind(
            &connection_metadata::on_open,
            metadata_ptr,
            &m_WebsocketClient,
            websocketpp::lib::placeholders::_1
        ));

        // 注册连接失败的Handler
        con->set_fail_handler(websocketpp::lib::bind(
            &connection_metadata::on_fail,
            metadata_ptr,
            &m_WebsocketClient,
            websocketpp::lib::placeholders::_1
        ));

        // 注册连接关闭的Handler
        con->set_close_handler(websocketpp::lib::bind(
            &connection_metadata::on_close,
            metadata_ptr,
            &m_WebsocketClient,
            websocketpp::lib::placeholders::_1
        ));

        // 注册连接接收消息的Handler
        con->set_message_handler(websocketpp::lib::bind(
            &connection_metadata::on_message,
            metadata_ptr,
            websocketpp::lib::placeholders::_1,
            websocketpp::lib::placeholders::_2
        ));

        // 进行连接
        m_WebsocketClient.connect(con);
		// this->status = m_ConnectionMetadataPtr->get_status();
        std::cout << "Websocket连接中" << std::endl;

    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }

	return true;
}

bool MakerbaseClient::Close(std::string reason) {
	websocketpp::lib::error_code ec;

	if (m_ConnectionMetadataPtr != nullptr)
	{
		int close_code = websocketpp::close::status::normal;
		// 关闭连接
		m_WebsocketClient.close(m_ConnectionMetadataPtr->get_hdl(), close_code, reason, ec);
		if (ec) {
			std::cout << "> Error initiating close: " << ec.message() << std::endl;
			return false;
		}
		std::cout << "关闭Websocket连接成功" << std::endl;
	}

	return true;
}

bool MakerbaseClient::Send(std::string message) {
	websocketpp::lib::error_code ec;

	if (m_ConnectionMetadataPtr != nullptr)
	{
		// 连接发送数据
		m_WebsocketClient.send(m_ConnectionMetadataPtr->get_hdl(), message, websocketpp::frame::opcode::text, ec);
		if (ec) {
			std::cout << "> Error sending message: " << ec.message() << std::endl;
			return false;
		} else {
			this->sending_message = message;
			std::cout << "发送数据成功, 数据内容" + this->sending_message << std::endl;
		}
	}

	return true;
}

bool MakerbaseClient::GetIsConnected() {
    return this->is_connected;
}

std::string MakerbaseClient::GetStatus() {
	if (m_ConnectionMetadataPtr != nullptr) {
		this->status = m_ConnectionMetadataPtr->get_status();
	}
	if (this->status == "Open") {
		this->is_connected = true;
	} else {
		this->is_connected = false;
	}
	return this->status;
}

connection_metadata::ptr MakerbaseClient::GetConnectionMetadataPtr() {
	return m_ConnectionMetadataPtr;
}

std::string MakerbaseClient::GetURL() {
		return "ws://" + this->host + ":" + this->port + "/websocket?";
}
