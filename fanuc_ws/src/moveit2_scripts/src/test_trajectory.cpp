#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/msg/pose.hpp>
#include <rclcpp/rclcpp.hpp>

static const rclcpp::Logger LOGGER = rclcpp::get_logger("straight_trajectory_demo");

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions node_options;
  node_options.automatically_declare_parameters_from_overrides(true);
  auto move_group_node =
      rclcpp::Node::make_shared("move_group_interface_demo", node_options);

  // Define the planning group for the manipulator
  static const std::string PLANNING_GROUP = "manipulator";
  moveit::planning_interface::MoveGroupInterface move_group(move_group_node, PLANNING_GROUP);

  const double eef_step = 0.01;  // Small step size for smooth motion
  const double jump_threshold = 0.0;  // Disable jumps in trajectory

  RCLCPP_INFO(LOGGER, "Starting straight trajectory loop");

  // Get the starting pose of the manipulator
  geometry_msgs::msg::Pose start_pose = move_group.getCurrentPose().pose;

  while (rclcpp::ok()) {
    // Define waypoints for a straight trajectory
    std::vector<geometry_msgs::msg::Pose> waypoints;

    // Move forward (along X-axis)
    geometry_msgs::msg::Pose forward_pose = start_pose;
    forward_pose.position.x += 0.3;  // Move 30 cm forward
    waypoints.push_back(forward_pose);

    // Move back to the start position
    waypoints.push_back(start_pose);

    // Plan and execute the Cartesian path
    moveit_msgs::msg::RobotTrajectory trajectory;
    double fraction = move_group.computeCartesianPath(
        waypoints, eef_step, jump_threshold, trajectory);

    if (fraction < 1.0) {
      RCLCPP_WARN(LOGGER, "Trajectory incomplete. Fraction: %.2f", fraction);
    } else {
      RCLCPP_INFO(LOGGER, "Executing trajectory");
      moveit::planning_interface::MoveGroupInterface::Plan plan;
      plan.trajectory_ = trajectory;
      move_group.execute(plan);
    }
  }

  rclcpp::shutdown();
  return 0;
}

