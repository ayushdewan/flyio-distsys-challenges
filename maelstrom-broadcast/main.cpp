#include <iostream>
#include <set>
#include <vector>

#include "../maelstrom-shared/json.hpp"
#include "../maelstrom-shared/message.hpp"
#include "../maelstrom-shared/node.hpp"

struct state {
  std::set<int> values;
};

void handle_init(node &n, state &s, const message &msg);
void handle_broadcast(node &n, state &s, const message &msg);
void handle_read(node &n, state &s, const message &msg);
void handle_topology(node &n, state &s, const message &msg);

int main(int argc, char *argv[]) {
  node n;
  n.seq = 0;

  state node_state;

  nlohmann::json json_msg;
  while (std::cin >> json_msg) {
    message msg = json_to_message(json_msg);
    if (msg.body["type"] == "init") {
      handle_init(n, node_state, msg);
    } else if (msg.body["type"] == "broadcast") {
      handle_broadcast(n, node_state, msg);
    } else if (msg.body["type"] == "read") {
      handle_read(n, node_state, msg);
    } else if (msg.body["type"] == "topology") {
      handle_topology(n, node_state, msg);
    }
  }

  return 0;
}

void handle_init(node &n, state &s, const message &msg) {
  n.id = msg.body["node_id"];
  n.node_ids = std::move(msg.body["node_ids"]);

  nlohmann::json reply_body;
  reply_body["type"] = "init_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];

  message reply = {msg.dest, msg.src, reply_body};
  std::cout << message_to_json(reply).dump() << std::endl;
}

void handle_broadcast(node &n, state &s, const message &msg) {
  int value = msg.body["message"];
  if (s.values.find(value) == s.values.end()) {
    s.values.insert(value);
    for (auto &node_id : n.node_ids) {
      if (node_id == n.id) {
        continue;
      }

      nlohmann::json broadcast_msg_body;
      broadcast_msg_body["type"] = "broadcast";
      broadcast_msg_body["msg_id"] = n.seq++;
      broadcast_msg_body["message"] = value;

      message broadcast_msg = {n.id, node_id, broadcast_msg_body};
      std::cout << message_to_json(broadcast_msg).dump() << std::endl;
    }
  }

  nlohmann::json reply_body;
  reply_body["type"] = "broadcast_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];

  message reply = {msg.dest, msg.src, reply_body};
  std::cout << message_to_json(reply).dump() << std::endl;
}

void handle_read(node &n, state &s, const message &msg) {
  nlohmann::json reply_body;
  reply_body["type"] = "read_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];

  std::vector<int> output_values(s.values.begin(), s.values.end());
  reply_body["messages"] = output_values;

  message reply = {msg.dest, msg.src, reply_body};
  std::cout << message_to_json(reply).dump() << std::endl;
}

void handle_topology(node &n, state &s, const message &msg) {
  nlohmann::json reply_body;
  reply_body["type"] = "topology_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];

  message reply = {msg.dest, msg.src, reply_body};
  std::cout << message_to_json(reply).dump() << std::endl;
}
