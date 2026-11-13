"""Generate a solar-panel test image for the channel-splitting exercise."""

from pathlib import Path

import cv2
import numpy as np


WIDTH, HEIGHT = 1280, 720
ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "images" / "solar_panel.png"
VIDEO = ROOT / "images" / "solar_panel_demo.mp4"


def lerp(a, b, t):
    return a + (b - a) * t


def draw_sky(image):
    top = np.array([215, 150, 72], dtype=np.float32)
    horizon = np.array([245, 225, 190], dtype=np.float32)
    for y in range(300):
        t = y / 299.0
        image[y, :] = (top * (1.0 - t) + horizon * t).astype(np.uint8)

    # Soft sun and haze near the horizon.
    overlay = image.copy()
    for radius, alpha in ((150, 0.08), (105, 0.12), (62, 0.35)):
        cv2.circle(overlay, (1020, 120), radius, (225, 245, 255), -1, cv2.LINE_AA)
        cv2.addWeighted(overlay, alpha, image, 1.0 - alpha, 0, image)
    cv2.circle(image, (1020, 120), 34, (235, 250, 255), -1, cv2.LINE_AA)

    # Thin clouds.
    cloud = image.copy()
    cv2.ellipse(cloud, (235, 110), (130, 24), 0, 0, 360, (245, 248, 250), -1, cv2.LINE_AA)
    cv2.ellipse(cloud, (335, 96), (92, 20), 0, 0, 360, (250, 250, 250), -1, cv2.LINE_AA)
    cv2.ellipse(cloud, (740, 160), (150, 18), 0, 0, 360, (235, 242, 246), -1, cv2.LINE_AA)
    cv2.addWeighted(cloud, 0.55, image, 0.45, 0, image)


def draw_ground(image):
    horizon = 292
    grass_far = np.array([58, 118, 66], dtype=np.float32)
    grass_near = np.array([38, 92, 52], dtype=np.float32)
    for y in range(horizon, HEIGHT):
        t = (y - horizon) / float(HEIGHT - horizon)
        image[y, :] = (grass_far * (1.0 - t) + grass_near * t).astype(np.uint8)

    # Distant tree line.
    points = []
    for x in range(0, WIDTH, 18):
        h = 8 + int(7 * abs(np.sin(x * 0.045)) + 4 * abs(np.sin(x * 0.013)))
        points.append((x, horizon - h))
    poly = np.array(points + [(WIDTH, horizon), (0, horizon)], dtype=np.int32)
    cv2.fillPoly(image, [poly], (52, 96, 58), cv2.LINE_AA)

    # Perspective grass rows.
    for offset in (25, 80, 150, 235, 335):
        cv2.line(image, (0, horizon + offset), (WIDTH, horizon + offset + 22),
                 (70, 124, 72), 1, cv2.LINE_AA)


def panel_polygon(x, y, width, height, lean=16):
    return np.array([
        [x + lean, y],
        [x + width + lean, y],
        [x + width, y + height],
        [x, y + height],
    ], dtype=np.int32)


def draw_panel(image, x, y, width, height, lean=16):
    pts = panel_polygon(x, y, width, height, lean)
    cv2.fillPoly(image, [pts], (52, 45, 30), cv2.LINE_AA)
    cv2.polylines(image, [pts], True, (185, 190, 192), 3, cv2.LINE_AA)

    cols, rows = 4, 3
    for c in range(1, cols):
        t = c / cols
        p1 = (int(lerp(x + lean, x + width + lean, t)), y)
        p2 = (int(lerp(x, x + width, t)), y + height)
        cv2.line(image, p1, p2, (135, 140, 145), 1, cv2.LINE_AA)
    for r in range(1, rows):
        t = r / rows
        p1 = (int(lerp(x + lean, x, t)), int(y + height * t))
        p2 = (int(lerp(x + width + lean, x + width, t)), int(y + height * t))
        cv2.line(image, p1, p2, (135, 140, 145), 1, cv2.LINE_AA)

    # Blue reflection along the upper edge of each cell.
    for c in range(cols):
        x1 = lerp(x + lean, x + width + lean, c / cols)
        x2 = lerp(x + lean, x + width + lean, (c + 1) / cols)
        cv2.line(image, (int(x1) + 3, y + 8), (int(x2) - 4, y + 13),
                 (115, 80, 42), 4, cv2.LINE_AA)

    # Support legs.
    cv2.line(image, (int(x + width * 0.18), y + height), (int(x + width * 0.12), y + height + 18),
             (70, 75, 78), 4, cv2.LINE_AA)
    cv2.line(image, (int(x + width * 0.82), y + height), (int(x + width * 0.88), y + height + 18),
             (70, 75, 78), 4, cv2.LINE_AA)


def draw_array(image):
    rows = [
        (300, 352, 168, 9, 88),
        (382, 474, 224, 13, 58),
        (512, 642, 300, 18, 30),
    ]
    for row_index, (y, y2, width, gap, start) in enumerate(rows):
        count = 6 if row_index < 2 else 4
        x = start
        while x < WIDTH + width:
            draw_panel(image, x, y, width, y2 - y, lean=10 + row_index * 5)
            x += width + gap


def add_foreground_details(image):
    # A few small grass tufts break up the lower foreground.
    rng = np.random.default_rng(7)
    for _ in range(260):
        x = int(rng.integers(0, WIDTH))
        y = int(rng.integers(565, HEIGHT))
        length = int(rng.integers(3, 9))
        shade = (45 + int(rng.integers(0, 25)), 118 + int(rng.integers(0, 35)), 48)
        cv2.line(image, (x, y), (x + int(rng.integers(-3, 4)), y - length), shade, 1, cv2.LINE_AA)


def make_still():
    image = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)
    draw_sky(image)
    draw_ground(image)
    draw_array(image)
    add_foreground_details(image)

    # Subtle vignette.
    yy, xx = np.mgrid[0:HEIGHT, 0:WIDTH]
    d = np.sqrt(((xx - WIDTH / 2) / (WIDTH / 2)) ** 2 + ((yy - HEIGHT / 2) / (HEIGHT / 2)) ** 2)
    mask = np.clip(1.0 - 0.17 * np.clip(d - 0.45, 0, None), 0.82, 1.0).astype(np.float32)
    image = np.clip(image.astype(np.float32) * mask[..., None], 0, 255).astype(np.uint8)

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    cv2.imwrite(str(OUTPUT), image)
    return image


def make_demo_video(still):
    writer = cv2.VideoWriter(str(VIDEO), cv2.VideoWriter_fourcc(*"mp4v"), 20.0, (WIDTH, HEIGHT))
    if not writer.isOpened():
        return
    for frame_index in range(100):
        scale = 1.0 + 0.035 * np.sin(frame_index * 0.08)
        matrix = cv2.getRotationMatrix2D((WIDTH / 2, HEIGHT / 2), 0.0, scale)
        frame = cv2.warpAffine(still, matrix, (WIDTH, HEIGHT), borderMode=cv2.BORDER_REFLECT)
        cv2.putText(frame, "Local camera test video", (36, 54),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.8, (245, 245, 245), 2, cv2.LINE_AA)
        writer.write(frame)
    writer.release()


if __name__ == "__main__":
    still_image = make_still()
    make_demo_video(still_image)
    print(f"Saved {OUTPUT}")
    print(f"Saved {VIDEO}")
