#!/usr/bin/env python3
import tensorflow as tf
import numpy as np
import cv2

img = cv2.imread("/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/car_imgs/lane6.png",cv2.IMREAD_COLOR)
print(img.shape)
img = cv2.resize(img,(512,896))[:,:,::-1]
# Load the TFLite model and allocate tensors.
interpreter = tf.lite.Interpreter(model_path="/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/models/saved_model/model_float32.tflite")
interpreter.allocate_tensors()

# Get input and output tensors.
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()
print(input_details)
print(output_details)

interpreter.set_tensor(input_details[0]['index'], np.array(img.transpose([1,0,2])[None,...],dtype=np.float32))

interpreter.invoke()

# The function `get_tensor()` returns a copy of the tensor data.
# Use `tensor()` in order to get a pointer to the tensor.
output_data = interpreter.get_tensor(output_details[0]['index'])
print(output_data)
for i in range(4):
    cv2.imshow("result",output_data[0,:,:,i])
    while True:
        if cv2.waitKey(1)&0xff == ord("q"):
            break

# img = cv2.imread("/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/car_imgs/lane6.png",cv2.IMREAD_COLOR)
# print(img.shape)
# loaded = tf.saved_model.load("/home/tan/saved_model")

# infer = loaded.signatures["serving_default"]
# img = cv2.resize(img,(512,896))
# x = tf.keras.preprocessing.image.img_to_array(img[:,:,::1])
# print(x)
# out = infer(tf.constant(np.transpose(x[None,...],axes=[0,2,1,3])))
# print(out["tf.identity"][0].numpy()[:,:,0].shape)
# for i in range(4):
#     cv2.imshow("result",out["tf.identity"][0].numpy()[:,:,i])
#     while True:
#         if cv2.waitKey(1)&0xff == ord("q"):
#             break
# cv2.imshow("result",np.array(np.argmax(out["tf.identity"][0].numpy(),axis=2,keepdims=False),dtype=np.float32)/3)
# while True:
#         if cv2.waitKey(1)&0xff == ord("q"):
#             break

# import onnx
# onnx_model = onnx.load("/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/model_float32.onnx")
# onnx.checker.check_model(onnx_model)

# import onnxruntime as ort

# img = cv2.imread("/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/car_imgs/lane1.png")
# print(img.shape)
# ort_sess = ort.InferenceSession('/home/tan/Desktop/car_d/car_ws/src/car_vision/car_vision/model_float32.onnx',providers=['TensorrtExecutionProvider', 'CUDAExecutionProvider', 'CPUExecutionProvider'])
# outputs = ort_sess.run(None, {'data': img})

# # Print Result 
# print(img.shape)
# cv2.imshow(img[:,:,:3])
# while True:
#     if cv2.waitKey(1)&0xff == "q":
#         break