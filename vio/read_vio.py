import json
import zmq

context = zmq.Context()
socket = context.socket(zmq.SUB)

socket.connect("tcp://127.0.0.1:5555")
socket.setsockopt_string(zmq.SUBSCRIBE, "")

while True:
    try:
        message = socket.recv_string()
        data = json.loads(message)

        print(
            f"state={data['tracking_state']} "
            f"pos=({data['position']['x']:.3f}, "
            f"{data['position']['y']:.3f}, "
            f"{data['position']['z']:.3f}) "
            f"vel=({data['linear_velocity']['x']:.3f}, "
            f"{data['linear_velocity']['y']:.3f}, "
            f"{data['linear_velocity']['z']:.3f}) "
            f"ang_vel=({data['angular_velocity']['x']:.3f}, "
            f"{data['angular_velocity']['y']:.3f}, "
            f"{data['angular_velocity']['z']:.3f}) "
            f"quat=({data['orientation']['x']:.3f}, "
            f"{data['orientation']['y']:.3f}, "
            f"{data['orientation']['z']:.3f}, "
            f"{data['orientation']['w']:.3f})"
        )

    except json.JSONDecodeError as e:
        print(f"Invalid JSON message: {e}")

    except KeyError as e:
        print(f"Missing field in VIO message: {e}")

    except KeyboardInterrupt:
        print("\nStopping VIO subscriber")
        break

socket.close()
context.term()