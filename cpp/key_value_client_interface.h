#include <deque>
#include <string>

class KeyValueClientInterface {
 public:
  virtual ~KeyValueClientInterface() = 0;
  // puts key value pair in store_, appending value if there is already a value associated with key
  virtual void put(const std::string& key, const std::string& value) = 0;
  // gets values associated with the key
  virtual const std::deque<std::string>& get(const std::string& key) = 0;
  // deletes values associated with the key
  virtual void deletekey(const std::string& key) = 0;
};
