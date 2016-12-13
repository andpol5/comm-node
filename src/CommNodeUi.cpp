/*
 *  Communications Node
 *  @author Andrei Polzounov
 */

#ifndef _WIN32
#include <ncurses.h>
#else
#include <conio.h>
#endif

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>

#include "CommNode.h"
#include "CommNodeUi.h"

CommNodeUi::CommNodeUi()
{
#ifndef _WIN32
  initscr();
#else

#endif
}

CommNodeUi::~CommNodeUi()
{
#ifndef _WIN32
  endwin();
#else

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
#ifndef _WIN32
  clear();
#else

#endif
  int count = 1;
  std::stringstream ss;
  ss << "############################################################\n";
  for(auto itr = nodes.begin(), end = nodes.end(); itr != end; ++itr, ++count)
  {
    ss << "CommNode ("<< count <<"/"<<nodes.size() << ")"
        << ", IP: " << itr->tcpServerAddress.to_string()
        << ", port: " << itr->tcpServerPort << ", id: " << itr->sessionId
        << ", timestamp: " << itr->timeStampFirstSeen
        << ", first seen: " << itr->timeStampFirstSeen
        << ", round trip: " << itr->roundTripTime
        << ", to time: " << itr->toOtherTripTime
        << ", from time: " << itr->fromOtherTripTime
        << "\n";
  }
  ss << "############################################################\n";
  std::string string = ss.str();

#ifndef _WIN32
  printw(string.c_str());
  refresh();
#else

#endif
}
