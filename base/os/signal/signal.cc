
#include "os/signal/signal.h"

#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>

#include <cerrno>
#include <csignal>

namespace kingfisher {
namespace os {

SignalHandler::SignalHandler() {}
SignalHandler::~SignalHandler() {}

void SignalHandler::exitHandler(int signum) {
#if 0
  std::unique_lock<std::timed_mutex> lock(sigMutex_, std::defer_lock);
  if (!lock.try_lock_for(std::chrono::seconds(1))) {
    // unable to get the lock. should be a strange case
    return;
  }
#endif

  auto it = sigHandlers_.find(signum);
  if (it != sigHandlers_.end()) {
    it->second(signum);
  }
}

void SignalHandler::registerHandler(int signum,
                                    const std::function<void(int)>& handler) {
  // std::lock_guard<std::timed_mutex> lock(sigMutex_);
  sigHandlers_.emplace(signum, handler);
}

int SignalHandler::SetSignal(int signum, const std::function<void(int)>& cb) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &SignalHandler::exitHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags |= SA_RESETHAND;
  if (sigaction(signum, nullptr, &sa) == -1) {
    return errno;
  }

  registerHandler(signum, cb);
  return 0;
}

int SignalHandler::SetCoreDump(bool effect, int coreFileSize) {
  if (effect) {
    struct rlimit rlim;
    rlim.rlim_cur = (coreFileSize < 0) ? RLIM_INFINITY : coreFileSize;
    rlim.rlim_max = rlim.rlim_cur;

    if (-1 == setrlimit(RLIMIT_CORE, &rlim)) {
      return errno;
    }

    getrlimit(RLIMIT_CORE, &rlim);

    // set limit only root use
    rlim.rlim_cur = 655350;
    rlim.rlim_max = 655350;
    if (-1 == setrlimit(RLIMIT_NOFILE, &rlim)) {
      return errno;
    }
    getrlimit(RLIMIT_NOFILE, &rlim);
  }
  if (-1 == prctl(PR_SET_DUMPABLE, effect ? 1 : 0)) {
    return errno;
  }
  return 0;
}
}  // namespace os
}  // namespace kingfisher
