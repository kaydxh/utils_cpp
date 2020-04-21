#include <stdio.h>
#include <string>
#include "djson/djson.h"
using namespace kingfisher;
using namespace serializer_djson;

struct Test1 {
  int a;
  std::string b;
  double c;
  std::vector<int> e;
  std::map<std::string, std::string> f;
};

int main() {
  // init
  std::vector<std::pair<std::string, JsonBase *> > vec;
  vec.push_back(std::make_pair("a", new JsonObjector<Test1, int>(&Test1::a)));
  vec.push_back(
      std::make_pair("b", new JsonObjector<Test1, std::string>(&Test1::b)));
  vec.push_back(
      std::make_pair("c", new JsonObjector<Test1, double>(&Test1::c)));
  vec.push_back(std::make_pair(
      "e", new JsonObjector<Test1, std::vector<int> >(&Test1::e)));
  vec.push_back(std::make_pair(
      "f",
      new JsonObjector<Test1, std::map<std::string, std::string> >(&Test1::f)));

  Singleton<JsonObjsManager>::instance().put(typeid(Test1).name(), vec);

  // obj2json
  Test1 t1 = {10, "abc", 3.4, {1, 2, 3, 4}, {{"1", "a"}, {"2", "b"}}};

  std::string js = JsonPraser::toJson(t1);

  // print
  std::cout << js << std::endl;

  // json2Obj
  Test1 t2;
  if (JsonPraser::toObj(t2, js)) {
    // print
    std::cout << "a:" << t2.a << " b:" << t2.b << " c:" << t2.c << std::endl;

    std::cout << "e:";
    for (auto &e : t2.e) {
      std::cout << e << " ";
    }
    std::cout << std::endl;

    std::cout << "f:";
    for (auto &e : t2.f) {
      std::cout << e.first << ":" << e.second << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "toObj failed." << std::endl;
  }

  return 0;
}
