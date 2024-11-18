import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import xacro

# NOT FUNCTIONING

def generate_launch_description():

    # File paths
    xacro_file = os.path.join(get_package_share_directory('crx_description'), 'urdf', 'softgripper.urdf.xacro')
    rviz_config_file = os.path.join(get_package_share_directory('crx10ia_l_moveit_config'), 'config', 'softgripper_config.rviz')

    # Process xacro 
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="both",
        parameters=[{'robot_description': doc.toxml()}],
    )
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        arguments=["-d", rviz_config_file],
    )

    return LaunchDescription([
        robot_state_publisher,
        rviz,
    ])
