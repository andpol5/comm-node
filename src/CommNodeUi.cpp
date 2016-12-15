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

#include "CommNode.h"
#include "CommNodeUi.h"
#include "UtilityFunctions.h"

#define DEBUG_UI

namespace
{
  std::string ipAndPort(const boost::asio::ip::address& ip, int port)
  {
    std::stringstream ss1, ss2;
    ss1 << std::setw(15) << ip.to_string() << ":" << port;
    ss2 << std::setw(21) << ss1.str();
    return ss2.str();
  }

  std::string setWidth(double val, int width, int precision = 3)
  {
    std::stringstream ss;
    ss.precision(precision);          // 3 decimal points
    ss << std::setw(width) << val;
    return ss.str();
  }
}

CommNodeUi::CommNodeUi(const std::string sessionId,
    const boost::asio::ip::address& multicastAddress, int multicastPort)
{
  std::stringstream ss;
  ss << " Comm Node " << sessionId.substr(0, 13) << ", multicast: " <<
      ipAndPort(multicastAddress, multicastPort) << "\n";
  titleLine_ = ss.str();

#ifndef DEBUG_UI
#ifndef _WIN32
  initscr();
#else

#endif
#endif
}

CommNodeUi::~CommNodeUi()
{
#ifndef DEBUG_UI
#ifndef _WIN32
  endwin();
#else

#endif
#endif
}

void CommNodeUi::updateScreen(const std::vector<CommNode>& nodes) const
{
#ifndef DEBUG_UI
#ifndef _WIN32
  clear();
#else
  system("cls");
#endif
#endif
  int numNodes = nodes.size();
  std::stringstream ss;
  ss << "----------------------------------------------------------------------------\n"
     << titleLine_
     << "----------------------------------------------------------------------------\n";
  if(numNodes==1)
    ss << " 1 communication node was detected on the network\n";
  else
    ss << " " << numNodes << " communication nodes were detected on the network\n";
  ss << "-------------+---------------------+---------+---------+---------+---------+\n"
     << "             |                     |   Time  |   Round |    To   |   From  |\n"
     << " Session ID  |   TCP <ip>::<port>  |  Alive  |   Trip  |   Node  |   Node  |\n"
     << "             |                     |    (s)  |   (ms)  |   (ms)  |   (ms)  |\n"
     << "-------------+---------------------+---------+---------+---------+---------+\n";

  for(auto itr = nodes.begin(), end = nodes.end(); itr != end; ++itr)
  {
    double timeAliveSeconds = static_cast<double>(
        UtilityFunctions::microsecondsSinceEpoch()-itr->timeStampFirstSeen)/1000000.0;
    double roundTripTimeMs = static_cast<double>(itr->roundTripTime)/1000.0;
    double toTripTimeMs = static_cast<double>(itr->toOtherTripTime)/1000.0;
    double fromTripTimeMs = static_cast<double>(itr->fromOtherTripTime)/1000.0;
    ss << itr->sessionId.substr(0, 13) << "|"
       << ipAndPort(itr->tcpServerAddress, itr->tcpServerPort) << "|"
       << setWidth(timeAliveSeconds, 9, 4) << "|"
       << setWidth(roundTripTimeMs, 9) << "|"
       << setWidth(toTripTimeMs, 9) << "|"
       << setWidth(fromTripTimeMs, 9) << "|"
       << "\n";
  }
  ss << "-------------+---------------------+---------+---------+---------+---------+\n";
  std::string string = ss.str();

#ifndef DEBUG_UI
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
