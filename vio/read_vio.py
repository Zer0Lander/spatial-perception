import subprocess

proc = subprocess.Popen(
    ["./build/vio"],
    cwd="/home/abdul/spatial_perception/vio",
    stdout=subprocess.PIPE,
    text=True,
    bufsize=1,
)

for line in proc.stdout:
    line = line.strip()

    if not line:
        continue

    parts = line.split(",")

    if len(parts) != 15:
        continue

    timestamp_ns = int(parts[0])

    x = float(parts[1])
    y = float(parts[2])
    z = float(parts[3])

    qx = float(parts[4])
    qy = float(parts[5])
    qz = float(parts[6])
    qw = float(parts[7])

    vx = float(parts[8])
    vy = float(parts[9])
    vz = float(parts[10])

    wx = float(parts[11])
    wy = float(parts[12])
    wz = float(parts[13])

    state = parts[14]

    print(
        f"state={state} "
        f"pos=({x:.3f}, {y:.3f}, {z:.3f}) "
        f"vel=({vx:.3f}, {vy:.3f}, {vz:.3f}) "
        f"ang_vel=({wx:.3f}, {wy:.3f}, {wz:.3f}) "
        f"quat=({qx:.3f}, {qy:.3f}, {qz:.3f}, {qw:.3f})"
    )