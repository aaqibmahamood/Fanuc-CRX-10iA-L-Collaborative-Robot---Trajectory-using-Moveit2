import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import xacro

def generate_launch_description():

    # File paths
    xacro_file = os.path.join(get_package_share_directory('crx_description'), 'urdf', 'crx10ia_l.xacro')
    rviz_config_file = os.path.join(get_package_share_directory('crx10ia_l_moveit_config'), 'config', 'config.rviz')

    # Process xacro 
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)    

    joint_state_publisher = Node(
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
    )
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="both",
        parameters=[{'crx_description': doc.toxml()}],
    )
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        arguments=["-d", rviz_config_file],
    )

    return LaunchDescription([
        joint_state_publisher,
        robot_state_publisher,
        rviz,
    ])
