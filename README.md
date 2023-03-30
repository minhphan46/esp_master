# esp_master

** Thiết bị sử dụng:
- ESP8266
- Cảm biến siêu âm Ultrasonic Sensor - HC-SR04  

** Xây dựng hệ thống:
- Master - ESP8266:
+ Nhận tín hiệu phát hiện rác từ cảm biến HC-SR04.
+ ESP8266 sử dụng giao thức web socket để gửi tín hiệu chụp khuôn mặt và rác đến 2 điện thoại.
+ Sau khi chụp xong, điện thoại chụp rác sẽ gửi label về cho ESP8266 và ESP8266 sẽ gửi đến điện thoại chụp khuôn mặt. Sau đó, ở điện thoại chụp khuôn mặt sẽ chờ tín hiệu đầu vào của người dùng (chọn phân loại rác gì), tiến hành so sánh đối chiếu, cuối cùng gửi tín hiệu về cho ESP8266 để biết loại rác ấy được phân loại ở phía bên nào của thùng rác.
+ ESP8266 giao tiếp với Arduino Uno R3 được kết nối với mạch CNC Shiled V3 dùng driver A8825 thông qua giao thức trao đổi dữ liệu I2C bằng cổng SDA, SCL.

** Thư viện sử dụng:
+ AccelStepper
+ Wire
+ ESP8266WiFi
+ ArduinoJson
+ ESP8266WiFiMulti
+ WebSocketsServer
+ ESP8266WebServer

** IDE Platform: PlatformIO + Arduino IDE

** Hướng dẫn cài đặt:
- Đối với mạch ESP8266, tải code về, tải các thư viện phù hợp, sau đó tiến hành upload code dành cho Master.

** LƯU Ý:
- Khi upload code, tháo chân cắm SDA và SCL ra, sau khi code upload xong có thể cắm vào.
- Tiến hành kết nối động cơ với mạch trước rồi mới cấp nguồn cho CNC Shield V3.
