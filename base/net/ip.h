#ifndef KINGFISHER_BASE_NET_IP_H_
#define KINGFISHER_BASE_NET_IP_H_

#include <iostream>

namespace kingfisher {
namespace net {

class IPAddress {
 public:
  IPAddress() {}
  ~IPAddress(){};

  static int GetHostIP(std::string &ip_out);
};
}  // namespace net
}  // namespace kingfisher

#endif
