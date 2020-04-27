#ifndef SERIALIZER_DJSON_OBJ_H
#define SERIALIZER_DJSON_OBJ_H

#ifndef KINGFISHER_NAMESPACE
#define KINGFISHER_NAMESPACE kingfisher
#endif

#ifndef SERIALIZER_DJSON_NAMESPACE
#define SERIALIZER_DJSON_NAMESPACE serializer_djson
#endif

#ifndef KINGFISHER_NAMESPACE_BEGIN
#define KINGFISHER_NAMESPACE_BEGIN namespace KINGFISHER_NAMESPACE {
#endif

#ifndef SERIALIZER_DJSON_NAMESPACE_BEGIN
#define SERIALIZER_DJSON_NAMESPACE_BEGIN namespace SERIALIZER_DJSON_NAMESPACE {
#endif

#ifndef KINGFISHER_NAMESPACE_END
#define KINGFISHER_NAMESPACE_END }
#endif

#ifndef SERIALIZER_DJSON_NAMESPACE_END
#define SERIALIZER_DJSON_NAMESPACE_END }
#endif

#include <map>
#include <mutex>
#include <vector>

KINGFISHER_NAMESPACE_BEGIN
SERIALIZER_DJSON_NAMESPACE_BEGIN

class JsonBase;

#if 0
#define SAFE_DELETE(x) \
  do {                 \
    if (x) delete x;   \
    x = nullptr;       \
  } while (0)

#if 0
template <class T>
class singleton
{
private:
    static T & m_instance;
    static void use(T const *) {}

    static T & get_instance() {
        class singleton_wrapper : public T {};
        static singleton_wrapper t;

        use(& m_instance);
        return static_cast<T &>(t);
    }
};

template<class T>
T & singleton< T >::m_instance = singleton< T >::get_instance();
#endif

namespace detail {
// SFINA原则，测试是否有成员函数no_destroy，但是不会探测到继承的成员函数
template <typename T>
struct has_no_destroy {
#if __cplusplus >= 201103L
  template <typename C>
  static char test(decltype(&C::no_destroy));
#else
  template <typename C>
  static char test(typeof(&C::no_destroy));
#endif
  template <typename C>
  static int32_t test(...);
  const static bool value =
      sizeof(test<T>(0)) ==
      1;  //如果C有no_destroy函数，就会匹配char test(typeof(&C::no_destroy))，
          //因此返回值的类型大小就是1（char），否则，就会匹配int32_t
          // test(...)，返回值的类型大小的是4（int32_t)，test<T>(0),相当于将T传给C，
          // 0代表传给的形参值（typeof(&C::no_destroy)类型的值
          // 或...任意类型的值)
};
}  // end namespace detail

template <class T>
class Singleton {
 public:
  static T& instance() {
    std::call_once(
        once_, [&]() { Singleton::init(); });  //多个线程只会执行一次init函数
    return *value_;
  }

 private:
  Singleton();
  ~Singleton();

  static void init() {
    value_ = new T();
    if (!detail::has_no_destroy<T>::
            value) {  // T没有no_destroy成员函数，就注册destory，程序退出时调用destroy函数
      ::atexit(destory);
    }
  }

  static void destory() {
    using T_must_be_complete_type = char[sizeof(T) == 0 ? -1 : 1];
    // typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    //类T一定要完整，否则delete时会出错，这里当T不完整时将数组的长度置为-1，会报错
    T_must_be_complete_type dumpy;
    (void)dumpy;

    SAFE_DELETE(value_);
  }

 private:
  static std::once_flag once_;
  static T* value_;
};

template <typename T>
std::once_flag Singleton<T>::once_;

template <typename T>
T* Singleton<T>::value_ = nullptr;
#endif

using OBJDETAIL = std::vector<std::pair<std::string, JsonBase*> >;
using OBJCITER = std::map<std::string, OBJDETAIL>::const_iterator;

class JsonObjsManager {
 public:
  JsonObjsManager() {}
  ~JsonObjsManager() {}

  bool find(OBJCITER& _return, const std::string& typeName) {
    if ((_return = objs_.find(typeName)) == objs_.end()) {
      return false;
    }

    return true;
  }

  void put(const std::string& typeName, const OBJDETAIL& objdetail) {
    objs_.insert(std::make_pair(typeName, objdetail));
  }

 private:
  std::map<std::string, OBJDETAIL> objs_;
};

SERIALIZER_DJSON_NAMESPACE_END
KINGFISHER_NAMESPACE_END

#endif
