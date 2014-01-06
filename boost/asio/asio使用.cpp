basic_stream_socket:
async_read_some - "This function is used to asynchronously read data from the stream socket. The function call always returns immediately."
async_receive - "This function is used to asynchronously receive data from the stream socket. The function call always returns immediately."
async_send - "This function is used to asynchronously send data on the stream socket. The function call always returns immediately."
async_write_some - "This function is used to asynchronously write data to the stream socket. The function call always returns immediately."
receive - "This function is used to receive data on the stream socket. The function call will block until one or more bytes of data has been received successfully, or until an error occurs."
read_some - "This function is used to read data from the stream socket. The function call will block until one or more bytes of data has been read successfully, or until an error occurs."
send - "This function is used to send data on the stream socket. The function call will block until one or more bytes of the data has been sent successfully, or an until error occurs."
write_some - "This function is used to write data to the stream socket. The function call will block until one or more bytes of the data has been written successfully, or until an error occurs."

boost::asio Free Functions
async_read - "Start an asynchronous operation to read a certain amount of data from a stream."
async_read_at - "Start an asynchronous operation to read a certain amount of data at the specified offset."
async_read_until - "Start an asynchronous operation to read data into a streambuf until it contains a delimiter, matches a regular expression, or a function object indicates a match."
async_write - "Start an asynchronous operation to write a certain amount of data to a stream."
async_write_at - "Start an asynchronous operation to write a certain amount of data at the specified offset."
read - "Attempt to read a certain amount of data from a stream before returning."
read_at - "Attempt to read a certain amount of data at the specified offset before returning."
read_until - "Read data into a streambuf until it contains a delimiter, matches a regular expression, or a function object indicates a match."
write - "Write a certain amount of data to a stream before returning. "
write_at - "Write a certain amount of data at a specified offset before returning. "


解析网址
boost::asio::ip::tcp::socket sock( *io_service );
boost::asio::ip::tcp::resolver resolver( *io_service );
boost::asio::ip::tcp::resolver::query query( 
	"www.google.com", 
	boost::lexical_cast< std::string >( 80 )
);
boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
boost::asio::ip::tcp::endpoint endpoint = *iterator;

global_stream_lock.lock();
std::cout << "Connecting to: " << endpoint << std::endl;
global_stream_lock.unlock();

sock.connect( endpoint );

///////////////////////////////////////////
Source listing for Daytime.1

//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], "daytime");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////
http://www.gamedev.net/blog/950/entry-2249317-a-guide-to-getting-started-with-boostasio/?pg=8
系列文章，选几个经典的出来

// 异步监听
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id()
		<< "] Thread Start" << std::endl;
	global_stream_lock.unlock();

	while( true )
	{
		try
		{
			boost::system::error_code ec;
			io_service->run( ec );
			if( ec )
			{
				global_stream_lock.lock();
				std::cout << "[" << boost::this_thread::get_id()
					<< "] Error: " << ec << std::endl;
				global_stream_lock.unlock();
			}
			break;
		}
		catch( std::exception & ex )
		{
			global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;
			global_stream_lock.unlock();
		}
	}

	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id()
		<< "] Thread Finish" << std::endl;
	global_stream_lock.unlock();
}

void OnAccept( const boost::system::error_code & ec, boost::shared_ptr< boost::asio::ip::tcp::socket > sock )
{
	if( ec )
	{
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Error: " << ec << std::endl;
		global_stream_lock.unlock();
	}
	else
	{
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Accepted!" << std::endl;
		global_stream_lock.unlock();
	}
}

int main( int argc, char * argv[] )
{
	boost::shared_ptr< boost::asio::io_service > io_service(
		new boost::asio::io_service
	);
	boost::shared_ptr< boost::asio::io_service::work > work(
		new boost::asio::io_service::work( *io_service )
	);
	boost::shared_ptr< boost::asio::io_service::strand > strand(
		new boost::asio::io_service::strand( *io_service )
	);

	global_stream_lock.lock();
	std::cout << "[" << boost::this_thread::get_id()
		<< "] Press [return] to exit." << std::endl;
	global_stream_lock.unlock();

	boost::thread_group worker_threads;
	for( int x = 0; x < 2; ++x )
	{
		worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
	}

	boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor(
		new boost::asio::ip::tcp::acceptor( *io_service )
	);
	boost::shared_ptr< boost::asio::ip::tcp::socket > sock(
		new boost::asio::ip::tcp::socket( *io_service )
	);

	try
	{
		boost::asio::ip::tcp::resolver resolver( *io_service );
		boost::asio::ip::tcp::resolver::query query( 
			"127.0.0.1", 
			boost::lexical_cast< std::string >( 7777 )
		);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
		acceptor->open( endpoint.protocol() );
		acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
		acceptor->bind( endpoint );
		acceptor->listen( boost::asio::socket_base::max_connections );
		acceptor->async_accept( *sock, boost::bind( OnAccept, _1, sock ) );

		global_stream_lock.lock();
		std::cout << "Listening on: " << endpoint << std::endl;
		global_stream_lock.unlock();
	}
	catch( std::exception & ex )
	{
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Exception: " << ex.what() << std::endl;
		global_stream_lock.unlock();
	}

	std::cin.get();

	boost::system::error_code ec;
	acceptor->close( ec );

	sock->shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
	sock->close( ec );

	io_service->stop();

	worker_threads.join_all();

	return 0;
}