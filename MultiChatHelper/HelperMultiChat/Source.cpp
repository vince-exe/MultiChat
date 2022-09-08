#include <iostream>

#include <boost/asio.hpp>

std::string read_until(boost::asio::ip::tcp::socket* socket, const std::string& del) {
	boost::asio::streambuf buff;
	std::string message;
	boost::system::error_code er;

	boost::asio::read_until(*socket, buff, del, er);
	message = boost::asio::buffer_cast<const char*>(buff.data());

	return message;
}

void send(boost::asio::ip::tcp::socket* socket, const std::string& msg) {
	boost::system::error_code er;
	boost::asio::write(*socket, boost::asio::buffer(msg), er);
}

int main() {
	std::string nickname;

	std::cout << "Nickname: ";
	std::getline(std::cin, nickname);

	boost::asio::io_context context;

	boost::asio::ip::tcp::socket socket(context);

	socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8000));
	send(&socket, nickname + "\n");

	std::string messageFromServer = read_until(&socket, "\n");
	std::cout << "\nMessage From Server: " << messageFromServer;
	
	system("pause");
	return 0;
}