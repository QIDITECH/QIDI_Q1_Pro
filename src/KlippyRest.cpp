#include "../include/KlippyRest.h"
#include "../include/mks_log.h"
#include "../include/event.h"

std::string server_files_metadata(std::string ip, std::string port, std::string filename) {
    return send_request(ip, port, "server/files/metadata?filename=" + filename, "GET");
}

std::string printer_objects_query(std::string ip, std::string port, std::string parameter) {
    return send_request(ip, port, "printer/objects/query?" + parameter, "GET");
}

std::string printer_print_start(std::string ip, std::string port, std::string filename) {
    return send_request(ip, port, "printer/print/start?filename=" + filename, "POST");
}

std::string printer_print_pause(std::string ip, std::string port) {
    return send_request(ip, port, "printer/print/pause", "POST");
}

std::string printer_print_resume(std::string ip, std::string port) {
    return send_request(ip, port, "printer/print/resume", "POST");
}

std::string printer_print_cancel(std::string ip, std::string port) {
    return send_request(ip, port, "printer/print/cancel", "POST");
}

std::string get_server_files_list(std::string ip, std::string port) {
    return send_request(ip, port, "server/files/list?", "GET");
}

std::string get_server_files_directory(std::string ip, std::string port) {
    return send_request(ip, port, "server/files/directory?", "GET");
}

std::string get_server_info(std::string ip, std::string port) {
    return send_request(ip, port, "server/info", "GET");
}

std::string get_oneshot_token(std::string ip, std::string port) {
    return send_request(ip, port, "access/oneshot_token", "GET");
}

std::string get_printer_info(std::string ip, std::string port) {
    return send_request(ip, port, "printer/info", "GET");
}

// File delete
std::string delete_file_delete(std::string ip, std::string port, std::string filepath) {
    return send_request(ip, port, "server/files/" + filepath, "DELETE");
}

std::string get_thumbnail_stream(std::string ip, std::string port, std::string thumbnail) {
    std::string url = "http://" + ip + ":" + port + "/server/files/gcodes/" + thumbnail;
    http::Request request{url};
    struct http::Response response = request.send("GET");
    if (response.status.code == 200) {
        return std::string{response.body.begin(), response.body.end()};
    } else {
        return "";
    }
}

std::string send_request(std::string ip, std::string port, std::string method, std::string request_type) {
    std::string url = "http://" + ip + ":" + port + "/" + method;
    std::string str_response = "";
    url=replaceCharacters(url," ","%20");
    MKSLOG_BLUE("Sending request to %s", url.data());
    try
    {
        http::Request request{url};
        struct http::Response response = request.send(request_type);
        str_response = std::string{response.body.begin(), response.body.end()};
    }
    catch(const std::exception& e)
    {
        std::cerr << "Request failed, error" << e.what() << '\n';
    }
    return str_response;
}

/* 
std::string send_post_request(std::string ip, std::string port, std::string method) {
    std::string url = "http://" + ip + ":" + port + "/" + method;
    std::cout << url << std::endl;
    std::string str_response = "";
    MKSLOG_BLUE("Sending request to %s", url.data());
    try
    {
        http::Request request{url};
        struct http::Response response = request.send("POST");
        str_response = std::string{response.body.begin(), response.body.end()};
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return str_response;
}
 */