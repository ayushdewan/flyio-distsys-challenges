#include <iostream>
#include <random>

#include "../maelstrom-shared/json.hpp"
#include "../maelstrom-shared/message.hpp"
#include "../maelstrom-shared/node.hpp"

void handle_init(node &n, const message &msg);
void handle_generate(node &n, const message &msg);

std::random_device rd;
std::mt19937_64 gen(rd());

int main(int argc, char *argv[]) {
  node n;
  n.seq = 0;

  nlohmann::json json_msg;
  while (std::cin >> json_msg) {
    message msg = json_to_message(json_msg);
    if (msg.body["type"] == "init") {
      handle_init(n, msg);
    } else if (msg.body["type"] == "generate") {
      handle_generate(n, msg);
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

std::string get_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  return std::to_string(millis.count());
}

void handle_generate(node &n, const message &msg) {
  nlohmann::json reply_body;
  reply_body["type"] = "generate_ok";
  reply_body["msg_id"] = n.seq++;
  reply_body["in_reply_to"] = msg.body["msg_id"];
  reply_body["id"] = get_timestamp() + "-" + std::to_string(gen());

  message reply = { msg.dest, msg.src, reply_body };
  std::cout << message_to_json(reply).dump() << std::endl;
}
