import json
import math
import sys

import zmq

ENDPOINT = "tcp://127.0.0.1:5555"
EXPECTED_FRAMES = {
    "pose_frame": "zed_world",
    "twist_frame": "zed_left_camera",
}


def require_number(value, path):
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"{path} must be a number")
    if isinstance(value, float) and not math.isfinite(value):
        raise ValueError(f"{path} must be finite")


def validate_vector(data, field, components):
    value = data.get(field)
    if not isinstance(value, dict):
        raise ValueError(f"{field} must be an object")

    for component in components:
        if component not in value:
            raise ValueError(f"{field}.{component} is missing")
        require_number(value[component], f"{field}.{component}")


def validate_message(data):
    if not isinstance(data, dict):
        raise ValueError("message must be a JSON object")

    timestamp_ns = data.get("timestamp_ns")
    if isinstance(timestamp_ns, bool) or not isinstance(timestamp_ns, int):
        raise ValueError("timestamp_ns must be an integer")
    if timestamp_ns < 0:
        raise ValueError("timestamp_ns must be non-negative")

    if not isinstance(data.get("pose_valid"), bool):
        raise ValueError("pose_valid must be a boolean")

    pose_confidence = data.get("pose_confidence")
    if isinstance(pose_confidence, bool) or not isinstance(pose_confidence, int):
        raise ValueError("pose_confidence must be an integer")
    if not 0 <= pose_confidence <= 100:
        raise ValueError("pose_confidence must be between 0 and 100")

    tracking_state = data.get("tracking_state")
    if not isinstance(tracking_state, str) or not tracking_state:
        raise ValueError("tracking_state must be a non-empty string")

    for field, expected in EXPECTED_FRAMES.items():
        if data.get(field) != expected:
            raise ValueError(f"{field} must be {expected!r}")

    validate_vector(data, "position", ("x", "y", "z"))
    validate_vector(data, "orientation", ("x", "y", "z", "w"))
    validate_vector(data, "linear_velocity", ("x", "y", "z"))
    validate_vector(data, "angular_velocity", ("x", "y", "z"))


def format_message(data):
    return "\n".join(
        (
            f"Timestamp:        {data['timestamp_ns']} ns",
            f"Position:         x={data['position']['x']: .3f}  "
            f"y={data['position']['y']: .3f}  "
            f"z={data['position']['z']: .3f} m",
            f"Linear velocity:  x={data['linear_velocity']['x']: .3f}  "
            f"y={data['linear_velocity']['y']: .3f}  "
            f"z={data['linear_velocity']['z']: .3f} m/s",
            f"Angular velocity: x={data['angular_velocity']['x']: .3f}  "
            f"y={data['angular_velocity']['y']: .3f}  "
            f"z={data['angular_velocity']['z']: .3f} rad/s",
            f"Orientation:      x={data['orientation']['x']: .3f}  "
            f"y={data['orientation']['y']: .3f}  "
            f"z={data['orientation']['z']: .3f}  "
            f"w={data['orientation']['w']: .3f}",
            f"Tracking:         state={data['tracking_state']}  "
            f"valid={data['pose_valid']}  "
            f"confidence={data['pose_confidence']}",
            f"Frames:           pose={data['pose_frame']}  "
            f"twist={data['twist_frame']}",
        )
    )


def print_message(data):
    display = format_message(data)
    if sys.stdout.isatty():
        print(f"\033[H\033[J{display}", end="", flush=True)
    else:
        print(display, flush=True)


def main():
    context = None
    socket = None

    try:
        context = zmq.Context()
        socket = context.socket(zmq.SUB)
        socket.setsockopt_string(zmq.SUBSCRIBE, "")
        socket.connect(ENDPOINT)

        while True:
            try:
                message = socket.recv_string()
                data = json.loads(message)
                validate_message(data)
                print_message(data)
            except (json.JSONDecodeError, UnicodeDecodeError, ValueError) as error:
                print(f"Invalid VIO message: {error}")
            except KeyboardInterrupt:
                print("\nStopping VIO subscriber")
                return 0
            except zmq.ZMQError as error:
                print(f"ZeroMQ receive failed: {error}")
                return 1
    except zmq.ZMQError as error:
        print(f"ZeroMQ setup failed: {error}")
        return 1
    finally:
        if socket is not None:
            socket.close(linger=0)
        if context is not None:
            context.term()


if __name__ == "__main__":
    raise SystemExit(main())
