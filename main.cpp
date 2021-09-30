#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "ftd2xx.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#define d2xxStatus(x) \
	if (x != FT_OK) \
	{ \
		printf("Error status: %d", x); \
		return 1; \
	}

#define BOOST_ENABLED

int main(int argc, char *argv[])
{
	FT_STATUS ftStatus;
	FT_HANDLE ftHandle0;
	static FT_DEVICE ftDevice;
	PVOID ID = (void*)"FT000001";
	boost::asio::io_context io_context;
	
	d2xxStatus(ftStatus = FT_OpenEx(ID, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle0));
	
	printf("Port handle: %p\n", ftHandle0);

	d2xxStatus(ftStatus = FT_GetDeviceInfo(ftHandle0, &ftDevice, NULL, NULL, NULL, NULL));

	printf("FT_GetDeviceInfo succeeded. Device: %p.\n", ftDevice);

#ifdef BOOST_ENABLED

	printf("Boost serial port using %p\n", ftDevice);

    boost::asio::serial_port serialPort(io_context, ftDevice);

	printf("Port native handle: %p\n", serialPort.native_handle());

    serialPort.set_option(boost::asio::serial_port_base::baud_rate(9600));
    serialPort.set_option(boost::asio::serial_port_base::character_size(8));
    serialPort.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    serialPort.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serialPort.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

	io_context.run();

#else 

	int baud_rate = 115200;

	d2xxStatus(ftStatus = FT_SetBaudRate(ftHandle0, baud_rate));

	printf("Baud rate set to %d\n", baud_rate);

	d2xxStatus(ftStatus = FT_SetFlowControl(ftHandle0, FT_FLOW_RTS_CTS, 0x11, 0x13));


#endif
		
	FT_Close(ftHandle0);
}
