#include "chat_utilities.h"

std::string ChatUtilities::read_until(boost::asio::ip::tcp::socket* socket, const std::string& del) {
    boost::asio::streambuf buff;
    std::string message;
    boost::system::error_code er;

    boost::asio::read_until(*socket, buff, del, er);
    message = boost::asio::buffer_cast<const char*>(buff.data());

    return message;
}
