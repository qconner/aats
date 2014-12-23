#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>

#include <boost/asio.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/thread/thread.hpp>
#include <boost/math/special_functions/round.hpp>


void vt100_session(boost::asio::ip::tcp::socket client_socket, unsigned int baud, std::string filename)
{
  try
  {
    // identify client endpoint
    std::string clientIP = client_socket.remote_endpoint().address().to_string();
    std::cout << "connection from " << clientIP << std::endl;
    std::cout << "sending " << filename << " at " << baud << " baud" << std::endl;

    // open file
    boost::iostreams::file_source infile(filename, BOOST_IOS::binary);
    char buf[1] = { 0 };

    // calculate intercharacter delay
    // 9600 baud = 960 chars/sec.  period = 1 / frequency
    const long long ns_delay = lround(1000000000.0 * (1.0 / ((double)baud / 10.0)));
    //std::cout << "us delay is " << ns_delay/1000 << std::endl;
    struct timespec sleepTime;
    struct timespec time_left_to_sleep;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = ns_delay;

    while (infile.read(buf, 1) > 0)
    {
      //std::cout.write(buf, 1);
      //std::cout.flush();
      boost::asio::write(client_socket, boost::asio::buffer(buf, 1));

      //boost::this_thread::sleep(boost::posix_time::microseconds(us_delay));
      sleepTime.tv_sec = 0;
      sleepTime.tv_nsec = ns_delay;
      nanosleep(&sleepTime, &time_left_to_sleep);
    }
    infile.close();
    boost::this_thread::sleep(boost::posix_time::seconds(3));
    // 1b 5b 48 1b 5b 32 4a
    client_socket.close();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in vt100 server thread: " << e.what() << std::endl;
    //std::cerr << e.message << std::endl;
  }
}



void vt100_server(boost::asio::io_service& my_io_service, unsigned short port, unsigned int baud, std::string filename)
{
  boost::asio::ip::tcp::acceptor vt100_ascii_art_server(my_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
  while (true)
  {
    boost::asio::ip::tcp::socket vt100_socket(my_io_service);
    vt100_ascii_art_server.accept(vt100_socket);
    std::thread(vt100_session, std::move(vt100_socket), baud, filename).detach();
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 4)
    {
      std::cerr << "Usage: vt100 port baud filename\n";
      return 1;
    }

    boost::asio::io_service io_service;
    vt100_server(io_service, std::atoi(argv[1]), std::atoi(argv[2]), argv[3]);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}

