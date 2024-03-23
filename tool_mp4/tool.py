import cv2
import numpy as np

# Input the name of the video from the user
video_filename = input("Enter the name of the video (including .mp4 extension): ")

cap = cv2.VideoCapture(video_filename)

# Check if the video file was opened successfully
if not cap.isOpened():
    print("Error opening video file")
    exit()

frames_data = []
flip = False

# Loop through each frame in the video
while True: 
    ret, frame = cap.read()

    # Break the loop if no more frames are available
    if not ret:
        break
    flip = not flip
    if flip:
        # Resize the frame to 64x32 pixels
        resized_frame = cv2.resize(frame, (64, 32), interpolation=cv2.INTER_CUBIC)

        # Convert the resized frame to grayscale
        grayscaled_frame = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2GRAY)

        # Threshold the grayscale frame to get black and white pixels
        _, binary_frame = cv2.threshold(grayscaled_frame, 64, 1, cv2.THRESH_BINARY)

        # Convert the binary frame to a 1-D array of 64-bit integers
        binary_frame_simplified = [int(''.join(map(str, row)), 2) for row in binary_frame]

        frames_data.append(binary_frame_simplified)

# Convert frames_data to a NumPy array
frames_array = np.array(frames_data, dtype=np.int64)

# Save to video.bin with corresponding name
output_filename = video_filename.replace('.mp4', '.bin')
frames_array.tofile(output_filename)

# Release the video capture object
cap.release()

print(f"{len(frames_data)} frames extracted and saved in {output_filename}")
