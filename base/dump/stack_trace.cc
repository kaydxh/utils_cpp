#include "stack_trace.h"
#include <cxxabi.h>
#include <execinfo.h>  // backtrace and backtrace_symbols func
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <memory>

namespace kingfisher {
namespace dump {

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))
#endif

const char kMangledSymbolPrefix[] = "_Z";

const char kSymbolCharacters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

const struct {
  int signal_number;
  const char* name;
} kFailureSignals[] = {
    {SIGSEGV, "SIGSEGV"}, {SIGILL, "SIGILL"},   {SIGFPE, "SIGFPE"},
    {SIGABRT, "SIGABRT"}, {SIGTERM, "SIGTERM"},
};

int invokeDefaultSignalHandler(int signal_number);
void failureSignalHandler(int signal_number, siginfo_t* signal_info,
                          void* context);

int invokeDefaultSignalHandler(int signal_number) {
  struct sigaction sig_action;
  memset(&sig_action, 0, sizeof(sig_action));
  sigemptyset(&sig_action.sa_mask);
  sig_action.sa_handler = SIG_DFL;
  sigaction(signal_number, &sig_action, nullptr);
  kill(getpid(), signal_number);
  return 0;
}

void failureSignalHandler(int signal_number, siginfo_t* signal_info,
                          void* context) {
  StackTrace st;
  auto stack_trace_info = st.GetStackTrace();
  std::cerr << stack_trace_info << std::endl;
  invokeDefaultSignalHandler(signal_number);
}

StackTrace::StackTrace() {}

StackTrace::~StackTrace() {}

int StackTrace::InstallFailureSignalHandler() {
  struct sigaction sig_action;
  memset(&sig_action, 0, sizeof(sig_action));
  sigemptyset(&sig_action.sa_mask);
  sig_action.sa_flags |= SA_SIGINFO;
  sig_action.sa_sigaction = failureSignalHandler;

  for (size_t i = 0; i < ARRAYSIZE(kFailureSignals); ++i) {
    int ret = sigaction(kFailureSignals[i].signal_number, &sig_action, nullptr);
    if (0 != ret) {
      return ret;
    }
  }

  return 0;
}

void StackTrace::demangleSymbol(std::string& symbol) {
  std::string::size_type from_pos = 0;
  while (from_pos < symbol.size()) {
    std::string::size_type mangled_start =
        symbol.find(kMangledSymbolPrefix, from_pos);
    if (mangled_start == std::string::npos) {
      break;
    }
    std::string::size_type mangled_end =
        symbol.find_first_not_of(kSymbolCharacters, mangled_start);
    if (mangled_end == std::string::npos) {
      mangled_end = symbol.size();
    }
    std::string mangled_symbol =
        std::move(symbol.substr(mangled_start, mangled_end - mangled_start));

    int status = -4;

    std::unique_ptr<char, void (*)(void*)> demangled_symbol{
        abi::__cxa_demangle(mangled_symbol.c_str(), nullptr, 0, &status),
        std::free};

    if (0 == status) {
      // remove the mangled symbol
      symbol.erase(mangled_start, mangled_end - mangled_start);
      // insert the demangled symbol
      symbol.insert(mangled_start, demangled_symbol.get());
      // next time, start right after the demangled symbol
      from_pos = mangled_start + strlen(demangled_symbol.get());

    } else {
      // failed to demangled, retry after the '_z' we just found
      from_pos = mangled_start + 2;
    }
  }
}

const char* StackTrace::GetStackTrace() {
  static const int kStackLenght = 128;
  void* stack_buf[kStackLenght];
  int size = ::backtrace(stack_buf, kStackLenght);

  std::unique_ptr<char*, void (*)(void*)> stacks{
      ::backtrace_symbols(stack_buf, size), std::free};

  if (nullptr != stacks.get()) {
    for (int i = 0; i < size; ++i) {
      std::string symbol(stacks.get()[i]);
      demangleSymbol(symbol);
      stack_.append(symbol);
      stack_.push_back('\n');
    }
  }

  return stack_.c_str();
}

}  // namespace dump
}  // namespace kingfisher
