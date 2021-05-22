#ifndef KINGFISHER_BASE_OS_SIGNAL_SIGNAL_H_
#define KINGFISHER_BASE_OS_SIGNAL_SIGNAL_H_

#include <functional>
#include <mutex>
#include <unordered_map>

namespace kingfisher {
namespace os {

class SignalHandler {
 public:
  SignalHandler();
  ~SignalHandler();

  int SetSignal(int signum, const std::function<void(int)>& cb);
  int SetCoreDump(bool effect, int coreFileSize);

 private:
  static void exitHandler(int signum);
  void registerHandler(int signum, const std::function<void(int)>& handler);

 private:
  static std::timed_mutex sigMutex_;
  static std::unordered_map<int, std::function<void(int)>> sigHandlers_;
};

}  // namespace os
}  // namespace kingfisher

#endif
