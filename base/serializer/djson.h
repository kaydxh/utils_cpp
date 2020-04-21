#ifndef SERIALIZER_DJSON_DJSON_H
#define SERIALIZER_DJSON_DJSON_H

#include <iostream>
#include <sstream>
#include <typeinfo>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "obj.hpp"

KINGFISHER_NAMESPACE_BEGIN
SERIALIZER_DJSON_NAMESPACE_BEGIN

template <typename T>
class JsonHelper;

class JsonBase {
 public:
  virtual ~JsonBase() {}
  virtual void toJson(std::ostream& os, void* obj) = 0;
  virtual bool toObj(rapidjson::Value& rjson, void* obj) = 0;
};

template <typename T1, typename T2>
class JsonObjector : public JsonBase {
 public:
  JsonObjector(T2 T1::*field) : field_(field) {}

  virtual void toJson(std::ostream& os, void* obj) {
    T1* t1 = static_cast<T1*>(obj);
    return JsonHelper<T2>::toJson(os, t1->*field_);
  }

  virtual bool toObj(rapidjson::Value& rjson, void* obj) {
    T1* t1 = static_cast<T1*>(obj);
    return JsonHelper<T2>::toObj(rjson, t1->*field_);
  }

 private:
  T2 T1::*field_;
};

template <typename T>
class JsonHelper {
 public:
  static void toJson(std::ostream& os, const T& t) {
    OBJCITER oit;
    if (!Singleton<JsonObjsManager>::instance().find(oit, typeid(t).name())) {
      return;
    }

    os << "{";
    auto& detailObj = oit->second;
    auto dit = detailObj.begin();

    bool firsted = false;
    for (auto& e : detailObj) {
      if (!firsted) {
        firsted = true;
      } else {
        os << ",";
      }

      os << "\"" << e.first << "\"";
      os << ":";
      e.second->toJson(os, (void*)(&t));
    }

    os << "}";
  }

  static bool toObj(rapidjson::Value& rjson, T& t) {
    if (!rjson.IsObject()) {
      return false;
    }

    OBJCITER oit;
    if (!Singleton<JsonObjsManager>::instance().find(oit, typeid(t).name())) {
      return false;
    }

    auto& detailObj = oit->second;
    auto dit = detailObj.begin();

    for (auto& e : detailObj) {
      if (!rjson.HasMember(e.first.c_str())) {
        continue;
      }

      if (!e.second->toObj(rjson[e.first.c_str()], &t)) {
        return false;
      }
    }

    return true;
  }
};

template <>
class JsonHelper<int8_t> {
 public:
  static void toJson(std::ostream& os, const int8_t& t) {
    os << static_cast<int>(t);
  }

  static bool toObj(rapidjson::Value& rjson, int8_t& t) {
    if (rjson.IsInt()) {
      t = rjson.GetInt();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<uint8_t> {
 public:
  static void toJson(std::ostream& os, const uint8_t& t) {
    os << static_cast<uint>(t);
  }

  static bool toObj(rapidjson::Value& rjson, uint8_t& t) {
    if (rjson.IsUint()) {
      t = rjson.GetUint();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<int16_t> {
 public:
  static void toJson(std::ostream& os, const int16_t& t) {
    os << static_cast<int16_t>(t);
  }

  static bool toObj(rapidjson::Value& rjson, int16_t& t) {
    if (rjson.IsInt()) {
      t = rjson.GetInt();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<uint16_t> {
 public:
  static void toJson(std::ostream& os, const uint16_t& t) {
    os << static_cast<uint16_t>(t);
  }

  static bool toObj(rapidjson::Value& rjson, uint16_t& t) {
    if (rjson.IsUint()) {
      t = rjson.GetUint();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<int32_t> {
 public:
  static void toJson(std::ostream& os, const int32_t& t) {
    os << static_cast<int32_t>(t);
  }

  static bool toObj(rapidjson::Value& rjson, int32_t& t) {
    if (rjson.IsInt()) {
      t = rjson.GetInt();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<uint32_t> {
 public:
  static void toJson(std::ostream& os, const uint32_t& t) {
    os << static_cast<uint32_t>(t);
  }

  static bool toObj(rapidjson::Value& rjson, uint32_t& t) {
    if (rjson.IsUint()) {
      t = rjson.GetUint();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<int64_t> {
 public:
  static void toJson(std::ostream& os, const int64_t& t) {
    os << static_cast<int64_t>(t);
  }

  static bool toObj(rapidjson::Value& rjson, int64_t& t) {
    if (rjson.IsInt64()) {
      t = rjson.GetInt64();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<uint64_t> {
 public:
  static void toJson(std::ostream& os, const uint64_t& t) {
    os << static_cast<uint64_t>(t);
  }

  static bool toObj(rapidjson::Value& rjson, uint64_t& t) {
    if (rjson.IsUint64()) {
      t = rjson.GetUint64();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<double> {
 public:
  static void toJson(std::ostream& os, const double& t) {
    os << static_cast<double>(t);
  }

  static bool toObj(rapidjson::Value& rjson, double& t) {
    if (rjson.IsDouble()) {
      t = rjson.GetDouble();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<bool> {
 public:
  static void toJson(std::ostream& os, const bool& t) {
    os << (t ? "true" : "false");
  }

  static bool toObj(rapidjson::Value& rjson, bool& t) {
    if (rjson.IsBool()) {
      t = rjson.GetBool();
      return true;
    }

    return false;
  }
};

template <>
class JsonHelper<std::string> {
 public:
  static void toJson(std::ostream& os, const std::string& t) {
    rapidjson::Document doc;
    rapidjson::Value val(t.c_str(), doc.GetAllocator());
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    val.Accept(writer);

    os << strbuf.GetString();
  }

  static bool toObj(rapidjson::Value& rjson, std::string& t) {
    if (rjson.IsString()) {
      t = rjson.GetString();
      return true;
    }

    return false;
  }
};

template <typename T>
class JsonHelper<std::vector<T> > {
 public:
  static void toJson(std::ostream& os, const std::vector<T>& t) {
    os << "[";

    bool firsted = false;
    for (auto e : t) {
      if (!firsted) {
        firsted = true;
      } else {
        os << ",";
      }

      JsonHelper<T>::toJson(os, e);
    }

    os << "]";
  }

  static bool toObj(rapidjson::Value& rjson, std::vector<T>& t) {
    if (rjson.IsArray()) {
      t.clear();

      T e;
      for (size_t i = 0; i < rjson.Capacity(); ++i) {
        rapidjson::Value& v = rjson[i];
        if (!JsonHelper<T>::toObj(v, e)) {
          return false;
        } else {
          t.push_back(e);
        }
      }

      return true;
    }

    return false;
  }
};

template <typename T1, typename T2>
class JsonHelper<std::map<T1, T2> > {
 public:
  static void toJson(std::ostream& os, const std::map<T1, T2>& t) {
    os << "{";

    bool firsted = false;
    for (auto& e : t) {
      if (!firsted) {
        firsted = true;
      } else {
        os << ",";
      }

      os << "\"" << e.first << "\"";
      os << ":";
      JsonHelper<T2>::toJson(os, e.second);
    }

    os << "}";
  }

  static bool toObj(rapidjson::Value& rjson, std::map<T1, T2>& t) {
    if (rjson.IsObject()) {
      t.clear();

      T1 k;
      T2 v;
      auto it = rjson.MemberBegin();
      for (; it != rjson.MemberEnd(); ++it) {
        if (!JsonHelper<T1>::toObj(it->name, k)) {
          return false;
        }
        if (!JsonHelper<T2>::toObj(it->value, v)) {
          return false;
        }

        t.insert(std::make_pair(k, v));
      }

      return true;
    }

    return false;
  }
};

class JsonPraser {
 public:
  template <typename T>
  static std::string toJson(const T& obj) {
    std::ostringstream os;
    JsonHelper<T>::toJson(os, obj);
    return os.str();
  }

  template <typename T>
  static bool toObj(T& obj, const std::string& json) {
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    if (doc.HasParseError()) {
      return false;
    }

    return JsonHelper<T>::toObj(doc, obj);
  }
};

SERIALIZER_DJSON_NAMESPACE_END
KINGFISHER_NAMESPACE_END

#endif
