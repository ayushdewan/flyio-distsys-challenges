#include <iostream>
#include <random>

#include "../maelstrom-shared/json.hpp"
#include "../maelstrom-shared/message.hpp"
#include "../maelstrom-shared/node.hpp"

void handle_init(node &n, const message &msg);
void handle_echo(node &n, const message &msg);

int main(int argc, char *argv[]) {
  node n;
  n.seq = 0;

  nlohmann::json json_msg;
  while (std::cin >> json_msg) {
    message msg = json_to_message(json_msg);
    if (msg.body["type"] == "init") {
      handle_init(n, msg);
    } else if (msg.body["type"] == "echo") {
      handle_echo(n, msg);
    }
  }

  return 0;
}

void handle_init(node &n, const message &msg) {
  n.id = msg.body["node_id"];
  n.node_ids = std::move(msg.body["node_ids"]);

  nlohmann::json reply_body;
  reply_body["type"] = "init_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];

  message reply = { msg.dest, msg.src, reply_body };
  std::cout << message_to_json(reply).dump() << std::endl;
}

void handle_echo(node &n, const message &msg) {
  nlohmann::json reply_body;
  reply_body["type"] = "echo_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];
  reply_body["echo"] = msg.body["echo"];

  message reply = { msg.dest, msg.src, reply_body };
  std::cout << message_to_json(reply).dump() << std::endl;
}
