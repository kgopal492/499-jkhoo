#ifndef CPP_KEY_VALUE_CLIENT_INTERFACE_H_
#define CPP_KEY_VALUE_CLIENT_INTERFACE_H_
#include <deque>
#include <string>

// Interface for KeyValueStore interactions from the Service Layer
class KeyValueClientInterface {
 public:
  virtual ~KeyValueClientInterface() = 0;
  // puts key value pair in store_, appending value if there is already a value
  // associated with key
  virtual void put(const std::string& key, const std::string& value) = 0;
  // gets values associated with the key, return deque is not guaranteed to be
  // unmodified
  virtual std::deque<std::string> get(const std::string& key) = 0;
  // deletes values associated with the key
  virtual void deletekey(const std::string& key) = 0;
};
#endif
