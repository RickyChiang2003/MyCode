import cv2
import numpy as np
from keras.datasets import mnist
from keras import utils

# Choose your webcam: 0, 1, ...
cap = cv2.VideoCapture(1)
thres = 96
thres_2 = 46
combined_circles = np.zeros((380,380), dtype=np.uint8)
counter=0
start_writing=False
prediction=""

knn = cv2.ml.KNearest_load('mnist_knn.xml')

if not cap.isOpened():
	cap.open()

def set_threshold(value):
	global thres
	thres = value

def set_threshold_2(value):
	global thres_2
	thres_2 = value

def predict(tmp):
	img_num=tmp.copy()
	img_num = cv2.resize(img_num,(28,28))
	img_num = img_num.astype(np.float32)
	img_num = img_num.reshape(-1,)
	img_num = img_num.reshape(1,-1)
	img_num = img_num/255

	img_pre = knn.predict(img_num)
	num = int(img_pre[1][0][0])
	return num

while(True):
	# Get one frame from the camera
	ret, frame = cap.read()

	# crop
	frame=frame[50:430, 100:480]
	frame=cv2.flip(frame, 0)

	# Split RGB channels
	b, g, r = cv2.split(frame)

	# view result (optional)
	zeros = np.zeros(frame.shape[:2], dtype="uint8")

	# Perform thresholding to each channel
	_, r = cv2.threshold(r, thres, 255, cv2.THRESH_BINARY)
	_, b = cv2.threshold(b, thres_2, 255, cv2.THRESH_BINARY_INV)

	# Get the final result using bitwise operation
	zeros = np.zeros(frame.shape[:2], dtype="uint8")
	result = cv2.bitwise_and(r, b, mask = None)

	# Find and draw contours
	contours, hierarchy = cv2.findContours(result, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

	# Draw the contours (for debugging)
	display = cv2.cvtColor(result, cv2.COLOR_GRAY2BGR)
	cv2.drawContours(display, contours, -1, (0,0,255))

	# Iterate through each contour, check the area and find the center
	circle_count=0
	for cnt in contours:
		# Calculate the area of the contour
		area = cv2.contourArea(cnt)
		# Find the centroid
		(x,y), radius = cv2.minEnclosingCircle(cnt)
		if radius > 10:
			circle_count+=1
			cv2.circle(frame, (int(x),int(y)), int(radius), (0,255,0), thickness=2)
			cv2.circle(combined_circles, (int(x), int(y)), 30, 255, thickness=-1)

	if circle_count==0:
		if start_writing:
			counter+=1
	else:
		start_writing=True
		counter=0
	if counter>=30:
		counter=0
		start_writing=False
		prediction=str(predict(combined_circles))
		print(prediction)
		combined_circles = np.zeros((380,380), dtype=np.uint8)

	# show prediction
	combined_circles_text=cv2.merge([combined_circles, combined_circles, combined_circles])
	org = (0,25)
	fontFace = cv2.FONT_HERSHEY_SIMPLEX
	fontScale = 1
	color = (0,255,0)
	cv2.putText(combined_circles_text, f"prediction: {prediction}", org, fontFace, fontScale, color)

	# Show the frame
	cv2.imshow('frame', frame)
	cv2.imshow("Red", cv2.merge([zeros, zeros, result]))
	cv2.imshow('Combined Circles with Text', combined_circles_text)

	# Press q to quit
	key= cv2.waitKey(1) & 0xFF
	if key==ord('q'):
		break

# Release the camera
cap.release()

# Close all OpenCV windows
cv2.destroyAllWindows()
