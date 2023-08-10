// Cactus RT
#include <cactus_rt/rt.h>

#include "inverted_pendulum/ros_pendulum_node.h"
#include "inverted_pendulum/rt_thread.h"

using cactus_rt::App;

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<RosPendulumNode>("pendulum_node");

  cactus_rt::CyclicThreadConfig rt_thread_config;
  rt_thread_config.period_ns = 1'000'000;
  rt_thread_config.SetFifoScheduler(80);
  auto rt_thread = std::make_shared<RtThread>(node, rt_thread_config);

  App app;
  app.RegisterThread(rt_thread);

  app.Start();

  auto ros_thread = std::thread(
    [&]() {
      rclcpp::spin(node);
    }
  );

  rt_thread->Join();  // This thread will terminate on its own.
  ros_thread.join();

  return 0;
}
