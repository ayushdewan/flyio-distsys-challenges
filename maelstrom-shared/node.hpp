#ifndef NODE_HPP_
#define NODE_HPP_

#include <string>
#include <atomic>

struct node {
  std::string id;
  std::vector<std::string> node_ids;
  std::atomic<int> seq;
};

#endif // NODE_HPP_