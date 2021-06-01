#include "file_util.h"

#include <libgen.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "core/scope_guard.h"

namespace kingfisher {
namespace fileutil {

int Open(const char *filename, int flags /* = O_RDWR | O_LARGEFILE | O_CREAT*/,
         mode_t mode /* = 0666 */) {
  std::string dir = Dir(filename);
  int ret = MakeDirAll(dir);
  if (ret != 0) {
    return ret;
  }

  return detail::WrapFuncT(open, filename, flags, mode);
}

ssize_t ReadFull(int fd, void *buf, size_t length) {
  return detail::WrapFileOpFuncT(read, fd, buf, length);
}

ssize_t WriteFull(int fd, const void *buf, size_t length) {
  return detail::WrapFileOpFuncT(write, fd, const_cast<void *>(buf), length);
}

ssize_t WriteFile(const std::string &filename, const std::string &content,
                  bool apppended) {
  int fd = -1;
  if (apppended) {
    fd = Open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND);
  } else {
    fd = Open(filename.c_str(), O_CREAT | O_RDWR | O_TRUNC);
  }
  if (-1 == fd) {
    return 0;
  }

  SCOPE_EXIT {
    if (fd != -1) {
      std::cout << "close fd: " << fd << std::endl;
      close(fd);
    }
  };

  return WriteFull(fd, content.c_str(), content.length());
}

bool CopyFile(const char *from_path, const char *to_path) {
  ssize_t in_fd = -1;
  ssize_t out_fd = -1;

  SCOPE_EXIT {
    if (in_fd != -1) {
      std::cout << "close in_fd: " << in_fd << std::endl;
      close(in_fd);
    }
    if (out_fd != -1) {
      std::cout << "close out_fd: " << out_fd << std::endl;
      close(out_fd);
    }
  };

  in_fd = Open(from_path, O_RDONLY);
  if (-1 == in_fd) {
    return false;
  }

  out_fd = Open(to_path);
  if (-1 == out_fd) {
    return false;
  }

  const size_t BUFFER_SIZE = 1024;
  char buf[BUFFER_SIZE] = {'\0'};

  int sz = 0;
  while ((sz = ReadFull(in_fd, buf, BUFFER_SIZE)) > 0) {
    if (sz != WriteFull(out_fd, buf, sz)) {
      return false;
    }
  }

  return true;
}

std::string Dir(const std::string &filename) {
  char *dirc = strdup(filename.c_str());
  char *dir = dirname(dirc);
  if (dir != nullptr) {
    return std::string(dir);
  }

  return "";
}

int MakeDirAll(const std::string &path) {
  if (IsDir(path)) {
    return 0;
  }

  int i = path.length();
  for (; i > 0 && path[i - 1] == '/';) {
    i--;
  }

  int j = i;
  for (; j > 0 && path[j - 1] != '/';) {
    j--;
  }

  if (j > 1) {
    int ret = MakeDirAll(path.substr(0, j - 1));
    if (ret != 0) {
      return ret;
    }
  }

  return ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

bool IsDir(const std::string &path) {
  struct stat buf;
  return stat(path.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}

bool DeleteFile(const char *filename) {
  if (nullptr == filename) {
    return false;
  }

  return 0 == ::unlink(filename);
}

int Flock(int fd, int operation) {
  return detail::WrapFuncT(flock, fd, operation);
}

}  // namespace fileutil
}  // namespace kingfisher
