#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>
#include "json.hpp"

struct message {
  std::string src;
  std::string dest;
  nlohmann::json body;
};

// Conversion functions
message json_to_message(const nlohmann::json &j) {
  return { j["src"], j["dest"], j["body"] };
}

nlohmann::json message_to_json(const message &m) {
  nlohmann::json j;
  j["src"] = m.src;
  j["dest"] = m.dest;
  j["body"] = m.body;
  return j;
}

#endif // MESSAGE_HPP_