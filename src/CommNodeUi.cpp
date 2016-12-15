/*
 *  Communications Node
 *  @author Andrei Polzounov
 */

#ifndef _WIN32
// ncurses for Linux
#include <ncurses.h>
#endif

#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>

#include "CommNode.h"
#include "CommNodeUi.h"
#include "UtilityFunctions.h"

#define DEBUG_UI_OFF

namespace
{
  std::string ipAndPort(boost::asio::ip::address ip, int port)
  {
    std::stringstream ss;
    ss << std::setw(14) << ip.to_string() << ":" << std::setw(5) << port;
    return ss.str();
  }

  std::string setWidth(double val, int width, int precision = 3)
  {
    std::stringstream ss;
    ss.precision(precision);          // 3 decimal points
    ss << std::setw(width) << val;
    return ss.str();
  }
}

CommNodeUi::CommNodeUi()
{
#ifndef DEBUG_UI_OFF
#ifndef _WIN32
  initscr();
#else

#endif
#endif
}

CommNodeUi::~CommNodeUi()
{
#ifndef DEBUG_UI_OFF
#ifndef _WIN32
  endwin();
#else

#endif
#endif
}

//void CommNodeUi::updateScreen(const std::vector<CommNode>& nodes) const
//{
//  int count = 1;
//  std::cout << "############################################################" << std::endl;
//  for(auto itr = nodes.begin(), end = nodes.end(); itr != end; ++itr, ++count)
//  {
//    std::cout << "CommNode ("<< count <<"/"<<nodes.size() << ")"
//        << ", IP: " << itr->tcpServerAddress.to_string()
//        << ", port: " << itr->tcpServerPort << ", id: " << itr->sessionId
//        << ", timestamp: " << itr->timeStampFirstSeen
//        << ", first seen: " << itr->timeStampFirstSeen
//        << ", round trip: " << itr->roundTripTime
//        << ", to time: " << itr->toOtherTripTime
//        << ", from time: " << itr->fromOtherTripTime
//        << std::endl;
//  }
//  std::cout << "############################################################" << std::endl;
//}

void CommNodeUi::updateScreen(const std::vector<CommNode>& nodes) const
{
#ifndef DEBUG_UI_OFF
#ifndef _WIN32
  clear();
#else
  system("cls");
#endif
#endif
  int numNodes = nodes.size();
  std::stringstream ss;
  ss << "-----------------------------------------------------------------------\n"
     << "                           Comm Nodes\n"
     << "-----------------------------------------------------------------------\n";
  if(numNodes==1)
    ss << " 1 communication node was detected on the network\n";
  else
    ss << " " << numNodes << " communication nodes were detected on the network\n";
  ss << "-------------+--------------------+--------+--------+--------+--------+\n"
     << "             |                    |  Time  |   Round|    To  |   From |\n"
     << " Session ID  |  TCP <ip>::<port>  |  Alive |   Trip |   Node |   Node |\n"
     << "             |                    |   (s)  |   (ms) |   (ms) |   (ms) |\n"
     << "-------------+--------------------+--------+--------+--------+--------+\n";

  for(auto itr = nodes.begin(), end = nodes.end(); itr != end; ++itr)
  {
    double timeAliveSeconds = static_cast<double>(
        UtilityFunctions::microsecondsSinceEpoch()-itr->timeStampFirstSeen)/1000000.0;
    double roundTripTimeMs = static_cast<double>(itr->roundTripTime)/1000.0;
    double toTripTimeMs = static_cast<double>(itr->toOtherTripTime)/1000.0;
    double fromTripTimeMs = static_cast<double>(itr->fromOtherTripTime)/1000.0;
    ss << itr->sessionId.substr(0, 13) << "|"
       << ipAndPort(itr->tcpServerAddress, itr->tcpServerPort) << "|"
       << setWidth(timeAliveSeconds, 8, 4) << "|"
       << setWidth(roundTripTimeMs, 8) << "|"
       << setWidth(toTripTimeMs, 8) << "|"
       << setWidth(fromTripTimeMs, 8) << "|"
       << "\n";
  }
  ss << "-------------+--------------------+--------+--------+--------+--------+\n"; 
  std::string string = ss.str();

#ifndef DEBUG_UI_OFF
#ifndef _WIN32
  printw(string.c_str());
  refresh();
#else
  std::cout << string;
#endif
#else
  std::cout << string << std::endl;
#endif
}
